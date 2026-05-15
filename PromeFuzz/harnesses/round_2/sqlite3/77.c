// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *arg, int count) {
    return 0;
}

static int dummy_progress_handler(void *arg) {
    return 0;
}

static int dummy_trace_handler(unsigned int type, void *arg1, void *arg2, void *arg3) {
    return 0;
}

static void dummy_profile_handler(void *arg, const char *sql, sqlite3_uint64 time) {
}

static int dummy_authorize_handler(void *arg, int action, const char *p1, const char *p2, const char *p3, const char *p4) {
    return SQLITE_OK;
}

static void dummy_update_handler(void *arg, int action, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
}

static void dummy_preupdate_handler(void *arg, sqlite3 *db, int action, const char *db_name, const char *table_name, sqlite3_int64 rowid1, sqlite3_int64 rowid2) {
}

static int dummy_commit_handler(void *arg) {
    return SQLITE_OK;
}

static void dummy_rollback_handler(void *arg) {
}

static void dummy_collation_needed_handler(void *arg, sqlite3 *db, int eTextRep, const char *name) {
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    char *err_msg = NULL;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2);", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    if (Size > 0) {
        sqlite3_busy_handler(db, dummy_busy_handler, NULL);
        sqlite3_progress_handler(db, 100, dummy_progress_handler, NULL);
        sqlite3_trace_v2(db, SQLITE_TRACE_STMT, dummy_trace_handler, NULL);
        sqlite3_profile(db, dummy_profile_handler, NULL);
        sqlite3_set_authorizer(db, dummy_authorize_handler, NULL);
        sqlite3_update_hook(db, dummy_update_handler, NULL);
        sqlite3_preupdate_hook(db, dummy_preupdate_handler, NULL);
        sqlite3_commit_hook(db, dummy_commit_handler, NULL);
        sqlite3_rollback_hook(db, dummy_rollback_handler, NULL);
        sqlite3_collation_needed(db, NULL, dummy_collation_needed_handler);
    }
    
    if (Size >= 1) {
        int config_op = Data[0] % 20;
        int val = (int)Data[0];
        int old_val;
        
        switch (config_op) {
            case 0:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, val & 1, &old_val);
                break;
            case 1:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, val & 1, &old_val);
                break;
            case 2:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, val & 1, &old_val);
                break;
            case 3:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER, val & 1, &old_val);
                break;
            case 4:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, val & 1, &old_val);
                break;
            default:
                if (config_op < 10) {
                    sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, 0, 0);
                }
                break;
        }
    }
    
    if (Size >= 2) {
        int status_op = Data[1] % 15;
        int current = 0;
        int highwater = 0;
        int reset_flag = (Data[1] >> 4) & 1;
        
        sqlite3_db_status(db, status_op, &current, &highwater, reset_flag);
    }
    
    if (Size >= 3) {
        int status_op = Data[2] % 10;
        int current = 0;
        int highwater = 0;
        int reset_flag = (Data[2] >> 4) & 1;
        
        sqlite3_status(status_op, &current, &highwater, reset_flag);
    }
    
    if (Size >= 4) {
        int reset_flag = Data[3] & 1;
        sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
        (void)highwater;
    }
    
    if (Size >= 5) {
        sqlite3_db_release_memory(db);
    }
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT * FROM t1";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
        }
        sqlite3_finalize(stmt);
    }
    
    rc = sqlite3_close_v2(db);
    (void)rc;
    
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
    }
    
    return 0;
}