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
/** @file DMBasicAccess.h
 *  @author darrenyin
 *  @date 2019-04-24
 */


#pragma once

#include "Storage.h"
#include "Table.h"
#include <json/json.h>


// const static uint32_t maxPlaceHolderCnt = 60000;
const static uint32_t maxDMPlaceHolderCnt = 90;
#define DMBasicAccess_LOG(LEVEL) LOG(LEVEL) << "[DMBasicAccess] "
std::string base64_decode(const std::string& encoded,int size);
std::string base64_encode(const std::string& encoded);
std::string decompressString(const std::string& input, size_t compressedSize);
std::string compressString(const std::string& input);
namespace dev
{
namespace storage
{

class DMConnectionPool;
struct DMPlaceholderItem
{
    std::string sql;
    uint32_t placeholderCnt;

    DMPlaceholderItem() : placeholderCnt(0) {}
};

class DMBasicAccess
{
public:
    DMBasicAccess();
    virtual ~DMBasicAccess() {}
    typedef std::shared_ptr<DMBasicAccess> Ptr;
    virtual int Select(int64_t _num, const std::string& _table, const std::string& _key,
        Condition::Ptr _condition, std::vector<std::map<std::string, std::string>>& _values);
    virtual int Commit(int64_t _num, const std::vector<TableData::Ptr>& _datas);

private:
    std::string BuildQuerySql(std::string _table, Condition::Ptr _condition);

    std::string BuildConditionSql(const std::string& _strPrefix,
        std::map<std::string, Condition::Range>::const_iterator& _it, Condition::Ptr _condition);

    std::vector<DMPlaceholderItem> BuildCommitSql(const std::string& _table,
        const std::string& _fieldStr, const std::vector<std::string>& _fieldValue);
    
    std::string BuildCreateTableSql(const Entry::Ptr& _data);
    std::string BuildCreateTableSql(const std::string& _tableName, const std::string& _keyField,
        const std::string& _valueField);

    void GetCommitFieldNameAndValue(const Entries::Ptr& _data, const std::string& _strNum,
        std::map<std::string, std::vector<std::string>>& _fieldValue);

    void GetCommitFieldNameAndValueEachTable(const std::string& _num, const Entries::Ptr& _data,
        const std::vector<size_t>& _indexList, std::string& _fieldList,
        std::vector<std::string>& _valueList);

    int CommitDo(int64_t _num, const std::vector<TableData::Ptr>& _datas, std::string& _errorMsg);

    DMFieldType getFieldType(std::string const& _tableName);
    bool inline isBlobType(std::string const& _tableName)
    {
        auto fieldType = getFieldType(_tableName);
        if (fieldType == DMFieldType::ClobType)
        {
            return true;
        }
        return false;
    }

public:
    virtual void ExecuteSql(const std::string& _sql);
    void setConnPool(std::shared_ptr<DMConnectionPool>& _connPool);

private:
    std::shared_ptr<DMConnectionPool> m_connPool;
    std::string m_rowFormat = "";
    std::map<std::string, std::map<std::string, std::string>> tableColumnType;
};

}  // namespace storage

}  // namespace dev
