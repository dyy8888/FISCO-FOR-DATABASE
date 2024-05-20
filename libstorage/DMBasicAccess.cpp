/*
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2019 fisco-dev contributors.
 */
/** @file SQLBasicAccess.cpp
 *  @author darrenyin
 *  @date 2019-04-24
 */

#include "DMBasicAccess.h"
#include "DMConnectionPool.h"
#include "StorageException.h"
#include "libconfig/GlobalConfigure.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/dynamic_bitset.hpp>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <zlib.h>
#include <cstring> 
using namespace dev::storage;
using namespace std;

DMBasicAccess::DMBasicAccess(){}

int DMBasicAccess::Select(int64_t, const string& _table, const string&, Condition::Ptr _condition,
    vector<map<string, string>>& _values)
{
    string sql = this->BuildQuerySql(_table, _condition);
    
    Connection_T conn = m_connPool->GetConnection();
    uint32_t retryCnt = 0;
    uint32_t retryMax = 30;
    while (conn == NULL && retryCnt++ < retryMax)
    {
        DMBasicAccess_LOG(WARNING)
            << "table:" << _table << "sql:" << sql << " get connection failed";
        sleep(1);
        conn = m_connPool->GetConnection();
    }

    if (conn == NULL)
    {
        DMBasicAccess_LOG(ERROR) << "table:" << _table << "sql:" << sql
                                  << " get connection failed";
        return -1;
    }
    TRY
    {
        PreparedStatement_T _prepareStatement =
            Connection_prepareStatement(conn, "%s", sql.c_str());
        if (_condition)
        {
            uint32_t index = 0;
            for (auto& it : *(_condition))
            {
                PreparedStatement_setString(
                    _prepareStatement, ++index, it.second.right.second.c_str());             
            }
        }
        ResultSet_T result = PreparedStatement_executeQuery(_prepareStatement);
        int32_t columnCnt = ResultSet_getColumnCount(result);
        string tableName;
        tableName = boost::algorithm::replace_all_copy(_table, "\\", "\\\\");
        tableName = boost::algorithm::replace_all_copy(_table, "`", "\\`");

        auto outer_it = tableColumnType.find(tableName);
        //如果缓存中不包含该表格的表项和类型信息，则去查询
        if (outer_it == tableColumnType.end())
        {
             for (int32_t index = 1; index <= columnCnt; ++index)
            {
                auto fieldName = ResultSet_getColumnName(result, index);
                PreparedStatement_T _prepareStatement =
                    Connection_prepareStatement(conn, "select data_type from all_tab_columns where table_name=? and owner='SYSDBA' and column_name=?;");
                    PreparedStatement_setString(_prepareStatement, 1,tableName.c_str());
	                PreparedStatement_setString(_prepareStatement, 2,fieldName);
	            ResultSet_T resultson= PreparedStatement_executeQuery(_prepareStatement);
                int32_t columnTypeCnt = ResultSet_getColumnCount(resultson);
                string selectResult;
                while (ResultSet_next(resultson))
                {
                    for (int32_t indexson = 1; indexson <= columnTypeCnt; ++indexson)
                    {
                        auto columnName=ResultSet_getColumnName(resultson,indexson);
				        selectResult = ResultSet_getString(resultson, indexson);
                        // DMBasicAccess_LOG(INFO)<<"select查看哈希表tablename:"<<tableName<<" fieldname:"<<fieldName<<" selectResult:"<<selectResult;
                        tableColumnType[tableName][fieldName]=selectResult;
                    }
	            }
            }            

        }
        while (ResultSet_next(result))
        {
            map<string, string> value;
            for (int32_t index = 1; index <= columnCnt; ++index)
            {
                string selectResult;
                auto columnName=ResultSet_getColumnName(result,index);
                selectResult=tableColumnType[tableName][columnName];
                if (selectResult=="CLOB")
                {
                    int size;
                    auto bytes = ResultSet_getBlob(result,index, &size);
                    if (bytes)
                    {
                        string res=base64_decode(string((char*)bytes,size),size);
                        string ress=decompressString(res,res.size());
                        // DMBasicAccess_LOG(INFO)<<"查看CLOB查询结果："<<ress;
                        if (ress!="NULL"){
                            value[columnName] = ress;
                        }
                    }
                }else{    
                    auto selectResult = ResultSet_getString(result,index);
                    if (selectResult)
                    {
                        value[columnName] = selectResult;
                    }
                  
                }
                
            }
            _values.push_back(move(value));
        }
    }
    // Connection_execute(conn,"commit;");
    CATCH(SQLException)
    {
        m_connPool->ReturnConnection(conn);
        // Note: when select exception caused by table doesn't exist and sql error,
        //       here must return 0, in case of the DB is normal but the sql syntax error or the
        //       table will be created later
        DMBasicAccess_LOG(ERROR) << "select exception for sql error:" << Exception_frame.message;
        return 0;
    }
    END_TRY;
    m_connPool->ReturnConnection(conn);
    return 0;
}
string DMBasicAccess::BuildQuerySql(string _table, Condition::Ptr _condition)
{   
    _table = boost::algorithm::replace_all_copy(_table, "\\", "\\\\");
    _table = boost::algorithm::replace_all_copy(_table, "`", "\\`");
    string sql = "select * from \"SYSDBA\".\"";
    sql.append(_table).append("\"");
    if (_condition)
    {
        bool hasWhereClause = false;
        for (auto it = _condition->begin(); it != _condition->end(); ++it)
        {
            if (!hasWhereClause)
            {
                hasWhereClause = true;
                sql.append(BuildConditionSql(" where", it, _condition));
            }
            else
            {
                sql.append(BuildConditionSql(" and", it, _condition));
            }
        }
    }
    return sql;
}

