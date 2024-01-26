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

uint256 BuildMerkleTree(bool* fMutated, const std::vector<uint256> leaves, std::vector<uint256> &vMerkleTree);
uint256 ComputeMerkleRoot(std::vector<uint256> hashes, bool* mutated );

uint256 rtb_calcMoM(int32_t height,int32_t MoMdepth);

struct rtb_ccdata_entry *rtb_allMoMs(int32_t *nump,uint256 *MoMoMp,int32_t kmdstarti,int32_t kmdendi);

int32_t rtb_addpair(struct rtb_ccdataMoMoM *mdata,int32_t notarized_height,int32_t offset,int32_t maxpairs);

int32_t rtb_MoMoMdata(char *hexstr,int32_t hexsize,struct rtb_ccdataMoMoM *mdata,char *symbol,int32_t kmdheight,int32_t notarized_height);

void rtb_purge_ccdata(int32_t height);
