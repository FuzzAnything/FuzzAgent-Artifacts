// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 virtual table APIs.
 *
 * Target APIs:
 * 1. sqlite3_vtab_in
 * 2. sqlite3_vtab_config
 * 3. sqlite3_vtab_rhs_value
 * 4. sqlite3_vtab_collation
 * 5. sqlite3_vtab_distinct
 * 6. sqlite3_overload_function
 */

static int vtab_best_index(sqlite3_vtab *pVtab, sqlite3_index_info *pIdx) {
    int i;
    int nConstraint = pIdx->nConstraint;
    
    /* Fuzz sqlite3_vtab_in */
    for (i = 0; i < nConstraint; i++) {
        sqlite3_vtab_in(pIdx, i, -1);
        sqlite3_vtab_in(pIdx, i, 1);
        sqlite3_vtab_in(pIdx, i, 0);
    }

    /* Fuzz sqlite3_vtab_rhs_value */
    sqlite3_value *pVal = 0;
    for (i = 0; i < nConstraint; i++) {
        sqlite3_vtab_rhs_value(pIdx, i, &pVal);
    }

    /* Fuzz sqlite3_vtab_collation */
    for (i = 0; i < nConstraint; i++) {
        const char *coll = sqlite3_vtab_collation(pIdx, i);
        (void)coll;
    }

    /* Fuzz sqlite3_vtab_distinct */
    int distinct = sqlite3_vtab_distinct(pIdx);
    (void)distinct;

    /* Set up a minimal valid index strategy */
    pIdx->idxNum = 0;
    pIdx->idxStr = 0;
    pIdx->needToFreeIdxStr = 0;
    pIdx->orderByConsumed = 0;
    pIdx->estimatedCost = 10.0;
    pIdx->estimatedRows = 1;
    pIdx->idxFlags = 0;

    /* Mark the first constraint as used if available.
     * Note: nConstraintUsage is not a member; we use nConstraint to check bounds.
     * The array aConstraintUsage is allocated by SQLite based on nConstraint.
     */
    if (nConstraint > 0 && pIdx->aConstraintUsage != NULL) {
        pIdx->aConstraintUsage[0].argvIndex = 1;
        pIdx->aConstraintUsage[0].omit = 0;
    }

    return SQLITE_OK;
}

static int vtab_filter(sqlite3_vtab_cursor *pVtabCur, int idxNum, const char *idxStr,
                       int argc, sqlite3_value **argv) {
    (void)pVtabCur;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    return SQLITE_OK;
}

static int vtab_next(sqlite3_vtab_cursor *pVtabCur) {
    (void)pVtabCur;
    return SQLITE_DONE;
}

static int vtab_eof(sqlite3_vtab_cursor *pVtabCur) {
    (void)pVtabCur;
    return 1;
}

static int vtab_column(sqlite3_vtab_cursor *pVtabCur, sqlite3_context *ctx, int i) {
    (void)pVtabCur;
    (void)i;
    sqlite3_result_text(ctx, "dummy", -1, SQLITE_TRANSIENT);
    return SQLITE_OK;
}

static int vtab_rowid(sqlite3_vtab_cursor *pVtabCur, sqlite3_int64 *pRowid) {
    (void)pVtabCur;
    *pRowid = 1;
    return SQLITE_OK;
}

static int vtab_create(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                       sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);

    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (*ppVtab == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                        sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);

    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (*ppVtab == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int vtab_disconnect(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int vtab_destroy(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int vtab_open(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int vtab_close(sqlite3_vtab_cursor *pVtabCur) {
    free(pVtabCur);
    return SQLITE_OK;
}

static const sqlite3_module vtab_module = {
    0,
    vtab_create,
    vtab_connect,
    vtab_best_index,
    vtab_disconnect,
    vtab_destroy,
    vtab_open,
    vtab_close,
    vtab_filter,
    vtab_next,
    vtab_eof,
    vtab_column,
    vtab_rowid,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    const char *zDbName = ":memory:";

    rc = sqlite3_open(zDbName, &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    rc = sqlite3_create_module(db, "fuzz_vtab", &vtab_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    const char *zCreateSQL = "CREATE TABLE fuzz_table USING fuzz_vtab();";
    rc = sqlite3_exec(db, zCreateSQL, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 0;
    }

    if (Size > 0) {
        size_t name_len = Size > 10 ? 10 : Size;
        char func_name[12];
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';

        for (size_t k = 0; k < name_len; k++) {
            if (Data[k] < 32 || Data[k] > 126) {
                func_name[k] = 'a';
            }
        }

        int nArg = (Size > 1) ? (Data[0] % 10) : 0;
        sqlite3_overload_function(db, func_name, nArg);
    }

    const char *zQuery1 = "SELECT * FROM fuzz_table LIMIT 1;";
    sqlite3_exec(db, zQuery1, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    const char *zQuery2 = "SELECT * FROM fuzz_table WHERE 1=1;";
    sqlite3_exec(db, zQuery2, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    const char *zQuery3 = "SELECT * FROM fuzz_table ORDER BY 1;";
    sqlite3_exec(db, zQuery3, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    const char *zQuery4 = "SELECT DISTINCT * FROM fuzz_table;";
    sqlite3_exec(db, zQuery4, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    const char *zQuery5 = "SELECT * FROM fuzz_table WHERE 1 IN (1, 2, 3);";
    sqlite3_exec(db, zQuery5, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    const char *zDropSQL = "DROP TABLE fuzz_table;";
    sqlite3_exec(db, zDropSQL, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    sqlite3_close(db);

    return 0;
}