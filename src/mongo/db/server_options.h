/*    Copyright 2013 10gen Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects
 *    for all of the code used other than as permitted herein. If you modify
 *    file(s) with this exception, you may extend this exception to your
 *    version of the file(s), but you are not obligated to do so. If you do not
 *    wish to do so, delete this exception statement from your version. If you
 *    delete this exception statement from all source files in the program,
 *    then also delete it in the license file.
 */

#pragma once

#include "mongo/db/jsobj.h"
#include "mongo/platform/process_id.h"
#include "mongo/s/catalog/catalog_manager.h"
#include "mongo/util/net/listen.h"  // For DEFAULT_MAX_CONN
#include "mongo/util/net/whitelist.h"

namespace mongo {

const int DEFAULT_UNIX_PERMS = 0700;

struct ServerGlobalParams {
    ServerGlobalParams()
        : port(DefaultDBPort),
          rest(false),
          jsonp(false),
          indexBuildRetry(true),
          quiet(false),
          configsvr(false),
          configsvrMode(CatalogManager::ConfigServerMode::NONE),
          cpu(false),
          objcheck(true),
          defaultProfile(0),
          slowMS(100),
          defaultLocalThresholdMillis(15),
          moveParanoia(false),
          noUnixSocket(false),
          doFork(0),
          socket("/tmp"),
          maxConns(DEFAULT_MAX_CONN),
          unixSocketPermissions(DEFAULT_UNIX_PERMS),
          auditOpFilter(0),
          auditAuthSuccess(false),
          auditVipOnly(true),
          logAppend(false),
          logRenameOnRotate(true),
          logWithSyslog(false),
          isHttpInterfaceEnabled(false),
          readOnlyDurationSecond(0) {
        started = time(0);
    }

    std::string binaryName;  // mongod or mongos
    std::string cwd;         // cwd of when process started

    int port;  // --port
    enum { DefaultDBPort = 27017, ConfigServerPort = 27019, ShardServerPort = 27018 };
    bool isDefaultPort() const {
        return port == DefaultDBPort;
    }

    std::string bind_ip;  // --bind_ip
    bool rest;            // --rest
    bool jsonp;           // --jsonp

    bool indexBuildRetry;  // --noIndexBuildRetry

    std::atomic<bool> quiet;  // --quiet NOLINT

    bool configsvr;                                  // --configsvr
    CatalogManager::ConfigServerMode configsvrMode;  // -- configsvrMode

    bool cpu;  // --cpu show cpu time periodically

    bool objcheck;  // --objcheck

    int defaultProfile;               // --profile
    int slowMS;                       // --time in ms that is "slow"
    int defaultLocalThresholdMillis;  // --localThreshold in ms to consider a node local
    bool moveParanoia;                // for move chunk paranoia

    bool noUnixSocket;   // --nounixsocket
    bool doFork;         // --fork
    std::string socket;  // UNIX domain socket directory

    int maxConns;  // Maximum number of simultaneous open connections.

    int unixSocketPermissions;  // permissions for the UNIX domain socket

    std::string keyFile;  // Path to keyfile, or empty if none.
    std::string pidFile;  // Path to pid file, or empty if none.

    std::string logpath;     // Path to log file, if logging to a file; otherwise, empty.
    std::string auditLogpath;// Path to audit log file, if logging to a file; otherwise, empty.
    std::string auditLogFormat;// Format of audit log
    std::string auditOpFilterStr; // Filter ops that need to be audited, string format.
    int  auditOpFilter;      // Bitwise or result of ops that need to be audited, parsed from auditOpFilterStr.
    bool auditAuthSuccess;   // True if audit authorization success requests.
    bool auditVipOnly;       // True if audit vip only for CRUD requests.
    bool logAppend;          // True if logging to a file in append mode.
    bool logRenameOnRotate;  // True if logging should rename log files on rotate
    bool logWithSyslog;      // True if logging to syslog; must not be set if logpath is set.
    int syslogFacility;      // Facility used when appending messages to the syslog.

    bool isHttpInterfaceEnabled;  // True if the dbwebserver should be enabled.

#ifndef _WIN32
    ProcessId parentProc;  // --fork pid of initial process
    ProcessId leaderProc;  // --fork pid of leader process
#endif

    /**
     * Switches to enable experimental (unsupported) features.
     */
    struct ExperimentalFeatures {
        ExperimentalFeatures() : storageDetailsCmdEnabled(false) {}
        bool storageDetailsCmdEnabled;  // -- enableExperimentalStorageDetailsCmd
    } experimental;

    time_t started;

    BSONArray argvArray;
    BSONObj parsedOpts;
    bool isAuthEnabled = false;

    AtomicInt32 clusterAuthMode;  // --clusterAuthMode, the internal cluster auth mode

    enum ClusterAuthModes {
        ClusterAuthMode_undefined,
        /**
        * Authenticate using keyfile, accept only keyfiles
        */
        ClusterAuthMode_keyFile,

        /**
        * Authenticate using keyfile, accept both keyfiles and X.509
        */
        ClusterAuthMode_sendKeyFile,

        /**
        * Authenticate using X.509, accept both keyfiles and X.509
        */
        ClusterAuthMode_sendX509,

        /**
        * Authenticate using X.509, accept only X.509
        */
        ClusterAuthMode_x509
    };
    /**
     * read only duration second
     */
    long long readOnlyDurationSecond;

    /**
     * Host in adminWhiteList can always access instance via vip
     * It's not limited by 'maxIncomingConnections'
     */
    WhiteList adminWhiteList;

    /**
     * Only hosts in userWhiteList can access instance
     */
    WhiteList userWhiteList;

};

extern ServerGlobalParams serverGlobalParams;
}
