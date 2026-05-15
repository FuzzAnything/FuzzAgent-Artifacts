// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_int(context, 42);
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_value *val = NULL;
    sqlite3_value *out_val = NULL;
    sqlite3_index_info idx_info = {0};
    struct sqlite3_index_constraint constraint = {0};
    struct sqlite3_index_constraint_usage constraint_usage = {0};
    struct sqlite3_index_orderby orderby = {0};
    int rc;
    
    // Initialize database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Test sqlite3_overload_function
    if (Size > 0) {
        char func_name[256];
        size_t func_len = Size < 255 ? Size : 255;
        memcpy(func_name, Data, func_len);
        func_name[func_len] = '\0';
        
        int nArg = (Size > 0) ? (Data[0] % 10) : 1;
        sqlite3_overload_function(db, func_name, nArg);
    }
    
    // Test sqlite3_vtab_config
    if (db) {
        // Use some valid opcodes from sqlite3.h
        int ops[] = {SQLITE_VTAB_CONSTRAINT_SUPPORT, SQLITE_VTAB_DIRECTONLY, 
                    SQLITE_VTAB_INNOCUOUS};
        for (int i = 0; i < 3 && i * sizeof(int) < Size; i++) {
            int op = ops[i % 3];
            sqlite3_vtab_config(db, op);
        }
    }
    
    // Create a dummy function to get a sqlite3_value
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "SELECT dummy_func()", -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            val = sqlite3_column_value(stmt, 0);
            
            // Test sqlite3_vtab_in_first and sqlite3_vtab_in_next
            if (val) {
                rc = sqlite3_vtab_in_first(val, &out_val);
                while (rc == SQLITE_OK && out_val) {
                    // Try to get the value
                    int val_type = sqlite3_value_type(out_val);
                    rc = sqlite3_vtab_in_next(val, &out_val);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test sqlite3_vtab_rhs_value
    if (Size >= sizeof(sqlite3_index_info)) {
        // Initialize minimal index_info structure
        idx_info.nConstraint = 1;
        idx_info.aConstraint = &constraint;
        idx_info.nOrderBy = 1;
        idx_info.aOrderBy = &orderby;
        idx_info.aConstraintUsage = &constraint_usage;
        
        constraint.iColumn = -1;  // ROWID
        constraint.op = SQLITE_INDEX_CONSTRAINT_EQ;
        constraint.usable = 1;
        
        orderby.iColumn = -1;
        orderby.desc = 0;
        
        sqlite3_value *rhs_val = NULL;
        rc = sqlite3_vtab_rhs_value(&idx_info, 0, &rhs_val);
        // rc will likely be SQLITE_NOTFOUND, but that's OK
    }
    
    // Test sqlite3_vtab_in
    if (Size >= sizeof(sqlite3_index_info)) {
        // Try different bHandle values
        int handles[] = {-1, 0, 1};
        for (int i = 0; i < 3; i++) {
            int result = sqlite3_vtab_in(&idx_info, 0, handles[i]);
            (void)result;  // Result may be 0 or 1
        }
    }
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}