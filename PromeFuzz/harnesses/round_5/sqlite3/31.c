// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
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

static int dummy_busy_handler(void *data, int count) {
    // Simple busy handler that always returns 0 to abort
    (void)data;
    (void)count;
    return 0;
}

static void dummy_scalar_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Simple scalar function that returns NULL
    (void)argc;
    (void)argv;
    sqlite3_result_null(context);
}

static int dummy_callback(void *data, int argc, char **argv, char **col_names) {
    // Simple callback that does nothing
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    
    // Use a fixed database file name
    const char *filename = "./dummy_file";
    
    // Try to open database with various flags based on fuzz input
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        // Use first byte to determine some flags
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_PRIVATECACHE;
    }
    
    // Open database
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        // If open fails, try with just basic flags
        sqlite3_close(db);
        db = NULL;
        rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc != SQLITE_OK) {
            return 0;
        }
    }
    
    // Close database (testing close on newly opened db)
    sqlite3_close(db);
    
    // Re-open for remaining operations
    db = NULL;
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Create a function with parameters derived from fuzz input
    const char *func_name = "fuzz_func";
    int nArg = -1;  // Variable number of arguments
    int eTextRep = SQLITE_UTF8;
    
    if (Size > 1) {
        // Use second byte for number of arguments (-1 to 127)
        nArg = (Data[1] % 129) - 1;
        
        // Use third byte for text encoding
        if (Size > 2) {
            uint8_t enc_byte = Data[2];
            switch (enc_byte % 4) {
                case 0: eTextRep = SQLITE_UTF8; break;
                case 1: eTextRep = SQLITE_UTF16; break;
                case 2: eTextRep = SQLITE_UTF16LE; break;
                case 3: eTextRep = SQLITE_UTF16BE; break;
            }
            // Add deterministic flag sometimes
            if (enc_byte & 0x80) {
                eTextRep |= SQLITE_DETERMINISTIC;
            }
        }
    }
    
    rc = sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                                 dummy_scalar_function, NULL, NULL);
    // Ignore return code - function creation might fail for various reasons
    
    // Set busy handler
    rc = sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    // Ignore return code
    
    // Prepare SQL statement from fuzz data
    if (Size > 0) {
        // Create a null-terminated string from fuzz data for SQL
        // Limit to reasonable size to avoid huge allocations
        size_t sql_size = Size > 1024 ? 1024 : Size;
        char *sql = malloc(sql_size + 1);
        if (sql) {
            memcpy(sql, Data, sql_size);
            sql[sql_size] = '\0';
            
            // Try to execute SQL
            rc = sqlite3_exec(db, sql, dummy_callback, NULL, &err_msg);
            
            // Free error message if one was returned
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            
            free(sql);
        }
    }
    
    // Try some fixed SQL statements as well
    const char *fixed_sqls[] = {
        "CREATE TABLE IF NOT EXISTS t1(a,b);",
        "SELECT * FROM t1;",
        "DROP TABLE IF EXISTS t1;",
        NULL
    };
    
    for (int i = 0; fixed_sqls[i] != NULL; i++) {
        rc = sqlite3_exec(db, fixed_sqls[i], dummy_callback, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}