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

#include "GBaseBasicAccess.h"
#include "GBaseConnectionPool.h"
#include "StorageException.h"
#include "libconfig/GlobalConfigure.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/dynamic_bitset.hpp>
using namespace dev::storage;
using namespace std;

GBaseBasicAccess::GBaseBasicAccess(){}

int GBaseBasicAccess::Select(int64_t, const string& _table, const string&, Condition::Ptr _condition,
    vector<map<string, string>>& _values)
{
    string sql = this->BuildQuerySql(_table, _condition);
    
    Connection_T conn = m_connPool->GetConnection();
    uint32_t retryCnt = 0;
    uint32_t retryMax = 30;
    while (conn == NULL && retryCnt++ < retryMax)
    {
        GBaseBasicAccess_LOG(WARNING)
            << "table:" << _table << "sql:" << sql << " get connection failed";
        sleep(1);
        conn = m_connPool->GetConnection();
    }

    if (conn == NULL)
    {
        GBaseBasicAccess_LOG(ERROR) << "table:" << _table << "sql:" << sql
                                  << " get connection failed";
        return -1;
    }
    TRY
    {
        GBaseBasicAccess_LOG(INFO) << "select sql is : "  << sql;
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
        vector<string> fieldType;
        for (int32_t index = 1; index <= columnCnt; ++index)
        {
            auto fieldName = ResultSet_getColumnName(result, index);
            PreparedStatement_T _prepareStatement =
                Connection_prepareStatement(conn, "SELECT coltypename FROM syscolumnsext WHERE tabid=(select tabid from systables where tabname=?) AND colname=?;");
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
                        // GBaseBasicAccess_LOG(INFO) << "查询对应的字段是: "  << columnName<<" ,对应的类型是： "<<selectResult;
                        fieldType.push_back(selectResult);
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
                selectResult=fieldType[(index-1)%columnCnt];
                if (selectResult=="BLOB")
                {
                    int size;
                    auto bytes = ResultSet_getBlob(result,index, &size);
                    if (bytes)
                    {
                        GBaseBasicAccess_LOG(INFO) << "blob查询到的columnName是: "<<columnName<<" ,内容是： "<<string((char*)bytes,size)<<",内容长度是: "<<size; 
                        value[columnName] = string((char*)bytes,size);
                        
                    }
                }else{    
                    auto selectResult = ResultSet_getString(result,index);
                    if (selectResult)
                    {
                        GBaseBasicAccess_LOG(INFO) << "查询到的columnName是: "<<columnName<<",内容是: "<<selectResult; 
                        value[columnName] = selectResult;
                    }
                  
                }
                
            }
            _values.push_back(move(value));
        }
    }
    CATCH(SQLException)
    {
        m_connPool->ReturnConnection(conn);
        // Note: when select exception caused by table doesn't exist and sql error,
        //       here must return 0, in case of the DB is normal but the sql syntax error or the
        //       table will be created later
        GBaseBasicAccess_LOG(ERROR) << "select exception for sql error:" << Exception_frame.message;
        return 0;
    }
    END_TRY;
    m_connPool->ReturnConnection(conn);
    return 0;
}
string GBaseBasicAccess::BuildQuerySql(string _table, Condition::Ptr _condition)
{   
    _table = boost::algorithm::replace_all_copy(_table, "\\", "\\\\");
    _table = boost::algorithm::replace_all_copy(_table, "`", "\\`");
    string sql = "select * from ";
    sql.append(_table).append(" ");
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

string GBaseBasicAccess::BuildConditionSql(const string& _strPrefix,
    map<string, Condition::Range>::const_iterator& _it, Condition::Ptr _condition)
{
    string strConditionSql = _strPrefix;
    if (_it->second.left.second == _it->second.right.second && _it->second.left.first &&
        _it->second.right.first)
    {
        strConditionSql.append(" ").append(_it->first).append(" =").append("?");
    }
    else
    {
        if (_it->second.left.second != _condition->unlimitedField())
        {
            if (_it->second.left.first)
            {
                strConditionSql.append(" ").append(_it->first).append(" >=").append("?");
            }
            else
            {
                strConditionSql.append(" ").append(_it->first).append(" >").append("?");
            }
        }
        if (_it->second.right.second != _condition->unlimitedField())
        {
            if (_it->second.right.first)
            {
                strConditionSql.append(" ").append(_it->first).append(" <=").append("?");
            }
            else
            {
                strConditionSql.append(" ").append(_it->first).append(" <").append("?");
            }
        }
    }
    return strConditionSql;
}

GBaseFieldType GBaseBasicAccess::getFieldType(std::string const& _tableName)
{
    if (_tableName == SYS_HASH_2_BLOCK || _tableName == SYS_BLOCK_2_NONCES||_tableName == SYS_HASH_2_BLOCKHEADER )
    {
        return GBaseFieldType::BlobType;
    }
    if (_tableName==SYS_CNS||_tableName==SYS_CONFIG||_tableName==SYS_CURRENT_STATE||_tableName==SYS_NUMBER_2_HASH||_tableName==SYS_TX_HASH_2_BLOCK)
    {
        return GBaseFieldType::BlobType;
    }
    if (boost::starts_with(_tableName, string("c_")))
    {
        if (g_BCOSConfig.version() >= V2_5_0)
        {
            return GBaseFieldType::BlobType;
        }
    }
    if (g_BCOSConfig.version() >= V2_6_0)
    {
        return GBaseFieldType::BlobType;
    }
    return GBaseFieldType::BlobType;
}

string GBaseBasicAccess::BuildCreateTableSql(
    const string& _tableName, const string& _keyField, const string& _valueField)
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << _tableName << "(\n";
    ss << " _id_ BIGSERIAL NOT NULL,\n";
    ss << " _num_ BIGINT DEFAULT 0,\n";
    ss << " _status_ INT DEFAULT 0,\n";
    ss << _keyField << " varchar(255) default '',\n";
    vector<string> vecSplit;
    boost::split(vecSplit, _valueField, boost::is_any_of(","));
    auto it = vecSplit.begin();
    auto fieldType = getFieldType(_tableName);
    std::string fieldTypeName = GBaseFieldTypeName[fieldType];
    for (; it != vecSplit.end(); ++it)
    {
        *it = boost::algorithm::replace_all_copy(*it, "\\", "\\\\");
        *it = boost::algorithm::replace_all_copy(*it, "`", "\\`");
        ss << " " << *it << "  " << fieldTypeName << ",\n";
    }
    ss << " PRIMARY KEY(_id_));";
    return ss.str();
}

