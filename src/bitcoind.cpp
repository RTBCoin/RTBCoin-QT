// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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

#include "clientversion.h"
#include "rpc/server.h"
#include "init.h"
#include "main.h"
#include "noui.h"
#include "scheduler.h"
#include "util.h"
#include "httpserver.h"
#include "httprpc.h"

#include "rtb.h"
#include "rtb_defs.h"
#include "rtb_gateway.h"
#include "rtb_bitcoind.h"
#include "rtb_gateway.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <stdio.h>

#ifdef _WIN32
#define frpintf(...)
#define printf(...)
#endif

/* Introduction text for doxygen: */

/*! \mainpage Developer documentation
 *
 * \section intro_sec Introduction
 *
 * This is the developer documentation of the reference client for an experimental new digital currency called Bitcoin (https://www.bitcoin.org/),
 * which enables instant payments to anyone, anywhere in the world. Bitcoin uses peer-to-peer technology to operate
 * with no central authority: managing transactions and issuing money are carried out collectively by the network.
 *
 * The software is a community-driven open source project, released under the MIT license.
 *
 * \section Navigation
 * Use the buttons <code>Namespaces</code>, <code>Classes</code> or <code>Files</code> at the top of the page to start navigating the code.
 */

static bool fDaemon;

int32_t rtb_longestchain();
void WaitForShutdown(boost::thread_group* threadGroup)
{
    int32_t i,height; CBlockIndex *pindex; bool fShutdown = ShutdownRequested();
    static const uint256 zeroid; //!< null uint256 constant

    // Tell the main threads to shutdown.
    if (rtb_currentheight()>RTB_EARLYTXID_HEIGHT && RTB_EARLYTXID!=zeroid && ((height=tx_height(RTB_EARLYTXID))==0 || height>RTB_EARLYTXID_HEIGHT))
    {
        LogPrintf("error: earlytx must be before block height %d or tx does not exist\n",RTB_EARLYTXID_HEIGHT);
        StartShutdown();
    }

    while (!fShutdown)
    {
        /* TODO: move to ThreadUpdateKomodoInternals */
        if ( chainName.isKMD() )
        {
            if ( RTB_NSPV_FULLNODE ) {
                rtb_update_interest();
                rtb_longestchain();
            }
            for (i=0; i<10; i++)
            {
                fShutdown = ShutdownRequested();
                if ( fShutdown != 0 )
                    break;
                MilliSleep(1000);
            }
        }
        else
        {
            /* for ACs we do nothing at present */
            for (i=0; i<=ASSETCHAINS_BLOCKTIME/5; i++)
            {
                fShutdown = ShutdownRequested();
                if ( fShutdown != 0 )
                    break;
                MilliSleep(1000);
            }
        }
        fShutdown = ShutdownRequested();
    }
    if (threadGroup)
    {
        Interrupt(*threadGroup);
        threadGroup->join_all();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Start
//

bool AppInit(int argc, char* argv[])
{
    boost::thread_group threadGroup;
    CScheduler scheduler;

    bool fRet = false;

    //
    // Parameters
    //
    // If Qt is used, parameters/rtb.conf are parsed in qt/bitcoin.cpp's main()
    ParseParameters(argc, argv);

    // Process help and version before taking care about datadir
    if (mapArgs.count("-?") || mapArgs.count("-h") ||  mapArgs.count("-help") || mapArgs.count("-version"))
    {
        std::string strUsage = _("RTB Daemon") + " " + _("version") + " " + FormatFullVersion() + "\n" + PrivacyInfo();

        if (mapArgs.count("-version"))
        {
            strUsage += LicenseInfo();
        }
        else
        {
            strUsage += "\n" + _("Usage:") + "\n" +
                  "  rtbd [options]                     " + _("Start RTB Daemon") + "\n";

            strUsage += "\n" + HelpMessage(HMM_BITCOIND);
        }

        fprintf(stdout, "%s", strUsage.c_str());
        return true;
    }

    try
    {
        // Check for -testnet or -regtest parameter (Params() calls are only valid after this clause)
        if (!SelectParamsFromCommandLine()) {
            LogPrintf("Error: Invalid combination of -regtest and -testnet.\n");
            return false;
        }
        void rtb_args(char *argv0);
        rtb_args(argv[0]);
        void chainparams_commandline();
        chainparams_commandline();

        LogPrintf("call rtb_args.(%s) NOTARY_PUBKEY.(%s)\n",argv[0],NOTARY_PUBKEY.c_str());
        LogPrintf("initialized %s at %u\n",chainName.symbol().c_str(),(uint32_t)time(NULL));
        if (!boost::filesystem::is_directory(GetDataDir(false)))
        {
            LogPrintf("Error: Specified data directory \"%s\" does not exist.\n", mapArgs["-datadir"].c_str());
            return false;
        }
        try
        {
            ReadConfigFile(mapArgs, mapMultiArgs);
        } catch (const missing_zcash_conf& e) {
            LogPrintf(
                (_("Before starting rtbd, you need to create a configuration file:\n"
                   "%s\n"
                   "It can be completely empty! That indicates you are happy with the default\n"
                   "configuration of rtbd. But requiring a configuration file to start ensures\n"
                   "that rtbd won't accidentally compromise your privacy if there was a default\n"
                   "option you needed to change.\n"
                   "\n"
                   "You can look at the example configuration file for suggestions of default\n"
                   "options that you may want to change. It should be in one of these locations,\n"
                   "depending on how you installed RTB:\n") +
                 _("- Source code:  %s\n"
                   "- .deb package: %s\n")).c_str(),
                GetConfigFile().string().c_str(),
                "contrib/debian/examples/rtb.conf",
                "/usr/share/doc/rtb/examples/rtb.conf");
            return false;
        } catch (const std::exception& e) {
            LogPrintf("Error reading configuration file: %s\n", e.what());
            return false;
        }

        // Command-line RPC
        bool fCommandLine = false;
        for (int i = 1; i < argc; i++)
            if (!IsSwitchChar(argv[i][0]) && !boost::algorithm::istarts_with(argv[i], "rtb:"))
                fCommandLine = true;

        if (fCommandLine)
        {
            LogPrintf( "Error: There is no RPC client functionality in rtbd. Use the rtb-cli utility instead.\n");
            exit(EXIT_FAILURE);
        }

#ifndef _WIN32
        fDaemon = GetBoolArg("-daemon", false);
        if (fDaemon)
        {
            LogPrintf("RTB %s server starting\n",chainName.symbol().c_str());

            // Daemonize
            pid_t pid = fork();
            if (pid < 0)
            {
                LogPrintf( "Error: fork() returned %d errno %d\n", pid, errno);
                return false;
            }
            if (pid > 0) // Parent process, pid is child process id
            {
                return true;
            }
            // Child process falls through to rest of initialization

            pid_t sid = setsid();
            if (sid < 0)
                LogPrintf( "Error: setsid() returned %d errno %d\n", sid, errno);
        }
#endif
        SoftSetBoolArg("-server", true);

        fRet = AppInit2(threadGroup, scheduler);
    }
    catch (const std::exception& e) {
        PrintExceptionContinue(&e, "AppInit()");
    } catch (...) {
        PrintExceptionContinue(NULL, "AppInit()");
    }
    if (!fRet)
    {
        Interrupt(threadGroup);
        // threadGroup.join_all(); was left out intentionally here, because we didn't re-test all of
        // the startup-failure cases to make sure they don't result in a hang due to some
        // thread-blocking-waiting-for-another-thread-during-startup case
    } else {
        WaitForShutdown(&threadGroup);
    }
    Shutdown();

    return fRet;
}

int main(int argc, char* argv[])
{
    SetupEnvironment();

    // Connect bitcoind signal handlers
    noui_connect();

    return (AppInit(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE);
}
