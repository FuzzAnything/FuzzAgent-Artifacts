// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static int create_dummy_statement(sqlite3 **db, sqlite3_stmt **stmt) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        return 0;
    }
    
    const char *sql = "SELECT :param1, ?2, @param3, $param4, ?;";
    if (sqlite3_prepare_v2(*db, sql, -1, stmt, NULL) != SQLITE_OK) {
        sqlite3_close(*db);
        return 0;
    }
    return 1;
}

static void cleanup(sqlite3 *db, sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    if (!create_dummy_statement(&db, &stmt)) {
        return 0;
    }
    
    if (Size == 0) {
        cleanup(db, stmt);
        return 0;
    }
    
    // Extract parameter name from fuzzer input
    size_t name_len = Size > 100 ? 100 : Size;
    char *param_name = (char *)malloc(name_len + 1);
    if (!param_name) {
        cleanup(db, stmt);
        return 0;
    }
    memcpy(param_name, Data, name_len);
    param_name[name_len] = '\0';
    
    // Test sqlite3_bind_parameter_index
    int param_index = sqlite3_bind_parameter_index(stmt, param_name);
    
    // Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Test sqlite3_bind_parameter_name with various indices
    for (int i = 0; i <= param_count + 1; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        (void)name; // Use variable to avoid unused warning
    }
    
    // Test binding functions with various indices and values
    if (param_count > 0) {
        // Derive values from fuzzer input
        double dbl_val = Size > 0 ? (double)Data[0] / 255.0 : 0.0;
        int int_val = Size > 1 ? (int)Data[1] : 0;
        sqlite3_int64 int64_val = Size > 2 ? (sqlite3_int64)Data[2] : 0;
        
        // Try binding with valid and invalid indices
        for (int i = -1; i <= param_count + 1; i++) {
            sqlite3_bind_double(stmt, i, dbl_val);
            sqlite3_bind_int(stmt, i, int_val);
            sqlite3_bind_int64(stmt, i, int64_val);
        }
        
        // Try binding with the index found from parameter name
        if (param_index > 0 && param_index <= param_count) {
            sqlite3_bind_double(stmt, param_index, dbl_val);
            sqlite3_bind_int(stmt, param_index, int_val);
            sqlite3_bind_int64(stmt, param_index, int64_val);
        }
    }
    
    free(param_name);
    cleanup(db, stmt);
    return 0;
}