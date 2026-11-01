// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions related to collations, functions,
 * modules, and client data.
 *
 * Target APIs:
 * - sqlite3_create_collation
 * - sqlite3_create_collation_v2
 * - sqlite3_create_function_v2
 * - sqlite3_collation_needed
 * - sqlite3_set_clientdata
 * - sqlite3_create_module_v2
 */

/*
 * Helper: Extract a null-terminated string from the fuzz input.
 * Returns the string (allocated) and updates offset.
 * If no null terminator found, returns a copy of the remaining data.
 */
static char *extract_string(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) {
        return NULL;
    }
    const uint8_t *start = Data + *offset;
    const uint8_t *end = start;
    size_t max_len = Size - *offset;
    size_t len = 0;

    while (len < max_len && *(end + len) != '\0') {
        len++;
    }

    if (len == 0 && max_len > 0) {
        /* Null byte at start */
        *offset += 1;
        return NULL;
    }

    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, start, len);
    str[len] = '\0';

    *offset += len + 1;
    return str;
}

/*
 * Helper: Extract an integer from the fuzz input.
 */
static int extract_int(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int) > Size) {
        return 0;
    }
    int val;
    memcpy(&val, Data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/*
 * Helper: Extract a pointer-sized value (for pArg simulation).
 */
static void *extract_ptr(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(void *) > Size) {
        return NULL;
    }
    void *ptr;
    memcpy(&ptr, Data + *offset, sizeof(void *));
    *offset += sizeof(void *);
    /* For safety, we don't actually dereference this as a real pointer.
     * We use it as opaque data. */
    return ptr;
}

/*
 * Collation comparison function for fuzzing.
 * Must be consistent and transitive.
 */
static int fuzz_collation_compare(void *pArg, int nLen1, const void *pStr1, int nLen2, const void *pStr2) {
    /* Simple memcmp-based comparison for fuzzing purposes */
    return memcmp(pStr1, pStr2, (size_t)(nLen1 < nLen2 ? nLen1 : nLen2));
}

/*
 * Destructor for collation pArg.
 */
static void fuzz_collation_destroy(void *pArg) {
    /* No-op for fuzzing */
}

/*
 * Function implementation for fuzzing.
 */
static void fuzz_func_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* No-op for fuzzing */
}

/*
 * Step implementation for aggregate function.
 */
static void fuzz_step_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* No-op for fuzzing */
}

/*
 * Final implementation for aggregate function.
 */
static void fuzz_final_impl(sqlite3_context *ctx) {
    /* No-op for fuzzing */
}

/*
 * Destructor for function pApp.
 */
static void fuzz_func_destroy(void *pApp) {
    /* No-op for fuzzing */
}

/*
 * Collation needed callback.
 */
static void fuzz_collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    /* Register a dummy collation if needed */
    if (zName && db) {
        sqlite3_create_collation(db, zName, eTextRep, pArg, fuzz_collation_compare);
    }
}

/*
 * Minimal virtual table module for fuzzing.
 */
static int fuzz_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int fuzz_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int fuzz_vtab_disconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int fuzz_vtab_destroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int fuzz_vtab_bestindex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdx) {
    return SQLITE_OK;
}

