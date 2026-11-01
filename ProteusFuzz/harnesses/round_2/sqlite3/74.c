// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to safely open a database connection */
static sqlite3 *open_db(void) {
    sqlite3 *db = NULL;
    /* Use a temporary in-memory database to avoid file system issues during fuzzing */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/* Static helper to safely close a database connection */
static void close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/* Collation callback implementation for sqlite3_create_collation */
static int collation_compare(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    /* Simple byte-wise comparison */
    return memcmp(pLeft, pRight, (nLeft < nRight) ? nLeft : nRight);
}

/* Collation needed callback implementation */
static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    /* Register a simple collation when needed */
    if (zName) {
        sqlite3_create_collation(db, zName, eTextRep, pArg, collation_compare);
    }
}

/* Scalar function implementation */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_int(ctx, 0);
}

/* Aggregate step function implementation */
static void agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_int(ctx, 0);
}

/* Aggregate final function implementation */
static void agg_final(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 0);
}

/* Module destructor */
static void module_destroy(void *pClientData) {
    /* No-op for fuzzing */
}

/* Function destructor */
static void func_destroy(void *pApp) {
    /* No-op for fuzzing */
}

/* Collation destructor */
static void collation_destroy(void *pArg) {
    /* No-op for fuzzing */
}

/* Client data destructor */
static void clientdata_destroy(void *pData) {
    /* No-op for fuzzing */
}

/* Minimal virtual table module for sqlite3_create_module_v2 */
static int vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}
static int vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}
static int vtab_disconnect(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int vtab_destroy(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) { return SQLITE_OK; }
static int vtab_close(sqlite3_vtab_cursor *pCursor) { return SQLITE_OK; }
static int vtab_filter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) { return SQLITE_OK; }
static int vtab_next(sqlite3_vtab_cursor *pCursor) { return SQLITE_OK; }
static int vtab_eof(sqlite3_vtab_cursor *pCursor) { return 1; }
static int vtab_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) { sqlite3_result_null(ctx); return SQLITE_OK; }
static int vtab_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) { *pRowid = 1; return SQLITE_OK; }
static int vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) { return SQLITE_OK; }
static int vtab_begin(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int vtab_sync(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int vtab_commit(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int vtab_rollback(sqlite3_vtab *pVTab) { return SQLITE_OK; }

static const sqlite3_module vtab_module = {
    0, /* iVersion */
    vtab_create,
    vtab_connect,
    NULL, /* xBestIndex */
    vtab_disconnect,
    vtab_destroy,
    vtab_open,
    vtab_close,
    vtab_filter,
    vtab_next,
    vtab_eof,
    vtab_column,
    vtab_rowid,
    vtab_update,
    vtab_begin,
    vtab_sync,
    vtab_commit,
    vtab_rollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = open_db();
    if (!db) return 0;

    /* Ensure we have at least some data to work with */
    if (Size < 2) {
        close_db(db);
        return 0;
    }

    /* Extract parameters from fuzzing input */
    size_t idx = 0;
    int encoding = (Data[idx] % 3) + 1; /* 1, 2, or 3 */
    idx++;
    if (idx >= Size) { close_db(db); return 0; }
    
    int nArg = Data[idx];
    idx++;
    if (idx >= Size) { close_db(db); return 0; }

    /* Create a safe string buffer for names */
    char name_buf[256];
    size_t name_len = (Size - idx) < 255 ? (Size - idx) : 255;
    memcpy(name_buf, &Data[idx], name_len);
    name_buf[name_len] = '\0';
    
    /* Ensure the name is not empty for functions that require it */
    if (name_len == 0) {
        name_buf[0] = 'a';
        name_buf[1] = '\0';
    }

    /* 1. Fuzz sqlite3_collation_needed */
    sqlite3_collation_needed(db, (void*)0x12345, collation_needed_callback);

    /* 2. Fuzz sqlite3_create_function_v2 */
    sqlite3_create_function_v2(
        db, 
        name_buf, 
        nArg, 
        encoding, 
        (void*)0x67890, 
        scalar_func, 
        agg_step, 
        agg_final, 
        func_destroy
    );

    /* 3. Fuzz sqlite3_create_collation */
    sqlite3_create_collation(db, name_buf, encoding, (void*)0xABCDEF, collation_compare);

    /* 4. Fuzz sqlite3_set_clientdata */
    sqlite3_set_clientdata(db, "fuzz_key", (void*)0x11111, clientdata_destroy);

    /* 5. Fuzz sqlite3_create_module_v2 */
    sqlite3_create_module_v2(db, "fuzz_module", &vtab_module, (void*)0x22222, module_destroy);

    /* 6. Fuzz sqlite3_create_collation_v2 */
    sqlite3_create_collation_v2(db, name_buf, encoding, (void*)0x33333, collation_compare, collation_destroy);

    /* Cleanup */
    close_db(db);
    return 0;
}