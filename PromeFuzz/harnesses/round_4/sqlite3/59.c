// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_sqlite3_errstr(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int err_code;
    memcpy(&err_code, data, sizeof(int));
    
    const char *err_str = sqlite3_errstr(err_code);
    (void)err_str; // Use result to avoid unused variable warning
}

static void fuzz_sqlite3_error_functions(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db) return;
    
    // Get error codes
    int err_code = sqlite3_errcode(db);
    int extended_err_code = sqlite3_extended_errcode(db);
    
    // Get error messages
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_code;
    (void)extended_err_code;
    (void)err_msg;
    
    // Test extended result codes
    if (size > 0) {
        int enable = data[0] & 1;
        sqlite3_extended_result_codes(db, enable);
    }
    
    // Test setting custom error message
    if (size > sizeof(int)) {
        int custom_err;
        memcpy(&custom_err, data, sizeof(int));
        
        // Create a null-terminated string from fuzzer data
        size_t msg_len = size > sizeof(int) ? size - sizeof(int) : 0;
        if (msg_len > 0) {
            char *custom_msg = malloc(msg_len + 1);
            if (custom_msg) {
                memcpy(custom_msg, data + sizeof(int), msg_len);
                custom_msg[msg_len] = '\0';
                sqlite3_set_errmsg(db, custom_err, custom_msg);
                free(custom_msg);
            }
        } else {
            sqlite3_set_errmsg(db, custom_err, NULL);
        }
    }
}

static void fuzz_database_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db) return;
    
    // Create a simple table to generate potential errors
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);
    
    // Try to execute some SQL from fuzzer input
    if (size > 0) {
        // Create null-terminated SQL string
        char *sql = malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            
            sqlite3_exec(db, sql, NULL, NULL, NULL);
            free(sql);
        }
    }
    
    // Try to prepare a statement
    if (size > 0) {
        char *stmt_sql = malloc(size + 1);
        if (stmt_sql) {
            memcpy(stmt_sql, data, size);
            stmt_sql[size] = '\0';
            
            sqlite3_stmt *stmt;
            sqlite3_prepare_v2(db, stmt_sql, -1, &stmt, NULL);
            if (stmt) {
                sqlite3_finalize(stmt);
            }
            free(stmt_sql);
        }
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *data, size_t size) {
    // Fuzz sqlite3_errstr with raw error codes
    fuzz_sqlite3_errstr(data, size);
    
    // Open an in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc == SQLITE_OK && db) {
        // Fuzz error-related functions
        fuzz_sqlite3_error_functions(db, data, size);
        
        // Perform some database operations that might generate errors
        fuzz_database_operations(db, data, size);
        
        // Close the database
        sqlite3_close(db);
    } else {
        // Even if opening fails, we can still test some functions
        if (db) {
            fuzz_sqlite3_error_functions(db, data, size);
            sqlite3_close(db);
        }
    }
    
    // Also test with NULL database handle
    fuzz_sqlite3_error_functions(NULL, data, size);
    
    return 0;
}