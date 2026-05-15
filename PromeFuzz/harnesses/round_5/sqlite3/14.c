// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_libversion at sqlite3.c:170987:24 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_log(void *pArg, int iErrCode, const char *zMsg) {
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
}

static void dummy_sqllog(void *pArg, sqlite3 *db, const char *zSql, int eType) {
    (void)pArg;
    (void)db;
    (void)zSql;
    (void)eType;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Call sqlite3_libversion
    const char *libversion = sqlite3_libversion();
    (void)libversion;
    
    // Call sqlite3_sourceid
    const char *sourceid = sqlite3_sourceid();
    (void)sourceid;
    
    // Call sqlite3_config 7 times with various configuration options
    // Use fuzzer input to determine which options to try
    
    // First config call - try SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, or SQLITE_CONFIG_SERIALIZED
    if (Size > 0) {
        int config_option = Data[0] % 3;
        switch (config_option) {
            case 0:
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 1:
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 2:
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
        }
    }
    
    // Second config call - try SQLITE_CONFIG_MEMSTATUS
    if (Size > 1) {
        int memstatus = Data[1] % 2;
        sqlite3_config(SQLITE_CONFIG_MEMSTATUS, memstatus);
    }
    
    // Third config call - try SQLITE_CONFIG_LOOKASIDE
    if (Size > 3) {
        int sz = Data[2] + 1;
        int cnt = Data[3] + 1;
        sqlite3_config(SQLITE_CONFIG_LOOKASIDE, sz, cnt);
    }
    
    // Fourth config call - try SQLITE_CONFIG_LOG
    sqlite3_config(SQLITE_CONFIG_LOG, dummy_log, NULL);
    
    // Fifth config call - try SQLITE_CONFIG_URI
    if (Size > 4) {
        int uri = Data[4] % 2;
        sqlite3_config(SQLITE_CONFIG_URI, uri);
    }
    
    // Sixth config call - try SQLITE_CONFIG_COVERING_INDEX_SCAN
    if (Size > 5) {
        int covering_index_scan = Data[5] % 2;
        sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, covering_index_scan);
    }
    
    // Seventh config call - try SQLITE_CONFIG_SQLLOG
    sqlite3_config(SQLITE_CONFIG_SQLLOG, dummy_sqllog, NULL);
    
    // Call sqlite3_initialize
    sqlite3_initialize();
    
    // Call sqlite3_vfs_find
    if (Size > 0) {
        size_t name_len = Size < 256 ? Size : 256;
        char *vfs_name = (char *)malloc(name_len + 1);
        if (vfs_name) {
            memcpy(vfs_name, Data, name_len);
            vfs_name[name_len] = '\0';
            
            sqlite3_vfs_find(vfs_name);
            sqlite3_vfs_find(NULL);
            
            free(vfs_name);
        } else {
            sqlite3_vfs_find(NULL);
        }
    } else {
        sqlite3_vfs_find(NULL);
    }
    
    return 0;
}