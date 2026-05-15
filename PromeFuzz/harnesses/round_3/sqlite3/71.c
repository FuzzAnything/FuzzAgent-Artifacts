// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for testing
}

static void test_sqlite3_result_error(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine error message length
    int len = (data[0] % 256);
    if (len < 0) len = -1;
    
    // Cap len to actual available data size
    if (len > (int)size) {
        len = (int)size;
    }
    
    // Create error message from data (ensure null-terminated)
    char *err_msg = malloc(size + 1);
    if (!err_msg) return;
    
    memcpy(err_msg, data, size);
    err_msg[size] = '\0';
    
    sqlite3_result_error(ctx, err_msg, len);
    free(err_msg);
}

static void test_sqlite3_result_blob(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) {
        sqlite3_result_blob(ctx, NULL, 0, SQLITE_STATIC);
        return;
    }
    
    // Use first byte to choose destructor type
    uint8_t destructor_type = data[0] % 3;
    
    const void *blob_data = data;
    // Cap size to a reasonable value for fuzzing (1MB)
    int blob_size = (int)(size > 1048576 ? 1048576 : size);
    
    switch (destructor_type) {
        case 0:
            sqlite3_result_blob(ctx, blob_data, blob_size, SQLITE_STATIC);
            break;
        case 1:
            sqlite3_result_blob(ctx, blob_data, blob_size, SQLITE_TRANSIENT);
            break;
        case 2:
            sqlite3_result_blob(ctx, blob_data, blob_size, dummy_destructor);
            break;
    }
}

static void test_sqlite3_result_zeroblob(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) {
        sqlite3_result_zeroblob(ctx, 0);
        return;
    }
    
    // Use first 4 bytes to determine size
    int n = 0;
    if (size >= 4) {
        // Copy 4 bytes to avoid strict aliasing issues
        unsigned char bytes[4];
        memcpy(bytes, data, 4);
        n = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    } else {
        n = data[0];
    }
    
    sqlite3_result_zeroblob(ctx, n);
}

static void test_sqlite3_get_auxdata(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte as index
    int index = data[0] % 8;  // Use reasonable range
    
    // First set some auxdata if we have enough data
    if (size > 1) {
        void *aux_data = malloc(size - 1);
        if (aux_data) {
            memcpy(aux_data, data + 1, size - 1);
            sqlite3_set_auxdata(ctx, index, aux_data, free);
        }
    }
    
    // Then get it
    void *retrieved = sqlite3_get_auxdata(ctx, index);
    (void)retrieved;  // Use result to avoid unused variable warning
}

static void test_sqlite3_result_value(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create a simple value using sqlite3_result_* functions first
    // then try to retrieve it (simulating the behavior)
    
    // For testing, we'll create different types based on input
    uint8_t value_type = data[0] % 4;
    
    switch (value_type) {
        case 0:
            sqlite3_result_null(ctx);
            break;
        case 1:
            sqlite3_result_int(ctx, (int)data[1]);
            break;
        case 2:
            if (size > 1) {
                // Cap size to a reasonable value for fuzzing (1MB)
                int text_size = (int)((size - 1) > 1048576 ? 1048576 : (size - 1));
                sqlite3_result_text(ctx, (const char*)data + 1, text_size, SQLITE_TRANSIENT);
            }
            break;
        case 3:
            if (size > 1) {
                // Cap size to a reasonable value for fuzzing (1MB)
                int blob_size = (int)((size - 1) > 1048576 ? 1048576 : (size - 1));
                sqlite3_result_blob(ctx, data + 1, blob_size, SQLITE_TRANSIENT);
            }
            break;
    }
}

static void test_sqlite3_result_zeroblob64(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) {
        sqlite3_result_zeroblob64(ctx, 0);
        return;
    }
    
    // Use first 8 bytes to determine size
    sqlite3_uint64 n = 0;
    if (size >= 8) {
        // Copy 8 bytes to avoid strict aliasing issues
        unsigned char bytes[8];
        memcpy(bytes, data, 8);
        n = ((sqlite3_uint64)bytes[0] << 56) |
            ((sqlite3_uint64)bytes[1] << 48) |
            ((sqlite3_uint64)bytes[2] << 40) |
            ((sqlite3_uint64)bytes[3] << 32) |
            ((sqlite3_uint64)bytes[4] << 24) |
            ((sqlite3_uint64)bytes[5] << 16) |
            ((sqlite3_uint64)bytes[6] << 8) |
            (sqlite3_uint64)bytes[7];
    } else {
        n = data[0];
    }
    
    sqlite3_result_zeroblob64(ctx, n);
}

// Custom SQL function that will be called with our test context
static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Get the fuzzing data from the first argument
    if (argc != 1) {
        sqlite3_result_error(ctx, "Expected 1 argument", -1);
        return;
    }
    
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (size == 0) {
        sqlite3_result_null(ctx);
        return;
    }
    
    // Use first byte to choose which function to test
    uint8_t function_selector = data[0] % 6;
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size > 1 ? size - 1 : 0;
    
    switch (function_selector) {
        case 0:
            test_sqlite3_result_error(ctx, remaining_data, remaining_size);
            break;
        case 1:
            test_sqlite3_result_blob(ctx, remaining_data, remaining_size);
            break;
        case 2:
            test_sqlite3_result_zeroblob(ctx, remaining_data, remaining_size);
            break;
        case 3:
            test_sqlite3_get_auxdata(ctx, remaining_data, remaining_size);
            break;
        case 4:
            test_sqlite3_result_value(ctx, remaining_data, remaining_size);
            break;
        case 5:
            test_sqlite3_result_zeroblob64(ctx, remaining_data, remaining_size);
            break;
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Open an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create our test function
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 
                           NULL, test_function, NULL, NULL);
    
    // Prepare a statement to call our function with the fuzzing data
    sqlite3_stmt *stmt;
    const char *sql = "SELECT test_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind the fuzzing data as a BLOB parameter
    int bind_size = (int)(Size > 1048576 ? 1048576 : Size);
    if (sqlite3_bind_blob(stmt, 1, Data, bind_size, SQLITE_TRANSIENT) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Execute the statement (this will call our test function with a valid context)
    sqlite3_step(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}