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
/** @file KingBaseStorage.cpp
 *  @author darrenyin
 *  @date 2019-04-24
 */

#include "KingBaseStorage.h"
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

KingBaseStorage::KingBaseStorage() {}

Entries::Ptr KingBaseStorage::select(
    int64_t _num, TableInfo::Ptr _tableInfo, const std::string& _key, Condition::Ptr _condition)
{   
    std::vector<std::map<std::string, std::string> > values;
    int ret = 0, i = 0;
    for (i = 0; i < m_maxRetry; ++i)
    {
        ret = m_sqlBasicAcc->Select(_num, _tableInfo->name, _key, _condition, values);
        if (ret < 0)
        {
            KingBaseStorage_LOG(ERROR) << "Remote select datdbase return error:" << ret
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
        KingBaseStorage_LOG(ERROR) << "KingBase select return error: " << ret
                              << LOG_KV("table", _tableInfo->name) << LOG_KV("retry", m_maxRetry);
        auto e = StorageException(
            -1, "KingBase select return error:" + to_string(ret) + " table:" + _tableInfo->name);
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

void KingBaseStorage::setConnPool(std::shared_ptr<KingBaseConnectionPool>& _connPool)
{
    m_sqlBasicAcc->setConnPool(_connPool);
    this->initSysTables();
}

void KingBaseStorage::SetSqlAccess(KingBaseBasicAccess::Ptr _sqlBasicAcc)
{
    m_sqlBasicAcc = _sqlBasicAcc;
}

size_t KingBaseStorage::commit(int64_t _num, const std::vector<TableData::Ptr>& _datas)
{
    int32_t ret = 0;
    for (int i = 0; i < m_maxRetry; ++i)
    {
        ret = m_sqlBasicAcc->Commit((int32_t)_num, _datas);
        if (ret < 0)
        {
            KingBaseStorage_LOG(ERROR) << "KingBase commit return error:" << ret << LOG_KV("retry", i + 1);
            this_thread::sleep_for(chrono::milliseconds(1000));
            continue;
        }
        else
        {
            return ret;
        }
    }
    KingBaseStorage_LOG(ERROR) << "KingBase commit failed:" << ret << LOG_KV("retry", m_maxRetry);

    auto e = StorageException(-1, "KingBase commit return error:" + to_string(ret));
    m_fatalHandler(e);
    BOOST_THROW_EXCEPTION(e);
    return ret;
}

/*
    init system tables;
*/
void KingBaseStorage::initSysTables()
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

std::string KingBaseStorage::getCommonFileds()
{
    string commonFields(
        " \"_id_\" BIGSERIAL NOT NULL,\n"
        " \"_num_\" BIGINT DEFAULT 0,\n"
        " \"_status_\" INT DEFAULT 0,\n");
    KingBaseStorage_LOG(INFO) << "getCommonFileds:" << commonFields ;
    return commonFields;
}

void KingBaseStorage::createSysTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_TABLES << "\" (\n";
    ss << getCommonFileds();
    ss << "\"table_name\" varchar(128) DEFAULT '',\n";
    ss << "\"key_field\" varchar(1024) DEFAULT '',\n";
    ss << " \"value_field\" varchar(1024) DEFAULT '',\n";
    ss << " CONSTRAINT  \"_sys_tables_id_\" PRIMARY KEY (\"_id_\"));";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createSysTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    if (m_sqlBasicAcc -> JudgeIfCreateUnique())
    {
        stringstream unique;
	    unique<<"ALTER TABLE \"public\".\""<<SYS_TABLES<<"\" ADD CONSTRAINT \"table_name\" UNIQUE (\"table_name\")";
        m_sqlBasicAcc->ExecuteSql(unique.str());
    }    
}

void KingBaseStorage::createCnsTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_CNS << "\" (\n";
    ss << getCommonFileds();
    ss << "\"name\" varchar(128) DEFAULT NULL,\n";
    ss << "\"version\" varchar(128) DEFAULT NULL,\n";
    ss << "\"address\" varchar(256) DEFAULT NULL,\n";
    ss << "\"abi\" LONGTEXT,\n";
    ss << "CONSTRAINT \"_sys_cns_id_\" PRIMARY KEY (\"_id_\"));";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createCnsTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createCnsIndex;
    createCnsIndex<<"CREATE INDEX if not exists \"name\" ON \"public\".\""<<SYS_CNS<<"\"  USING BTREE (\"name\" ASC);";
    string indexCnsSql=createCnsIndex.str();
    KingBaseStorage_LOG(INFO) << "createCnsTablesIndex:" << indexCnsSql ;
    m_sqlBasicAcc->ExecuteSql(indexCnsSql);
}
void KingBaseStorage::createAccessTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_ACCESS_TABLE << "\" (\n";
    ss << getCommonFileds();
    ss << " \"table_name\" varchar(128) DEFAULT NULL,\n";
    ss << "\"address\" varchar(128) DEFAULT NULL,\n";
    ss << " \"enable_num\" varchar(256) DEFAULT NULL,\n";
    ss << " CONSTRAINT \"_sys_table_access_id_\" PRIMARY KEY (\"_id_\"));";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createAccessTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createTableAccessIndex;
    createTableAccessIndex<<"CREATE INDEX IF NOT EXISTS \"table_name\" ON \"public\".\""<< SYS_ACCESS_TABLE <<"\" USING BTREE (\"table_name\" ASC);";
    string indexTableAccessSql=createTableAccessIndex.str();
    KingBaseStorage_LOG(INFO) << "createAccessTablesIndex:" << indexTableAccessSql ;
    m_sqlBasicAcc->ExecuteSql(indexTableAccessSql);
}
void KingBaseStorage::createCurrentStateTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_CURRENT_STATE << "\" (\n";
    ss << getCommonFileds();
    ss << "\"key\" varchar(128) DEFAULT NULL,\n";
    ss << "\"value\" LONGTEXT,\n";
    ss << "CONSTRAINT \"_sys_current_state_id_\" PRIMARY KEY (\"_id_\"));";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createCurrentStateTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"key\" ON \"public\".\""<<SYS_CURRENT_STATE<<"\" USING BTREE (\"key\" ASC);";
    string indexSql=createIndex.str();
    KingBaseStorage_LOG(INFO) << "createCurrentStateTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void KingBaseStorage::createNumber2HashTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_NUMBER_2_HASH << "\" (\n";
    ss << getCommonFileds();
    ss << " \"number\" varchar(128) DEFAULT NULL,\n";
    ss << " \"value\" LONGTEXT,\n";
    ss << "CONSTRAINT \"_sys_number_2_hash_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createNumber2HashTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"number\" ON \"public\".\""<< SYS_NUMBER_2_HASH <<"\" USING BTREE (\"number\" ASC);";
    string indexSql=createIndex.str();
    KingBaseStorage_LOG(INFO) << "createNumber2HashTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void KingBaseStorage::createTxHash2BlockTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_TX_HASH_2_BLOCK << "\" (\n";
    ss << getCommonFileds();
    ss << "\"hash\" varchar(128) DEFAULT NULL,\n";
    ss << "\"value\" LONGTEXT,\n";
    ss << "\"index\" varchar(256) DEFAULT NULL,\n";
    ss << "CONSTRAINT \"_sys_tx_hash_2_block_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createTxHash2BlockTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"hash\" ON \"public\".\""<<SYS_TX_HASH_2_BLOCK<<"\" USING BTREE (\"hash\" ASC);";
    string indexSql=createIndex.str();
    KingBaseStorage_LOG(INFO) << "createTxHash2BlockTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void KingBaseStorage::createHash2BlockTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_HASH_2_BLOCK << "\" (\n";
    ss << getCommonFileds();
    ss << "\"hash\" varchar(128) DEFAULT NULL,\n";
    ss << "\"value\" LONGTEXT,\n";
    ss << "CONSTRAINT \"_sys_hash_2_block_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createHash2BlockTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createHash2BlockIndex;
    createHash2BlockIndex<<"CREATE INDEX IF NOT EXISTS \"hash\" ON \"public\".\""<<SYS_HASH_2_BLOCK<<"\" USING BTREE (\"hash\" ASC);";
    string indexHash2BlockSql=createHash2BlockIndex.str();
    KingBaseStorage_LOG(INFO) << "createHash2BlockTablesIndex:" << indexHash2BlockSql ;
    m_sqlBasicAcc->ExecuteSql(indexHash2BlockSql);
}

