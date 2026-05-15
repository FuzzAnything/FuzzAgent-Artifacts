// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_distinct at sqlite3.c:157041:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_xBestIndex(sqlite3_index_info *pIdxInfo) {
    // This is a dummy function to simulate being called from xBestIndex
    // We'll use it to call the functions that require an xBestIndex context
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    // Initialize a dummy sqlite3 connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Create a dummy sqlite3_index_info structure
    // We need to allocate enough memory for the structure and its arrays
    struct sqlite3_index_info *pIdxInfo = (struct sqlite3_index_info *)malloc(sizeof(struct sqlite3_index_info));
    if (!pIdxInfo) {
        sqlite3_close(db);
        return 0;
    }

    // Initialize basic fields
    pIdxInfo->nConstraint = 1;
    pIdxInfo->nOrderBy = 1;
    pIdxInfo->orderByConsumed = 0;
    pIdxInfo->estimatedCost = 100.0;
    pIdxInfo->estimatedRows = 100;
    pIdxInfo->idxFlags = 0;
    pIdxInfo->colUsed = 0xFFFFFFFF;
    pIdxInfo->idxNum = 0;
    pIdxInfo->idxStr = NULL;
    pIdxInfo->needToFreeIdxStr = 0;

    // Allocate and initialize constraint array
    struct sqlite3_index_constraint *constraints = (struct sqlite3_index_constraint *)malloc(sizeof(struct sqlite3_index_constraint));
    if (!constraints) {
        free(pIdxInfo);
        sqlite3_close(db);
        return 0;
    }
    
    constraints[0].iColumn = -1;  // ROWID
    constraints[0].op = SQLITE_INDEX_CONSTRAINT_EQ;
    constraints[0].usable = 1;
    constraints[0].iTermOffset = 0;
    pIdxInfo->aConstraint = constraints;

    // Allocate and initialize constraint usage array
    struct sqlite3_index_constraint_usage *constraintUsage = (struct sqlite3_index_constraint_usage *)malloc(sizeof(struct sqlite3_index_constraint_usage));
    if (!constraintUsage) {
        free(constraints);
        free(pIdxInfo);
        sqlite3_close(db);
        return 0;
    }
    
    constraintUsage[0].argvIndex = 1;
    constraintUsage[0].omit = 0;
    pIdxInfo->aConstraintUsage = constraintUsage;

    // Allocate and initialize orderby array
    struct sqlite3_index_orderby *orderBy = (struct sqlite3_index_orderby *)malloc(sizeof(struct sqlite3_index_orderby));
    if (!orderBy) {
        free(constraintUsage);
        free(constraints);
        free(pIdxInfo);
        sqlite3_close(db);
        return 0;
    }
    
    orderBy[0].iColumn = -1;  // ROWID
    orderBy[0].desc = 0;
    pIdxInfo->aOrderBy = orderBy;

    // Use fuzz data to vary some parameters
    if (Size > 0) {
        // Use first byte to vary constraint type
        constraints[0].op = Data[0] % 32;  // SQLITE_INDEX_CONSTRAINT values are 0-31
        
        // Use second byte to vary column
        if (Size > 1) {
            constraints[0].iColumn = Data[1];
            orderBy[0].iColumn = Data[1];
        }
        
        // Use third byte to vary usability
        if (Size > 2) {
            constraints[0].usable = Data[2] & 1;
        }
        
        // Use fourth byte to vary argvIndex
        if (Size > 3) {
            constraintUsage[0].argvIndex = Data[3];
        }
        
        // Use fifth byte to vary orderByConsumed
        if (Size > 4) {
            pIdxInfo->orderByConsumed = Data[4] & 1;
        }
    }

    // Test sqlite3_vtab_distinct
    int distinct_val = sqlite3_vtab_distinct(pIdxInfo);
    (void)distinct_val;  // Use the value to avoid unused variable warning

    // Test sqlite3_vtab_in with various parameters
    int in_result;
    in_result = sqlite3_vtab_in(pIdxInfo, 0, -1);  // Check feasibility
    in_result = sqlite3_vtab_in(pIdxInfo, 0, 0);   // Set preference to not handle
    in_result = sqlite3_vtab_in(pIdxInfo, 0, 1);   // Set preference to handle
    (void)in_result;

    // Test sqlite3_vtab_collation
    const char *collation = sqlite3_vtab_collation(pIdxInfo, 0);
    (void)collation;

    // Test sqlite3_vtab_rhs_value
    sqlite3_value *pVal = NULL;
    int rhs_result = sqlite3_vtab_rhs_value(pIdxInfo, 0, &pVal);
    (void)rhs_result;

    // Test sqlite3_vtab_config with various operations
    // Use fuzz data to determine which operations to try
    if (Size > 5) {
        int op = Data[5] % 10;  // There are several SQLITE_VTAB_* constants
        sqlite3_vtab_config(db, op);
        
        // Try with an additional integer argument for some ops
        if (Size > 6) {
            int arg = Data[6];
            sqlite3_vtab_config(db, op, arg);
        }
    }

    // Test sqlite3_overload_function
    // Create a dummy function name from fuzz data
    char funcName[32] = "fuzz_func";
    if (Size > 7) {
        // Use up to 24 bytes for function name
        size_t nameLen = Size - 7;
        if (nameLen > 24) nameLen = 24;
        memcpy(funcName, Data + 7, nameLen);
        funcName[nameLen] = '\0';
    }
    
    // Try with different numbers of arguments
    for (int nArg = 0; nArg <= 3; nArg++) {
        sqlite3_overload_function(db, funcName, nArg);
    }

    // Cleanup
    free(orderBy);
    free(constraintUsage);
    free(constraints);
    free(pIdxInfo);
    sqlite3_close(db);

    return 0;
}