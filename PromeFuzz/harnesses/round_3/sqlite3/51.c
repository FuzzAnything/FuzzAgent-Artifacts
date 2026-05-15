// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:78781:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:78781:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a dummy destructor for testing
}

static void test_sqlite3_result_pointer(sqlite3_context *context, const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte to determine pointer type string
    const char *type_str = "test_pointer";
    if (size > 1) {
        // Use data as pointer value (just for fuzzing, not real pointer)
        void *ptr = (void *)(uintptr_t)(data[0] | (data[1] << 8));
        
        // Use third byte to determine destructor choice
        void (*destructor)(void *) = NULL;
        if (size > 2) {
            if (data[2] % 3 == 0) {
                destructor = dummy_destructor;
            } else if (data[2] % 3 == 1) {
                destructor = SQLITE_STATIC;
            } else {
                destructor = SQLITE_TRANSIENT;
            }
        }
        
        sqlite3_result_pointer(context, ptr, type_str, destructor);
    } else {
        sqlite3_result_pointer(context, NULL, type_str, NULL);
    }
}

static void test_sqlite3_result_text(sqlite3_context *context, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine length and destructor
    int length = size;
    void (*destructor)(void *) = NULL;
    
    if (data[0] % 3 == 0) {
        destructor = SQLITE_STATIC;
        // Use the data directly as a string (ensure null termination for safety)
        char *text = (char *)malloc(size + 1);
        if (!text) return;
        memcpy(text, data, size);
        text[size] = '\0';
        sqlite3_result_text(context, text, length, destructor);
        free(text);
    } else if (data[0] % 3 == 1) {
        destructor = SQLITE_TRANSIENT;
        char *text = (char *)malloc(size + 1);
        if (!text) return;
        memcpy(text, data, size);
        text[size] = '\0';
        sqlite3_result_text(context, text, length, destructor);
        free(text);
    } else {
        destructor = dummy_destructor;
        // Use substring if enough data
        int sublen = (size > 1) ? (data[1] % size) : size;
        if (sublen > 0) {
            char *text = (char *)malloc(sublen + 1);
            if (!text) return;
            memcpy(text, data, sublen);
            text[sublen] = '\0';
            sqlite3_result_text(context, text, sublen, destructor);
            free(text);
        } else {
            char *text = (char *)malloc(size + 1);
            if (!text) return;
            memcpy(text, data, size);
            text[size] = '\0';
            sqlite3_result_text(context, text, length, destructor);
            free(text);
        }
    }
}

static void test_sqlite3_result_error(sqlite3_context *context, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Convert data to error message (ensure null termination)
    char *error_msg = (char *)malloc(size + 1);
    if (!error_msg) return;
    
    memcpy(error_msg, data, size);
    error_msg[size] = '\0';
    
    // Use first byte to determine error length
    int error_len = size;
    if (data[0] % 2 == 0) {
        // Use negative length (SQLite computes length)
        sqlite3_result_error(context, error_msg, -1);
    } else {
        // Use positive length
        int len = (size > 1) ? (data[1] % size) : size;
        sqlite3_result_error(context, error_msg, len);
    }
    
    free(error_msg);
}

static void test_sqlite3_aggregate_context(sqlite3_context *context, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine allocation size
    int nbytes = 0;
    if (size >= 1) {
        nbytes = data[0] % 256;
        
        // Allocate aggregate context
        void *agg_ctx = sqlite3_aggregate_context(context, nbytes);
        
        // If we got a context and have data, write to it
        if (agg_ctx && nbytes > 0 && size > 1) {
            size_t copy_size = (size - 1) < (size_t)nbytes ? (size - 1) : nbytes;
            memcpy(agg_ctx, data + 1, copy_size);
        }
        
        // Call again with different sizes to test behavior
        if (size >= 2) {
            int nbytes2 = data[1] % 256;
            sqlite3_aggregate_context(context, nbytes2);
        }
        
        // Final call with 0 bytes
        sqlite3_aggregate_context(context, 0);
    }
}

static void test_sqlite3_result_blob(sqlite3_context *context, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine destructor
    void (*destructor)(void *) = NULL;
    if (data[0] % 3 == 0) {
        destructor = SQLITE_STATIC;
    } else if (data[0] % 3 == 1) {
        destructor = SQLITE_TRANSIENT;
    } else {
        destructor = dummy_destructor;
    }
    
    // Determine blob length
    int blob_len = size;
    if (size >= 2) {
        blob_len = (data[1] % size) + 1;
        if (blob_len > (int)size) blob_len = size;
    }
    
    sqlite3_result_blob(context, data, blob_len, destructor);
}

static void test_sqlite3_result_value(sqlite3_context *context, const uint8_t *data, size_t size) {
    // We can't create a real sqlite3_value without internal APIs
    // Just call with NULL to test the function
    sqlite3_result_value(context, NULL);
}

// Global variables to pass fuzzer data to the callback
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

// The function implementation that will be called by SQLite
static void fuzz_func_impl(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (g_fuzz_size >= 1) {
        uint8_t func_selector = g_fuzz_data[0] % 6;
        const uint8_t *func_data = g_fuzz_data + 1;
        size_t func_size = g_fuzz_size - 1;
        
        switch (func_selector) {
            case 0:
                test_sqlite3_result_pointer(context, func_data, func_size);
                break;
            case 1:
                test_sqlite3_result_text(context, func_data, func_size);
                break;
            case 2:
                test_sqlite3_result_error(context, func_data, func_size);
                break;
            case 3:
                test_sqlite3_aggregate_context(context, func_data, func_size);
                break;
            case 4:
                test_sqlite3_result_blob(context, func_data, func_size);
                break;
            case 5:
                test_sqlite3_result_value(context, func_data, func_size);
                break;
            default:
                sqlite3_result_null(context);
                break;
        }
    } else {
        sqlite3_result_null(context);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize SQLite once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Create a dummy function to register
        sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, NULL, 
                               fuzz_func_impl, NULL, NULL);
        
        initialized = 1;
    }
    
    if (!db || Size == 0) {
        return 0;
    }
    
    // Set global variables for the callback
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    
    // Call the function to execute the fuzzing logic
    sqlite3_stmt *stmt;
    const char *sql = "SELECT fuzz_func()";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Reset global variables
    g_fuzz_data = NULL;
    g_fuzz_size = 0;
    
    return 0;
}