static int fuzz_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int fuzz_vtab_close(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int fuzz_vtab_filter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int fuzz_vtab_next(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int fuzz_vtab_eof(sqlite3_vtab_cursor *pCursor) {
    return 1; /* End of data */
}

static int fuzz_vtab_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int iCol) {
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int fuzz_vtab_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int fuzz_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int fuzz_vtab_begin(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int fuzz_vtab_sync(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int fuzz_vtab_commit(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int fuzz_vtab_rollback(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

/* Module destructor */
static void fuzz_module_destroy(void *pClientData) {
    /* No-op */
}

/* Define the module structure */
static sqlite3_module fuzz_module = {
    3, /* iVersion */
    fuzz_vtab_create,
    fuzz_vtab_connect,
    fuzz_vtab_bestindex,
    fuzz_vtab_disconnect,
    fuzz_vtab_destroy,
    fuzz_vtab_open,
    fuzz_vtab_close,
    fuzz_vtab_filter,
    fuzz_vtab_next,
    fuzz_vtab_eof,
    fuzz_vtab_column,
    fuzz_vtab_rowid,
    fuzz_vtab_update,
    fuzz_vtab_begin,
    fuzz_vtab_sync,
    fuzz_vtab_commit,
    fuzz_vtab_rollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

/*
 * Main fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }

    size_t offset = 0;
    int op = 0;

    /* Determine operation based on first byte */
    if (Size > 0) {
        op = Data[0] % 6;
        offset = 1;
    }

    switch (op) {
        case 0: {
            /* Fuzz sqlite3_create_collation */
            char *zName = extract_string(Data, Size, &offset);
            int eTextRep = extract_int(Data, Size, &offset);
            void *pArg = extract_ptr(Data, Size, &offset);

            /* Normalize eTextRep to valid values */
            int valid_encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16, SQLITE_UTF16_ALIGNED};
            if (eTextRep < 0 || eTextRep >= 5) {
                eTextRep = 0;
            }

            /* zName must not be NULL for sqlite3_create_collation to avoid assertion failure */
            if (zName) {
                sqlite3_create_collation(db, zName, valid_encodings[eTextRep], pArg, fuzz_collation_compare);
                free(zName);
            }
            break;
        }

        case 1: {
            /* Fuzz sqlite3_create_collation_v2 */
            char *zName = extract_string(Data, Size, &offset);
            int eTextRep = extract_int(Data, Size, &offset);
            void *pArg = extract_ptr(Data, Size, &offset);

            int valid_encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16, SQLITE_UTF16_ALIGNED};
            if (eTextRep < 0 || eTextRep >= 5) {
                eTextRep = 0;
            }

            /* zName must not be NULL */
            if (zName) {
                sqlite3_create_collation_v2(db, zName, valid_encodings[eTextRep], pArg, fuzz_collation_compare, fuzz_collation_destroy);
                free(zName);
            }
            break;
        }

        case 2: {
            /* Fuzz sqlite3_create_function_v2 */
            char *zFuncName = extract_string(Data, Size, &offset);
            int nArg = extract_int(Data, Size, &offset);
            int eTextRep = extract_int(Data, Size, &offset);
            void *pApp = extract_ptr(Data, Size, &offset);

            int valid_encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE};
            if (eTextRep < 0 || eTextRep >= 3) {
                eTextRep = 0;
            }

            /* zFuncName must not be NULL */
            if (zFuncName) {
                sqlite3_create_function_v2(db, zFuncName, nArg, valid_encodings[eTextRep], pApp, fuzz_func_impl, fuzz_step_impl, fuzz_final_impl, fuzz_func_destroy);
                free(zFuncName);
            }
            break;
        }

        case 3: {
            /* Fuzz sqlite3_collation_needed */
            void *pArg = extract_ptr(Data, Size, &offset);
            sqlite3_collation_needed(db, pArg, fuzz_collation_needed_callback);
            break;
        }

        case 4: {
            /* Fuzz sqlite3_set_clientdata */
            char *zName = extract_string(Data, Size, &offset);
            void *pData = extract_ptr(Data, Size, &offset);

            /* zName must not be NULL for sqlite3_set_clientdata */
            if (zName) {
                sqlite3_set_clientdata(db, zName, pData, fuzz_func_destroy);
                free(zName);
            }
            break;
        }

        case 5: {
            /* Fuzz sqlite3_create_module_v2 */
            char *zName = extract_string(Data, Size, &offset);
            void *pClientData = extract_ptr(Data, Size, &offset);

            /* zName must not be NULL */
            if (zName) {
                sqlite3_create_module_v2(db, zName, &fuzz_module, pClientData, fuzz_module_destroy);
                free(zName);
            }
            break;
        }

        default:
            break;
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}