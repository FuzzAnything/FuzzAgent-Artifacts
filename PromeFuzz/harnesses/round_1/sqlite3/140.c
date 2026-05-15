// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

typedef struct {
    uint8_t *data;
    size_t size;
} fuzz_data_t;

static fuzz_data_t g_udf_data;
static uint8_t g_fuzz_buffer[1024];

static void test_udf(sqlite3_context *ctx, int argc, sqlite3_value **argv);
static void xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv);
static void xFinal(sqlite3_context *ctx);

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Copy fuzzer data to static buffer (limited to 1024 bytes)
    size_t fuzz_size = Size < 1024 ? Size : 1024;
    memcpy(g_fuzz_buffer, Data, fuzz_size);
    
    g_udf_data.data = g_fuzz_buffer;
    g_udf_data.size = fuzz_size;
    
    // Register test function with fuzzer data as user data
    if (sqlite3_create_function(db, "test_func", 0, SQLITE_UTF8 | SQLITE_RESULT_SUBTYPE, 
                               &g_udf_data, test_udf, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Call the test function
    sqlite3_stmt *stmt;
    const char *sql = "SELECT test_func()";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Also test sqlite3_aggregate_context with an aggregate function
    if (sqlite3_create_function(db, "test_agg", 0, SQLITE_UTF8, 
                               &g_udf_data, NULL, xStep, xFinal) == SQLITE_OK) {
        sqlite3_stmt *agg_stmt;
        const char *agg_sql = "SELECT test_agg()";
        if (sqlite3_prepare_v2(db, agg_sql, -1, &agg_stmt, NULL) == SQLITE_OK) {
            sqlite3_step(agg_stmt);
            sqlite3_finalize(agg_stmt);
        }
    }
    
    sqlite3_close(db);
    return 0;
}

static void test_udf(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    fuzz_data_t *data = (fuzz_data_t*)sqlite3_user_data(ctx);
    if (!data || data->size == 0) {
        sqlite3_result_null(ctx);
        return;
    }
    
    uint8_t selector = data->data[0] % 6;
    
    switch (selector) {
        case 0:
            if (data->size >= sizeof(double)) {
                double d;
                memcpy(&d, data->data, sizeof(double));
                sqlite3_result_double(ctx, d);
            } else {
                sqlite3_result_null(ctx);
            }
            break;
        case 1:
            sqlite3_result_null(ctx);
            break;
        case 2:
            if (data->size >= sizeof(unsigned int)) {
                unsigned int subtype;
                memcpy(&subtype, data->data, sizeof(unsigned int));
                sqlite3_result_subtype(ctx, subtype);
                sqlite3_result_null(ctx); // Also set a result value
            }
            break;
        case 3:
            if (data->size >= sizeof(sqlite3_int64)) {
                sqlite3_int64 i;
                memcpy(&i, data->data, sizeof(sqlite3_int64));
                sqlite3_result_int64(ctx, i);
            }
            break;
        case 4:
            // Test sqlite3_user_data
            sqlite3_user_data(ctx);
            sqlite3_result_null(ctx);
            break;
        case 5:
            // Test combination
            sqlite3_result_null(ctx);
            if (data->size >= sizeof(unsigned int)) {
                unsigned int subtype;
                memcpy(&subtype, data->data, sizeof(unsigned int));
                sqlite3_result_subtype(ctx, subtype);
            }
            break;
    }
}

static void xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    fuzz_data_t *data = (fuzz_data_t*)sqlite3_user_data(ctx);
    if (!data || data->size < sizeof(int)) {
        return;
    }
    
    int nBytes;
    memcpy(&nBytes, data->data, sizeof(int));
    void *agg_ctx = sqlite3_aggregate_context(ctx, nBytes);
    if (agg_ctx && nBytes > 0 && data->size > sizeof(int)) {
        size_t copy_size = nBytes < (int)(data->size - sizeof(int)) ? 
                          nBytes : (data->size - sizeof(int));
        memcpy(agg_ctx, data->data + sizeof(int), copy_size);
    }
}

static void xFinal(sqlite3_context *ctx) {
    void *agg_ctx = sqlite3_aggregate_context(ctx, 0);
    if (agg_ctx) {
        sqlite3_result_int64(ctx, *(sqlite3_int64*)agg_ctx);
    } else {
        sqlite3_result_null(ctx);
    }
}