string DMBasicAccess::BuildConditionSql(const string& _strPrefix,
    map<string, Condition::Range>::const_iterator& _it, Condition::Ptr _condition)
{
    string strConditionSql = _strPrefix;
    if (_it->second.left.second == _it->second.right.second && _it->second.left.first &&
        _it->second.right.first)
    {
        strConditionSql.append(" \"").append(_it->first).append("\"=").append("?");
    }
    else
    {
        if (_it->second.left.second != _condition->unlimitedField())
        {
            if (_it->second.left.first)
            {
                strConditionSql.append(" \"").append(_it->first).append("\">=").append("?");
            }
            else
            {
                strConditionSql.append(" \"").append(_it->first).append("\">").append("?");
            }
        }
        if (_it->second.right.second != _condition->unlimitedField())
        {
            if (_it->second.right.first)
            {
                strConditionSql.append(" \"").append(_it->first).append("\"<=").append("?");
            }
            else
            {
                strConditionSql.append(" \"").append(_it->first).append("\"<").append("?");
            }
        }
    }
    return strConditionSql;
}

DMFieldType DMBasicAccess::getFieldType(std::string const& _tableName)
{
    if (_tableName == SYS_HASH_2_BLOCK || _tableName == SYS_BLOCK_2_NONCES||_tableName == SYS_HASH_2_BLOCKHEADER )
    {
        return DMFieldType::ClobType;
    }
    if (_tableName==SYS_CNS||_tableName==SYS_CONFIG||_tableName==SYS_CURRENT_STATE||_tableName==SYS_NUMBER_2_HASH||_tableName==SYS_TX_HASH_2_BLOCK)
    {
        return DMFieldType::ClobType;
    }
    if (boost::starts_with(_tableName, string("c_")))
    {
        if (g_BCOSConfig.version() >= V2_5_0)
        {
            return DMFieldType::ClobType;
        }
    }
    if (g_BCOSConfig.version() >= V2_6_0)
    {
        return DMFieldType::ClobType;
    }
    return DMFieldType::ClobType;
}

