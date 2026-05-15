// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Dummy destructor functions
static void dummy_destructor(void *ptr) {
    // Do nothing
}

// Simple wrapper structure to hold context data
typedef struct {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *error_msg;
} fuzz_context_t;

// Create a minimal fuzzing environment
static fuzz_context_t* create_fuzz_context(void) {
    fuzz_context_t *ctx = (fuzz_context_t*)calloc(1, sizeof(fuzz_context_t));
    if (!ctx) return NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &ctx->db) != SQLITE_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

// Clean up fuzzing context
static void free_fuzz_context(fuzz_context_t *ctx) {
    if (ctx) {
        if (ctx->stmt) {
            sqlite3_finalize(ctx->stmt);
        }
        if (ctx->db) {
            sqlite3_close(ctx->db);
        }
        if (ctx->error_msg) {
            free(ctx->error_msg);
        }
        free(ctx);
    }
}

// User-defined function that will be called with a valid sqlite3_context
static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Get user data (our fuzzing data)
    const uint8_t *data = (const uint8_t*)sqlite3_user_data(context);
    size_t size = 0;
    
    if (data) {
        // First 4 bytes store the size
        if (*(uint32_t*)data <= UINT32_MAX - 4) {
            size = *(uint32_t*)data;
            data += 4;
        }
    }
    
    if (size == 0 || !data) return;
    
    // Use first byte to decide which function to test
    uint8_t selector = data[0] % 6;
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size - 1;
    
    switch (selector) {
        case 0: { // sqlite3_result_error16
            if (remaining_size > 0) {
                int len = (remaining_size > 2147483647) ? 2147483647 : (int)remaining_size;
                sqlite3_result_error16(context, remaining_data, len);
            } else {
                sqlite3_result_error16(context, NULL, 0);
            }
            break;
        }
        
        case 1: { // sqlite3_result_error_code
            int error_code = 0;
            if (remaining_size >= sizeof(int)) {
                memcpy(&error_code, remaining_data, sizeof(int));
            } else if (remaining_size > 0) {
                error_code = remaining_data[0];
            }
            sqlite3_result_error_code(context, error_code);
            break;
        }
        
        case 2: { // sqlite3_result_error
            if (remaining_size > 0) {
                int len = (remaining_size > 2147483647) ? 2147483647 : (int)remaining_size;
                sqlite3_result_error(context, (const char*)remaining_data, len);
            } else {
                sqlite3_result_error(context, NULL, 0);
            }
            break;
        }
        
        case 3: { // sqlite3_get_auxdata
            int index = 0;
            if (remaining_size >= sizeof(int)) {
                memcpy(&index, remaining_data, sizeof(int));
            } else if (remaining_size > 0) {
                index = remaining_data[0];
            }
            
            // First set some auxdata if we have enough data
            if (remaining_size > sizeof(int)) {
                void *aux_data = malloc(16);
                if (aux_data) {
                    sqlite3_set_auxdata(context, index, aux_data, dummy_destructor);
                }
            }
            
            // Then get it
            void *retrieved = sqlite3_get_auxdata(context, index);
            (void)retrieved; // Use the result to avoid compiler warnings
            break;
        }
        
        case 4: { // sqlite3_result_text
            if (remaining_size > 0) {
                int len = (remaining_size > 2147483647) ? 2147483647 : (int)remaining_size;
                sqlite3_result_text(context, (const char*)remaining_data, len, SQLITE_TRANSIENT);
            } else {
                sqlite3_result_text(context, NULL, 0, SQLITE_TRANSIENT);
            }
            break;
        }
        
        case 5: { // sqlite3_result_int
            int value = 0;
            if (remaining_size >= sizeof(int)) {
                memcpy(&value, remaining_data, sizeof(int));
            } else if (remaining_size > 0) {
                value = remaining_data[0];
            }
            sqlite3_result_int(context, value);
            break;
        }
    }
}

// Fuzzer entry point
int LLVMFuzzerTestOneInput_87(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Create fuzzing context
    fuzz_context_t *ctx = create_fuzz_context();
    if (!ctx) return 0;
    
    // Prepare data for user function (size + actual data)
    size_t total_size = 4 + size;
    uint8_t *user_data = (uint8_t*)malloc(total_size);
    if (!user_data) {
        free_fuzz_context(ctx);
        return 0;
    }
    
    // Store size in first 4 bytes
    uint32_t size32 = (size > UINT32_MAX) ? UINT32_MAX : (uint32_t)size;
    memcpy(user_data, &size32, 4);
    memcpy(user_data + 4, data, (size < UINT32_MAX) ? size : UINT32_MAX);
    
    // Create a test function that will receive our fuzzing data
    sqlite3_create_function(
        ctx->db,
        "test_fuzz_function",
        0,  // Number of arguments
        SQLITE_UTF8,
        user_data,  // User data (our fuzzing input)
        test_function,
        NULL,
        NULL
    );
    
    // Execute the function to trigger the fuzzing
    const char *sql = "SELECT test_fuzz_function()";
    sqlite3_prepare_v2(ctx->db, sql, -1, &ctx->stmt, NULL);
    if (ctx->stmt) {
        sqlite3_step(ctx->stmt);
    }
    
    // Clean up
    free(user_data);
    free_fuzz_context(ctx);
    
    return 0;
}