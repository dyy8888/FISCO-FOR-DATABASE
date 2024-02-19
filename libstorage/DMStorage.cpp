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
/** @file DMStorage.cpp
 *  @author darrenyin
 *  @date 2019-04-24
 */

#include "DMStorage.h"
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

DMStorage::DMStorage() {}

Entries::Ptr DMStorage::select(
    int64_t _num, TableInfo::Ptr _tableInfo, const std::string& _key, Condition::Ptr _condition)
{   
    // DMStorage_LOG(INFO)<<"执行select";
    std::vector<std::map<std::string, std::string> > values;
    int ret = 0, i = 0;
    for (i = 0; i < m_maxRetry; ++i)
    {
        ret = m_sqlBasicAcc->Select(_num, _tableInfo->name, _key, _condition, values);
        if (ret < 0)
        {
            DMStorage_LOG(ERROR) << "Remote select datdbase return error:" << ret
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
        DMStorage_LOG(ERROR) << "DM select return error: " << ret
                              << LOG_KV("table", _tableInfo->name) << LOG_KV("retry", m_maxRetry);
        auto e = StorageException(
            -1, "DM select return error:" + to_string(ret) + " table:" + _tableInfo->name);
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

void DMStorage::setConnPool(std::shared_ptr<DMConnectionPool>& _connPool)
{
    m_sqlBasicAcc->setConnPool(_connPool);
    this->initSysTables();
}

void DMStorage::SetSqlAccess(DMBasicAccess::Ptr _sqlBasicAcc)
{
    m_sqlBasicAcc = _sqlBasicAcc;
}

size_t DMStorage::commit(int64_t _num, const std::vector<TableData::Ptr>& _datas)
{
    // DMStorage_LOG(INFO)<<"执行commit";
    int32_t ret = 0;
    for (int i = 0; i < m_maxRetry; ++i)
    {
        ret = m_sqlBasicAcc->Commit((int32_t)_num, _datas);
        if (ret < 0)
        {
            DMStorage_LOG(ERROR) << "DM commit return error:" << ret << LOG_KV("retry", i + 1);
            this_thread::sleep_for(chrono::milliseconds(1000));
            continue;
        }
        else
        {
            return ret;
        }
    }
    DMStorage_LOG(ERROR) << "DM commit failed:" << ret << LOG_KV("retry", m_maxRetry);

    auto e = StorageException(-1, "DM commit return error:" + to_string(ret));
    m_fatalHandler(e);
    BOOST_THROW_EXCEPTION(e);
    return ret;
}

/*
    init system tables;
*/
void DMStorage::initSysTables()
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

std::string DMStorage::getCommonFileds()
{
    string commonFields(
        " \"_id_\" BIGINT IDENTITY(1, 1) NOT NULL,\n"
        " \"_num_\" BIGINT DEFAULT 0,\n"
        " \"_status_\" int DEFAULT 0,\n");
    DMStorage_LOG(INFO) << "getCommonFileds:" << commonFields ;
    return commonFields;
}

void DMStorage::createSysTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_TABLES << "\" (\n";
    ss << getCommonFileds();
    ss << "\"table_name\" varchar(128) DEFAULT '',\n";
    ss << "\"key_field\" varchar(1024) DEFAULT '',\n";
    ss << " \"value_field\" varchar(1024) DEFAULT '',\n";
    ss << " NOT CLUSTER PRIMARY KEY(\"_id_\"),\n";
    ss << " UNIQUE(\"table_name\")) STORAGE(ON \"MAIN\",\n";
    ss << "CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createSysTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);
    m_sqlBasicAcc->ExecuteSql("commit");
}

void DMStorage::createCnsTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_CNS << "\" (\n";
    ss << getCommonFileds();
    ss << "\"name\" varchar(128) ,\n";
    ss << "\"version\" varchar(128) ,\n";
    ss << "\"address\" varchar(256) ,\n";
    ss << "\"abi\" CLOB,\n";
    ss <<"NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createCnsTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createCnsIndex;
    createCnsIndex<<"CREATE INDEX IF NOT EXISTS \"name\" ON \"SYSDBA\".\"_sys_cns_\"(\"name\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexCnsSql=createCnsIndex.str();
    DMStorage_LOG(INFO) << "createCnsTablesIndex:" << indexCnsSql ;
    m_sqlBasicAcc->ExecuteSql(indexCnsSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createAccessTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_ACCESS_TABLE << "\" (\n";
    ss << getCommonFileds();
    ss << " \"table_name\" varchar(128),\n";
    ss << "\"address\" varchar(128),\n";
    ss << " \"enable_num\" varchar(256) ,\n";
    ss << " NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createAccessTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createTableAccessIndex;
    createTableAccessIndex<<"CREATE INDEX IF NOT EXISTS \"table_name\" ON \"SYSDBA\".\"_sys_table_access_\"(\"table_name\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexTableAccessSql=createTableAccessIndex.str();
    DMStorage_LOG(INFO) << "createAccessTablesIndex:" << indexTableAccessSql ;
    m_sqlBasicAcc->ExecuteSql(indexTableAccessSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createCurrentStateTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_CURRENT_STATE << "\" (\n";
    ss << getCommonFileds();
    ss << "\"key\" varchar(128),\n";
    ss << "\"value\" CLOB,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createCurrentStateTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"key\" ON \"SYSDBA\".\"_sys_current_state_\"(\"key\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSql=createIndex.str();
    DMStorage_LOG(INFO) << "createCurrentStateTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createNumber2HashTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_NUMBER_2_HASH << "\" (\n";
    ss << getCommonFileds();
    ss << " \"number\" varchar(128) ,\n";
    ss << " \"value\" CLOB,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createNumber2HashTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"number\" ON \"SYSDBA\".\"_sys_number_2_hash_\"(\"number\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSql=createIndex.str();
    DMStorage_LOG(INFO) << "createNumber2HashTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createTxHash2BlockTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_TX_HASH_2_BLOCK << "\" (\n";
    ss << getCommonFileds();
    ss << "\"hash\" varchar(128),\n";
    ss << "\"value\" clob,\n";
    ss << "\"index\" varchar(256),\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createTxHash2BlockTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"hash\" ON \"SYSDBA\".\"_sys_tx_hash_2_block_\"(\"hash\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSql=createIndex.str();
    DMStorage_LOG(INFO) << "createTxHash2BlockTablesIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createHash2BlockTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_HASH_2_BLOCK << "\" (\n";
    ss << getCommonFileds();
    ss << "\"hash\" varchar(128),\n";
    ss << "\"value\" clob,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createHash2BlockTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createHash2BlockIndex;
    createHash2BlockIndex<<"CREATE INDEX IF NOT EXISTS \"hash\" ON \"SYSDBA\".\"_sys_hash_2_block_\"(\"hash\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexHash2BlockSql=createHash2BlockIndex.str();
    DMStorage_LOG(INFO) << "createHash2BlockTablesIndex:" << indexHash2BlockSql ;
    m_sqlBasicAcc->ExecuteSql(indexHash2BlockSql);
    m_sqlBasicAcc->ExecuteSql("commit");

}

void DMStorage::createBlobSysHash2BlockHeaderTable()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_HASH_2_BLOCKHEADER << "\" (\n";
    ss << getCommonFileds();
    ss << "\"hash\" varchar(128) ,\n";
    ss << "\"value\" clob,\n";
    ss << "\"sigs\" clob,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createBlobSysHash2BlockHeaderTable:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createIndex;
    createIndex<<"CREATE INDEX IF NOT EXISTS \"hash\" ON \"SYSDBA\".\"_sys_hash_2_header_\"(\"hash\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSql=createIndex.str();
    DMStorage_LOG(INFO) << "createBlobSysHash2BlockHeaderTableIndex:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}


void DMStorage::createSysConsensus()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_CONSENSUS << "\" (\n";
    ss << getCommonFileds();
    ss << "\"name\" varchar(128) DEFAULT 'node',\n";
    ss << "\"type\" varchar(128) ,\n";
    ss << "\"node_id\" varchar(256) ,\n";
    ss << " \"enable_num\" varchar(256) ,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createSysConsensus:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysConsensusIndex;
    createSysConsensusIndex<<"CREATE INDEX IF NOT EXISTS \"_num_\" ON \"SYSDBA\".\"_sys_consensus_\"(\"_num_\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSysConsensusSql=createSysConsensusIndex.str();
    DMStorage_LOG(INFO) << "createSysConsensusIndex1:" << indexSysConsensusSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysConsensusSql);

    stringstream createIndex;
    createIndex<<"CREATE  INDEX IF NOT EXISTS \"name\" ON \"SYSDBA\".\"_sys_consensus_\"(\"name\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSql=createIndex.str();
    DMStorage_LOG(INFO) << "createSysConsensusIndex2:" << indexSql ;
    m_sqlBasicAcc->ExecuteSql(indexSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createSysConfigTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_CONFIG << "\" (\n";
    ss << getCommonFileds();
    ss << "\"key\" varchar(128),\n";
    ss << "\"value\" clob,\n";
    ss << "\"enable_num\" varchar(256) ,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createSysConfigTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysConfigIndex;
    createSysConfigIndex<<"CREATE INDEX IF NOT EXISTS \"key\" ON \"SYSDBA\".\"_sys_config_\"(\"key\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSysConfigSql=createSysConfigIndex.str();
    DMStorage_LOG(INFO) << "createSysConfigTablesIndex:" << indexSysConfigSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysConfigSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}
void DMStorage::createSysBlock2NoncesTables()
{
    stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS \"SYSDBA\".\"" << SYS_BLOCK_2_NONCES << "\" (\n";
    ss << getCommonFileds();
    ss << "\"number\" varchar(128)  ,\n";
    ss << "\"value\" clob,\n";
    ss << "NOT CLUSTER PRIMARY KEY(\"_id_\")) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string sql = ss.str();
    DMStorage_LOG(INFO) << "createSysBlock2NoncesTables:" << sql ;
    m_sqlBasicAcc->ExecuteSql(sql);

    stringstream createSysBlock2NoncesIndex;
    createSysBlock2NoncesIndex<<"CREATE INDEX IF NOT EXISTS \"number\" ON \"SYSDBA\".\"_sys_block_2_nonces_\"(\"number\" ASC) STORAGE(ON \"MAIN\", CLUSTERBTR) ;";
    string indexSysBlock2NoncesSql=createSysBlock2NoncesIndex.str();
    DMStorage_LOG(INFO) << "createSysBlock2NoncesTablesIndex:" << indexSysBlock2NoncesSql ;
    m_sqlBasicAcc->ExecuteSql(indexSysBlock2NoncesSql);
    m_sqlBasicAcc->ExecuteSql("commit");
}

void DMStorage::insertSysTables()
{
    stringstream ss;
    ss << "insert /*+ IGNORE_ROW_ON_DUPKEY_INDEX(\"_sys_tables_\"(\"table_name\")) */ into  \"SYSDBA\".\"" << SYS_TABLES
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
    ss << "	('" << SYS_HASH_2_BLOCKHEADER << "', 'hash','value,sigs');";
    DMStorage_LOG(INFO) << "insertSysTables:" << ss.str() ;
    string sql = ss.str();
    m_sqlBasicAcc->ExecuteSql(sql);
    string commit="commit;";
    m_sqlBasicAcc->ExecuteSql(commit);
}
