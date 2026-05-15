// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Dummy destructor for testing */
static void dummy_destructor(void *ptr) {
    /* Do nothing */
}

/* Custom function that will use the fuzzer data */
static void custom_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Use the first byte to determine which function to test */
    uint8_t selector = 0;
    const uint8_t *func_data = NULL;
    size_t func_size = 0;
    
    /* Get fuzzer data from user data */
    const uint8_t *fuzz_data = (const uint8_t*)sqlite3_user_data(ctx);
    size_t fuzz_size = 0;
    
    if (fuzz_data) {
        /* The first 8 bytes store the size */
        if (*(size_t*)fuzz_data > 0) {
            fuzz_size = *(size_t*)fuzz_data;
            fuzz_data += sizeof(size_t);
            
            if (fuzz_size > 0) {
                selector = fuzz_data[0];
                func_data = fuzz_data + 1;
                func_size = fuzz_size > 1 ? fuzz_size - 1 : 0;
            }
        }
    }
    
    /* Test different API functions based on selector */
    switch (selector % 6) {
        case 0: /* sqlite3_result_blob */
            if (func_size > 0) {
                /* Test with different destructor modes */
                size_t blob_size = func_size > 1024 ? 1024 : func_size;
                switch ((func_data[0] % 3)) {
                    case 0:
                        sqlite3_result_blob(ctx, func_data, 
                                           (int)blob_size, 
                                           SQLITE_STATIC);
                        break;
                    case 1:
                        sqlite3_result_blob(ctx, func_data, 
                                           (int)blob_size, 
                                           SQLITE_TRANSIENT);
                        break;
                    case 2:
                        sqlite3_result_blob(ctx, func_data, 
                                           (int)blob_size, 
                                           dummy_destructor);
                        break;
                }
            } else {
                sqlite3_result_null(ctx);
            }
            break;
            
        case 1: /* sqlite3_result_error_code */
            if (func_size >= sizeof(int)) {
                int error_code;
                memcpy(&error_code, func_data, sizeof(int));
                sqlite3_result_error_code(ctx, error_code);
            } else if (func_size > 0) {
                sqlite3_result_error_code(ctx, (int)func_data[0]);
            }
            /* Also set an error message */
            sqlite3_result_error(ctx, "test error", -1);
            break;
            
        case 2: /* sqlite3_result_error */
            if (func_size > 0) {
                /* Test with different length specifications */
                if (func_size > 1 && (func_data[0] % 2 == 0)) {
                    /* Negative length (null-terminated) - create a null-terminated copy */
                    size_t len = func_size > 256 ? 256 : func_size;
                    char *null_term_str = (char*)malloc(len + 1);
                    if (null_term_str) {
                        memcpy(null_term_str, func_data, len);
                        null_term_str[len] = '\0';
                        sqlite3_result_error(ctx, null_term_str, -1);
                        free(null_term_str);
                    } else {
                        sqlite3_result_error_nomem(ctx);
                    }
                } else {
                    /* Positive length - use bounded length */
                    size_t len = func_size > 256 ? 256 : func_size;
                    sqlite3_result_error(ctx, (const char*)func_data, (int)len);
                }
            }
            break;
            
        case 3: /* sqlite3_get_auxdata and sqlite3_set_auxdata */
            if (func_size > 0) {
                /* First set some auxdata */
                if (func_data[0] % 2 == 0 && func_size > 1) {
                    /* We need to allocate memory since SQLite might call destructor */
                    size_t aux_size = func_size > 16 ? 16 : func_size;
                    void *aux_data = malloc(aux_size);
                    if (aux_data) {
                        memcpy(aux_data, func_data, aux_size);
                        sqlite3_set_auxdata(ctx, 0, aux_data, free);
                    }
                }
                /* Then try to get it */
                void *aux = sqlite3_get_auxdata(ctx, 0);
                (void)aux; /* Use aux to avoid unused variable warning */
            }
            sqlite3_result_null(ctx);
            break;
            
        case 4: /* sqlite3_result_value */
            if (argc > 0) {
                sqlite3_result_value(ctx, argv[0]);
            } else {
                sqlite3_result_null(ctx);
            }
            break;
            
        case 5: /* sqlite3_result_zeroblob64 */
            if (func_size >= sizeof(sqlite3_uint64)) {
                sqlite3_uint64 n;
                memcpy(&n, func_data, sizeof(sqlite3_uint64));
                /* Limit size to avoid excessive memory allocation */
                if (n > 1024 * 1024) { /* 1MB max */
                    n = 1024 * 1024;
                }
                sqlite3_result_zeroblob64(ctx, n);
            } else if (func_size > 0) {
                sqlite3_result_zeroblob64(ctx, (sqlite3_uint64)func_data[0]);
            } else {
                sqlite3_result_zeroblob64(ctx, 0);
            }
            break;
    }
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    /* Skip empty inputs */
    if (Size == 0) return 0;
    
    sqlite3 *db;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Prepare fuzzer data to pass to the function */
    size_t total_size = sizeof(size_t) + Size;
    uint8_t *fuzz_data_with_size = (uint8_t*)malloc(total_size);
    if (!fuzz_data_with_size) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Store size followed by actual data */
    *(size_t*)fuzz_data_with_size = Size;
    if (Size > 0) {
        memcpy(fuzz_data_with_size + sizeof(size_t), Data, Size);
    }
    
    /* Create the custom function with fuzzer data as user data */
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, 
                                fuzz_data_with_size, custom_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        free(fuzz_data_with_size);
        sqlite3_close(db);
        return 0;
    }
    
    /* Create a statement to call our function */
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_func(?)", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Bind a dummy value */
        sqlite3_bind_text(stmt, 1, "dummy", 5, SQLITE_STATIC);
        
        /* Execute the function */
        sqlite3_step(stmt);
        
        /* Finalize the statement */
        sqlite3_finalize(stmt);
    }
    
    /* Clean up */
    sqlite3_close(db);
    free(fuzz_data_with_size);
    
    return 0;
}