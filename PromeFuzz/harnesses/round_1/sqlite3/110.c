// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:170771:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static void fuzz_sqlite3_complete(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *sql = malloc(size + 1);
    if (!sql) return;
    
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    sqlite3_complete(sql);
    
    free(sql);
}

static void fuzz_sqlite3_complete16(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t utf16_size = size - (size % 2);
    void *sql = malloc(utf16_size + 2);
    if (!sql) return;
    
    memcpy(sql, data, utf16_size);
    ((uint8_t*)sql)[utf16_size] = 0;
    ((uint8_t*)sql)[utf16_size + 1] = 0;
    
    sqlite3_complete16(sql);
    
    free(sql);
}

static void fuzz_prepare_functions(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return;
    }
    
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const void *tail16 = NULL;
    
    if (size > 0) {
        char *sql = malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            
            sqlite3_prepare(db, sql, -1, &stmt, &tail);
            if (stmt) sqlite3_finalize(stmt);
            stmt = NULL;
            
            sqlite3_prepare_v3(db, sql, -1, 0, &stmt, &tail);
            if (stmt) sqlite3_finalize(stmt);
            stmt = NULL;
            
            free(sql);
        }
    }
    
    if (size >= 2) {
        size_t utf16_size = size - (size % 2);
        void *sql16 = malloc(utf16_size + 2);
        if (sql16) {
            memcpy(sql16, data, utf16_size);
            ((uint8_t*)sql16)[utf16_size] = 0;
            ((uint8_t*)sql16)[utf16_size + 1] = 0;
            
            sqlite3_prepare16(db, sql16, -1, &stmt, &tail16);
            if (stmt) sqlite3_finalize(stmt);
            stmt = NULL;
            
            sqlite3_prepare16_v2(db, sql16, -1, &stmt, &tail16);
            if (stmt) sqlite3_finalize(stmt);
            stmt = NULL;
            
            free(sql16);
        }
    }
    
    sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_110(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    fuzz_sqlite3_complete(data, size);
    fuzz_sqlite3_complete16(data, size);
    fuzz_prepare_functions(data, size);
    
    return 0;
}