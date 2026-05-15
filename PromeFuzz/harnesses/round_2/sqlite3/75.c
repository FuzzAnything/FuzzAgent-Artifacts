// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_trace(void *arg, const char *sql) {
    (void)arg;
    (void)sql;
}

static void dummy_profile(void *arg, const char *sql, sqlite3_uint64 ns) {
    (void)arg;
    (void)sql;
    (void)ns;
}

static int dummy_callback(void *arg, int cols, char **vals, char **names) {
    (void)arg;
    (void)cols;
    (void)vals;
    (void)names;
    return 0;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *dummy_file = "./dummy_file";
    
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Data[0] & 0x01) flags |= SQLITE_OPEN_MEMORY;
    if (Data[0] & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
    if (Data[0] & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
    
    int rc = sqlite3_open_v2(dummy_file, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        remove(dummy_file);
        return 0;
    }
    
    sqlite3_trace(db, dummy_trace, NULL);
    sqlite3_profile(db, dummy_profile, NULL);
    
    const char *sql = (const char *)Data;
    size_t sql_len = Size > 1024 ? 1024 : Size;
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        remove(dummy_file);
        return 0;
    }
    memcpy(sql_buf, sql, sql_len);
    sql_buf[sql_len] = '\0';
    
    rc = sqlite3_prepare_v2(db, sql_buf, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cols = sqlite3_column_count(stmt);
            for (int i = 0; i < cols && i < 5; i++) {
                (void)sqlite3_column_type(stmt, i);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    rc = sqlite3_exec(db, sql_buf, dummy_callback, NULL, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }
    
    const char *err = sqlite3_errmsg(db);
    (void)err;
    
    free(sql_buf);
    sqlite3_close(db);
    remove(dummy_file);
    
    return 0;
}