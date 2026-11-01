// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a dummy virtual table module for testing.
 */
static int dummy_vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                                sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                              sqlite3_vtab **ppVtab, char **pzErr) {
    return dummy_vtab_xConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

static int dummy_vtab_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab;
    (void)pIdxInfo;
    return SQLITE_OK;
}

static int dummy_vtab_xDisconnect(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int dummy_vtab_xDestroy(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int dummy_vtab_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_vtab_xClose(sqlite3_vtab_cursor *pCursor) {
    free(pCursor);
    return SQLITE_OK;
}

static int dummy_vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                              int argc, sqlite3_value **argv) {
    (void)pCursor;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    return SQLITE_OK;
}

static int dummy_vtab_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_DONE;
}

static int dummy_vtab_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int dummy_vtab_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor;
    (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_vtab_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                              sqlite3_int64 *pRowid) {
    (void)pVtab;
    (void)argc;
    (void)argv;
    (void)pRowid;
    return SQLITE_OK;
}

static int dummy_vtab_xBegin(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_xSync(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_xCommit(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_xRollback(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                                    void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                                    void **pxArg) {
    (void)pVtab;
    (void)nArg;
    (void)zName;
    (void)pxFunc;
    (void)pxArg;
    return SQLITE_OK;
}

static int dummy_vtab_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    (void)pVtab;
    (void)zNew;
    return SQLITE_OK;
}

static int dummy_vtab_xSavepoint(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab;
    (void)iSavepoint;
    return SQLITE_OK;
}

static int dummy_vtab_xRelease(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab;
    (void)iSavepoint;
    return SQLITE_OK;
}

static int dummy_vtab_xRollbackTo(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab;
    (void)iSavepoint;
    return SQLITE_OK;
}

static const sqlite3_module dummy_vtab_module = {
    0, /* iVersion */
    dummy_vtab_xCreate,
    dummy_vtab_xConnect,
    dummy_vtab_xBestIndex,
    dummy_vtab_xDisconnect,
    dummy_vtab_xDestroy,
    dummy_vtab_xOpen,
    dummy_vtab_xClose,
    dummy_vtab_xFilter,
    dummy_vtab_xNext,
    dummy_vtab_xEof,
    dummy_vtab_xColumn,
    dummy_vtab_xRowid,
    dummy_vtab_xUpdate,
    dummy_vtab_xBegin,
    dummy_vtab_xSync,
    dummy_vtab_xCommit,
    dummy_vtab_xRollback,
    dummy_vtab_xFindFunction,
    dummy_vtab_xRename,
    dummy_vtab_xSavepoint,
    dummy_vtab_xRelease,
    dummy_vtab_xRollbackTo
};

/*
 * Helper to safely parse integers from fuzz data.
 */
static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/*
 * Helper to safely parse strings from fuzz data.
 */
static char *get_string_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    size_t len = 0;
    while (*offset + len < size && data[*offset + len] != 0) {
        len++;
    }
    if (len == 0 && *offset < size && data[*offset] == 0) {
        (*offset)++;
        return NULL;
    }
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = 0;
    *offset += len + 1;
    return str;
}

/*
 * Helper to create a dummy sqlite3_index_info structure for testing.
 */
static sqlite3_index_info *create_dummy_index_info(int nConstraint, int nOrderBy) {
    sqlite3_index_info *info = calloc(1, sizeof(sqlite3_index_info));
    if (!info) return NULL;

    info->nConstraint = nConstraint;
    if (nConstraint > 0) {
        info->aConstraint = calloc(nConstraint, sizeof(info->aConstraint[0]));
        for (int i = 0; i < nConstraint; i++) {
            info->aConstraint[i].iColumn = i;
            info->aConstraint[i].op = SQLITE_INDEX_CONSTRAINT_EQ;
            info->aConstraint[i].usable = 1;
        }
    }

    info->nOrderBy = nOrderBy;
    if (nOrderBy > 0) {
        info->aOrderBy = calloc(nOrderBy, sizeof(info->aOrderBy[0]));
        for (int i = 0; i < nOrderBy; i++) {
            info->aOrderBy[i].iColumn = i;
            info->aOrderBy[i].desc = 0;
        }
    }

    info->aConstraintUsage = calloc(nConstraint > 0 ? nConstraint : 1, sizeof(info->aConstraintUsage[0]));
    info->idxNum = 0;
    info->idxStr = NULL;
    info->needToFreeIdxStr = 0;
    info->orderByConsumed = 0;
    info->estimatedCost = 1.0;
    info->estimatedRows = 1;
    info->idxFlags = 0;
    info->colUsed = 0;

    return info;
}

/*
 * Helper to free a dummy sqlite3_index_info structure.
 */
static void free_dummy_index_info(sqlite3_index_info *info) {
    if (!info) return;
    if (info->aConstraint) free(info->aConstraint);
    if (info->aOrderBy) free(info->aOrderBy);
    if (info->aConstraintUsage) free(info->aConstraintUsage);
    if (info->idxStr && info->needToFreeIdxStr) sqlite3_free(info->idxStr);
    free(info);
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_index_info *idxInfo = NULL;
    sqlite3_value *rhsVal = NULL;
    int rc;

    /* Initialize database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register dummy virtual table module */
    rc = sqlite3_create_module(db, "dummy_vtab", &dummy_vtab_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy virtual table */
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE test_vtab USING dummy_vtab;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Parse fuzz data to determine test scenarios */
    size_t offset = 0;
    int scenario = 0;
    if (offset < Size) {
        scenario = Data[offset++] % 10;
    }

    switch (scenario) {
        case 0:
            /* Test sqlite3_overload_function */
            {
                char *funcName = get_string_from_data(Data, Size, &offset);
                int nArg = get_int_from_data(Data, Size, &offset);
                if (funcName) {
                    rc = sqlite3_overload_function(db, funcName, nArg);
                    (void)rc;
                    free(funcName);
                }
            }
            break;

        case 1:
            /* Test sqlite3_vtab_config */
            {
                int op = get_int_from_data(Data, Size, &offset);
                int arg1 = get_int_from_data(Data, Size, &offset);
                int arg2 = get_int_from_data(Data, Size, &offset);
                rc = sqlite3_vtab_config(db, op, arg1, arg2);
                (void)rc;
            }
            break;

        case 2:
            /* Test sqlite3_vtab_collation, sqlite3_vtab_distinct, sqlite3_vtab_rhs_value, sqlite3_vtab_in */
            {
                int nConstraint = get_int_from_data(Data, Size, &offset) % 10;
                int nOrderBy = get_int_from_data(Data, Size, &offset) % 5;
                idxInfo = create_dummy_index_info(nConstraint, nOrderBy);
                if (!idxInfo) break;

                int constraintIdx = get_int_from_data(Data, Size, &offset);
                if (constraintIdx >= 0 && constraintIdx < idxInfo->nConstraint) {
                    /* Test sqlite3_vtab_collation */
                    const char *collation = sqlite3_vtab_collation(idxInfo, constraintIdx);
                    (void)collation;

                    /* Test sqlite3_vtab_distinct */
                    int distinct = sqlite3_vtab_distinct(idxInfo);
                    (void)distinct;

                    /* Test sqlite3_vtab_rhs_value */
                    rc = sqlite3_vtab_rhs_value(idxInfo, constraintIdx, &rhsVal);
                    if (rc == SQLITE_OK && rhsVal) {
                        (void)rhsVal;
                    }

                    /* Test sqlite3_vtab_in */
                    int handleMode = get_int_from_data(Data, Size, &offset);
                    int inResult = sqlite3_vtab_in(idxInfo, constraintIdx, handleMode);
                    (void)inResult;
                }

                free_dummy_index_info(idxInfo);
            }
            break;

        default:
            /* Fallback: just open and close */
            break;
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}