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
/** @file GBaseConnectionPool.cpp
 *  @author darrenyin
 *  @date 2019-04-25
 */

#include "GBaseConnectionPool.h"
#include "StorageException.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <csignal>
#include <memory>

using namespace dev::storage;
using namespace std;

bool GBaseConnectionPool::InitConnectionPool(const storage::GBaseConnectionPoolConfig& _dbConfig)
{
    if (_dbConfig.dbType == "gbase")
    {   //用于进行字符串的输入和输出操作
        stringstream ss;
        // Note: [auth-plugin=mysql_native_password] only appliable for mysql-connector with version
        // no smaller than 8.0
        //       in current period, this param is useless since mysql-connector-c has not been
        //       upgraded, we can only configure this option in mysql server and configure ssl=0
        //       when using mysql 8.x
        ss << "oracle://" << _dbConfig.dbIP << ":" << _dbConfig.dbPort
           << "/"<< _dbConfig.dbName <<"?user=" << _dbConfig.dbUsername
           << "&password=" << _dbConfig.dbPasswd;
        //将ss转换为c风格的然后调用URL_new
        m_url = URL_new(ss.str().c_str());
        if (m_url == NULL)
        {
            stringstream exitInfo;
            exitInfo << "parse IP[" << _dbConfig.dbIP << ":" << _dbConfig.dbPort
                     << "] error please check";
            errorExitOut(exitInfo);
        }
        GBaseConnectionPool_LOG(DEBUG)
            << "init connection pool IP:" << _dbConfig.dbIP << ":" << _dbConfig.dbPort;

        TRY
        {
            m_connectionPool = ConnectionPool_new(m_url);
            ConnectionPool_setInitialConnections(m_connectionPool, _dbConfig.initConnections);
            ConnectionPool_setMaxConnections(m_connectionPool, _dbConfig.maxConnections);
            ConnectionPool_setConnectionTimeout(m_connectionPool, 28800);
            ConnectionPool_setReaper(m_connectionPool, 10);
            ConnectionPool_start(m_connectionPool);
        }
        CATCH(SQLException)
        {
            GBaseConnectionPool_LOG(ERROR)
                << "init connection pool failed IP:" << _dbConfig.dbIP << ":" << _dbConfig.dbPort
                << " error msg:" << Exception_frame.message;
            stringstream exitInfo;
            exitInfo << "init connection pool failed IP:" << _dbConfig.dbIP << ":"
                     << _dbConfig.dbPort << " error msg:" << Exception_frame.message;
            errorExitOut(exitInfo);
        }
        END_TRY;
    }
    else
    {
        stringstream exitInfo;
        exitInfo << "not support db type:" << _dbConfig.dbType;
        errorExitOut(exitInfo);
    }
    return true;
}


/*
    this function is used to obtain a new connection from the pool,
    If there are no connections available, a new connection is created
    and returned. If the pool has already handed out maxConnections,
    this call will return NULL
*/
Connection_T GBaseConnectionPool::GetConnection()
{
    return ConnectionPool_getConnection(m_connectionPool);
}

/*
    Returns a connection to the pool
*/
int GBaseConnectionPool::ReturnConnection(const Connection_T& _connection)
{
    ConnectionPool_returnConnection(m_connectionPool, _connection);
    return 0;
}

int GBaseConnectionPool::BeginTransaction(const Connection_T& _connection)
{
    Connection_beginTransaction(_connection);
    return 0;
}

int GBaseConnectionPool::Commit(const Connection_T& _connection)
{
    Connection_commit(_connection);
    return 0;
}

int GBaseConnectionPool::RollBack(const Connection_T& _connection)
{
    Connection_rollback(_connection);
    return 0;
}

inline void dev::storage::errorExitOut(std::stringstream& _exitInfo)
{
    GBaseConnectionPool_LOG(ERROR) << _exitInfo.str();
    raise(SIGTERM);
    BOOST_THROW_EXCEPTION(StorageException(-1, _exitInfo.str()));
}
//析构函数
GBaseConnectionPool::~GBaseConnectionPool()
{
    if (m_connectionPool)
    {
        ConnectionPool_stop(m_connectionPool);
        ConnectionPool_free(&m_connectionPool);
    }
    if (m_url)
    {
        URL_free(&m_url);
    }
}

int GBaseConnectionPool::GetActiveConnections()
{
    return ConnectionPool_active(m_connectionPool);
}

int GBaseConnectionPool::GetMaxConnections()
{
    return ConnectionPool_getMaxConnections(m_connectionPool);
}

int GBaseConnectionPool::GetTotalConnections()
{
    return ConnectionPool_size(m_connectionPool);
}

void GBaseConnectionPool::createDataBase(const GBaseConnectionPoolConfig& _dbConfig)
{
    if (_dbConfig.dbType == "gbase")
    {
        stringstream ss;
        // Note: [auth-plugin=mysql_native_password] only appliable for mysql-connector with version
        // no smaller than 8.0
        //       in current period, this param is useless since mysql-connector-c has not been
        //       upgraded, we can only configure this option in mysql server and configure ssl=0
        //       when using mysql 8.x
        //URL:=oracle://101.201.81.164:5236/?user=SYSDBA&password=Dameng111
        ss << "oracle://" << _dbConfig.dbIP << ":" << _dbConfig.dbPort
           << "/"<< _dbConfig.dbName <<"?user=" << _dbConfig.dbUsername
           << "&password=" << _dbConfig.dbPasswd;
        URL_T url = URL_new(ss.str().c_str());
        if (url == NULL)
        {
            stringstream _exitInfo;
            _exitInfo << "parse url[" << _dbConfig.dbIP << ":" << _dbConfig.dbPort
                      << "] error please check";
            errorExitOut(_exitInfo);
        }
        GBaseConnectionPool_LOG(DEBUG)
            << "init connection pool  IP:" << _dbConfig.dbIP << ":" << _dbConfig.dbPort;
        ConnectionPool_T _connectionPool = nullptr;
        volatile Connection_T _connection = nullptr;

        TRY
        {
            _connectionPool = ConnectionPool_new(url);
            ConnectionPool_setInitialConnections(_connectionPool, 2);
            ConnectionPool_setMaxConnections(_connectionPool, 2);
            ConnectionPool_start(_connectionPool);
            _connection = ConnectionPool_getConnection(_connectionPool);
            if (_connection == nullptr)
            {
                THROW(SQLException, "create database get connection failed");
            }
        }
        CATCH(SQLException)
        {
            if (_connection)
            {
                ConnectionPool_returnConnection(_connectionPool, _connection);
            }
            ConnectionPool_stop(_connectionPool);
            ConnectionPool_free(&_connectionPool);
            URL_free(&url);
            GBaseConnectionPool_LOG(ERROR)
                << "init connection pool failed url:" << _dbConfig.dbIP << ":" << _dbConfig.dbPort
                << " error msg:" << Exception_frame.message;
            stringstream _exitInfo;
            _exitInfo << "init connection pool failed url:" << _dbConfig.dbIP << ":"
                      << _dbConfig.dbPort << " error msg:" << Exception_frame.message;
            errorExitOut(_exitInfo);
        }


        END_TRY;
        ConnectionPool_returnConnection(_connectionPool, _connection);
        ConnectionPool_stop(_connectionPool);
        ConnectionPool_free(&_connectionPool);
        URL_free(&url);
    }
    else
    {
        stringstream exitInfo;
        exitInfo << "not support db type:" << _dbConfig.dbType;
        errorExitOut(exitInfo);
    }
}
