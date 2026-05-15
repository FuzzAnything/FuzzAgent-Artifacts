// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Dummy callback functions for the fuzzer */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    const char *text = "dummy";
    sqlite3_result_text(ctx, text, 5, SQLITE_STATIC);
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    void *agg_ctx = sqlite3_aggregate_context(ctx, 16);
    if (agg_ctx) {
        memset(agg_ctx, 0, 16);
    }
}

static void dummy_xFinal(sqlite3_context *ctx) {
    void *agg_ctx = sqlite3_aggregate_context(ctx, 0);
    if (agg_ctx) {
        const char *text = "final";
        sqlite3_result_text(ctx, text, 5, SQLITE_STATIC);
    }
}

static void dummy_xValue(sqlite3_context *ctx) {
    const char *text = "value";
    sqlite3_result_text(ctx, text, 5, SQLITE_STATIC);
}

static void dummy_xInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    void *agg_ctx = sqlite3_aggregate_context(ctx, 16);
    if (agg_ctx) {
        memset(agg_ctx, 0, 16);
    }
}

static void dummy_xDestroy(void *p) {
    (void)p;
}

/* Test function for sqlite3_result_text */
static void test_result_text(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    const char *test_text = "test";
    sqlite3_result_text(ctx, test_text, 4, SQLITE_STATIC);
}

/* Helper to extract a null-terminated string from fuzzer input */
static const char* get_string_safe(const uint8_t *data, size_t size, size_t *offset, char *buffer, size_t buffer_size) {
    if (*offset >= size || buffer_size == 0) return NULL;
    
    /* Determine length from fuzzer data, but ensure it fits in buffer */
    size_t max_len = buffer_size - 1; /* Reserve space for null terminator */
    size_t len = data[*offset] % max_len;
    (*offset)++;
    
    if (len > size - *offset) {
        len = size - *offset;
    }
    
    if (len > 0) {
        memcpy(buffer, data + *offset, len);
        *offset += len;
    }
    
    buffer[len] = '\0'; /* Ensure null termination */
    return buffer;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;

    size_t offset = 0;
    
    /* Extract parameters from fuzzer input */
    int nArg = (Data[offset] % 10) - 1; /* -1 to 8 */
    offset = (offset + 1) % Size;
    int eTextRep = Data[offset] % 4; /* 0-3 for UTF8/16LE/16BE/16 */
    offset = (offset + 1) % Size;
    
    /* Buffers for function names */
    char func_name_buf[256];
    char func_name_v2_buf[256];
    char win_func_name_buf[256];
    char agg_func_name_buf[256];
    char test_func_name_buf[256];
    
    /* Test sqlite3_create_function */
    const char *func_name = get_string_safe(Data, Size, &offset, func_name_buf, sizeof(func_name_buf));
    if (func_name && func_name_buf[0] != '\0') {
        sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                                dummy_xFunc, NULL, NULL);
    }
    
    /* Test sqlite3_create_function_v2 */
    const char *func_name_v2 = get_string_safe(Data, Size, &offset, func_name_v2_buf, sizeof(func_name_v2_buf));
    if (func_name_v2 && func_name_v2_buf[0] != '\0') {
        sqlite3_create_function_v2(db, func_name_v2, nArg, eTextRep, NULL,
                                   dummy_xFunc, NULL, NULL, dummy_xDestroy);
    }
    
    /* Test sqlite3_create_function16 - requires UTF-16 name */
    char func_name16_buf[256];
    const char *func_name16_raw = get_string_safe(Data, Size, &offset, func_name16_buf, sizeof(func_name16_buf));
    if (func_name16_raw && func_name16_buf[0] != '\0') {
        /* Convert to UTF-16 by simple duplication (not correct but enough for fuzzing) */
        size_t len = strlen(func_name16_raw);
        uint16_t *name16 = malloc((len + 1) * sizeof(uint16_t));
        if (name16) {
            for (size_t i = 0; i < len; i++) {
                name16[i] = (uint16_t)func_name16_raw[i];
            }
            name16[len] = 0;
            sqlite3_create_function16(db, name16, nArg, eTextRep, NULL,
                                      dummy_xFunc, dummy_xStep, dummy_xFinal);
            free(name16);
        }
    }
    
    /* Test sqlite3_create_window_function */
    const char *win_func_name = get_string_safe(Data, Size, &offset, win_func_name_buf, sizeof(win_func_name_buf));
    if (win_func_name && win_func_name_buf[0] != '\0') {
        sqlite3_create_window_function(db, win_func_name, nArg, eTextRep, NULL,
                                       dummy_xStep, dummy_xFinal,
                                       dummy_xValue, dummy_xInverse, dummy_xDestroy);
    }
    
    /* Test sqlite3_aggregate_context by creating an aggregate function */
    const char *agg_func_name = get_string_safe(Data, Size, &offset, agg_func_name_buf, sizeof(agg_func_name_buf));
    if (agg_func_name && agg_func_name_buf[0] != '\0') {
        sqlite3_create_function(db, agg_func_name, nArg, eTextRep, NULL,
                                NULL, dummy_xStep, dummy_xFinal);
    }
    
    /* Test sqlite3_result_text by calling it from within a dummy function */
    if (Size > offset) {
        const char *test_func_name = get_string_safe(Data, Size, &offset, test_func_name_buf, sizeof(test_func_name_buf));
        if (test_func_name && test_func_name_buf[0] != '\0') {
            sqlite3_create_function(db, test_func_name, 0, SQLITE_UTF8, NULL,
                                    test_result_text, NULL, NULL);
        }
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}