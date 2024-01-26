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
#include "rtb_defs.h"
#include "rtb_structs.h"

void rtb_eventadd_notarized(rtb_state *sp, const char *symbol,int32_t height, rtb::event_notarized& ntz);

void rtb_eventadd_pubkeys(rtb_state *sp, const char *symbol,int32_t height, rtb::event_pubkeys& pk);

void rtb_eventadd_pricefeed(rtb_state *sp, const char *symbol,int32_t height, rtb::event_pricefeed& pf);

void rtb_eventadd_opreturn(rtb_state *sp, const char *symbol,int32_t height, rtb::event_opreturn& opret);

void rtb_eventadd_kmdheight(rtb_state *sp, const char *symbol,int32_t height, rtb::event_kmdheight& kmd_ht);

void rtb_event_rewind(rtb_state *sp, const char *symbol,int32_t height);

void rtb_setkmdheight(rtb_state *sp,int32_t kmdheight,uint32_t timestamp);
