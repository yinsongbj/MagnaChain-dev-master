// Copyright (c) 2016-2019 The MagnaChain Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BRANCHCHAIN_H
#define BRANCHCHAIN_H

#include <string>
#include <univalue.h>
#include <map>
#include <memory>

#include "primitives/transaction.h"

class MCBlockIndex;
class MCValidationState;
class MCBlockHeader;
class MCBlock;
class BranchCache;
class BranchData;

class MCRPCConfig {
public:
	std::string strIp;
	uint16_t    iPort;
	std::string strUser;
	std::string strPassword;
    std::string strWallet;

	void Reset();
	bool IsValid();
};

//链配置管理 
typedef std::map<std::string, MCRPCConfig> MAP_RPC_CONFIG; //
class MCBranchChainMan {
public:
	MCBranchChainMan();
	~MCBranchChainMan();
	
	void Init();
	static bool ParseRpcConfig(const std::string& strCfg, MCRPCConfig& rpccfg, std::string& branchid);
	bool GetRpcConfig(const std::string& strName, MCRPCConfig& rpccfg);
	bool CheckRpcConfig(MCRPCConfig& rpccfg);
	void ReplaceRpcConfig(const std::string& strName, MCRPCConfig& rpccfg);
private:
	MAP_RPC_CONFIG mapRpcConfig;
};

extern std::unique_ptr<MCBranchChainMan> g_branchChainMan;

enum branch_script_type
{
    BST_INVALID = 0,
    BST_MORTGAGE_MINE = 1, // 抵押
    BST_MORTGAGE_COIN = 1 << 1, // 挖矿
    BST_MORTGAGE_ALL = BST_MORTGAGE_MINE | BST_MORTGAGE_COIN,
};

UniValue CallRPC(const std::string& host, const int port, const std::string& strMethod, const UniValue& params,
	const std::string& rpcuser = "", const std::string& rpcpassword = "", const std::string& rpcwallet = "");

UniValue CallRPC(const MCRPCConfig& rpccfg, const std::string& strMethod, const UniValue& params);

void ProcessBlockBranchChain();

MCSpvProof* NewSpvProof(const MCBlock &block, const std::set<uint256>& txids);
int CheckSpvProof(const uint256& merkleRoot, MCPartialMerkleTree& pmt, const uint256 &querytxhash);
bool CheckBranchTransaction(const MCTransaction& tx, MCValidationState &state, const bool fVerifingDB, const MCTransactionRef& pFromTx);

MCAmount GetBranchChainCreateTxOut(const MCTransaction& tx);
MCAmount GetBranchChainTransOut(const MCTransaction& branchTransStep1Tx);
MCAmount GetBranchChainOut(const MCTransaction& tx);
MCAmount GetMortgageMineOut(const MCTransaction& tx, bool bWithBranchOut);
MCAmount GetMortgageCoinOut(const MCTransaction& tx, bool bWithBranchOut);

branch_script_type QuickGetBranchScriptType(const MCScript& scriptPubKey);
bool GetMortgageMineData(const MCScript& scriptPubKey, uint256* pBranchHash = nullptr, MCKeyID *pKeyID = nullptr, int64_t *pnHeight = nullptr);
bool GetMortgageCoinData(const MCScript& scriptPubKey, uint256* pFromTxid = nullptr, MCKeyID *pKeyID = nullptr, int64_t *pnHeight = nullptr);
bool GetRedeemSriptData(const MCScript& scriptPubKey, uint256* pFromTxid);

bool BranchChainTransStep2(const MCTransactionRef& tx, const MCBlock &block, std::string* pStrErrorMsg);

bool SendBranchBlockHeader(const std::shared_ptr<const MCBlock> pBlockHeader, std::string *pStrErr=nullptr);
bool CheckBranchBlockInfoTx(const MCTransaction& tx, MCValidationState& state, BranchCache* pBranchCache);
bool CheckBranchDuplicateTx(const MCTransaction& tx, MCValidationState& state, BranchCache* pBranchCache);

uint256 GetReportTxHashKey(const MCTransaction& tx);
uint256 GetProveTxHashKey(const MCTransaction& tx);

bool CheckReportCheatTx(const MCTransaction& tx, MCValidationState& state, BranchCache *pBranchCache);
bool CheckProveTx(const MCTransaction& tx, MCValidationState& state, BranchCache *pBranchCache);
bool CheckReportRewardTransaction(const MCTransaction& tx, MCValidationState& state, MCBlockIndex* pindex, BranchCache *pBranchCache);
bool CheckLockMortgageMineCoinTx(const MCTransaction& tx, MCValidationState& state);
bool CheckUnlockMortgageMineCoinTx(const MCTransaction& tx, MCValidationState& state);
bool CheckProveContractData(const MCTransaction& tx, MCValidationState& state, BranchCache *pBranchCache);

MCMutableTransaction RevertTransaction(const MCTransaction& tx, const MCTransactionRef &pFromTx, bool fDeepRevert = false);

bool ReqMainChainRedeemMortgage(const MCTransactionRef& tx, const MCBlock& block, std::string *pStrErr = nullptr);
#endif //  BRANCHCHAIN_H
