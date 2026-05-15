// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite with protected sqlite3_value objects
    if (argc > 0) {
        sqlite3_value *value = argv[0];
        
        // Test all the target functions on the protected value
        double d = sqlite3_value_double(value);
        (void)d;
        
        const void *text16be = sqlite3_value_text16be(value);
        (void)text16be;
        
        const void *text16le = sqlite3_value_text16le(value);
        (void)text16le;
        
        const void *text16 = sqlite3_value_text16(value);
        (void)text16;
        
        const void *blob = sqlite3_value_blob(value);
        (void)blob;
        
        // Test sqlite3_result_value
        sqlite3_result_value(ctx, value);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *err_msg = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Register our test function
    if (sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                                test_function, NULL, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare a statement that calls our function with the fuzzer data
    const char *sql = "SELECT test_func(?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Bind the fuzzer data as a blob parameter
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1000 ? 1000 : Size), SQLITE_STATIC);
    } else {
        // Bind empty blob for zero-sized input
        sqlite3_bind_zeroblob(stmt, 1, 0);
    }
    
    // Execute the statement - this will call our test_function with a protected value
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        // Ignore errors - they're expected with random input
    }
    
    // Also test with text input
    sqlite3_reset(stmt);
    if (Size > 0) {
        // Create null-terminated string from fuzzer data
        char *text = (char *)malloc(Size + 1);
        if (text) {
            memcpy(text, Data, Size);
            text[Size] = '\0';
            sqlite3_bind_text(stmt, 1, text, -1, SQLITE_TRANSIENT);
            
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
                // Ignore errors
            }
            
            free(text);
        }
    }
    
    // Test with integer input
    sqlite3_reset(stmt);
    int int_val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&int_val, Data, sizeof(int));
    }
    sqlite3_bind_int(stmt, 1, int_val);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        // Ignore errors
    }
    
    // Test with double input
    sqlite3_reset(stmt);
    double double_val = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&double_val, Data, sizeof(double));
    }
    sqlite3_bind_double(stmt, 1, double_val);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        // Ignore errors
    }
    
    // Create a dummy file and write fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 100 ? 100 : Size, fp);
        }
        fclose(fp);
    }
    
cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}