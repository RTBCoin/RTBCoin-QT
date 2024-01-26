/******************************************************************************
 * Copyright © 2014-2019 The SuperNET Developers.                             *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/
#pragma once
// rtb functions that interact with bitcoind C++

#include <curl/curl.h>
#include <curl/easy.h>
#include "consensus/params.h"
#include "rtb_defs.h"
#include "script/standard.h"
#include "cc/CCinclude.h"
#include "rtb_globals.h"

bool EnsureWalletIsAvailable(bool avoidException);

uint32_t rtb_heightstamp(int32_t height);

struct MemoryStruct { char *memory; size_t size; };
struct return_string { char *ptr; size_t len; };

// return data from the server
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32)
#define CURL_GLOBAL_SSL (1<<0)
#define CURL_GLOBAL_WIN32 (1<<1)


/************************************************************************
 *
 * Initialize the string handler so that it is thread safe
 *
 ************************************************************************/

void init_string(struct return_string *s);

int tx_height( const uint256 &hash );


/************************************************************************
 *
 * Use the "writer" to accumulate text until done
 *
 ************************************************************************/

size_t accumulatebytes(void *ptr,size_t size,size_t nmemb,struct return_string *s);

/************************************************************************
 *
 * return the current system time in milliseconds
 *
 ************************************************************************/

#define EXTRACT_BITCOIND_RESULT  // if defined, ensures error is null and returns the "result" field
#ifdef EXTRACT_BITCOIND_RESULT

/************************************************************************
 *
 * perform post processing of the results
 *
 ************************************************************************/

char *post_process_bitcoind_RPC(char *debugstr,char *command,char *rpcstr,char *params);
#endif

/************************************************************************
 *
 * perform the query
 *
 ************************************************************************/

char *bitcoind_RPC(char **retstrp,char *debugstr,char *url,char *userpass,char *command,char *params);

static size_t WriteMemoryCallback(void *ptr,size_t size,size_t nmemb,void *data);

char *curl_post(CURL **cHandlep,char *url,char *userpass,char *postfields,char *hdr0,char *hdr1,char *hdr2,char *hdr3);

char *rtb_issuemethod(char *userpass,char *method,char *params,uint16_t port);

int32_t notarizedtxid_height(char *dest,char *txidstr,int32_t *kmdnotarized_heightp);

int32_t rtb_verifynotarizedscript(int32_t height,uint8_t *script,int32_t len,uint256 NOTARIZED_HASH);

void rtb_reconsiderblock(uint256 blockhash);

int32_t rtb_verifynotarization(const char *symbol, const char *dest,int32_t height,int32_t NOTARIZED_HEIGHT,uint256 NOTARIZED_HASH,uint256 NOTARIZED_DESTTXID);

CScript rtb_makeopret(CBlock *pblock, bool fNew);

uint64_t rtb_seed(int32_t height);

uint32_t rtb_txtime(CScript &opret,uint64_t *valuep,uint256 hash, int32_t n, char *destaddr);

CBlockIndex *rtb_getblockindex(uint256 hash);

uint32_t rtb_txtime2(uint64_t *valuep,uint256 hash,int32_t n,char *destaddr);

CScript EncodeStakingOpRet(uint256 merkleroot);

uint8_t DecodeStakingOpRet(CScript scriptPubKey, uint256 &merkleroot);

int32_t rtb_newStakerActive(int32_t height, uint32_t timestamp);

int32_t rtb_hasOpRet(int32_t height, uint32_t timestamp);

bool rtb_checkopret(CBlock *pblock, CScript &merkleroot);

bool rtb_hardfork_active(uint32_t time);

uint256 rtb_calcmerkleroot(CBlock *pblock, uint256 prevBlockHash, int32_t nHeight, bool fNew, CScript scriptPubKey);

int32_t rtb_isPoS(CBlock *pblock, int32_t height,CTxDestination *addressout);

int32_t rtb_is_notarytx(const CTransaction& tx);

int32_t rtb_block2height(CBlock *block);

bool rtb_block2pubkey33(uint8_t *pubkey33,CBlock *block);

int32_t rtb_blockload(CBlock& block,CBlockIndex *pindex);

uint32_t rtb_chainactive_timestamp();

CBlockIndex *rtb_chainactive(int32_t height);

void rtb_index2pubkey33(uint8_t *pubkey33,CBlockIndex *pindex,int32_t height);

int32_t rtb_eligiblenotary(uint8_t pubkeys[66][33],int32_t *mids,uint32_t blocktimes[66],int32_t *nonzpkeysp,int32_t height);

int32_t rtb_minerids(uint8_t *minerids,int32_t height,int32_t width);

int32_t rtb_is_special(uint8_t pubkeys[66][33],int32_t mids[66],uint32_t blocktimes[66],int32_t height,uint8_t pubkey33[33],uint32_t blocktime);

