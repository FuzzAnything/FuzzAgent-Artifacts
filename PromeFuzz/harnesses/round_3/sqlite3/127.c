// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:157041:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3 database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a dummy sqlite3_index_info structure
    sqlite3_index_info idx_info;
    memset(&idx_info, 0, sizeof(idx_info));

    // Use fuzzer data to populate some fields
    if (Size >= sizeof(sqlite3_index_info)) {
        // Copy as much data as possible into the structure
        size_t copy_size = Size < sizeof(sqlite3_index_info) ? Size : sizeof(sqlite3_index_info);
        memcpy(&idx_info, Data, copy_size);
    }

    // Set some reasonable limits to avoid crashes from invalid data
    if (idx_info.nConstraint > 100) idx_info.nConstraint = 100;
    if (idx_info.nOrderBy > 100) idx_info.nOrderBy = 100;

    // Allocate arrays if needed
    if (idx_info.nConstraint > 0) {
        idx_info.aConstraint = (struct sqlite3_index_constraint*)calloc(idx_info.nConstraint, sizeof(struct sqlite3_index_constraint));
        idx_info.aConstraintUsage = (struct sqlite3_index_constraint_usage*)calloc(idx_info.nConstraint, sizeof(struct sqlite3_index_constraint_usage));
    }

    if (idx_info.nOrderBy > 0) {
        idx_info.aOrderBy = (struct sqlite3_index_orderby*)calloc(idx_info.nOrderBy, sizeof(struct sqlite3_index_orderby));
    }

    // Test sqlite3_vtab_in with various bHandle values
    if (idx_info.nConstraint > 0) {
        int iCons = 0;
        if (Size > 0) iCons = Data[0] % idx_info.nConstraint;
        
        // Test with bHandle = -1 (feasibility check)
        sqlite3_vtab_in(&idx_info, iCons, -1);
        
        // Test with bHandle = 0 (no preference)
        sqlite3_vtab_in(&idx_info, iCons, 0);
        
        // Test with bHandle = 1 (preference for batching)
        sqlite3_vtab_in(&idx_info, iCons, 1);
        
        // Test with random bHandle value from fuzzer data
        if (Size > 1) {
            int bHandle = (int8_t)Data[1];
            sqlite3_vtab_in(&idx_info, iCons, bHandle);
        }
    }

    // Test sqlite3_vtab_collation
    if (idx_info.nConstraint > 0) {
        int iCons = 0;
        if (Size > 2) iCons = Data[2] % idx_info.nConstraint;
        const char *collation = sqlite3_vtab_collation(&idx_info, iCons);
        (void)collation; // Use result to avoid unused variable warning
    }

    // Test sqlite3_vtab_rhs_value
    if (idx_info.nConstraint > 0) {
        int iCons = 0;
        if (Size > 3) iCons = Data[3] % idx_info.nConstraint;
        sqlite3_value *pVal = NULL;
        sqlite3_vtab_rhs_value(&idx_info, iCons, &pVal);
        // pVal is owned by SQLite, don't free it
    }

    // Test sqlite3_vtab_distinct
    int distinct_result = sqlite3_vtab_distinct(&idx_info);
    (void)distinct_result; // Use result to avoid unused variable warning

    // Test sqlite3_overload_function
    if (Size > 4) {
        // Create a function name from fuzzer data (limited to safe length)
        char func_name[256] = {0};
        size_t name_len = Size - 4;
        if (name_len > 255) name_len = 255;
        memcpy(func_name, Data + 4, name_len);
        func_name[name_len] = '\0';
        
        // Test with various argument counts
        int nArg = 0;
        if (Size > 5) nArg = Data[5] % 10;
        sqlite3_overload_function(db, func_name, nArg);
        
        // Also test with empty function name
        sqlite3_overload_function(db, "", 1);
    }

    // Test sqlite3_vtab_config with various operations
    // Use some common SQLITE_VTAB_* constants
    if (Size > 6) {
        int op = Data[6] % 10; // Use modulo to get reasonable op values
        
        // Test with different numbers of additional arguments
        switch (op % 4) {
            case 0:
                sqlite3_vtab_config(db, op);
                break;
            case 1:
                sqlite3_vtab_config(db, op, (void*)Data);
                break;
            case 2:
                sqlite3_vtab_config(db, op, (void*)Data, (void*)(Data + 1));
                break;
            case 3:
                sqlite3_vtab_config(db, op, (void*)Data, (void*)(Data + 1), (void*)(Data + 2));
                break;
        }
    }

    // Clean up allocated memory
    if (idx_info.aConstraint) free(idx_info.aConstraint);
    if (idx_info.aConstraintUsage) free(idx_info.aConstraintUsage);
    if (idx_info.aOrderBy) free(idx_info.aOrderBy);
    
    // Close database connection
    sqlite3_close(db);
    
    return 0;
}