string DMBasicAccess::BuildCreateTableSql(
    const string& _tableName, const string& _keyField, const string& _valueField)
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << _tableName << "\"(\n";
    ss << " \"_id_\" BIGINT IDENTITY(1, 1) NOT NULL,\n";
    ss << " \"_num_\" BIGINT NOT NULL,\n";
    ss << " \"_status_\" INT NOT NULL,\n";
    ss << "\"" << _keyField << "\" varchar(255) default '',\n";
    vector<string> vecSplit;
    boost::split(vecSplit, _valueField, boost::is_any_of(","));
    auto it = vecSplit.begin();
    auto fieldType = getFieldType(_tableName);
    std::string fieldTypeName = DMFieldTypeName[fieldType];
    for (; it != vecSplit.end(); ++it)
    {
        *it = boost::algorithm::replace_all_copy(*it, "\\", "\\\\");
        *it = boost::algorithm::replace_all_copy(*it, "`", "\\`");
        ss << "\"" << *it << "\" " << fieldTypeName << ",\n";
    }
    ss << " NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;\n";
    return ss.str();
}

string DMBasicAccess::BuildCreateTableSql(const Entry::Ptr& _entry)
{
    string tableName(_entry->getField("table_name"));
    string keyField(_entry->getField("key_field"));
    string valueField(_entry->getField("value_field"));

    tableName = boost::algorithm::replace_all_copy(tableName, "\\", "\\\\");
    tableName = boost::algorithm::replace_all_copy(tableName, "`", "\\`");

    keyField = boost::algorithm::replace_all_copy(keyField, "\\", "\\\\");
    keyField = boost::algorithm::replace_all_copy(keyField, "`", "\\`");

    string sql = BuildCreateTableSql(tableName, keyField, valueField);
    stringstream indexNumSql;
    indexNumSql<<"CREATE INDEX IF NOT EXISTS \"_num_\" ON \"SYSDBA\".\""<<tableName<<"\"(\"_num_\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    stringstream indexKeySql;
    indexKeySql<<"CREATE INDEX IF NOT EXISTS \""<<keyField<<"\" ON \"SYSDBA\".\""<<tableName<<"\"(\""<<keyField<<"\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    DMBasicAccess_LOG(DEBUG) << "create table:" << tableName << " keyfield:" << keyField
                              << " value field:" << valueField << " sql:" << sql;
    string combined = sql + "###" + indexNumSql.str()+"###"+indexKeySql.str();
    return combined;
}

void DMBasicAccess::GetCommitFieldNameAndValueEachTable(const string& _num,
    const Entries::Ptr& _data, const vector<size_t>& _indexList, string& _fieldStr,
    vector<string>& _valueList)
{
    bool isFirst = true;
    for (auto index : _indexList)
    {
        const auto& entry = _data->get(index);
        for (auto fieldIt : *entry)
        {
            if (fieldIt.first == NUM_FIELD || fieldIt.first == "_hash_" ||
                fieldIt.first == ID_FIELD || fieldIt.first == STATUS)
            {
                continue;
            }
            if (isFirst)
            {
                _fieldStr.append("\"").append(fieldIt.first).append("\",");
            }
            _valueList.push_back(fieldIt.second);
        }
        _valueList.push_back(_num);
        _valueList.push_back(boost::lexical_cast<string>(entry->getID()));
        _valueList.push_back(boost::lexical_cast<string>(entry->getStatus()));
        isFirst = false;
    }
    if (!_fieldStr.empty())
    {
        _fieldStr.append("\"").append(NUM_FIELD).append("\",");
        _fieldStr.append("\"").append(ID_FIELD).append("\",");
        _fieldStr.append("\"").append(STATUS).append("\"");
    }
}

