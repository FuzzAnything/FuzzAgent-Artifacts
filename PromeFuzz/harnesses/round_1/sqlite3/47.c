// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *ptr) {
    // Do nothing - just for testing
}

static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called as a user-defined function
    // We'll use the fuzzer input to decide which API to test
    if (argc < 1) return;
    
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (size < 1) return;
    
    // Use first byte to decide which function to test
    uint8_t selector = data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: {
            // Test sqlite3_result_error16
            if (size - offset > 0) {
                // Use remaining data as UTF-16 error message
                // Note: we're just passing the data as-is, it may not be valid UTF-16
                sqlite3_result_error16(context, data + offset, size - offset);
            }
            break;
        }
        case 1: {
            // Test sqlite3_result_int
            int value = 0;
            if (size - offset >= sizeof(int)) {
                memcpy(&value, data + offset, sizeof(int));
            } else if (size - offset > 0) {
                value = data[offset];
            }
            sqlite3_result_int(context, value);
            break;
        }
        case 2: {
            // Test sqlite3_get_auxdata
            int index = 0;
            if (size - offset >= sizeof(int)) {
                memcpy(&index, data + offset, sizeof(int));
                offset += sizeof(int);
            } else if (size - offset > 0) {
                index = data[offset];
                offset += 1;
            }
            
            // First set some auxdata if we have more data
            if (size - offset > 0) {
                void *aux_data = malloc(size - offset);
                if (aux_data) {
                    memcpy(aux_data, data + offset, size - offset);
                    sqlite3_set_auxdata(context, index, aux_data, free);
                }
            }
            
            // Then get it
            void *retrieved = sqlite3_get_auxdata(context, index);
            // We don't use the retrieved data, just testing the call
            break;
        }
        case 3: {
            // Test sqlite3_result_text
            if (size - offset > 0) {
                char *text = malloc(size - offset + 1);
                if (text) {
                    memcpy(text, data + offset, size - offset);
                    text[size - offset] = '\0';
                    sqlite3_result_text(context, text, size - offset, free);
                }
            }
            break;
        }
        case 4: {
            // Test sqlite3_result_error
            if (size - offset > 0) {
                char *error = malloc(size - offset + 1);
                if (error) {
                    memcpy(error, data + offset, size - offset);
                    error[size - offset] = '\0';
                    sqlite3_result_error(context, error, size - offset);
                    free(error);
                }
            }
            break;
        }
        case 5: {
            // Test sqlite3_result_error_code
            int error_code = 0;
            if (size - offset >= sizeof(int)) {
                memcpy(&error_code, data + offset, sizeof(int));
            } else if (size - offset > 0) {
                error_code = data[offset];
            }
            sqlite3_result_error_code(context, error_code);
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Register our test function
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                           test_function, NULL, NULL);
    
    // Create a temporary table to store the fuzzer data
    sqlite3_exec(db, "CREATE TEMP TABLE fuzz_data (data BLOB);", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Insert fuzzer data into the table
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "INSERT INTO fuzz_data (data) VALUES (?);", -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Call our test function with the fuzzer data
    if (sqlite3_prepare_v2(db, "SELECT test_func(data) FROM fuzz_data;", -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_exec(db, "DROP TABLE fuzz_data;", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
    
    return 0;
}