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
#include "uint256.h"
#include "chain.h"
#include "rtb_structs.h"
#include "rtb_utils.h"
#include "rtb_curve25519.h"

//#include "rtb_cJSON.h"
//#include "rtb_bitcoind.h"
//#include "rtb_interest.h"

// Todo:
// verify: reorgs


//#include "rtb_kv.h"
//#include "rtb_gateway.h"
//#include "rtb_events.h"
//#include "rtb_ccdata.h"
#include <cstdint>

const char RTB_STATE_FILENAME[] = "rtbevents";

int32_t rtb_parsestatefile(struct rtb_state *sp,FILE *fp,char *symbol, const char *dest);

void rtb_currentheight_set(int32_t height);

int32_t rtb_currentheight();

int32_t rtb_parsestatefiledata(struct rtb_state *sp,uint8_t *filedata,long *fposp,long datalen,const char *symbol, const char *dest);

void rtb_stateupdate(int32_t height,uint8_t notarypubs[][33],uint8_t numnotaries,uint8_t notaryid,
        uint256 txhash,uint32_t *pvals,uint8_t numpvals,int32_t KMDheight,uint32_t KMDtimestamp,
        uint64_t opretvalue,uint8_t *opretbuf,uint16_t opretlen,uint16_t vout,uint256 MoM,int32_t MoMdepth);

int32_t rtb_voutupdate(bool fJustCheck,int32_t *isratificationp,int32_t notaryid,
        uint8_t *scriptbuf,int32_t scriptlen,int32_t height,uint256 txhash,int32_t i,
        int32_t j,uint64_t *voutmaskp,int32_t *specialtxp,int32_t *notarizedheightp,
        uint64_t value,int32_t notarized,uint64_t signedmask,uint32_t timestamp);

int32_t rtb_connectblock(bool fJustCheck, CBlockIndex *pindex,CBlock& block);