void DMBasicAccess::GetCommitFieldNameAndValue(
    const Entries::Ptr& _data, const string& _num, map<string, vector<string>>& _field2Values)
{
    set<string> keySet;
    for (size_t i = 0; i < _data->size(); ++i)
    {
        const auto& entry = _data->get(i);
        for (auto fieldIt : *entry)
        {
            if (fieldIt.first == NUM_FIELD || fieldIt.first == "_hash_" ||
                fieldIt.first == ID_FIELD || fieldIt.first == STATUS)
            {
                continue;
            }
            keySet.insert(fieldIt.first);
        }
    }

    map<string, vector<size_t>> groupedEntries;
    map<string, uint32_t> field2Position;
    uint32_t currentFieldPostion = 0;
    for (size_t i = 0; i < _data->size(); ++i)
    {
        boost::dynamic_bitset<> keyVec(keySet.size(), 0);
        const auto& entry = _data->get(i);
        for (const auto& fieldIt : *entry)
        {
            if (fieldIt.first == NUM_FIELD || fieldIt.first == "_hash_" ||
                fieldIt.first == ID_FIELD || fieldIt.first == STATUS)
            {
                continue;
            }

            if (field2Position.find(fieldIt.first) == field2Position.end())
            {
                field2Position[fieldIt.first] = currentFieldPostion;
                keyVec[currentFieldPostion] = 1;
                ++currentFieldPostion;
            }
            else
            {
                keyVec[field2Position[fieldIt.first]] = 1;
            }
        }
        string key;
        boost::to_string(keyVec, key);
        groupedEntries[key].push_back(i);
    }

    for (const auto& it : groupedEntries)
    {
        string fieldStr;
        vector<string> valueList;
        GetCommitFieldNameAndValueEachTable(_num, _data, it.second, fieldStr, valueList);
        _field2Values[fieldStr].insert(_field2Values[fieldStr].end(),
            make_move_iterator(valueList.begin()), make_move_iterator(valueList.end()));
    }
}

int DMBasicAccess::Commit(int64_t _num, const vector<TableData::Ptr>& _datas)
{
    
    string errorMsg;
    volatile uint32_t retryCnt = 0;
    uint32_t retryMax = 10;
    volatile int ret = 0;
    TRY
    {
        ret = CommitDo(_num, _datas, errorMsg);
        while (ret < 0 && ++retryCnt < retryMax)
        {
            sleep(1);
            ret = CommitDo(_num, _datas, errorMsg);
        }
        if (ret < 0)
        {
            DMBasicAccess_LOG(ERROR) << "commit failed error message:" << errorMsg;
            return -1;
        }
    }
    ELSE
    {
        DMBasicAccess_LOG(ERROR) << "commit failed just return";
        return -1;
    }
    END_TRY;
    return ret;
}

