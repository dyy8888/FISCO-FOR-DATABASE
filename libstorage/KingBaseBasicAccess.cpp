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

#include "KingBaseBasicAccess.h"
#include "KingBaseConnectionPool.h"
#include "DMBasicAccess.h"
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

KingBaseBasicAccess::KingBaseBasicAccess(){}

int KingBaseBasicAccess::Select(int64_t, const string& _table, const string&, Condition::Ptr _condition,
    vector<map<string, string>>& _values)
{
    string sql = this->BuildQuerySql(_table, _condition);
    
    Connection_T conn = m_connPool->GetConnection();
    uint32_t retryCnt = 0;
    uint32_t retryMax = 30;
    while (conn == NULL && retryCnt++ < retryMax)
    {
        KingBaseBasicAccess_LOG(WARNING)
            << "table:" << _table << "sql:" << sql << " get connection failed";
        sleep(1);
        conn = m_connPool->GetConnection();
    }

    if (conn == NULL)
    {
        KingBaseBasicAccess_LOG(ERROR) << "table:" << _table << "sql:" << sql
                                  << " get connection failed";
        return -1;
    }
    TRY
    {
        PreparedStatement_T _prepareStatement =
            Connection_prepareStatement(conn, "%s", sql.c_str());
        KingBaseBasicAccess_LOG(INFO) << "查看执行的sql语句：" << sql.c_str();

        if (_condition)
        {
            uint32_t index = 0;
            for (auto& it : *(_condition))
            {
                PreparedStatement_setString(
                    _prepareStatement, ++index, it.second.right.second.c_str());
                KingBaseBasicAccess_LOG(INFO) << "查看执行的sql语句插入的参数：" << it.second.right.second.c_str();             
            }
        }
        ResultSet_T result = PreparedStatement_executeQuery(_prepareStatement);
        int32_t columnCnt = ResultSet_getColumnCount(result);
        KingBaseBasicAccess_LOG(INFO) << "查询到的行数：" << columnCnt;

        map<string, map<string, string>> tableColumnType;
        string tableName;
        tableName = boost::algorithm::replace_all_copy(_table, "\\", "\\\\");
        tableName = boost::algorithm::replace_all_copy(_table, "`", "\\`");

        for (int32_t index = 1; index <= columnCnt; ++index)
        {
            auto fieldName = ResultSet_getColumnName(result, index);
            PreparedStatement_T _prepareStatement =
                Connection_prepareStatement(conn, "SELECT data_type FROM information_schema.columns WHERE TABLE_NAME=? AND \"column_name\" = ?;");
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
                    tableColumnType[tableName][fieldName]=selectResult;
                }
	        }
        }            

        // bool tableWithBlobField = isBlobType(_table);

        while (ResultSet_next(result))
        {
            map<string, string> value;
            for (int32_t index = 1; index <= columnCnt; ++index)
            {
                auto columnName = ResultSet_getColumnName(result, index);
                string selectResult=tableColumnType[tableName][columnName];
                KingBaseBasicAccess_LOG(INFO) << "当前查询的数据类型是：" << selectResult;
                if (selectResult=="text")
                {
                    auto coded = ResultSet_getString(result, index);
                    // Note: Since the field may not be set, it must be added here to determine
                    //       whether the value of the obtained field is nullptr
                     KingBaseBasicAccess_LOG(INFO) << "当前查到的是：" << coded;
                    if (coded)
                    {
                        //  value[columnName] = coded;
                        string temp=base64_decode(string(coded),string(coded).size());
                        KingBaseBasicAccess_LOG(INFO) << "text 进行decodd是：" << temp;
                        value[columnName] =temp;
                    }
                }
                else
                {
                    auto selectResult = ResultSet_getString(result, index);
                    KingBaseBasicAccess_LOG(INFO) << "当前查到的是：" << selectResult;
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
        KingBaseBasicAccess_LOG(ERROR) << "select exception for sql error:" << Exception_frame.message;
        return 0;
    }
    END_TRY;
    m_connPool->ReturnConnection(conn);
    return 0;
}
string KingBaseBasicAccess::BuildQuerySql(string _table, Condition::Ptr _condition)
{   
    _table = boost::algorithm::replace_all_copy(_table, "\\", "\\\\");
    _table = boost::algorithm::replace_all_copy(_table, "`", "\\`");
    string sql = "select * from \"";
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

string KingBaseBasicAccess::BuildConditionSql(const string& _strPrefix,
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

KingBaseFieldType KingBaseBasicAccess::getFieldType(std::string const& _tableName)
{
    if (_tableName == SYS_HASH_2_BLOCK || _tableName == SYS_BLOCK_2_NONCES||_tableName == SYS_CNS )
    {
        return KingBaseFieldType::KLongBlobType;
    }
    if (_tableName==SYS_HASH_2_BLOCKHEADER)
    {
        return KingBaseFieldType::KLongBlobType;
    }
    if (boost::starts_with(_tableName, string("c_")))
    {
        if (g_BCOSConfig.version() >= V2_5_0)
        {
            return KingBaseFieldType::KMediumBlobType;
        }
    }
    if (g_BCOSConfig.version() >= V2_6_0)
    {
        return KingBaseFieldType::KMediumBlobType;
    }
    return KingBaseFieldType::KMediumStringType;
}

string KingBaseBasicAccess::BuildCreateTableSql(
    const string& _tableName, const string& _keyField, const string& _valueField)
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << _tableName << "\"(\n";
    ss << " \"_id_\" BIGSERIAL NOT NULL,\n";
    ss << " \"_num_\" BIGINT NOT NULL,\n";
    ss << " \"_status_\" INT NOT NULL,\n";
    ss << "\"" << _keyField << "\" varchar(255) default '',\n";
    vector<string> vecSplit;
    boost::split(vecSplit, _valueField, boost::is_any_of(","));
    auto it = vecSplit.begin();
    auto fieldType = getFieldType(_tableName);
    std::string fieldTypeName = KingBaseFieldTypeName[fieldType];
    for (; it != vecSplit.end(); ++it)
    {
        *it = boost::algorithm::replace_all_copy(*it, "\\", "\\\\");
        *it = boost::algorithm::replace_all_copy(*it, "`", "\\`");
        ss << "\"" << *it << "\" " << fieldTypeName << ",\n";
    }
    ss << " CONSTRAINT \""<<_tableName<<"_id_\" PRIMARY KEY (\"_id_\"));\n";
    
    return ss.str();
}

string KingBaseBasicAccess::BuildCreateTableSql(const Entry::Ptr& _entry)
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
    indexNumSql<<"CREATE INDEX IF NOT EXISTS \"_num_\" ON \"public\".\""<<tableName<<"\" USING BTREE (\"_num_\" ASC);";
    stringstream indexKeySql;
    indexKeySql<<"CREATE INDEX IF NOT EXISTS \""<<keyField<<"\" ON \"public\".\""<<tableName<<"\" USING BTREE (\""<<keyField<<"\" ASC);";
    KingBaseBasicAccess_LOG(DEBUG) << "create table:" << tableName << " keyfield:" << keyField
                              << " value field:" << valueField << " sql:" << sql;
    string combined = sql + "###" + indexNumSql.str()+"###"+indexKeySql.str();
    return combined;
}

void KingBaseBasicAccess::GetCommitFieldNameAndValueEachTable(const string& _num,
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

void KingBaseBasicAccess::GetCommitFieldNameAndValue(
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

int KingBaseBasicAccess::Commit(int64_t _num, const vector<TableData::Ptr>& _datas)
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
            KingBaseBasicAccess_LOG(ERROR) << "commit failed error message:" << errorMsg;
            return -1;
        }
    }
    ELSE
    {
        KingBaseBasicAccess_LOG(ERROR) << "commit failed just return";
        return -1;
    }
    END_TRY;
    return ret;
}

