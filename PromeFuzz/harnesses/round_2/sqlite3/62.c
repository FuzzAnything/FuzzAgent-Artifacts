// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Use fuzzer data to determine parameters
    int op = Data[0] % 10;
    int iCons = Data[1] % 10;
    int bHandle = (Data[2] % 3) - 1;
    int nArg = Data[3] % 10;
    
    // Extract a function name from fuzzer data
    char zFuncName[32];
    size_t funcNameLen = Size > 4 ? (Data[4] % 28) + 1 : 5;
    if (funcNameLen > 31) funcNameLen = 31;
    if (funcNameLen > Size - 5) funcNameLen = Size > 5 ? Size - 5 : 1;
    
    if (funcNameLen > 0 && Size >= 5 + funcNameLen) {
        memcpy(zFuncName, Data + 5, funcNameLen);
        zFuncName[funcNameLen] = '\0';
    } else {
        strcpy(zFuncName, "dummy_func");
    }
    
    // Test sqlite3_vtab_config
    // We can't set up proper VtabCtx context, but we can still call the function
    // to see how it handles invalid contexts
    sqlite3_vtab_config(db, op);
    sqlite3_vtab_config(db, op, 1);
    sqlite3_vtab_config(db, op, 1, 2);
    
    // Test sqlite3_vtab_in
    if (Size > 10) {
        // Create a minimal sqlite3_index_info structure
        struct sqlite3_index_info idx_info;
        struct sqlite3_index_constraint constraint;
        struct sqlite3_index_constraint *aConstraint[1];
        
        memset(&idx_info, 0, sizeof(idx_info));
        memset(&constraint, 0, sizeof(constraint));
        
        idx_info.nConstraint = 1;
        idx_info.aConstraint = &constraint;
        constraint.iColumn = Data[5] % 10;
        constraint.op = SQLITE_INDEX_CONSTRAINT_EQ;
        constraint.usable = 1;
        
        // Test with various bHandle values
        sqlite3_vtab_in(&idx_info, iCons, bHandle);
        sqlite3_vtab_in(&idx_info, iCons, 0);
        sqlite3_vtab_in(&idx_info, iCons, 1);
        sqlite3_vtab_in(&idx_info, iCons, -1);
    }
    
    // Test sqlite3_vtab_rhs_value
    if (Size > 15) {
        struct sqlite3_index_info idx_info2;
        struct sqlite3_index_constraint constraint2;
        
        memset(&idx_info2, 0, sizeof(idx_info2));
        memset(&constraint2, 0, sizeof(constraint2));
        
        idx_info2.nConstraint = 1;
        idx_info2.aConstraint = &constraint2;
        
        sqlite3_value *ppVal = NULL;
        sqlite3_vtab_rhs_value(&idx_info2, iCons % 5, &ppVal);
    }
    
    // Test sqlite3_vtab_in_first and sqlite3_vtab_in_next
    // These functions require valid sqlite3_value pointers from xFilter context
    // We can't create valid ones, but we can test with NULL to see error handling
    sqlite3_value *ppOut = NULL;
    
    // Test with NULL pointer
    sqlite3_vtab_in_first(NULL, &ppOut);
    sqlite3_vtab_in_next(NULL, &ppOut);
    
    // Test sqlite3_overload_function
    sqlite3_overload_function(db, zFuncName, nArg);
    
    // Test with different function names and argument counts
    if (Size > 20) {
        char altFuncName[32];
        size_t altLen = (Data[7] % 20) + 1;
        if (altLen > 31) altLen = 31;
        if (altLen > Size - 20) altLen = Size > 20 ? Size - 20 : 1;
        
        if (altLen > 0 && Size >= 20 + altLen) {
            memcpy(altFuncName, Data + 20, altLen);
            altFuncName[altLen] = '\0';
            sqlite3_overload_function(db, altFuncName, Data[8] % 10);
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif