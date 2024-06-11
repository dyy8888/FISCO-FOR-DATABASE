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
/** @file GBaseStorage.cpp
 *  @author darrenyin
 *  @date 2019-04-24
 */

#include "GBaseStorage.h"
#include "Common.h"
#include "StorageException.h"
#include "Table.h"
#include <libdevcore/FixedHash.h>
#include <thread>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

using namespace std;
using namespace dev;
using namespace dev::storage;

GBaseStorage::GBaseStorage() {}

Entries::Ptr GBaseStorage::select(
    int64_t _num, TableInfo::Ptr _tableInfo, const std::string& _key, Condition::Ptr _condition)
{   
    std::vector<std::map<std::string, std::string> > values;
    int ret = 0, i = 0;
    for (i = 0; i < m_maxRetry; ++i)
    {
        ret = m_sqlBasicAcc->Select(_num, _tableInfo->name, _key, _condition, values);
        if (ret < 0)
        {
            GBaseStorage_LOG(ERROR) << "Remote select datdbase return error:" << ret
                                  << " table:" << _tableInfo->name << LOG_KV("retry", i + 1);
            this_thread::sleep_for(chrono::milliseconds(1000));
            continue;
        }
        else
        {
            break;
        }
    }
    if (i == m_maxRetry && ret < 0)
    {
        GBaseStorage_LOG(ERROR) << "GBase select return error: " << ret
                              << LOG_KV("table", _tableInfo->name) << LOG_KV("retry", m_maxRetry);
        auto e = StorageException(
            -1, "GBase select return error:" + to_string(ret) + " table:" + _tableInfo->name);
        m_fatalHandler(e);
        BOOST_THROW_EXCEPTION(e);
    }
    Entries::Ptr entries = std::make_shared<Entries>();
    for (auto it : values)
    {
        Entry::Ptr entry = std::make_shared<Entry>();
        for (auto it2 : it)
        {
            if (it2.first == ID_FIELD)
            {
                entry->setID(it2.second);
            }
            else if (it2.first == NUM_FIELD)
            {
                entry->setNum(it2.second);
            }
            else if (it2.first == STATUS)
            {
                entry->setStatus(it2.second);
            }
            else
            {
                GBaseStorage_LOG(INFO) << "查看first: "<<it2.first<<",查看second: "<<it2.second<<", 长度是："<<it2.second.size();
                entry->setField(it2.first, it2.second);
            }
        }
        if (entry->getStatus() == 0)
        {
            entry->setDirty(false);
            entries->addEntry(entry);
        }
    }
    entries->setDirty(false);
    return entries;
}

void GBaseStorage::setConnPool(std::shared_ptr<GBaseConnectionPool>& _connPool)
{
    m_sqlBasicAcc->setConnPool(_connPool);
    this->initSysTables();
}

void GBaseStorage::SetSqlAccess(GBaseBasicAccess::Ptr _sqlBasicAcc)
{
    m_sqlBasicAcc = _sqlBasicAcc;
}

size_t GBaseStorage::commit(int64_t _num, const std::vector<TableData::Ptr>& _datas)
{
    int32_t ret = 0;
    for (int i = 0; i < m_maxRetry; ++i)
    {
        ret = m_sqlBasicAcc->Commit((int32_t)_num, _datas);
        if (ret < 0)
        {
            GBaseStorage_LOG(ERROR) << "GBase commit return error:" << ret << LOG_KV("retry", i + 1);
            this_thread::sleep_for(chrono::milliseconds(1000));
            continue;
        }
        else
        {
            return ret;
        }
    }
    GBaseStorage_LOG(ERROR) << "GBase commit failed:" << ret << LOG_KV("retry", m_maxRetry);

    auto e = StorageException(-1, "GBase commit return error:" + to_string(ret));
    m_fatalHandler(e);
    BOOST_THROW_EXCEPTION(e);
    return ret;
}

/*
    init system tables;
*/
void GBaseStorage::initSysTables()
{
    createSysTables();
    createSysConsensus();
    createAccessTables();
    createCurrentStateTables();
    createNumber2HashTables();
    createTxHash2BlockTables();
    createCnsTables();
    createSysConfigTables();
    createHash2BlockTables();
    createSysBlock2NoncesTables();
    createBlobSysHash2BlockHeaderTable();
    insertSysTables();
}