int KingBaseBasicAccess::CommitDo(int64_t _num, const vector<TableData::Ptr>& _datas, string& _errorMsg)
{
    string strNum = to_string(_num);
    if (_datas.size() == 0)
    {
        KingBaseBasicAccess_LOG(DEBUG) << "empty data just return";
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
                    }
                   
                }
            }
        }
    }
    CATCH(SQLException)
    {
        _errorMsg = Exception_frame.message;
        KingBaseBasicAccess_LOG(ERROR) << "create table exception:" << _errorMsg;
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
            KingBaseBasicAccess_LOG(INFO) << "table:" << tableName << " split to "
                                      << field2Values.size() << " parts to commit";

            tableName = boost::algorithm::replace_all_copy(tableName, "\\", "\\\\");
            tableName = boost::algorithm::replace_all_copy(tableName, "`", "\\`");

            auto tableWithBlobField = isBlobType(tableName);
            for (auto item : field2Values)
            {
                const auto& name = item.first;
                const auto& values = item.second;
                
                vector<KingBasePlaceholderItem> sqlPlaceholders =
                    this->BuildCommitSql(tableName, name, values);

                vector<string> fieldNames;
                boost::split(fieldNames, name, boost::is_any_of(","));
                for (const std::string& item : fieldNames) 
                {
                    PreparedStatement_T _prepareStatement_select =
                    Connection_prepareStatement(conn, "SELECT data_type FROM information_schema.columns WHERE TABLE_NAME=? AND \"column_name\" = ?;");
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
                            field2Type.push_back(selectResult);
                        }      
                    }
                }
                uint32_t size1 = field2Type.size();
                vector<string> dealedStr;
                for (size_t i = 0; i < values.size(); ++i) {
                     KingBaseBasicAccess_LOG(INFO)
                        << "当前的类型:" << field2Type[i%size1]<<" i:"<<i<<" size1:"<<size1;
                    if (field2Type[i%size1]=="text")
                    {
                        KingBaseBasicAccess_LOG(INFO)<<"base64 编码"<<values[i];
                        dealedStr.push_back(base64_encode(values[i]));

                    }else{
                        dealedStr.push_back(values[i]);
                    }
                }

                auto itValue = dealedStr.begin();
                for (auto& placeholder : sqlPlaceholders)
                {
                    PreparedStatement_T preStatement =
                        Connection_prepareStatement(conn, "%s", placeholder.sql.c_str());
                    KingBaseBasicAccess_LOG(INFO)
                        << "table:" << tableName << " sql:" << placeholder.sql;

                    uint32_t index = 0;
                    uint32_t size = field2Type.size();
                    for (; itValue != dealedStr.end(); ++itValue)
                    {
                        KingBaseBasicAccess_LOG(INFO) << " 当前类型为:" << field2Type[index%size];
                        // if (field2Type[index%size]=="text")
                        // {
                        //     KingBaseBasicAccess_LOG(INFO) << " 为text:" << base64_encode(*itValue).c_str() ;
                        //     PreparedStatement_setString(
                        //         preStatement, ++index, base64_encode(*itValue).c_str());

                                
                        //         KingBaseBasicAccess_LOG(INFO) << " index:" << index << " num:" << _num 
                        //                               <<" stringsize is :"<<itValue->size()<< "char is: "<<strlen(itValue->c_str())
                        //                               << " setBlob:" << itValue->c_str()<<" base64is dyy  : "<<base64_encode(*itValue);
                                                      
                        // }
                        // else
                        // {
                            PreparedStatement_setString(preStatement, ++index, itValue->c_str());
                            KingBaseBasicAccess_LOG(INFO) << " index:" << index << " num:" << _num
                                                      << " setString:" << itValue->c_str();
                        // }

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
    CATCH(SQLException)
    {
        _errorMsg = Exception_frame.message;
        KingBaseBasicAccess_LOG(ERROR) << "insert data exception:" << _errorMsg;
        KingBaseBasicAccess_LOG(DEBUG) << "active connections:" << m_connPool->GetActiveConnections()
                                  << " max connetions:" << m_connPool->GetMaxConnections()
                                  << " now connections:" << m_connPool->GetTotalConnections();
        m_connPool->RollBack(conn);
        m_connPool->ReturnConnection(conn);
        return -1;
    }
    END_TRY;

    KingBaseBasicAccess_LOG(INFO) << "commit now active connections:"
                             << m_connPool->GetActiveConnections()
                             << " max connections:" << m_connPool->GetMaxConnections();
    m_connPool->Commit(conn);
    m_connPool->ReturnConnection(conn);
    return rowCount;
}

vector<KingBasePlaceholderItem> KingBaseBasicAccess::BuildCommitSql(
    const string& _table, const string& _fieldStr, const vector<string>& _fieldValues)
{
    vector<string> fieldNames;
    boost::split(fieldNames, _fieldStr, boost::is_any_of(","));
    vector<KingBasePlaceholderItem> sqlPlaceholders;
    if (fieldNames.size() == 0 || _fieldValues.size() == 0 ||
        (_fieldValues.size() % fieldNames.size()))
    {
        /*throw exception*/
        KingBaseBasicAccess_LOG(ERROR) << "table name:" << _table << "field size:" << fieldNames.size()
                                  << " value size:" << _fieldValues.size()
                                  << " field size and value should be greate than 0";
        THROW(SQLException, "PreparedStatement_executeQuery");
    }
    string sqlHeader = "replace into `";
    sqlHeader.append(_table).append("`(");
    sqlHeader.append(_fieldStr);
    sqlHeader.append(") values");

    KingBaseBasicAccess_LOG(INFO) << "table name:" << _table << "field size:" << fieldNames.size()
                             << " value size:" << _fieldValues.size();

    string sql = sqlHeader;
    uint32_t placeholderCnt = 0;
    uint32_t valueSize = _fieldValues.size();
    uint32_t columnSize = fieldNames.size();
    for (uint32_t index = 0; index < valueSize; ++index)
    {
        ++placeholderCnt;
        if (index % columnSize == 0)
        {
            sql.append("(?,");
        }
        else
        {
            sql.append("?,");
        }
        if (index % columnSize == (columnSize - 1))
        {
            sql = sql.substr(0, sql.size() - 1);
            sql.append("),");
            /*
                if placeholders count is great than 65535 sql will execute failed
                so we need to execute in multiple sqls
            */
            if (placeholderCnt >= maxKingBasePlaceHolderCnt)
            {
                sql = sql.substr(0, sql.size() - 1);
                KingBasePlaceholderItem item;
                item.sql = sql;
                item.placeholderCnt = placeholderCnt;
                sqlPlaceholders.emplace_back(item);
                sql = sqlHeader;
                placeholderCnt = 0;
            }
        }
    }
    if (placeholderCnt > 0)
    {
        sql = sql.substr(0, sql.size() - 1);
        KingBasePlaceholderItem item;
        item.sql = sql;
        item.placeholderCnt = placeholderCnt;
        sqlPlaceholders.emplace_back(item);
    }
    
    return sqlPlaceholders;
}

void KingBaseBasicAccess::setConnPool(KingBaseConnectionPool::Ptr& _connPool)
{
    this->m_connPool = _connPool;
}

void KingBaseBasicAccess::ExecuteSql(const string& _sql)
{
    Connection_T conn = m_connPool->GetConnection();
    if (conn == NULL)
    {
        KingBaseBasicAccess_LOG(ERROR) << "get connection failed sql:" << _sql;
        THROW(SQLException, "PreparedStatement_executeQuery");
    }

    TRY {Connection_execute(conn, "%s", _sql.c_str()); }
    CATCH(SQLException)
    {
        KingBaseBasicAccess_LOG(ERROR) << "execute sql failed sql:" << _sql;
        m_connPool->ReturnConnection(conn);
        throw StorageException(-1, "execute sql failed sql:" + _sql);
    }
    END_TRY;
    KingBaseBasicAccess_LOG(INFO) << "execute sql success sql:" << _sql
                             << " now active connection:" << m_connPool->GetActiveConnections()
                             << " max connection :" << m_connPool->GetMaxConnections();

    m_connPool->ReturnConnection(conn);
}
bool KingBaseBasicAccess::JudgeIfCreateUnique()
{
    bool flag = false;
    Connection_T conn = m_connPool->GetConnection();
    if (conn == NULL)
    {
        KingBaseBasicAccess_LOG(ERROR) << "get connection failed in judge if create unique";
        THROW(SQLException, "PreparedStatement_executeQuery");
    }

    TRY 
    {
        
        ResultSet_T raw= Connection_executeQuery(conn,"SELECT COUNT(*) FROM \"information_schema\".\"table_constraints\" where table_name='_sys_tables_' and constraint_type='UNIQUE' and constraint_name='table_name'");
        while (ResultSet_next(raw))
        {
            int res=ResultSet_getInt(raw,1);
            if (res==0){
                flag=true;
            }
        }
    }
    CATCH(SQLException)
    {
        KingBaseBasicAccess_LOG(ERROR) << "execute sql failed in judge if create unique:" ;
        m_connPool->ReturnConnection(conn);
        throw StorageException(-1, "execute sql failed judge if create unique");
    }
    END_TRY;
    KingBaseBasicAccess_LOG(INFO) << "execute sql success judge if create unique"
                             << " now active connection:" << m_connPool->GetActiveConnections()
                             << " max connection :" << m_connPool->GetMaxConnections();

    m_connPool->ReturnConnection(conn);
    return flag;
    
}

// std::string base64_decode(const std::string& encoded,int size) {
//     BIO *bio, *b64;
//     char outbuf[size];
//     std::string decoded;

//     b64 = BIO_new(BIO_f_base64());
//     BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
//     bio = BIO_new_mem_buf(encoded.c_str(), static_cast<int>(encoded.length()));
//     bio = BIO_push(b64, bio);
//     int bytesRead = BIO_read(bio, outbuf, encoded.length());

//     decoded.assign(outbuf, bytesRead);

//     BIO_free_all(bio);

//     return decoded;
// }
// std::string base64_encode(const std::string& input) {
//     BIO *bio, *b64;
//     BUF_MEM *bufferPtr;
//     std::string encoded;

//     b64 = BIO_new(BIO_f_base64());
//     BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
//     bio = BIO_new(BIO_s_mem());
//     BIO_push(b64, bio);
//     BIO_write(b64, input.c_str(), static_cast<int>(input.length()));
//     BIO_flush(b64);
//     BIO_get_mem_ptr(b64, &bufferPtr);

//     encoded.assign(bufferPtr->data, bufferPtr->length);

//     BIO_free_all(b64);

//     return encoded;
// }
// std::string compressString(const std::string& input) {
//     std::string compressed;

//     z_stream zs;
//     std::memset(&zs, 0, sizeof(zs));

//     if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
//         return "";
//     }

//     zs.next_in = (Bytef*)input.data();
//     zs.avail_in = input.size();

//     int ret;
//     char outbuffer[input.size()];
//     do {
//         zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//         zs.avail_out = sizeof(outbuffer);

//         ret = deflate(&zs, Z_FINISH);
//         if (compressed.size() < zs.total_out) {
//             compressed.append(outbuffer, zs.total_out - compressed.size());
//         }
//     } while (ret == Z_OK);

//     deflateEnd(&zs);
//     return compressed;
// }


// std::string decompressString(const std::string& input, size_t compressedSize) {
//     std::string decompressed;

//     z_stream zs;
//     std::memset(&zs, 0, sizeof(zs));

//     if (inflateInit(&zs) != Z_OK) {
//         return "";
//     }

//     zs.next_in = (Bytef*)input.data();
//     zs.avail_in = compressedSize;

//     int ret;
//     char outbuffer[compressedSize*20];
//     do {
//         zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//         zs.avail_out = sizeof(outbuffer);

//         ret = inflate(&zs, Z_FINISH);
//         if (decompressed.size() < zs.total_out) {
//             decompressed.append(outbuffer, zs.total_out - decompressed.size());
//         }
//     } while (ret == Z_OK);

//     inflateEnd(&zs);

//     return decompressed;
// }
