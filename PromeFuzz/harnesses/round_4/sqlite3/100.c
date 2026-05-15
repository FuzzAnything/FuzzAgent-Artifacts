// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static void test_value_functions(sqlite3_value *value) {
    // Call all target API functions on the protected sqlite3_value
    int bytes = sqlite3_value_bytes(value);
    int nochange = sqlite3_value_nochange(value);
    const void *blob = sqlite3_value_blob(value);
    int bytes16 = sqlite3_value_bytes16(value);
    int int_val = sqlite3_value_int(value);
    int type = sqlite3_value_type(value);
    
    // Use the results to avoid dead code elimination
    (void)bytes;
    (void)nochange;
    (void)blob;
    (void)bytes16;
    (void)int_val;
    (void)type;
}

static sqlite3_value *create_protected_value(const uint8_t *data, size_t size) {
    // Create a minimal database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a simple table
    char *err_msg = NULL;
    if (sqlite3_exec(db, "CREATE TABLE t1(x);", NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    // Prepare an INSERT statement with a parameter
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, "INSERT INTO t1 VALUES(?);", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind the fuzzer data as different types based on size and content
    if (size == 0) {
        sqlite3_bind_null(stmt, 1);
    } else if (size % 2 == 0) {
        // Bind as blob for even sizes
        sqlite3_bind_blob(stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_TRANSIENT);
    } else if (size % 3 == 0) {
        // Bind as text for sizes divisible by 3
        char *text = malloc(size + 1);
        if (text) {
            memcpy(text, data, size);
            text[size] = '\0';
            sqlite3_bind_text(stmt, 1, text, (int)(size > 1024 ? 1024 : size), free);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
    } else {
        // Bind as integer for other sizes
        int int_to_bind = 0;
        if (size >= sizeof(int)) {
            memcpy(&int_to_bind, data, sizeof(int));
        } else {
            for (size_t i = 0; i < size; i++) {
                int_to_bind = (int_to_bind << 8) | data[i];
            }
        }
        sqlite3_bind_int(stmt, 1, int_to_bind);
    }
    
    // Execute the statement
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    // Now prepare a SELECT statement to get a protected value
    sqlite3_stmt *select_stmt = NULL;
    if (sqlite3_prepare_v2(db, "SELECT x FROM t1;", -1, &select_stmt, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    // Step to get the value
    if (sqlite3_step(select_stmt) != SQLITE_ROW) {
        sqlite3_finalize(select_stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    // Get the value as a protected sqlite3_value using sqlite3_column_value
    sqlite3_value *unprotected_value = sqlite3_column_value(select_stmt, 0);
    
    // Create a protected value by duplicating it
    sqlite3_value *protected_value = sqlite3_value_dup(unprotected_value);
    
    // Cleanup
    sqlite3_finalize(select_stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected_value;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *data, size_t size) {
    // Limit input size to prevent excessive memory usage
    if (size > 4096) {
        return 0;
    }
    
    // Create a protected sqlite3_value from fuzzer input
    sqlite3_value *protected_value = create_protected_value(data, size);
    if (!protected_value) {
        return 0;
    }
    
    // Test all target functions with the protected value
    test_value_functions(protected_value);
    
    // Also test with NULL value
    test_value_functions(NULL);
    
    // Free the protected value
    sqlite3_value_free(protected_value);
    
    return 0;
}