std::string GBaseStorage::getCommonFileds()
{
    string commonFields(
        " _id_ BIGSERIAL NOT NULL,\n"
        " _num_ BIGINT DEFAULT 0,\n"
        " _status_ INT DEFAULT 0,\n");
    GBaseStorage_LOG(INFO) << "getCommonFileds:" << commonFields ;
    return commonFields;
}

void GBaseStorage::createSysTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_TABLES << " (\n";
    ss << getCommonFileds();
    ss << " table_name varchar(128) DEFAULT '',\n";
    ss << " key_field varchar(1024) DEFAULT '',\n";
    ss << " value_field varchar(1024) DEFAULT '',\n";
    ss << " PRIMARY KEY(_id_)) ;";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createSysTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);
    stringstream index;
    index<<"CREATE UNIQUE INDEX IF NOT EXISTS table_name ON "<<SYS_TABLES<<" (table_name);";
    m_sqlBasicAcc->ExecuteSql(index.str().c_str()); 
}

void GBaseStorage::createCnsTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_CNS << " (\n";
    ss << getCommonFileds();
    ss << "name varchar(128) ,\n";
    ss << "version varchar(128) ,\n";
    ss << "address varchar(256) ,\n";
    ss << "abi BLOB,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createCnsTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createCnsIndex;
    createCnsIndex<<"CREATE INDEX IF NOT EXISTS name ON "<<SYS_CNS<<" (name);";
    string indexCnsSql=createCnsIndex.str();
    GBaseStorage_LOG(INFO) << "createCnsTablesIndex:" << indexCnsSql ;
    m_sqlBasicAcc->ExecuteSql(indexCnsSql);
}
void GBaseStorage::createAccessTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_ACCESS_TABLE << " (\n";
    ss << getCommonFileds();
    ss << " table_name varchar(128),\n";
    ss << " address varchar(128),\n";
    ss << " enable_num varchar(256) ,\n";
    ss << " PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createAccessTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createTableAccessIndex;
    createTableAccessIndex<<"CREATE INDEX IF NOT EXISTS table_name ON "<<SYS_ACCESS_TABLE<<" (table_name);";
    string indexTableAccessSql=createTableAccessIndex.str();
    GBaseStorage_LOG(INFO) << "createAccessTablesIndex:" << indexTableAccessSql ;
    m_sqlBasicAcc->ExecuteSql(indexTableAccessSql);
}
void GBaseStorage::createCurrentStateTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_CURRENT_STATE << " (\n";
    ss << getCommonFileds();
    ss << "key varchar(128),\n";
    ss << "value BLOB,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createCurrentStateTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS key ON "<<SYS_CURRENT_STATE<<" (key);";
    string indexSql=createIndex.str();
    GBaseStorage_LOG(INFO) << "createCurrentStateTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void GBaseStorage::createNumber2HashTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_NUMBER_2_HASH << " (\n";
    ss << getCommonFileds();
    ss << " number varchar(128) ,\n";
    ss << " value BLOB,\n";
    ss << " PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createNumber2HashTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS number ON "<<SYS_NUMBER_2_HASH<<" (number);";
    string indexSql=createIndex.str();
    GBaseStorage_LOG(INFO) << "createNumber2HashTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void GBaseStorage::createTxHash2BlockTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_TX_HASH_2_BLOCK << " (\n";
    ss << getCommonFileds();
    ss << "hash varchar(128),\n";
    ss << "value blob,\n";
    ss << "index varchar(256),\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createTxHash2BlockTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS hash ON "<<SYS_TX_HASH_2_BLOCK<< " (hash);";
    string indexSql=createIndex.str();
    GBaseStorage_LOG(INFO) << "createTxHash2BlockTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void GBaseStorage::createHash2BlockTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_HASH_2_BLOCK << " (\n";
    ss << getCommonFileds();
    ss << "hash varchar(128),\n";
    ss << "value blob,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createHash2BlockTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createHash2BlockIndex;
    createHash2BlockIndex<<"CREATE INDEX IF NOT EXISTS hash ON "<< SYS_HASH_2_BLOCK << " (hash);";
    string indexHash2BlockSql=createHash2BlockIndex.str();
    GBaseStorage_LOG(INFO) << "createHash2BlockTablesIndex:" << indexHash2BlockSql ;
    m_sqlBasicAcc->ExecuteSql(indexHash2BlockSql);

}

void GBaseStorage::createBlobSysHash2BlockHeaderTable()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_HASH_2_BLOCKHEADER << " (\n";
    ss << getCommonFileds();
    ss << "hash varchar(128) ,\n";
    ss << "value blob,\n";
    ss << "sigs blob,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createBlobSysHash2BlockHeaderTable:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS hash ON "<<SYS_HASH_2_BLOCKHEADER<< " (hash);";
    string indexSql=createIndex.str();
    GBaseStorage_LOG(INFO) << "createBlobSysHash2BlockHeaderTableIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}


void GBaseStorage::createSysConsensus()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_CONSENSUS << " (\n";
    ss << getCommonFileds();
    ss << "name varchar(128) DEFAULT 'node',\n";
    ss << "type varchar(128) ,\n";
    ss << "node_id varchar(256) ,\n";
    ss << "enable_num varchar(256) ,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createSysConsensus:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysConsensusIndex;
    createSysConsensusIndex<<"CREATE INDEX IF NOT EXISTS _num_ ON "<< SYS_CONSENSUS<< " (_num_);";
    string indexSysConsensusSql=createSysConsensusIndex.str();
    GBaseStorage_LOG(INFO) << "createSysConsensusIndex1:" << indexSysConsensusSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysConsensusSql);

    stringstream createIndex;
    createIndex<<"CREATE  INDEX IF NOT EXISTS name ON "<<SYS_CONSENSUS<< " (name);";
    string indexSql=createIndex.str();
    GBaseStorage_LOG(INFO) << "createSysConsensusIndex2:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void GBaseStorage::createSysConfigTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_CONFIG << " (\n";
    ss << getCommonFileds();
    ss << "key varchar(128),\n";
    ss << "value blob,\n";
    ss << "enable_num varchar(256) ,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createSysConfigTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysConfigIndex;
    createSysConfigIndex<<"CREATE INDEX IF NOT EXISTS key ON "<<SYS_CONFIG<<" (key);";
    string indexSysConfigSql=createSysConfigIndex.str();
    GBaseStorage_LOG(INFO) << "createSysConfigTablesIndex:" << indexSysConfigSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysConfigSql);
}
void GBaseStorage::createSysBlock2NoncesTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << SYS_BLOCK_2_NONCES << " (\n";
    ss << getCommonFileds();
    ss << "number varchar(128)  ,\n";
    ss << "value blob,\n";
    ss << "PRIMARY KEY(_id_));";
    string sql = ss.str();
    GBaseStorage_LOG(INFO) << "createSysBlock2NoncesTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysBlock2NoncesIndex;
    createSysBlock2NoncesIndex<<"CREATE INDEX IF NOT EXISTS number ON "<<SYS_BLOCK_2_NONCES<< " (number);";
    string indexSysBlock2NoncesSql=createSysBlock2NoncesIndex.str();
    GBaseStorage_LOG(INFO) << "createSysBlock2NoncesTablesIndex:" << indexSysBlock2NoncesSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysBlock2NoncesSql);
}

