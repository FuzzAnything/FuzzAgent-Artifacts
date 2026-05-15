// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:146910:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy implementations for virtual table callbacks */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv,
                         sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                               void (**pxFunc)(sqlite3_context *, int, sqlite3_value **),
                               void **ppArg) {
    return 0;
}

static void dummy_destructor(void *p) {
    /* Do nothing */
}

/* Dummy module structure */
static sqlite3_module dummy_module = {
    .iVersion = 1,
    .xCreate = dummy_xCreate,
    .xConnect = dummy_xConnect,
    .xBestIndex = dummy_xBestIndex,
    .xUpdate = dummy_xUpdate,
    .xFindFunction = dummy_xFindFunction,
    /* Other members left as NULL */
};

/* Helper to extract values from fuzzer input */
static int extract_int(const uint8_t **data, size_t *size, int min, int max) {
    if (*size < sizeof(int)) return min;
    int val = *(const int *)(*data);
    *data += sizeof(int);
    *size -= sizeof(int);
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static const char *extract_string(const uint8_t **data, size_t *size, size_t max_len) {
    if (*size == 0) return "";
    size_t len = (*data)[0] % (max_len + 1);
    if (len > *size - 1) len = *size - 1;
    static char buf[256];
    if (len > sizeof(buf) - 1) len = sizeof(buf) - 1;
    memcpy(buf, *data + 1, len);
    buf[len] = '\0';
    *data += len + 1;
    *size -= len + 1;
    return buf;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    /* Open in-memory database */
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    /* Extract parameters from fuzzer input */
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    /* Parameters for sqlite3_vtab_config */
    int op = extract_int(&ptr, &remaining, 0, 5);
    int arg = extract_int(&ptr, &remaining, 0, 1);

    /* Parameters for sqlite3_create_module_v2 */
    const char *module_name = extract_string(&ptr, &remaining, 50);
    int use_destructor = extract_int(&ptr, &remaining, 0, 1);

    /* Parameters for sqlite3_vtab_in */
    int iCons = extract_int(&ptr, &remaining, 0, 10);
    int bHandle = extract_int(&ptr, &remaining, -1, 1);

    /* Parameters for sqlite3_overload_function */
    const char *func_name = extract_string(&ptr, &remaining, 50);
    int nArg = extract_int(&ptr, &remaining, 0, 10);

    /* 1. Test sqlite3_vtab_config */
    /* Note: This should be called within xConnect/xCreate, but we test anyway */
    sqlite3_vtab_config(db, op, arg);

    /* 2. Test sqlite3_create_module_v2 */
    sqlite3_create_module_v2(db, module_name, &dummy_module, 
                            (void *)0x1234, 
                            use_destructor ? dummy_destructor : NULL);

    /* 3. Test sqlite3_vtab_in - only call if we have enough data for the structure */
    /* We need to ensure the structure is properly initialized */
    struct sqlite3_index_info idx_info;
    struct sqlite3_index_constraint constraint;
    struct sqlite3_index_constraint_usage usage;
    
    /* Initialize the structures to avoid uninitialized memory reads */
    memset(&idx_info, 0, sizeof(idx_info));
    memset(&constraint, 0, sizeof(constraint));
    memset(&usage, 0, sizeof(usage));
    
    idx_info.nConstraint = 1;
    idx_info.aConstraint = &constraint;
    idx_info.aConstraintUsage = &usage;
    constraint.iColumn = -1;
    constraint.op = SQLITE_INDEX_CONSTRAINT_EQ;
    constraint.usable = 1;
    usage.argvIndex = 1;
    usage.omit = 0;
    
    /* Only call if iCons is within bounds (0 to nConstraint-1) */
    if (iCons >= 0 && iCons < idx_info.nConstraint) {
        sqlite3_vtab_in(&idx_info, iCons, bHandle);
    }

    /* 4. Test sqlite3_vtab_on_conflict */
    /* Note: This should be called within xUpdate, but we test anyway */
    sqlite3_vtab_on_conflict(db);

    /* 5. Test sqlite3_vtab_rhs_value - only call if we have a valid constraint */
    if (idx_info.nConstraint > 0) {
        sqlite3_value *pVal = NULL;
        sqlite3_vtab_rhs_value(&idx_info, 0, &pVal);
    }

    /* 6. Test sqlite3_overload_function */
    sqlite3_overload_function(db, func_name, nArg);

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}