void KingBaseStorage::createBlobSysHash2BlockHeaderTable()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_HASH_2_BLOCKHEADER << "\" (\n";
    ss << getCommonFileds();
    ss << "\"hash\" varchar(128) DEFAULT NULL,\n";
    ss << "\"value\" LONGTEXT,\n";
    ss << "\"sigs\" LONGTEXT,\n";
    ss << "CONSTRAINT \"_sys_hash_2_header_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createBlobSysHash2BlockHeaderTable:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"hash\" ON \"public\".\""<<SYS_HASH_2_BLOCKHEADER<<"\" USING BTREE (\"hash\" ASC);";
    string indexSql=createIndex.str();
    KingBaseStorage_LOG(INFO) << "createBlobSysHash2BlockHeaderTableIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}


void KingBaseStorage::createSysConsensus()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_CONSENSUS << "\" (\n";
    ss << getCommonFileds();
    ss << "\"name\" varchar(128) DEFAULT 'node',\n";
    ss << "\"type\" varchar(128) DEFAULT NULL,\n";
    ss << "\"node_id\" varchar(256) DEFAULT NULL,\n";
    ss << " \"enable_num\" varchar(256) DEFAULT NULL,\n";
    ss << "CONSTRAINT \"_sys_consensus_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createSysConsensus:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysConsensusIndex;
    createSysConsensusIndex<<"CREATE INDEX IF NOT EXISTS \"_num_\" ON \"public\".\""<<SYS_CONSENSUS<<"\" USING BTREE (\"_num_\" ASC);";
    string indexSysConsensusSql=createSysConsensusIndex.str();
    KingBaseStorage_LOG(INFO) << "createSysConsensusIndex1:" << indexSysConsensusSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysConsensusSql);

    stringstream createIndex;
    createIndex<<"CREATE  INDEX IF NOT EXISTS \"name\" ON \"public\".\""<<SYS_CONSENSUS<<"\" USING BTREE (\"name\" ASC);";
    string indexSql=createIndex.str();
    KingBaseStorage_LOG(INFO) << "createSysConsensusIndex2:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
}
void KingBaseStorage::createSysConfigTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_CONFIG << "\" (\n";
    ss << getCommonFileds();
    ss << "\"key\" varchar(128) DEFAULT NULL,\n";
    ss << "\"value\" LONGTEXT,\n";
    ss << "\"enable_num\" varchar(256) DEFAULT NULL,\n";
    ss << "CONSTRAINT \"_sys_config_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createSysConfigTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysConfigIndex;
    createSysConfigIndex<<"CREATE INDEX IF NOT EXISTS \"key\" ON \"public\".\""<<SYS_CONFIG<<"\" USING BTREE (\"key\" ASC);";
    string indexSysConfigSql=createSysConfigIndex.str();
    KingBaseStorage_LOG(INFO) << "createSysConfigTablesIndex:" << indexSysConfigSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysConfigSql);
}
void KingBaseStorage::createSysBlock2NoncesTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"public\".\"" << SYS_BLOCK_2_NONCES << "\" (\n";
    ss << getCommonFileds();
    ss << "\"number\" varchar(128) DEFAULT NULL,\n";
    ss << "\"value\" LONGTEXT,\n";
    ss << "CONSTRAINT \"_sys_block_2_nonces_id_\" PRIMARY KEY (\"_id_\")) ;";
    string sql = ss.str();
    KingBaseStorage_LOG(INFO) << "createSysBlock2NoncesTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysBlock2NoncesIndex;
    createSysBlock2NoncesIndex<<"CREATE INDEX IF NOT EXISTS \"number\" ON \"public\".\""<<SYS_BLOCK_2_NONCES<<"\" USING BTREE (\"number\" ASC);";
    string indexSysBlock2NoncesSql=createSysBlock2NoncesIndex.str();
    KingBaseStorage_LOG(INFO) << "createSysBlock2NoncesTablesIndex:" << indexSysBlock2NoncesSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysBlock2NoncesSql);
}

