// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
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

static void profile_callback(void *arg, const char *sql, sqlite3_uint64 time) {
    (void)arg;
    (void)sql;
    (void)time;
}

static void trace_callback(void *arg, const char *sql) {
    (void)arg;
    (void)sql;
}

static int exec_callback(void *arg, int argc, char **argv, char **col) {
    (void)arg;
    (void)argc;
    (void)argv;
    (void)col;
    return 0;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *tail;
    int rc;
    
    if (Size < 1) return 0;
    
    const char *filename = "./dummy_file";
    
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Data[0] & 0x01) flags |= SQLITE_OPEN_MEMORY;
    if (Data[0] & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
    if (Data[0] & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
    
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        remove(filename);
        return 0;
    }
    
    void *profile_arg = (void *)(uintptr_t)Data[0];
    void *trace_arg = (void *)(uintptr_t)Data[1 % Size];
    
    void *prev_profile = sqlite3_profile(db, profile_callback, profile_arg);
    (void)prev_profile;
    
    void *prev_trace = sqlite3_trace(db, trace_callback, trace_arg);
    (void)prev_trace;
    
    if (Size > 2) {
        char *sql = malloc(Size - 1);
        if (sql) {
            memcpy(sql, Data + 1, Size - 1);
            sql[Size - 2] = '\0';
            
            rc = sqlite3_exec(db, sql, exec_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            
            free(sql);
        }
    }
    
    const char *simple_sql = "CREATE TABLE IF NOT EXISTS t1(a,b); INSERT INTO t1 VALUES(1,2); SELECT * FROM t1;";
    rc = sqlite3_exec(db, simple_sql, NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    const char *select_sql = "SELECT * FROM t1";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
        }
        sqlite3_finalize(stmt);
    }
    
    const char *err = sqlite3_errmsg(db);
    (void)err;
    
    sqlite3_profile(db, NULL, NULL);
    sqlite3_trace(db, NULL, NULL);
    
    sqlite3_close(db);
    remove(filename);
    
    return 0;
}