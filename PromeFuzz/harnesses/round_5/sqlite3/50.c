// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Global variables to pass fuzzer data to the test function */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;
static size_t g_fuzz_offset = 0;

static void dummy_destructor(void *ptr) {
    /* Do nothing - just a placeholder destructor */
}

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* This function simulates a user-defined SQL function that uses the target APIs */
    (void)argc;
    (void)argv;
    
    if (g_fuzz_offset >= g_fuzz_size) return;
    
    /* Extract parameters from fuzzer input */
    int aux_index = 0;
    int int_value = 0;
    int error_code = 0;
    const char *text_data = NULL;
    int text_len = 0;
    const void *error16_data = NULL;
    int error16_len = 0;
    
    /* Read aux_index (1 byte) */
    if (g_fuzz_offset < g_fuzz_size) {
        aux_index = g_fuzz_data[g_fuzz_offset] % 8; /* Limit to reasonable range */
        g_fuzz_offset++;
    }
    
    /* Read int_value (4 bytes) */
    if (g_fuzz_offset + 3 < g_fuzz_size) {
        int_value = *(int*)(g_fuzz_data + g_fuzz_offset);
        g_fuzz_offset += 4;
    }
    
    /* Read error_code (4 bytes) */
    if (g_fuzz_offset + 3 < g_fuzz_size) {
        error_code = *(int*)(g_fuzz_data + g_fuzz_offset);
        g_fuzz_offset += 4;
    }
    
    /* Set some auxiliary data first so get_auxdata might return something */
    static const char dummy_aux[] = "dummy_aux_data";
    sqlite3_set_auxdata(ctx, aux_index, (void*)dummy_aux, NULL);
    
    /* 1. Test sqlite3_get_auxdata */
    void *aux_data = sqlite3_get_auxdata(ctx, aux_index);
    (void)aux_data;
    
    /* Also test with invalid index */
    sqlite3_get_auxdata(ctx, aux_index + 100);
    
    /* 2. Test sqlite3_result_int */
    sqlite3_result_int(ctx, int_value);
    
    /* 3. Test sqlite3_result_error_code */
    sqlite3_result_error_code(ctx, error_code);
    
    /* 4. Test sqlite3_result_error */
    if (g_fuzz_offset < g_fuzz_size) {
        /* Use remaining bytes for error message, up to 256 bytes */
        text_len = g_fuzz_size - g_fuzz_offset;
        if (text_len > 256) text_len = 256;
        text_data = (const char*)(g_fuzz_data + g_fuzz_offset);
        sqlite3_result_error(ctx, text_data, text_len);
        g_fuzz_offset += text_len;
    } else {
        sqlite3_result_error(ctx, "fuzz error", -1);
    }
    
    /* 5. Test sqlite3_result_text */
    if (g_fuzz_offset < g_fuzz_size) {
        /* Use next chunk for text result */
        text_len = g_fuzz_size - g_fuzz_offset;
        if (text_len > 256) text_len = 256;
        text_data = (const char*)(g_fuzz_data + g_fuzz_offset);
        sqlite3_result_text(ctx, text_data, text_len, dummy_destructor);
        g_fuzz_offset += text_len;
    } else {
        sqlite3_result_text(ctx, "fuzz text", -1, SQLITE_TRANSIENT);
    }
    
    /* 6. Test sqlite3_result_error16 */
    if (g_fuzz_offset < g_fuzz_size) {
        /* Use remaining bytes for UTF-16 error message */
        error16_len = g_fuzz_size - g_fuzz_offset;
        if (error16_len > 512) error16_len = 512;
        error16_data = g_fuzz_data + g_fuzz_offset;
        sqlite3_result_error16(ctx, error16_data, error16_len);
        g_fuzz_offset += error16_len;
    } else {
        static const unsigned short utf16_error[] = {'f','u','z','z',0};
        sqlite3_result_error16(ctx, utf16_error, -1);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    
    /* Reset global state */
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    g_fuzz_offset = 0;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Register our test function */
    rc = sqlite3_create_function(
        db,
        "fuzz_func",
        0, /* Number of arguments - we'll use 0 for simplicity */
        SQLITE_UTF8,
        NULL,
        test_function,
        NULL,
        NULL
    );
    
    if (rc == SQLITE_OK) {
        /* Execute the function multiple times with different contexts */
        for (int i = 0; i < 3 && g_fuzz_offset < Size; i++) {
            /* Reset offset for each iteration to reuse data differently */
            g_fuzz_offset = i * (Size / 3);
            
            /* Create a simple statement that calls our function */
            sqlite3_stmt *stmt;
            const char *sql = "SELECT fuzz_func()";
            
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                /* Step through to execute the function */
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            /* Also test with sqlite3_exec */
            rc = sqlite3_exec(db, "SELECT fuzz_func()", NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = NULL;
            }
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}