void KingBaseStorage::insertSysTables()
{
    stringstream ss;
    ss << "insert into \"public\".\"" << SYS_TABLES
       << "\" (\"table_name\",\"key_field\",\"value_field\")VALUES\n";
    ss << "	('" << SYS_TABLES << "', 'table_name','key_field,value_field'),\n";
    ss << "	('" << SYS_CONSENSUS << "', 'name','type,node_id,enable_num'),\n";
    ss << "	('" << SYS_ACCESS_TABLE << "', 'table_name','address,enable_num'),\n";
    ss << "	('" << SYS_CURRENT_STATE << "', 'key','value'),\n";
    ss << "	('" << SYS_NUMBER_2_HASH << "', 'number','value'),\n";
    ss << "	('" << SYS_TX_HASH_2_BLOCK << "', 'hash','value,index'),\n";
    ss << "	('" << SYS_HASH_2_BLOCK << "', 'hash','value'),\n";
    ss << "	('" << SYS_CNS << "', 'name','version,address,abi'),\n";
    ss << "	('" << SYS_CONFIG << "', 'key','value,enable_num'),\n";
    ss << "	('" << SYS_BLOCK_2_NONCES << "', 'number','value'),\n";
    ss << "	('" << SYS_HASH_2_BLOCKHEADER << "', 'hash','value,sigs') on conflict(table_name) do nothing;";
    KingBaseStorage_LOG(INFO) << "insertSysTables:" << ss.str() ;
    string sql = ss.str();
    m_sqlBasicAcc->ExecuteSql(sql);
}
