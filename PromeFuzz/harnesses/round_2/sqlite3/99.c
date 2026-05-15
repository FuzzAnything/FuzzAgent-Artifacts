// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Static function to avoid name conflicts
static void udf_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        const uint8_t *data = sqlite3_value_blob(argv[0]);
        int data_size = sqlite3_value_bytes(argv[0]);
        
        if (data && data_size > 0) {
            uint8_t func_selector = data[0] % 6;
            
            switch (func_selector) {
                case 0: { // sqlite3_result_zeroblob
                    int n = 0;
                    if (data_size >= 2) {
                        n = data[1];
                    }
                    sqlite3_result_zeroblob(ctx, n);
                    break;
                }
                case 1: { // sqlite3_result_error
                    const char *err_msg = "fuzz error";
                    int len = -1;
                    if (data_size > 1) {
                        // Use up to 256 bytes for error message
                        size_t msg_len = data_size - 1;
                        if (msg_len > 256) msg_len = 256;
                        char *msg = (char*)malloc(msg_len + 1);
                        if (msg) {
                            memcpy(msg, data + 1, msg_len);
                            msg[msg_len] = '\0';
                            err_msg = msg;
                            len = (int)msg_len;
                        }
                    }
                    sqlite3_result_error(ctx, err_msg, len);
                    if (data_size > 1 && err_msg != "fuzz error") {
                        free((void*)err_msg);
                    }
                    break;
                }
                case 2: // sqlite3_result_error_toobig
                    sqlite3_result_error_toobig(ctx);
                    break;
                case 3: // sqlite3_result_null
                    sqlite3_result_null(ctx);
                    break;
                case 4: // sqlite3_result_error_nomem
                    sqlite3_result_error_nomem(ctx);
                    break;
                case 5: { // sqlite3_result_int
                    int val = 0;
                    if (data_size >= 2) {
                        val = data[1];
                    }
                    sqlite3_result_int(ctx, val);
                    break;
                }
            }
            return;
        }
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize SQLite
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Register the user-defined function
    sqlite3_create_function(db, "fuzz_udf", 1, SQLITE_UTF8, NULL, udf_function, NULL, NULL);
    
    // Prepare and execute a statement that calls our UDF with the fuzzing data
    sqlite3_stmt *udf_stmt = NULL;
    const char *udf_sql = "SELECT fuzz_udf(?)";
    if (sqlite3_prepare_v2(db, udf_sql, -1, &udf_stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzing data as a blob parameter
        int bind_size = (int)(Size > 256 ? 256 : Size);
        sqlite3_bind_blob(udf_stmt, 1, Data, bind_size, SQLITE_TRANSIENT);
        
        // Execute the statement - this will call our UDF with a valid context
        sqlite3_step(udf_stmt);
        
        sqlite3_finalize(udf_stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}