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
 * (c) 2016-2018 fisco-dev contributors.
 */
/** @file BlockVerifier.h
 *  @author mingzhenliu
 *  @date 20180921
 */
#pragma once

#include "BlockVerifierInterface.h"
#include "ExecutiveContext.h"
#include "ExecutiveContextFactory.h"
#include "libprecompiled/Precompiled.h"
#include <libdevcore/FixedHash.h>
#include <libdevcore/ThreadPool.h>
#include <libdevcrypto/Common.h>
#include <libethcore/Block.h>
#include <libethcore/Protocol.h>
#include <libethcore/Transaction.h>
#include <libethcore/TransactionReceipt.h>
#include <libexecutive/Executive.h>
#include <libmptstate/State.h>
#include <boost/function.hpp>
#include <algorithm>
#include <memory>
#include <thread>

namespace dev
{
namespace eth
{
class TransactionReceipt;

}  // namespace eth

namespace blockverifier
{
class BlockVerifier : public BlockVerifierInterface,
                      public std::enable_shared_from_this<BlockVerifier>
{
public:
    typedef std::shared_ptr<BlockVerifier> Ptr;
    typedef boost::function<dev::h256(int64_t x)> NumberHashCallBackFunction;
    BlockVerifier(bool _enableParallel = false)
      : m_enableParallel(_enableParallel), m_orgEnableParallel(_enableParallel)
    {
        if (_enableParallel)
        {
            m_threadNum = std::max(std::thread::hardware_concurrency(), (unsigned int)1);
        }
    }

    virtual ~BlockVerifier() {}

    ExecutiveContext::Ptr executeBlock(
        dev::eth::Block& block, BlockInfo const& parentBlockInfo) override;
    ExecutiveContext::Ptr serialExecuteBlock(
        dev::eth::Block& block, BlockInfo const& parentBlockInfo);
    ExecutiveContext::Ptr parallelExecuteBlock(
        dev::eth::Block& block, BlockInfo const& parentBlockInfo);


    dev::eth::TransactionReceipt::Ptr executeTransaction(
        const dev::eth::BlockHeader& blockHeader, dev::eth::Transaction::Ptr _t) override;

    dev::eth::TransactionReceipt::Ptr execute(dev::eth::Transaction::Ptr _t,
        dev::blockverifier::ExecutiveContext::Ptr executiveContext,
        dev::executive::Executive::Ptr executive);


    void setExecutiveContextFactory(ExecutiveContextFactory::Ptr executiveContextFactory)
    {
        m_executiveContextFactory = executiveContextFactory;
    }
    ExecutiveContextFactory::Ptr getExecutiveContextFactory() { return m_executiveContextFactory; }
    void setNumberHash(const NumberHashCallBackFunction& _pNumberHash)
    {
        m_pNumberHash = _pNumberHash;
    }

    dev::executive::Executive::Ptr createAndInitExecutive(
        std::shared_ptr<executive::StateFace> _s, dev::executive::EnvInfo const& _envInfo);
    void setEvmFlags(VMFlagType const& _evmFlags) { m_evmFlags = _evmFlags; }

    void setEnableGasCharge(bool _enableGasCharge) override
    {
        m_enableGasCharge = _enableGasCharge;
        // when enableGasCharge, disable parallel
        if (true == _enableGasCharge)
        {
            m_enableParallel = false;
        }
        else
        {
            m_enableParallel = m_orgEnableParallel;
        }
        BLOCKVERIFIER_LOG(INFO) << LOG_DESC("setEnableGasCharge")
                                << LOG_KV("enableGasCharge", m_enableGasCharge)
                                << LOG_KV("enableParallel", m_enableParallel);
    }

    void setGasFreeAccounts(std::set<Address> const& _gasFreeAccounts) override
    {
        WriteGuard l(x_gasFreeAccounts);
        m_gasFreeAccounts = _gasFreeAccounts;
    }

private:
    ExecutiveContextFactory::Ptr m_executiveContextFactory;
    NumberHashCallBackFunction m_pNumberHash;
    std::atomic_bool m_enableParallel = {false};
    bool m_orgEnableParallel;
    unsigned int m_threadNum = -1;

    std::mutex m_executingMutex;
    std::atomic<int64_t> m_executingNumber = {0};

    VMFlagType m_evmFlags = 0;
    std::atomic_bool m_enableGasCharge = {false};

    mutable SharedMutex x_gasFreeAccounts;
    std::set<Address> m_gasFreeAccounts = std::set<Address>();
};

}  // namespace blockverifier

}  // namespace dev
