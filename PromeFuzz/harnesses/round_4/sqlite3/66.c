// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void profile_callback(void *user_data, const char *sql, sqlite3_uint64 time_ns) {
    (void)user_data;
    (void)sql;
    (void)time_ns;
}

static void trace_callback(void *user_data, const char *sql) {
    (void)user_data;
    (void)sql;
}

static int exec_callback(void *user_data, int argc, char **argv, char **col_names) {
    (void)user_data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *err_msg = NULL;
    int rc;
    void *prev_data;
    
    if (Size < 1) return 0;
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    uint8_t flags_byte = Data[0];
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (flags_byte & 0x01) flags |= SQLITE_OPEN_MEMORY;
    if (flags_byte & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
    if (flags_byte & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
    if (flags_byte & 0x08) flags |= SQLITE_OPEN_PRIVATECACHE;
    if (flags_byte & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
    
    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        remove("./dummy_file");
        return 0;
    }
    
    if (Size > 1) {
        uint8_t mode = Data[1] % 4;
        
        switch (mode) {
            case 0:
                prev_data = sqlite3_profile(db, profile_callback, (void*)0x1234);
                sqlite3_profile(db, NULL, NULL);
                break;
            case 1:
                prev_data = sqlite3_trace(db, trace_callback, (void*)0x5678);
                sqlite3_trace(db, NULL, NULL);
                break;
            case 2:
                sqlite3_profile(db, profile_callback, (void*)0x1234);
                sqlite3_trace(db, trace_callback, (void*)0x5678);
                sqlite3_profile(db, NULL, NULL);
                sqlite3_trace(db, NULL, NULL);
                break;
            case 3:
            default:
                break;
        }
    }
    
    if (Size > 2) {
        size_t sql_len = Size - 2;
        if (sql_len > 1024) sql_len = 1024;
        
        char *sql = (char*)malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data + 2, sql_len);
            sql[sql_len] = '\0';
            
            rc = sqlite3_exec(db, sql, exec_callback, NULL, &err_msg);
            if (rc != SQLITE_OK && err_msg) {
                sqlite3_free(err_msg);
                err_msg = NULL;
            }
            
            free(sql);
        }
    }
    
    if (Size > 10) {
        const char *simple_sql = "SELECT 1;";
        rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
            }
            sqlite3_finalize(stmt);
        }
    }
    
    const char *err_str = sqlite3_errmsg(db);
    (void)err_str;
    
    sqlite3_close(db);
    remove("./dummy_file");
    
    return 0;
}