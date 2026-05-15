// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

/* Static callback functions for sqlite3_create_function */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Test sqlite3_value_type on all arguments */
    for (int i = 0; i < argc; i++) {
        int type = sqlite3_value_type(argv[i]);
        (void)type; /* Use the result to avoid compiler warnings */
    }
    
    /* Test sqlite3_aggregate_count - should return 0 for scalar functions */
    int agg_count = sqlite3_aggregate_count(ctx);
    (void)agg_count;
    
    /* Set result to an integer */
    sqlite3_result_int(ctx, 42);
}

static void scalar_func_error(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Test sqlite3_result_error_code */
    sqlite3_result_error_code(ctx, SQLITE_ERROR);
}

static void aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Aggregate step function - just count */
    int *count = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
    if (count) {
        (*count)++;
    }
}

static void aggregate_final(sqlite3_context *ctx) {
    /* Test sqlite3_aggregate_count in finalize */
    int agg_count = sqlite3_aggregate_count(ctx);
    
    int *count = (int*)sqlite3_aggregate_context(ctx, 0);
    int result = count ? *count : 0;
    
    /* Use the aggregate count to influence result */
    if (agg_count > 0) {
        sqlite3_result_int(ctx, result);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

/* UTF-16 function name buffer */
static void init_utf16_name(const uint8_t *data, size_t size, void *buf, size_t buf_size) {
    size_t i;
    uint16_t *utf16_buf = (uint16_t*)buf;
    size_t max_chars = buf_size / 2 - 1; /* Leave room for null terminator */
    
    for (i = 0; i < size && i < max_chars; i++) {
        utf16_buf[i] = data[i] | (data[(i + 1) % size] << 8);
    }
    utf16_buf[i] = 0;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char func_name[256];
    void *utf16_name = NULL;
    size_t func_name_len;
    
    /* Initialize database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Create a dummy table to test functions on */
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(x INTEGER, y TEXT)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test')", NULL, NULL, NULL);
    
    /* Generate function name from fuzzer input */
    func_name_len = Size < 255 ? Size : 255;
    if (func_name_len > 0) {
        memcpy(func_name, Data, func_name_len);
        func_name[func_name_len] = '\0';
    } else {
        strcpy(func_name, "fuzz_func");
    }
    
    /* Test sqlite3_create_function with various parameters */
    int nArg = (Size > 0) ? (Data[0] % 8) - 1 : -1; /* Range: -1 to 6 */
    int eTextRep = SQLITE_UTF8;
    
    /* Try different encodings and flags */
    if (Size > 1) {
        switch (Data[1] & 0x07) {
            case 0: eTextRep = SQLITE_UTF8; break;
            case 1: eTextRep = SQLITE_UTF16LE; break;
            case 2: eTextRep = SQLITE_UTF16BE; break;
            case 3: eTextRep = SQLITE_UTF16; break;
            case 4: eTextRep = SQLITE_UTF8 | SQLITE_DETERMINISTIC; break;
            case 5: eTextRep = SQLITE_UTF8 | SQLITE_DIRECTONLY; break;
            case 6: eTextRep = SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY; break;
            default: eTextRep = SQLITE_UTF8; break;
        }
    }
    
    /* Test scalar function creation */
    rc = sqlite3_create_function(
        db,
        func_name,
        nArg,
        eTextRep,
        NULL,
        scalar_func,
        NULL,
        NULL
    );
    
    /* Test aggregate function creation */
    if (Size > 2 && (Data[2] & 0x01)) {
        char agg_name[256];
        snprintf(agg_name, sizeof(agg_name), "%s_agg", func_name);
        sqlite3_create_function(
            db,
            agg_name,
            nArg,
            eTextRep,
            NULL,
            NULL,
            aggregate_step,
            aggregate_final
        );
    }
    
    /* Test error function creation */
    if (Size > 3 && (Data[3] & 0x01)) {
        char err_name[256];
        snprintf(err_name, sizeof(err_name), "%s_err", func_name);
        sqlite3_create_function(
            db,
            err_name,
            nArg,
            eTextRep,
            NULL,
            scalar_func_error,
            NULL,
            NULL
        );
    }
    
    /* Test sqlite3_create_function16 with UTF-16 name */
    if (Size > 4 && (Data[4] & 0x01)) {
        utf16_name = malloc(512);
        if (utf16_name) {
            init_utf16_name(Data, Size, utf16_name, 512);
            sqlite3_create_function16(
                db,
                utf16_name,
                nArg,
                eTextRep,
                NULL,
                scalar_func,
                NULL,
                NULL
            );
            free(utf16_name);
        }
    }
    
    /* Execute SQL to trigger the functions */
    if (rc == SQLITE_OK) {
        sqlite3_exec(db, "SELECT fuzz_func(1, 'test') FROM t1", NULL, NULL, NULL);
        
        if (Size > 2 && (Data[2] & 0x01)) {
            char agg_sql[512];
            snprintf(agg_sql, sizeof(agg_sql), "SELECT %s_agg(x) FROM t1", func_name);
            sqlite3_exec(db, agg_sql, NULL, NULL, NULL);
        }
        
        if (Size > 3 && (Data[3] & 0x01)) {
            char err_sql[512];
            snprintf(err_sql, sizeof(err_sql), "SELECT %s_err(x) FROM t1", func_name);
            sqlite3_exec(db, err_sql, NULL, NULL, NULL);
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}