string GBaseBasicAccess::BuildCreateTableSql(const Entry::Ptr& _entry)
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
    indexNumSql<<"CREATE INDEX IF NOT EXISTS _num_ ON "<<tableName<<" (_num_);";
    stringstream indexKeySql;
    indexKeySql<<"CREATE INDEX IF NOT EXISTS "<<keyField<<" ON "<<tableName<<" ("<<keyField<<");";
    GBaseBasicAccess_LOG(DEBUG) << "create table:" << tableName << " keyfield:" << keyField
                              << " value field:" << valueField << " sql:" << sql;
    string combined = sql + "###" + indexNumSql.str()+"###"+indexKeySql.str();
    return combined;
}

void GBaseBasicAccess::GetCommitFieldNameAndValueEachTable(const string& _num,
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

void GBaseBasicAccess::GetCommitFieldNameAndValue(
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

int GBaseBasicAccess::Commit(int64_t _num, const vector<TableData::Ptr>& _datas)
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
            GBaseBasicAccess_LOG(ERROR) << "commit failed error message:" << errorMsg;
            return -1;
        }
    }
    ELSE
    {
        GBaseBasicAccess_LOG(ERROR) << "commit failed just return";
        return -1;
    }
    END_TRY;
    return ret;
}

int GBaseBasicAccess::CommitDo(int64_t _num, const vector<TableData::Ptr>& _datas, string& _errorMsg)
{
    
    string strNum = to_string(_num);
    if (_datas.size() == 0)
    {
        GBaseBasicAccess_LOG(DEBUG) << "empty data just return";
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
                        GBaseBasicAccess_LOG(INFO)<<"check commit sql is "<<s;
                        Connection_execute(conn, "%s", s.c_str());
                    }
                   
                }
            }
        }
    }
    CATCH(SQLException)
    {
        _errorMsg = Exception_frame.message;
        GBaseBasicAccess_LOG(ERROR) << "create table exception:" << _errorMsg;
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
            GBaseBasicAccess_LOG(DEBUG) << "table:" << tableName << " split to "
                                      << field2Values.size() << " parts to commit";

            tableName = boost::algorithm::replace_all_copy(tableName, "\\", "\\\\");
            tableName = boost::algorithm::replace_all_copy(tableName, "`", "\\`");
            for (auto item : field2Values)
            {
                const auto& name = item.first;
                const auto& values = item.second;
                
                vector<GBasePlaceholderItem> sqlPlaceholders =
                    this->BuildCommitSql(tableName, name, values);
                
                vector<string> fieldNames;
                boost::split(fieldNames, name, boost::is_any_of(","));
              
                for (const std::string& item : fieldNames) 
                {
                    PreparedStatement_T _prepareStatement_select = Connection_prepareStatement(conn, "SELECT coltypename FROM syscolumnsext WHERE tabid=(select tabid from systables where tabname=?) AND colname=?;");
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
                            GBaseBasicAccess_LOG(INFO)<<"查询的字段名称是： "<<fieldName<<"，查询的字段类型是： "<<selectResult;
                            field2Type.push_back(selectResult);
                        }      
                    }
                }
                uint32_t size1 = field2Type.size();
                string searchSql;
                auto itValue = values.begin();
                // auto itValueSearch = dealedStr.begin();
                for (auto& placeholder : sqlPlaceholders)
                {
                    GBaseBasicAccess_LOG(INFO)<<"提交的sql语句是： "<<placeholder.sql;
                    PreparedStatement_T preStatement =
                        Connection_prepareStatement(conn, "%s", placeholder.sql.c_str());
                       
                    uint32_t index = 0;
                    uint32_t size = field2Type.size();
                    if (placeholder.sql.find("insert") != std::string::npos)
                    {
                         for (; itValue != values.end(); ++itValue)
                        {
                                if (field2Type[index%size]=="BLOB"){
                                    PreparedStatement_setBlob(preStatement, ++index, itValue->c_str(),itValue->size());
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
                    else
                    {
                        bool helpFlag=false;
                        for (; itValue != values.end(); ++itValue)
                        {
                         
                            if (fieldNames[index%size]=="\"_id_\"")
                            {
                                GBaseBasicAccess_LOG(INFO)<<"当前字段是 _id_";
                                PreparedStatement_setString(preStatement,size,itValue->c_str());
                                helpFlag=true;
                                index++;
                                continue;
                            }
                            if (helpFlag)
                                {
                                    if (field2Type[index%size]=="BLOB"){
                                        PreparedStatement_setBlob(preStatement, index++, itValue->c_str(),itValue->size());
                                    }else{
                                        PreparedStatement_setBlob(preStatement, index++, itValue->c_str(),itValue->size());
                                    }
                                    
                                }else{ 
                                    if (field2Type[index%size]=="BLOB"){
                                        PreparedStatement_setBlob(preStatement, ++index, itValue->c_str(),itValue->size());
                                    }else{
                                        PreparedStatement_setBlob(preStatement, ++index, itValue->c_str(),itValue->size());
                                    }                                                        
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
            GBaseBasicAccess_LOG(INFO)<<"end end end end";
            
        }
        
    }
    CATCH(SQLException)
    {
        _errorMsg = Exception_frame.message;
        GBaseBasicAccess_LOG(ERROR) << "insert data exception:" << _errorMsg;
        GBaseBasicAccess_LOG(INFO) << "active connections:" << m_connPool->GetActiveConnections()
                                  << " max connetions:" << m_connPool->GetMaxConnections()
                                  << " now connections:" << m_connPool->GetTotalConnections();
        m_connPool->RollBack(conn);
        m_connPool->ReturnConnection(conn);
        return -1;
    }
    END_TRY;

    GBaseBasicAccess_LOG(INFO) << "commit now active connections:"
                             << m_connPool->GetActiveConnections()
                             << " max connections:" << m_connPool->GetMaxConnections();
    m_connPool->Commit(conn);
    m_connPool->ReturnConnection(conn);
    return rowCount;
}

vector<GBasePlaceholderItem> GBaseBasicAccess::BuildCommitSql(
    const string& _table, const string& _fieldStr, const vector<string>& _fieldValues)
{
    vector<string> fieldNames;
    boost::split(fieldNames, _fieldStr, boost::is_any_of(","));
    vector<GBasePlaceholderItem> sqlPlaceholders;
    if (fieldNames.size() == 0 || _fieldValues.size() == 0 ||
        (_fieldValues.size() % fieldNames.size()))
    {
        /*throw exception*/
        GBaseBasicAccess_LOG(ERROR) << "table name:" << _table << "field size:" << fieldNames.size()
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
            searchSql="select count(*) from ";
            searchSql.append(_table).append(" where _id_= ?;");
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
                sqlHeader = "insert into ";
                sqlHeader.append(_table).append(" (");
                for (uint32_t index2= 0; index2 < fieldNames.size(); index2++)
                {
                    if (index2!=(fieldNames.size()-1))
                    {
                        fieldNames[index2] = boost::algorithm::replace_all_copy(fieldNames[index2], "\"", "");
                        sqlHeader.append(fieldNames[index2]).append(",");
                    }else
                    {
                        fieldNames[index2] = boost::algorithm::replace_all_copy(fieldNames[index2], "\"", "");
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
                sqlHeader = "update  ";
                sqlHeader.append(_table).append(" set ");
                 for (uint32_t index2= 0; index2 < fieldNames.size(); index2++)
                {
                    if (fieldNames[index2]=="\"_id_\"")
                    {
                        placeholderCnt++;
                        continue;
                    }else
                    {
                        placeholderCnt++;
                        fieldNames[index2] = boost::algorithm::replace_all_copy(fieldNames[index2], "\"", "");
                        sqlHeader.append(fieldNames[index2]).append("=? ,");
                    }
                   
                }
                if (!sqlHeader.empty() && sqlHeader.back() == ',') 
                {
                    sqlHeader.pop_back(); 
                }
                sqlHeader.append(" where _id_=?; ");
            }
            if (placeholderCnt > 0)
            {
                sqlHeader = sqlHeader.substr(0, sqlHeader.size() - 1);               
                GBasePlaceholderItem item;
                item.sql = sqlHeader;
                item.placeholderCnt = placeholderCnt;               
                sqlPlaceholders.emplace_back(item);
                placeholderCnt=0;
            }
        }
    }
    CATCH(SQLException)
    {
        GBaseBasicAccess_LOG(DEBUG) << "active connections:" << m_connPool->GetActiveConnections()
                                  << " max connetions:" << m_connPool->GetMaxConnections()
                                  << " now connections:" << m_connPool->GetTotalConnections();
        m_connPool->ReturnConnection(conn);
    }
    END_TRY;
    m_connPool->ReturnConnection(conn);
    return sqlPlaceholders;
}

void GBaseBasicAccess::setConnPool(GBaseConnectionPool::Ptr& _connPool)
{
    this->m_connPool = _connPool;
}

void GBaseBasicAccess::ExecuteSql(const string& _sql)
{
    Connection_T conn = m_connPool->GetConnection();
    if (conn == NULL)
    {
        GBaseBasicAccess_LOG(ERROR) << "get connection failed sql:" << _sql;
        THROW(SQLException, "PreparedStatement_executeQuery");
    }

    TRY {Connection_execute(conn, "%s", _sql.c_str()); }
    CATCH(SQLException)
    {
        GBaseBasicAccess_LOG(ERROR) << "execute sql failed sql:" << _sql;
        m_connPool->ReturnConnection(conn);
        throw StorageException(-1, "execute sql failed sql:" + _sql);
    }
    END_TRY;
    GBaseBasicAccess_LOG(INFO) << "execute sql success sql:" << _sql
                             << " now active connection:" << m_connPool->GetActiveConnections()
                             << " max connection :" << m_connPool->GetMaxConnections();

    m_connPool->ReturnConnection(conn);
}
