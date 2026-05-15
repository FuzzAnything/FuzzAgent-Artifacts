// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <wchar.h>

static void dummy_destructor(void *ptr) {
    // Do nothing - just a placeholder destructor
}

static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite and will use the fuzzed data
    // to test the target API functions
    
    if (argc < 1) return;
    
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (size < 4) return;
    
    // Use the fuzzed data to determine which functions to call and with what parameters
    uint8_t selector = data[0] % 6;
    int param1 = 0;
    const void *param2 = NULL;
    int param3 = 0;
    
    if (size > 4) {
        memcpy(&param1, data + 1, sizeof(int));
        param2 = data + 5;
        param3 = size > 9 ? (*(data + 9) % 256) : 0;
    }
    
    // Call the target functions based on selector
    switch (selector) {
        case 0:
            // Test sqlite3_result_error16
            if (size > 10) {
                // Convert some data to UTF-16 for testing
                size_t utf16_len = (size - 10) / 2;
                wchar_t *utf16_str = malloc((utf16_len + 1) * sizeof(wchar_t));
                if (utf16_str) {
                    for (size_t i = 0; i < utf16_len && (10 + i * 2 + 1) < size; i++) {
                        utf16_str[i] = (wchar_t)((data[10 + i * 2] << 8) | data[10 + i * 2 + 1]);
                    }
                    utf16_str[utf16_len] = 0;
                    sqlite3_result_error16(context, utf16_str, utf16_len * sizeof(wchar_t));
                    free(utf16_str);
                }
            }
            break;
            
        case 1:
            // Test sqlite3_result_error
            if (size > 5) {
                const char *error_msg = (const char *)(data + 5);
                int msg_len = size - 5;
                if (msg_len > 255) msg_len = 255;
                sqlite3_result_error(context, error_msg, msg_len);
            }
            break;
            
        case 2:
            // Test sqlite3_result_int
            sqlite3_result_int(context, param1);
            break;
            
        case 3:
            // Test sqlite3_result_text
            if (size > 5) {
                const char *text = (const char *)(data + 5);
                int text_len = size - 5;
                if (text_len > 255) text_len = 255;
                
                // Use different destructors based on data
                void (*destructor)(void*) = NULL;
                if ((data[4] & 0x01) == 0) {
                    destructor = SQLITE_TRANSIENT;
                } else if ((data[4] & 0x02) == 0) {
                    destructor = SQLITE_STATIC;
                } else {
                    destructor = dummy_destructor;
                }
                
                sqlite3_result_text(context, text, text_len, destructor);
            }
            break;
            
        case 4:
            // Test sqlite3_result_error_code
            sqlite3_result_error_code(context, param1);
            break;
            
        case 5:
            // Test sqlite3_get_auxdata
            // First set some auxdata, then get it
            if (size > 5) {
                int aux_value = param1;
                sqlite3_set_auxdata(context, 0, &aux_value, NULL);
                void *retrieved = sqlite3_get_auxdata(context, 0);
                (void)retrieved; // Use the value to avoid compiler warnings
            }
            break;
    }
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Initialize SQLite database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Create a user-defined function
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, test_function, NULL, NULL);
    
    // Prepare a statement to call the function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func(?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind the fuzzed data as a blob parameter
    sqlite3_bind_blob(stmt, 1, Data, Size > 1024 ? 1024 : Size, SQLITE_STATIC);

    // Execute the statement, which will call our test_function
    sqlite3_step(stmt);

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}