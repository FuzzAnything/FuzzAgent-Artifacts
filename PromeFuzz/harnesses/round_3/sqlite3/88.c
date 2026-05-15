// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:174799:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:170771:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static void initialize_database(sqlite3 **db) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
    }
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static void test_sqlite3_complete(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *sql = malloc(size + 1);
    if (!sql) return;
    
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    int result = sqlite3_complete(sql);
    (void)result; // Use result to avoid unused variable warning
    
    free(sql);
}

static void test_sqlite3_complete16(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Ensure we have at least 2 bytes for UTF-16 null terminator
    size_t utf16_size = size + 2;
    uint8_t *utf16_sql = malloc(utf16_size);
    if (!utf16_sql) return;
    
    memcpy(utf16_sql, data, size);
    utf16_sql[size] = '\0';
    utf16_sql[size + 1] = '\0';
    
    int result = sqlite3_complete16(utf16_sql);
    (void)result; // Use result to avoid unused variable warning
    
    free(utf16_sql);
}

static void test_sqlite3_prepare(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    initialize_database(&db);
    if (!db) return;
    
    if (size == 0) {
        cleanup_database(db);
        return;
    }
    
    char *sql = malloc(size + 1);
    if (!sql) {
        cleanup_database(db);
        return;
    }
    
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Test with different nByte values
    int nByte = (int)(size % (size + 1)); // Will be between 0 and size
    sqlite3_prepare(db, sql, nByte, &stmt, &tail);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Test with negative nByte (read until null terminator)
    sqlite3_prepare(db, sql, -1, &stmt, &tail);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    free(sql);
    cleanup_database(db);
}

static void test_sqlite3_prepare_v3(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    initialize_database(&db);
    if (!db) return;
    
    if (size == 0) {
        cleanup_database(db);
        return;
    }
    
    char *sql = malloc(size + 1);
    if (!sql) {
        cleanup_database(db);
        return;
    }
    
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Test with different prepFlags
    unsigned int prepFlags = (unsigned int)(data[0] % 4); // Use first byte for flags
    int nByte = (int)(size % (size + 1));
    
    sqlite3_prepare_v3(db, sql, nByte, prepFlags, &stmt, &tail);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Test with negative nByte
    sqlite3_prepare_v3(db, sql, -1, prepFlags, &stmt, &tail);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    free(sql);
    cleanup_database(db);
}

static void test_sqlite3_prepare16(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    initialize_database(&db);
    if (!db) return;
    
    if (size == 0) {
        cleanup_database(db);
        return;
    }
    
    // Ensure we have at least 2 bytes for UTF-16 null terminator
    size_t utf16_size = size + 2;
    uint8_t *utf16_sql = malloc(utf16_size);
    if (!utf16_sql) {
        cleanup_database(db);
        return;
    }
    
    memcpy(utf16_sql, data, size);
    utf16_sql[size] = '\0';
    utf16_sql[size + 1] = '\0';
    
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    
    // Test with different nByte values
    int nByte = (int)(size % (size + 1));
    // Ensure nByte is even for UTF-16
    if (nByte % 2 != 0) nByte--;
    if (nByte < 0) nByte = -1;
    
    sqlite3_prepare16(db, utf16_sql, nByte, &stmt, &tail);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Test with negative nByte
    sqlite3_prepare16(db, utf16_sql, -1, &stmt, &tail);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    free(utf16_sql);
    cleanup_database(db);
}

int LLVMFuzzerTestOneInput_88(const uint8_t *data, size_t size) {
    // Test sqlite3_global_recover (always succeeds, no input needed)
    sqlite3_global_recover();
    
    // Test sqlite3_complete
    test_sqlite3_complete(data, size);
    
    // Test sqlite3_complete16
    test_sqlite3_complete16(data, size);
    
    // Test sqlite3_prepare
    test_sqlite3_prepare(data, size);
    
    // Test sqlite3_prepare_v3
    test_sqlite3_prepare_v3(data, size);
    
    // Test sqlite3_prepare16
    test_sqlite3_prepare16(data, size);
    
    return 0;
}