void GBaseStorage::insertSysTables()
{
    stringstream ss;
    ss << "merge into " << SYS_TABLES << " as target \n";
    ss << "using( \n";
    ss << " SELECT '_sys_tables_' AS table_name, 'table_name' AS key_field, 'key_field,value_field' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_consensus_' AS table_name, 'name' AS key_field, 'type,node_id,enable_num' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_table_access_' AS table_name, 'table_name' AS key_field, 'address,enable_num' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_current_state_' AS table_name, 'key' AS key_field, 'value' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_number_2_hash_' AS table_name, 'number' AS key_field, 'value' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_tx_hash_2_block_' AS table_name, 'hash' AS key_field, 'value,index' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_hash_2_block_' AS table_name, 'hash' AS key_field, 'value' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_cns_' AS table_name, 'name' AS key_field, 'version,address,abi' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_config_' AS table_name, 'key' AS key_field, 'value,enable_num' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_block_2_nonces_' AS table_name, 'number' AS key_field, 'value' AS value_field FROM dual \n";
    ss << " union all \n";
    ss << " SELECT '_sys_hash_2_header_' AS table_name, 'hash' AS key_field, 'value,sigs' AS value_field FROM dual \n";
    ss << ") as src \n";
    ss << " on (target.table_name = src.table_name) \n";
    ss << " WHEN NOT MATCHED THEN\n";
    ss << " INSERT (table_name, key_field, value_field) \n";
    ss << " VALUES (src.table_name, src.key_field, src.value_field);";
    GBaseStorage_LOG(INFO) << "insertSysTables:" << ss.str() ;
    string sql = ss.str();
    m_sqlBasicAcc->ExecuteSql(sql);
}
