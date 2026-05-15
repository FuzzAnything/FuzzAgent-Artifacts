// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:78882:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:78864:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:78864:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:78864:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:78873:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

// Global variables to pass fuzzing data to the callback
const uint8_t* g_fuzz_data = NULL;
size_t g_fuzz_size = 0;
uint8_t g_api_selector = 0;

// Custom destructor functions for testing
static void custom_destructor(void* ptr) {
    // Do nothing - just for testing
}

static void custom_destructor_free(void* ptr) {
    free(ptr);
}

// A simple user-defined function that will be called to get a valid sqlite3_context
static void test_function(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // This function will be called by SQLite and provides a valid sqlite3_context
    // We'll use the fuzzing data passed via global variables
    
    if (g_fuzz_size < 1) {
        sqlite3_result_null(ctx);
        return;
    }
    
    // Prepare data for the function calls
    const void* blob_data = (g_fuzz_size > 1) ? (const void*)(g_fuzz_data + 1) : (const void*)"";
    int data_len = (g_fuzz_size > 1) ? (int)(g_fuzz_size - 1) : 0;
    
    // Limit data length to prevent excessive memory usage
    if (data_len > 1024) data_len = 1024;
    
    // Test different APIs based on selector
    switch (g_api_selector) {
        case 0:
            // Test sqlite3_result_blob with different destructors
            if (data_len > 0) {
                void* copy = malloc(data_len);
                if (copy) {
                    memcpy(copy, blob_data, data_len);
                    uint8_t destructor_sel = g_fuzz_data[0] % 3;
                    if (destructor_sel == 0) {
                        sqlite3_result_blob(ctx, copy, data_len, SQLITE_STATIC);
                    } else if (destructor_sel == 1) {
                        sqlite3_result_blob(ctx, copy, data_len, SQLITE_TRANSIENT);
                        free(copy);  // SQLITE_TRANSIENT makes a copy
                    } else {
                        sqlite3_result_blob(ctx, copy, data_len, custom_destructor_free);
                    }
                }
            }
            break;
            
        case 1:
            // Test sqlite3_result_text16
            if (data_len > 0) {
                // Ensure we have at least 2 bytes for UTF-16
                int utf16_len = data_len;
                if (utf16_len % 2 != 0) utf16_len--;
                if (utf16_len > 0) {
                    uint8_t destructor_sel = g_fuzz_data[0] % 3;
                    if (destructor_sel == 0) {
                        sqlite3_result_text16(ctx, blob_data, utf16_len, SQLITE_STATIC);
                    } else if (destructor_sel == 1) {
                        sqlite3_result_text16(ctx, blob_data, utf16_len, SQLITE_TRANSIENT);
                    } else {
                        sqlite3_result_text16(ctx, blob_data, utf16_len, custom_destructor);
                    }
                }
            }
            break;
            
        case 2:
            // Test sqlite3_result_text16be
            if (data_len > 0) {
                int utf16_len = data_len;
                if (utf16_len % 2 != 0) utf16_len--;
                if (utf16_len > 0) {
                    sqlite3_result_text16be(ctx, blob_data, utf16_len, 
                                          (g_fuzz_data[0] & 1) ? SQLITE_STATIC : SQLITE_TRANSIENT);
                }
            }
            break;
            
        case 3:
            // Test sqlite3_result_text
            if (data_len > 0) {
                const char* text_data = (const char*)blob_data;
                int text_len = data_len;
                // Ensure null termination for negative length test
                if (text_len > 0) {
                    char* null_terminated = malloc(text_len + 1);
                    if (null_terminated) {
                        memcpy(null_terminated, text_data, text_len);
                        null_terminated[text_len] = '\0';
                        uint8_t len_sel = g_fuzz_data[0] % 4;
                        if (len_sel == 0) {
                            sqlite3_result_text(ctx, null_terminated, text_len, custom_destructor_free);
                        } else if (len_sel == 1) {
                            sqlite3_result_text(ctx, null_terminated, -1, custom_destructor_free);
                        } else if (len_sel == 2) {
                            sqlite3_result_text(ctx, null_terminated, text_len, SQLITE_STATIC);
                        } else {
                            sqlite3_result_text(ctx, null_terminated, text_len, SQLITE_TRANSIENT);
                            free(null_terminated);
                        }
                    }
                }
            }
            break;
            
        case 4:
            // Test sqlite3_result_text16le
            if (data_len > 0) {
                int utf16_len = data_len;
                if (utf16_len % 2 != 0) utf16_len--;
                if (utf16_len > 0) {
                    sqlite3_result_text16le(ctx, blob_data, utf16_len,
                                          (g_fuzz_data[0] & 1) ? SQLITE_STATIC : SQLITE_TRANSIENT);
                }
            }
            break;
            
        case 5:
        case 6:
            // Test sqlite3_set_auxdata
            if (data_len > 0) {
                int aux_index = (g_fuzz_data[0] % 4);  // 0-3 for different indices
                
                // Create some auxiliary data
                void* aux_data = malloc(data_len + 1);
                if (aux_data) {
                    memcpy(aux_data, blob_data, data_len);
                    ((char*)aux_data)[data_len] = '\0';
                    
                    uint8_t destructor_sel = (g_fuzz_data[0] >> 2) % 3;
                    if (destructor_sel == 0) {
                        sqlite3_set_auxdata(ctx, aux_index, aux_data, custom_destructor_free);
                    } else if (destructor_sel == 1) {
                        sqlite3_set_auxdata(ctx, aux_index, aux_data, NULL);
                        // If destructor is NULL, we need to free manually
                        free(aux_data);
                    } else {
                        sqlite3_set_auxdata(ctx, aux_index, aux_data, SQLITE_STATIC);
                        // For SQLITE_STATIC, we shouldn't free as it assumes static data
                    }
                    
                    // Try to set auxdata again with different data to test replacement
                    if (data_len > 10) {
                        void* aux_data2 = malloc(10);
                        if (aux_data2) {
                            memcpy(aux_data2, blob_data, 10);
                            sqlite3_set_auxdata(ctx, aux_index, aux_data2, custom_destructor_free);
                        }
                    }
                }
            }
            break;
    }
    
    // If no result was set, set null
    sqlite3_result_null(ctx);
}

// Main fuzzing entry point
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Store fuzzing data in global variables
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    g_api_selector = Data[0] % 7;
    
    // Create an in-memory database
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Register our test function
    sqlite3_create_function(db, "test_func", 0, SQLITE_UTF8, NULL, test_function, NULL, NULL);
    
    // Execute a query that calls our function
    sqlite3_stmt* stmt = NULL;
    const char* sql = "SELECT test_func()";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Step once to execute the function
        sqlite3_step(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}