int DMBasicAccess::CommitDo(int64_t _num, const vector<TableData::Ptr>& _datas, string& _errorMsg)
{
    string strNum = to_string(_num);
    if (_datas.size() == 0)
    {
        DMBasicAccess_LOG(DEBUG) << "empty data just return";
        return 0;
    }
    Connection_T conn = m_connPool->GetConnection();
    TRY
    {
        for (auto tableDataPtr : _datas)
        {
            if (tableDataPtr->info->name == SYS_TABLES)
            {
                for (size_t i = 0; i < tableDataPtr->newEntries->size(); ++i)
                {
                    Entry::Ptr entry = tableDataPtr->newEntries->get(i);
                    string combined = BuildCreateTableSql(entry);
                    vector<std::string> splitted;
                    string delimiter = "###";
                    size_t pos = 0;
                    string token;
                    while ((pos = combined.find(delimiter)) != std::string::npos) {
                        token = combined.substr(0, pos);
                        splitted.push_back(token);
                        combined.erase(0, pos + delimiter.length());
                    }
                    splitted.push_back(combined);
                    for (const auto& s : splitted) {
                        Connection_execute(conn, "%s", s.c_str());
                        Connection_execute(conn,"commit;");
                    }
                   
                }
            }
        }
    }
    CATCH(SQLException)
    {
        _errorMsg = Exception_frame.message;
        DMBasicAccess_LOG(ERROR) << "create table exception:" << _errorMsg;
        m_connPool->ReturnConnection(conn);
        return -1;
    }
    END_TRY;
    volatile int32_t rowCount = 0;
    m_connPool->BeginTransaction(conn);
    TRY
    {
        for (auto data : _datas)
        {
            auto tableInfo = data->info;
            string tableName = tableInfo->name;
            map<string, vector<string>> field2Values;
            vector<string> field2Type;
            this->GetCommitFieldNameAndValue(data->dirtyEntries, strNum, field2Values);
            this->GetCommitFieldNameAndValue(data->newEntries, strNum, field2Values);
            DMBasicAccess_LOG(DEBUG) << "table:" << tableName << " split to "
                                      << field2Values.size() << " parts to commit";

            tableName = boost::algorithm::replace_all_copy(tableName, "\\", "\\\\");
            tableName = boost::algorithm::replace_all_copy(tableName, "`", "\\`");
            string _sql="set identity_insert \"SYSDBA\".\"";
            _sql.append(tableName).append("\" on;");
            Connection_execute(conn,_sql.c_str());
            for (auto item : field2Values)
            {
                const auto& name = item.first;
                const auto& values = item.second;
                
                vector<DMPlaceholderItem> sqlPlaceholders =
                    this->BuildCommitSql(tableName, name, values);
                vector<string> fieldNames;
                boost::split(fieldNames, name, boost::is_any_of(","));
                auto outer_it = tableColumnType.find(tableName);
                if (outer_it == tableColumnType.end())
                {
                    for (const std::string& item : fieldNames) 
                    {
                        PreparedStatement_T _prepareStatement_select =
                        Connection_prepareStatement(conn, "select data_type from all_tab_columns where table_name=? and owner='SYSDBA' and column_name=?;");
                        auto field = boost::algorithm::replace_all_copy(item, "\"", "");
                        PreparedStatement_setString(_prepareStatement_select, 1,tableName.c_str());
                        PreparedStatement_setString(_prepareStatement_select, 2,field.c_str());
                        ResultSet_T result= PreparedStatement_executeQuery(_prepareStatement_select);
                        int32_t columnCnt = ResultSet_getColumnCount(result);
                        while (ResultSet_next(result))
                        {
                            for (int32_t index = 1; index <= columnCnt; ++index)
                            {
                                auto fieldName = ResultSet_getColumnName(result, index);
                                auto selectResult = ResultSet_getStringByName(result, fieldName);
                                tableColumnType[tableName][field]=selectResult;
                                field2Type.push_back(selectResult);
                            }      
                        }
                    }
                }else{
                    for (const std::string& item : fieldNames) 
                    {
                        auto field = boost::algorithm::replace_all_copy(item, "\"", "");
                        string checkRes=tableColumnType[tableName][field];
                        field2Type.push_back(checkRes);
                    }
                }
                uint32_t size1 = field2Type.size();
                vector<string> dealedStr;
                for (size_t i = 0; i < values.size(); ++i) {
                    if (field2Type[i%size1]=="CLOB")
                    {
                        dealedStr.push_back(base64_encode(compressString(values[i])));

                    }else{
                        dealedStr.push_back(values[i]);
                    }
                }
                string searchSql;
                auto itValue = dealedStr.begin();
                auto itValueSearch = dealedStr.begin();
                for (auto& placeholder : sqlPlaceholders)
                {
                    PreparedStatement_T preStatement =
                        Connection_prepareStatement(conn, "%s", placeholder.sql.c_str());
                    uint32_t index = 0;
                    uint32_t size = field2Type.size();
                    if (placeholder.sql.find("insert") != std::string::npos)
                    {
                         for (; itValue != values.end(); ++itValue)
                        {
                                PreparedStatement_setString(preStatement, ++index, itValue->c_str());
                            if (index == placeholder.placeholderCnt)
                            {
                                PreparedStatement_execute(preStatement);
                                rowCount += (int32_t)PreparedStatement_rowsChanged(preStatement);
                                ++itValue;
                                break;
                            }
                        }
                   
                    }
                    else
                    {
                        bool helpFlag=false;
                        for (; itValue != values.end(); ++itValue)
                        {
                         
                            if (fieldNames[index%size]=="\"_id_\"")
                            {
                                PreparedStatement_setString(preStatement,size,itValue->c_str());
                                helpFlag=true;
                                index++;
                                continue;
                            }
                            if (helpFlag)
                                {
                                    PreparedStatement_setString(preStatement, index++, itValue->c_str());
                                }else{                           
                                    PreparedStatement_setString(preStatement, ++index, itValue->c_str());                                    
                                }
                                if (index == placeholder.placeholderCnt)
                            {
                                PreparedStatement_execute(preStatement);                               
                                rowCount += (int32_t)PreparedStatement_rowsChanged(preStatement);                               
                                ++itValue;
                                break;
                            }

                        }

                    }
                }
                
            }
            
        }
        
    }
    CATCH(SQLException)
    {
        _errorMsg = Exception_frame.message;
        DMBasicAccess_LOG(ERROR) << "insert data exception:" << _errorMsg;
        DMBasicAccess_LOG(DEBUG) << "active connections:" << m_connPool->GetActiveConnections()
                                  << " max connetions:" << m_connPool->GetMaxConnections()
                                  << " now connections:" << m_connPool->GetTotalConnections();
        m_connPool->RollBack(conn);
        m_connPool->ReturnConnection(conn);
        return -1;
    }
    END_TRY;

    DMBasicAccess_LOG(INFO) << "commit now active connections:"
                             << m_connPool->GetActiveConnections()
                             << " max connections:" << m_connPool->GetMaxConnections();
    m_connPool->Commit(conn);
    Connection_execute(conn,"commit;");
    m_connPool->ReturnConnection(conn);
    return rowCount;
}

