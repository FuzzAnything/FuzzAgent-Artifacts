// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:189458:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to create a simple collation compare function */
static int simple_compare(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    if (nLeft < nRight) return -1;
    if (nLeft > nRight) return 1;
    return memcmp(pLeft, pRight, nLeft);
}

/* Static helper for UTF-16 collation compare */
static int simple_compare_utf16(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    /* Treat as raw bytes for fuzzing purposes, ensuring consistency */
    if (nLeft < nRight) return -1;
    if (nLeft > nRight) return 1;
    return memcmp(pLeft, pRight, nLeft);
}

/* Static destructor for collation context - DO NOT free here if the caller also frees */
/* We will use a flag in the context to indicate ownership */
typedef struct {
    int should_free;
} CollationCtx;

static void collation_destroy(void *pArg) {
    if (pArg) {
        CollationCtx *ctx = (CollationCtx*)pArg;
        if (ctx->should_free) {
            free(ctx);
        }
    }
}

/* Callback for sqlite3_collation_needed */
static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    /* Register a default collation if needed */
    if (zName && db) {
        /* Allocate a context that indicates we should free it */
        CollationCtx *ctx = malloc(sizeof(CollationCtx));
        if (ctx) {
            ctx->should_free = 1;
            sqlite3_create_collation(db, zName, eTextRep, ctx, simple_compare);
        }
    }
}

/* Callback for sqlite3_collation_needed16 */
static void collation_needed16_callback(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    if (zName && db) {
        /* Allocate a context that indicates we should free it */
        CollationCtx *ctx = malloc(sizeof(CollationCtx));
        if (ctx) {
            ctx->should_free = 1;
            sqlite3_create_collation16(db, zName, eTextRep, ctx, simple_compare_utf16);
        }
    }
}

/* Simple SQL function to test sqlite3_context_db_handle */
static void dummy_sql_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Retrieve the database handle to ensure the context is valid */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    if (db) {
        /* Perform a trivial operation to use the handle */
        sqlite3_errcode(db);
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Initialize database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have enough data to parse */
    if (Size < 2) {
        sqlite3_close(db);
        return 0;
    }

    /* Use fuzz data to determine encoding and name length */
    int encoding = Data[0] % 5; /* 0 to 4: SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16, SQLITE_UTF16_ALIGNED */
    int name_len = Data[1] % (Size > 1 ? Size - 1 : 1);
    
    /* Ensure name_len is at least 1 to avoid empty names if possible, but allow 0 for edge cases */
    if (name_len == 0 && Size > 2) name_len = 1;

    /* Create a buffer for the collation name from fuzz data */
    char *utf8_name = NULL;
    void *utf16_name = NULL;
    CollationCtx *context_arg = NULL;

    if (Size > name_len + 2) {
        utf8_name = malloc(name_len + 1);
        if (utf8_name) {
            memcpy(utf8_name, &Data[2], name_len);
            utf8_name[name_len] = '\0';
        }
    }

    /* Create a UTF-16 name if needed (simple conversion for fuzzing: just copy bytes and pad) */
    if (utf8_name && name_len > 0) {
        utf16_name = malloc((name_len + 1) * 2);
        if (utf16_name) {
            for (int i = 0; i < name_len; i++) {
                ((char*)utf16_name)[i*2] = utf8_name[i];
                ((char*)utf16_name)[i*2+1] = 0;
            }
            ((char*)utf16_name)[name_len*2] = 0;
            ((char*)utf16_name)[name_len*2+1] = 0;
        }
    }

    /* Allocate a context argument for the collation */
    context_arg = malloc(sizeof(CollationCtx));
    if (context_arg) {
        context_arg->should_free = 0; /* We will manage this manually to avoid double free */
    }

    /* Fuzz sqlite3_create_collation */
    if (utf8_name && context_arg) {
        /* Pass a context that we will free manually later, so set flag to 0 */
        context_arg->should_free = 0;
        sqlite3_create_collation(db, utf8_name, encoding, context_arg, simple_compare);
    }

    /* Fuzz sqlite3_create_collation_v2 */
    if (utf8_name && context_arg) {
        /* For v2, we let SQLite manage the memory, so set flag to 1 */
        CollationCtx *ctx_v2 = malloc(sizeof(CollationCtx));
        if (ctx_v2) {
            ctx_v2->should_free = 1;
            sqlite3_create_collation_v2(db, utf8_name, encoding, ctx_v2, simple_compare, collation_destroy);
        }
    }

    /* Fuzz sqlite3_create_collation16 */
    if (utf16_name && context_arg) {
        /* Pass a context that we will free manually later, so set flag to 0 */
        context_arg->should_free = 0;
        sqlite3_create_collation16(db, utf16_name, encoding, context_arg, simple_compare_utf16);
    }

    /* Fuzz sqlite3_collation_needed */
    sqlite3_collation_needed(db, (void*)(uintptr_t)Data, collation_needed_callback);

    /* Fuzz sqlite3_collation_needed16 */
    sqlite3_collation_needed16(db, (void*)(uintptr_t)Data, collation_needed16_callback);

    /* Register a function to test sqlite3_context_db_handle */
    sqlite3_create_function(db, "fuzz_test_func", 0, SQLITE_UTF8, NULL, dummy_sql_function, NULL, NULL);

    /* Execute a query to trigger the function and potentially collation callbacks */
    const char *sql = "SELECT fuzz_test_func();";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Do nothing, just iterate */
        }
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    if (utf8_name) free(utf8_name);
    if (utf16_name) free(utf16_name);
    /* Free the context_arg only if we didn't let SQLite take ownership */
    if (context_arg && !context_arg->should_free) {
        free(context_arg);
    }
    sqlite3_close(db);

    return 0;
}