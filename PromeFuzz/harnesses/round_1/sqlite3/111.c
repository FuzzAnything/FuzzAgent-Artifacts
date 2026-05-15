// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:157041:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    // Initialize database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Create dummy sqlite3_index_info structure
    struct sqlite3_index_info idx_info;
    memset(&idx_info, 0, sizeof(idx_info));

    // Use fuzzer data to populate structure fields
    if (Size >= sizeof(idx_info)) {
        memcpy(&idx_info, Data, sizeof(idx_info));
        Data += sizeof(idx_info);
        Size -= sizeof(idx_info);
    }

    // Test sqlite3_vtab_config with various operations
    if (Size >= 4) {
        int op = *(const int*)Data;
        Data += 4;
        Size -= 4;
        
        // Test with different numbers of additional arguments
        sqlite3_vtab_config(db, op);
        sqlite3_vtab_config(db, op, 0);
        sqlite3_vtab_config(db, op, 0, 0);
    }

    // Test sqlite3_vtab_distinct
    if (Size >= 1) {
        // Set some fields to make the structure look more valid
        idx_info.nOrderBy = 1;
        idx_info.aOrderBy = malloc(sizeof(struct sqlite3_index_orderby));
        if (idx_info.aOrderBy) {
            idx_info.aOrderBy[0].iColumn = 0;
            idx_info.aOrderBy[0].desc = 0;
        }
        
        int distinct_val = sqlite3_vtab_distinct(&idx_info);
        (void)distinct_val;
        
        free(idx_info.aOrderBy);
        idx_info.aOrderBy = NULL;
    }

    // Test sqlite3_vtab_collation
    if (Size >= 4) {
        int iCons = *(const int*)Data % 10; // Limit to reasonable range
        Data += 4;
        Size -= 4;
        
        const char *collation = sqlite3_vtab_collation(&idx_info, iCons);
        (void)collation;
    }

    // Test sqlite3_vtab_in
    if (Size >= 8) {
        int iCons = *(const int*)Data % 10;
        Data += 4;
        int bHandle = *(const int*)Data;
        Data += 4;
        Size -= 8;
        
        int in_result = sqlite3_vtab_in(&idx_info, iCons, bHandle);
        (void)in_result;
    }

    // Test sqlite3_vtab_rhs_value
    if (Size >= 4) {
        int iCons = *(const int*)Data % 10;
        Data += 4;
        Size -= 4;
        
        sqlite3_value *val = NULL;
        int rhs_result = sqlite3_vtab_rhs_value(&idx_info, iCons, &val);
        (void)rhs_result;
        (void)val;
    }

    // Test sqlite3_overload_function
    if (Size >= 8) {
        // Extract function name from data
        size_t name_len = Size > 100 ? 100 : Size;
        char *func_name = malloc(name_len + 1);
        if (func_name) {
            memcpy(func_name, Data, name_len);
            func_name[name_len] = '\0';
            Data += name_len;
            Size -= name_len;
            
            if (Size >= 4) {
                int nArg = *(const int*)Data;
                Data += 4;
                Size -= 4;
                
                int overload_result = sqlite3_overload_function(db, func_name, nArg);
                (void)overload_result;
            }
            
            free(func_name);
        }
    }

    // Cleanup
    sqlite3_close(db);
    return 0;
}