vector<DMPlaceholderItem> DMBasicAccess::BuildCommitSql(
    const string& _table, const string& _fieldStr, const vector<string>& _fieldValues)
{
    vector<string> fieldNames;
    boost::split(fieldNames, _fieldStr, boost::is_any_of(","));
    vector<DMPlaceholderItem> sqlPlaceholders;
    if (fieldNames.size() == 0 || _fieldValues.size() == 0 ||
        (_fieldValues.size() % fieldNames.size()))
    {
        /*throw exception*/
        DMBasicAccess_LOG(ERROR) << "table name:" << _table << "field size:" << fieldNames.size()
                                  << " value size:" << _fieldValues.size()
                                  << " field size and value should be greate than 0";
        THROW(SQLException, "PreparedStatement_executeQuery");
    }
    Connection_T conn = m_connPool->GetConnection();
    TRY
    {
        uint32_t placeholderCnt = 0;
        uint32_t valueSize = _fieldValues.size();//18
        uint32_t columnSize = fieldNames.size();//6
        string searchSql;
        uint32_t position;
        bool isUpdate=false;        
        for (uint32_t i=0;i<columnSize;i++)
        {
            if (fieldNames[i]=="\"_id_\"")
            {
                position=i;
                break;
            }
        }
        for (uint32_t index = 0; index < (valueSize/columnSize); index++)
        
        {              
            string sqlHeader;
            searchSql="select count(*) from \"SYSDBA\".\"";
            searchSql.append(_table).append("\" where \"_id_\"= ?;");
            PreparedStatement_T preSearchStatement=Connection_prepareStatement(conn, "%s", searchSql.c_str());
            PreparedStatement_setString(preSearchStatement, 1,_fieldValues[index*columnSize+position].c_str());
            ResultSet_T searchResult= PreparedStatement_executeQuery(preSearchStatement);
            int32_t columnCnt = ResultSet_getColumnCount(searchResult);     
            while (ResultSet_next(searchResult))
            {
                for (int32_t selectindex = 1; selectindex <= columnCnt; ++selectindex)
                {
			        auto fieldName = ResultSet_getColumnName(searchResult, selectindex);        
				    auto getColumn = ResultSet_getStringByName(searchResult, fieldName);                   
                    if ((string((char*)getColumn)=="0"))
                    {
                        isUpdate=false;
                    }else{
                        isUpdate=true;
                    }
                }
	        }
            if (!isUpdate)
            {
                sqlHeader = "insert into SYSDBA.\"";
                sqlHeader.append(_table).append("\" (");
                for (uint32_t index2= 0; index2 < fieldNames.size(); index2++)
                {
                    if (index2!=(fieldNames.size()-1))
                    {
                        
                        sqlHeader.append(fieldNames[index2]).append(",");
                    }else
                    {
                        sqlHeader.append(fieldNames[index2]).append(")");
                    }
                }
                sqlHeader.append("values (");
                for (uint32_t index3 = 0; index3 < fieldNames.size(); index3++)
                {
                    placeholderCnt++;
                    sqlHeader.append("?,");
                }
                if (!sqlHeader.empty() && sqlHeader.back() == ',') 
                {
                    sqlHeader.pop_back(); // 删除最后一个字符（逗号）
                }
                sqlHeader.append(");");
                
            }else
            {
                sqlHeader = "update  SYSDBA.\"";
                sqlHeader.append(_table).append("\" set ");
                 for (uint32_t index2= 0; index2 < fieldNames.size(); index2++)
                {
                    if (fieldNames[index2]=="\"_id_\"")
                    {
                        placeholderCnt++;
                        continue;
                    }else
                    {
                        placeholderCnt++;
                        sqlHeader.append(fieldNames[index2]).append("=? ,");
                    }
                   
                }
                if (!sqlHeader.empty() && sqlHeader.back() == ',') 
                {
                    sqlHeader.pop_back(); 
                }
                sqlHeader.append(" where \"_id_\"=?; ");
            }
            if (placeholderCnt > 0)
            {
                sqlHeader = sqlHeader.substr(0, sqlHeader.size() - 1);               
                DMPlaceholderItem item;
                item.sql = sqlHeader;
                item.placeholderCnt = placeholderCnt;               
                sqlPlaceholders.emplace_back(item);
                placeholderCnt=0;
            }
        }
    }
    CATCH(SQLException)
    {
        DMBasicAccess_LOG(DEBUG) << "active connections:" << m_connPool->GetActiveConnections()
                                  << " max connetions:" << m_connPool->GetMaxConnections()
                                  << " now connections:" << m_connPool->GetTotalConnections();
        m_connPool->ReturnConnection(conn);
    }
    END_TRY;
    m_connPool->ReturnConnection(conn);
    return sqlPlaceholders;
}

