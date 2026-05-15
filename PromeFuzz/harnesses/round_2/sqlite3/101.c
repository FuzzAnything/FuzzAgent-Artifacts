// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct FuzzCtx {
    const uint8_t *data;
    size_t size;
};

static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    struct FuzzCtx *fctx = (struct FuzzCtx *)sqlite3_user_data(context);
    if (!fctx) {
        sqlite3_result_null(context);
        return;
    }

    const uint8_t *Data = fctx->data;
    size_t Size = fctx->size;

    (void)sqlite3_get_auxdata(context, 0);
    if (argc > 0) {
        (void)sqlite3_get_auxdata(context, argc - 1);
    }

    if (Size >= sizeof(double)) {
        double d;
        memcpy(&d, Data, sizeof(double));
        sqlite3_result_double(context, d);
        return;
    }

    if (Size > 0) {
        size_t text_len = Size > 1000 ? 1000 : Size;
        char *text = (char *)malloc(text_len + 1);
        if (text) {
            memcpy(text, Data, text_len);
            text[text_len] = '\0';
            sqlite3_result_text(context, text, -1, SQLITE_TRANSIENT);
            free(text);
            return;
        }
    }

    sqlite3_result_null(context);
}

static void dummy_agg_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    struct FuzzCtx *fctx = (struct FuzzCtx *)sqlite3_user_data(context);
    if (!fctx) {
        return;
    }

    const uint8_t *Data = fctx->data;
    size_t Size = fctx->size;
    int nBytes = Size > 0 ? (Data[0] % 256) : 0;

    void *agg_ctx = sqlite3_aggregate_context(context, nBytes);
    if (agg_ctx != NULL && nBytes > 0 && Size > 0) {
        size_t copy_size = (size_t)nBytes < Size ? (size_t)nBytes : Size;
        memcpy(agg_ctx, Data, copy_size);
        (void)sqlite3_aggregate_context(context, nBytes);
        (void)sqlite3_aggregate_context(context, 0);
    }
}

static void dummy_agg_final(sqlite3_context *context) {
    void *agg_ctx = sqlite3_aggregate_context(context, 0);
    if (agg_ctx) {
        sqlite3_result_int(context, 1);
    } else {
        sqlite3_result_null(context);
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    struct FuzzCtx fctx;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    fctx.data = Data;
    fctx.size = Size;

    // Register scalar and aggregate UDFs to exercise context-dependent APIs legally.
    rc = sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, 
                                 &fctx, dummy_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "dummy_agg", 1, SQLITE_UTF8,
                                 &fctx, NULL, dummy_agg_step, dummy_agg_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare and execute statement to invoke callback with a valid context.
    const char *sql = "SELECT dummy_func()";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    (void)sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    stmt = NULL;

    // Trigger aggregate callbacks so sqlite3_aggregate_context is called in valid scope.
    rc = sqlite3_prepare_v2(db, "SELECT dummy_agg(?)", -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        if (Size > 0) {
            int bind_len = (int)(Size > 1000 ? 1000 : Size);
            sqlite3_bind_blob(stmt, 1, Data, bind_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        (void)sqlite3_step(stmt);
    }
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    
    return 0;
}