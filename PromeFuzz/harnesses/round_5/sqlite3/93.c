// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:78603:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

/* Helper function to create a protected sqlite3_value from fuzzer input */
static sqlite3_value* create_protected_value(const uint8_t *data, size_t size, int value_type) {
    /* Create a temporary in-memory database */
    sqlite3 *db = NULL;
    sqlite3_open(":memory:", &db);
    if (!db) return NULL;
    
    /* Create a simple table and insert a value */
    sqlite3_exec(db, "CREATE TABLE t1(x)", 0, 0, 0);
    
    /* Prepare a statement to bind our fuzzer data */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO t1 VALUES(?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    /* Bind based on requested value type */
    switch (value_type) {
        case SQLITE_BLOB:
            sqlite3_bind_blob(stmt, 1, data, size > 0 ? (int)size : 0, SQLITE_TRANSIENT);
            break;
        case SQLITE_TEXT:
            /* Ensure null termination for text */
            if (size > 0) {
                char *text = malloc(size + 1);
                if (text) {
                    memcpy(text, data, size);
                    text[size] = 0;
                    sqlite3_bind_text(stmt, 1, text, -1, free);
                } else {
                    sqlite3_bind_null(stmt, 1);
                }
            } else {
                sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
            }
            break;
        case SQLITE_INTEGER:
            if (size >= sizeof(int)) {
                int val = *(int*)data;
                sqlite3_bind_int(stmt, 1, val);
            } else {
                sqlite3_bind_int(stmt, 1, 0);
            }
            break;
        case SQLITE_FLOAT:
            if (size >= sizeof(double)) {
                double val = *(double*)data;
                sqlite3_bind_double(stmt, 1, val);
            } else {
                sqlite3_bind_double(stmt, 1, 0.0);
            }
            break;
        default:
            sqlite3_bind_null(stmt, 1);
            break;
    }
    
    /* Execute the insert */
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    /* Now create a protected value using sqlite3_vtab_rhs_value */
    /* First create a virtual table to get protected values */
    sqlite3_exec(db, "CREATE VIRTUAL TABLE vt1 USING fts3(x)", 0, 0, 0);
    
    /* Prepare a statement that will give us a protected value */
    sqlite3_prepare_v2(db, "SELECT x FROM t1", -1, &stmt, 0);
    sqlite3_step(stmt);
    
    /* Get the unprotected value from column */
    sqlite3_value *unprotected_val = sqlite3_column_value(stmt, 0);
    
    /* Duplicate it to create a protected value */
    sqlite3_value *protected_val = sqlite3_value_dup(unprotected_val);
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected_val;
}

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Test with different value types to explore more states */
    int value_types[] = {SQLITE_BLOB, SQLITE_TEXT, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_NULL};
    int num_types = sizeof(value_types) / sizeof(value_types[0]);
    
    for (int i = 0; i < num_types; i++) {
        sqlite3_value *val = create_protected_value(Data, Size, value_types[i]);
        if (!val) continue;
        
        /* Test all target API functions */
        int bytes16 = sqlite3_value_bytes16(val);
        (void)bytes16; /* Use result to avoid unused variable warning */
        
        const void *blob = sqlite3_value_blob(val);
        (void)blob;
        
        int bytes = sqlite3_value_bytes(val);
        (void)bytes;
        
        int type = sqlite3_value_type(val);
        (void)type;
        
        int int_val = sqlite3_value_int(val);
        (void)int_val;
        
        int from_bind = sqlite3_value_frombind(val);
        (void)from_bind;
        
        /* Free the protected value */
        sqlite3_value_free(val);
    }
    
    return 0;
}