void DMBasicAccess::setConnPool(DMConnectionPool::Ptr& _connPool)
{
    this->m_connPool = _connPool;
}

void DMBasicAccess::ExecuteSql(const string& _sql)
{
    Connection_T conn = m_connPool->GetConnection();
    if (conn == NULL)
    {
        DMBasicAccess_LOG(ERROR) << "get connection failed sql:" << _sql;
        THROW(SQLException, "PreparedStatement_executeQuery");
    }

    TRY {Connection_execute(conn, "%s", _sql.c_str()); }
    CATCH(SQLException)
    {
        DMBasicAccess_LOG(ERROR) << "execute sql failed sql:" << _sql;
        m_connPool->ReturnConnection(conn);
        throw StorageException(-1, "execute sql failed sql:" + _sql);
    }
    END_TRY;
    DMBasicAccess_LOG(INFO) << "execute sql success sql:" << _sql
                             << " now active connection:" << m_connPool->GetActiveConnections()
                             << " max connection :" << m_connPool->GetMaxConnections();

    m_connPool->ReturnConnection(conn);
}
std::string base64_decode(const std::string& encoded,int size) {
    BIO *bio, *b64;
    char outbuf[size];
    std::string decoded;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new_mem_buf(encoded.c_str(), static_cast<int>(encoded.length()));
    bio = BIO_push(b64, bio);
    int bytesRead = BIO_read(bio, outbuf, encoded.length());

    decoded.assign(outbuf, bytesRead);

    BIO_free_all(bio);

    return decoded;
}
std::string base64_encode(const std::string& input) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    std::string encoded;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new(BIO_s_mem());
    BIO_push(b64, bio);
    BIO_write(b64, input.c_str(), static_cast<int>(input.length()));
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bufferPtr);

    encoded.assign(bufferPtr->data, bufferPtr->length);

    BIO_free_all(b64);

    return encoded;
}
std::string compressString(const std::string& input) {
    std::string compressed;

    z_stream zs;
    std::memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
        return "";
    }

    zs.next_in = (Bytef*)input.data();
    zs.avail_in = input.size();

    int ret;
    char outbuffer[input.size()];
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);
        if (compressed.size() < zs.total_out) {
            compressed.append(outbuffer, zs.total_out - compressed.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);
    return compressed;
}


std::string decompressString(const std::string& input, size_t compressedSize) {
    std::string decompressed;

    z_stream zs;
    std::memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK) {
        return "";
    }

    zs.next_in = (Bytef*)input.data();
    zs.avail_in = compressedSize;

    int ret;
    char outbuffer[compressedSize*20];
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, Z_FINISH);
        if (decompressed.size() < zs.total_out) {
            decompressed.append(outbuffer, zs.total_out - decompressed.size());
        }
    } while (ret == Z_OK);

    inflateEnd(&zs);

    return decompressed;
}
