// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_blob64 at sqlite3.c:78715:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:78781:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:78781:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Global variables to pass fuzzer data to the UDF
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

static void custom_destructor(void *ptr) {
    // Custom destructor that does nothing - used for testing
}

// A simple user-defined function that will be called to get a valid sqlite3_context
static void test_udf(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (g_fuzz_size == 0) {
        sqlite3_result_null(context);
        return;
    }
    
    // Create test buffers
    char *test_blob = (char *)malloc(g_fuzz_size);
    char *test_text = (char *)malloc(g_fuzz_size + 1);
    
    if (!test_blob || !test_text) {
        sqlite3_result_error_nomem(context);
        free(test_blob);
        free(test_text);
        return;
    }
    
    // Copy fuzzer data to buffers
    size_t copy_size = g_fuzz_size > 1024 ? 1024 : g_fuzz_size;
    memcpy(test_blob, g_fuzz_data, copy_size);
    memcpy(test_text, g_fuzz_data, copy_size);
    test_text[copy_size] = '\0';
    
    // Use fuzzer data to determine which functions to test
    unsigned char selector = g_fuzz_data[0];
    
    // Test sqlite3_result_blob
    if ((selector & 0x01) && copy_size > 10) {
        int blob_size = (copy_size > 100) ? 100 : (int)copy_size;
        void (*destructor)(void *) = NULL;
        
        switch ((selector >> 1) & 0x03) {
            case 0: destructor = SQLITE_STATIC; break;
            case 1: destructor = SQLITE_TRANSIENT; break;
            case 2: destructor = custom_destructor; break;
            default: destructor = NULL; break;
        }
        
        sqlite3_result_blob(context, test_blob, blob_size, destructor);
    }
    
    // Test sqlite3_result_text64
    else if ((selector & 0x02) && copy_size > 20) {
        unsigned char encoding = (selector >> 3) & 0x03;
        sqlite3_uint64 text_size = (copy_size > 200) ? 200 : copy_size;
        
        // Map to valid encodings
        unsigned char valid_encoding;
        switch (encoding) {
            case 0: valid_encoding = SQLITE_UTF8; break;
            case 1: valid_encoding = SQLITE_UTF16; break;
            case 2: valid_encoding = SQLITE_UTF16LE; break;
            case 3: valid_encoding = SQLITE_UTF16BE; break;
            default: valid_encoding = SQLITE_UTF8; break;
        }
        
        sqlite3_result_text64(context, test_text, text_size, 
                             custom_destructor, valid_encoding);
    }
    
    // Test sqlite3_result_pointer
    else if ((selector & 0x04) && copy_size > 30) {
        const char *type_str = "test_type";
        if ((selector >> 5) & 0x01) {
            sqlite3_result_pointer(context, test_blob, type_str, custom_destructor);
        } else {
            sqlite3_result_pointer(context, NULL, type_str, NULL);
        }
    }
    
    // Test sqlite3_result_text
    else if ((selector & 0x08) && copy_size > 40) {
        int text_len = (copy_size > 150) ? 150 : (int)copy_size;
        void (*text_destructor)(void *) = ((selector >> 6) & 0x01) ? 
                                         SQLITE_TRANSIENT : SQLITE_STATIC;
        sqlite3_result_text(context, test_text, text_len, text_destructor);
    }
    
    // Test sqlite3_result_error_nomem
    else if ((selector & 0x10)) {
        sqlite3_result_error_nomem(context);
    }
    
    // Test sqlite3_result_blob64
    else if ((selector & 0x20) && copy_size > 50) {
        sqlite3_uint64 blob64_size = (copy_size > 300) ? 300 : copy_size;
        void (*blob_destructor)(void *) = ((selector >> 7) & 0x01) ? 
                                         custom_destructor : SQLITE_TRANSIENT;
        sqlite3_result_blob64(context, test_blob, blob64_size, blob_destructor);
    }
    
    // Default: return null
    else {
        sqlite3_result_null(context);
    }
    
    // Clean up if not using SQLITE_TRANSIENT or SQLITE_STATIC
    // Note: In real usage, SQLite handles memory management based on destructor
    free(test_blob);
    free(test_text);
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Set global variables for UDF to access
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    
    // Initialize SQLite database
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Register our test user-defined function
    rc = sqlite3_create_function(db, "test_udf", 0, SQLITE_UTF8, NULL, 
                                 test_udf, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Execute the function to trigger the API calls
    rc = sqlite3_exec(db, "SELECT test_udf()", NULL, NULL, &err_msg);
    
    // Clean up
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
    
    return 0;
}