int32_t rtb_MoM(int32_t *notarized_heightp,uint256 *MoMp,uint256 *kmdtxidp,int32_t nHeight,uint256 *MoMoMp,int32_t *MoMoMoffsetp,int32_t *MoMoMdepthp,int32_t *kmdstartip,int32_t *kmdendip);

CBlockIndex *rtb_blockindex(uint256 hash);

int32_t rtb_blockheight(uint256 hash);

uint32_t rtb_blocktime(uint256 hash);

/******
 * @brief Verify that a height and hash match the most recent (based on height) notarized_checkpoint
 * @param[out] notarized_heightp the notarized height found
 * @param[in] nHeight the height that should be greater than the notarized height
 * @param[in] hash the hash that should match the notarized hash
 * @returns true on success
 */
bool rtb_checkpoint(int32_t *notarized_heightp,int32_t nHeight,uint256 hash);

int32_t rtb_nextheight();

int32_t rtb_isrealtime(int32_t *kmdheightp);

/*******
 * @brief validate interest in processing a transaction
 * @param tx the transaction
 * @param txheight the desired chain height to evaluate
 * @param cmptime the block time (often the median block time of a chunk of recent blocks)
 * @returns true if tx seems okay, false if tx has been in mempool too long (currently an hour + some)
 */
bool rtb_validate_interest(const CTransaction &tx,int32_t txheight,uint32_t cmptime);

/*
 rtb_checkPOW (fast) is called early in the process and should only refer to data immediately available. it is a filter to prevent bad blocks from going into the local DB. The more blocks we can filter out at this stage, the less junk in the local DB that will just get purged later on.

 rtb_checkPOW (slow) is called right before connecting blocks so all prior blocks can be assumed to be there and all checks must pass

 commission must be in coinbase.vout[1] and must be >= 10000 sats
 PoS stake must be without txfee and in the last tx in the block at vout[0]
 */

uint64_t rtb_commission(const CBlock *pblock,int32_t height);

uint32_t rtb_segid32(char *coinaddr);

int8_t rtb_segid(int32_t nocache,int32_t height);

void rtb_segids(uint8_t *hashbuf,int32_t height,int32_t n);

uint32_t rtb_stakehash(uint256 *hashp,char *address,uint8_t *hashbuf,uint256 txid,int32_t vout);

arith_uint256 rtb_PoWtarget(int32_t *percPoSp,arith_uint256 target,int32_t height,int32_t goalperc,int32_t newStakerActive);

uint32_t rtb_stake(int32_t validateflag,arith_uint256 bnTarget,int32_t nHeight,uint256 txid,int32_t vout,uint32_t blocktime,uint32_t prevtime,char *destaddr,int32_t PoSperc);

int32_t rtb_is_PoSblock(int32_t slowflag,int32_t height,CBlock *pblock,arith_uint256 bnTarget,arith_uint256 bhash);

// for now, we will ignore slowFlag in the interest of keeping success/fail simpler for security purposes

uint64_t rtb_notarypayamount(int32_t nHeight, int64_t notarycount);

int32_t rtb_getnotarizedheight(uint32_t timestamp,int32_t height, uint8_t *script, int32_t len);

uint64_t rtb_notarypay(CMutableTransaction &txNew, std::vector<int8_t> &NotarisationNotaries, uint32_t timestamp, int32_t height, uint8_t *script, int32_t len);

bool GetNotarisationNotaries(uint8_t notarypubkeys[64][33], int8_t &numNN, const std::vector<CTxIn> &vin, std::vector<int8_t> &NotarisationNotaries);

uint64_t rtb_checknotarypay(CBlock *pblock,int32_t height);

bool rtb_appendACscriptpub();

void GetKomodoEarlytxidScriptPub();

int64_t rtb_checkcommission(CBlock *pblock,int32_t height);

int32_t rtb_checkPOW(int64_t stakeTxValue, int32_t slowflag,CBlock *pblock,int32_t height);

int32_t rtb_acpublic(uint32_t tiptime);

int64_t rtb_newcoins(int64_t *zfundsp,int64_t *sproutfundsp,int32_t nHeight,CBlock *pblock);

int64_t rtb_coinsupply(int64_t *zfundsp,int64_t *sproutfundsp,int32_t height);

struct rtb_staking
{
    char address[64];
    uint256 txid;
    arith_uint256 hashval;
    uint64_t nValue;
    uint32_t segid32,txtime;
    int32_t vout;
    CScript scriptPubKey;
};

void rtb_addutxo(std::vector<rtb_staking> &array,uint32_t txtime,uint64_t nValue,uint256 txid,int32_t vout,char *address,uint8_t *hashbuf,CScript pk);

int32_t rtb_staked(CMutableTransaction &txNew,uint32_t nBits,uint32_t *blocktimep,uint32_t *txtimep,uint256 *utxotxidp,int32_t *utxovoutp,uint64_t *utxovaluep,uint8_t *utxosig, uint256 merkleroot);
