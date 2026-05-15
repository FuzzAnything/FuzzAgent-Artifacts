// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

/* Dummy callback functions for testing */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_text(ctx, "dummy result", -1, SQLITE_TRANSIENT);
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    /* Do nothing */
}

static void dummy_xFinal(sqlite3_context *ctx) {
    sqlite3_result_text(ctx, "final result", -1, SQLITE_TRANSIENT);
}

static void dummy_xValue(sqlite3_context *ctx) {
    sqlite3_result_text(ctx, "value result", -1, SQLITE_TRANSIENT);
}

static void dummy_xInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    /* Do nothing */
}

static void dummy_xDestroy(void *p) {
    (void)p;
    /* Do nothing */
}

/* Test functions for sqlite3_result_error and sqlite3_result_text */
static void test_error_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    const char *err_msg = "fuzzer generated error";
    sqlite3_result_error(ctx, err_msg, -1);
}

static void test_text_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    const char *text = "fuzzer generated text";
    sqlite3_result_text(ctx, text, -1, SQLITE_TRANSIENT);
}

/* Helper to extract strings from fuzzer input */
static const char* get_string(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) return NULL;
    
    size_t remaining = size - *offset;
    size_t str_len = remaining > max_len ? max_len : remaining;
    
    /* Ensure null termination */
    char *str = malloc(str_len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + *offset, str_len);
    str[str_len] = '\0';
    *offset += str_len;
    
    return str;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    /* Extract function name from fuzzer input */
    const char *func_name = get_string(Data, Size, &offset, 255);
    if (!func_name) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Extract parameters from fuzzer input */
    int nArg = (Size > offset) ? (Data[offset] % 128) : -1;
    offset = (offset + 1) % Size;
    
    int eTextRep = (Size > offset) ? (Data[offset] % 16) : SQLITE_UTF8;
    offset = (offset + 1) % Size;
    
    /* Try different API functions based on fuzzer input */
    uint8_t api_selector = (Size > 0) ? (Data[0] % 6) : 0;
    
    switch (api_selector) {
        case 0:
            /* Test sqlite3_create_function */
            sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                                   dummy_xFunc, NULL, NULL);
            break;
            
        case 1:
            /* Test sqlite3_create_function16 */
            {
                /* Convert to UTF-16 - simple conversion for testing */
                size_t len = strlen(func_name);
                void *func_name16 = malloc((len + 1) * 2);
                if (func_name16) {
                    /* Simple ASCII to UTF-16LE conversion */
                    for (size_t i = 0; i <= len; i++) {
                        ((uint16_t*)func_name16)[i] = (uint16_t)func_name[i];
                    }
                    sqlite3_create_function16(db, func_name16, nArg, eTextRep, NULL,
                                             dummy_xFunc, NULL, NULL);
                    free(func_name16);
                }
            }
            break;
            
        case 2:
            /* Test sqlite3_create_window_function */
            sqlite3_create_window_function(db, func_name, nArg, eTextRep, NULL,
                                          dummy_xStep, dummy_xFinal,
                                          dummy_xValue, dummy_xInverse,
                                          dummy_xDestroy);
            break;
            
        case 3:
            /* Test sqlite3_result_error */
            sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                                   test_error_func, NULL, NULL);
            break;
            
        case 4:
            /* Test sqlite3_result_text */
            sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                                   test_text_func, NULL, NULL);
            break;
            
        case 5:
            /* Test sqlite3_create_function_v2 */
            sqlite3_create_function_v2(db, func_name, nArg, eTextRep, NULL,
                                      dummy_xFunc, dummy_xStep, dummy_xFinal,
                                      dummy_xDestroy);
            break;
    }
    
    /* Try to use the created function */
    if (func_name) {
        char sql[512];
        snprintf(sql, sizeof(sql), "SELECT %s(1, 2, 3)", func_name);
        
        sqlite3_stmt *stmt = NULL;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            /* Execute once */
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    /* Cleanup */
    free((void*)func_name);
    sqlite3_close(db);
    
    return 0;
}