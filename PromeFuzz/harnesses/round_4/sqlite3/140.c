// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Simple function that returns the first argument as integer
    if (argc > 0) {
        sqlite3_result_int(ctx, sqlite3_value_int(argv[0]));
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Aggregate step function - does nothing
}

static void dummy_final(sqlite3_context *ctx) {
    // Aggregate final function
    sqlite3_result_int(ctx, 0);
}

static void dummy_destroy(void *p) {
    // Cleanup function for app data
    if (p) {
        free(p);
    }
}

static void update_hook_callback(void *arg, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
    // Dummy update hook callback
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a dummy table
    char *err_msg = NULL;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER PRIMARY KEY, val TEXT)", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Insert some data
    sqlite3_exec(db, "INSERT INTO t1(val) VALUES('test1'), ('test2'), ('123')", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Test sqlite3_create_function_v2
    // Use fuzz data to determine function name and parameters
    char func_name[256];
    size_t func_name_len = Size > 255 ? 255 : Size;
    if (func_name_len > 0) {
        memcpy(func_name, Data, func_name_len);
        func_name[func_name_len] = '\0';
    } else {
        strcpy(func_name, "dummy_func");
    }
    
    // Allocate some app data
    void *app_data = malloc(16);
    if (app_data) {
        memset(app_data, 0xAA, 16);
    }
    
    // Try different encodings based on fuzz data
    int encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16};
    int encoding_idx = Data[0] % 4;
    
    // Try different argument counts
    int nArg = (Data[0] % 8) - 1; // -1 to 6
    
    rc = sqlite3_create_function_v2(
        db,
        func_name,
        nArg,
        encodings[encoding_idx] | ((Data[0] & 0x80) ? SQLITE_DETERMINISTIC : 0),
        app_data,
        dummy_func,
        dummy_step,
        dummy_final,
        dummy_destroy
    );
    
    // Test sqlite3_create_function (without destroy callback)
    rc = sqlite3_create_function(
        db,
        "another_func",
        nArg,
        encodings[encoding_idx],
        NULL,
        dummy_func,
        NULL,
        NULL
    );
    
    // Test sqlite3_overload_function
    rc = sqlite3_overload_function(db, "overloaded_func", nArg);
    
    // Test sqlite3_update_hook
    void *hook_arg = malloc(8);
    if (hook_arg) {
        memset(hook_arg, 0xBB, 8);
    }
    void *prev_hook = sqlite3_update_hook(db, update_hook_callback, hook_arg);
    
    // Execute some SQL to trigger hooks and functions
    sqlite3_exec(db, "INSERT INTO t1(val) VALUES('fuzz')", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    sqlite3_exec(db, "UPDATE t1 SET val = 'updated' WHERE id = 1", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    sqlite3_exec(db, "DELETE FROM t1 WHERE id = 2", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Call the registered functions
    sqlite3_exec(db, "SELECT dummy_func(42)", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    sqlite3_exec(db, "SELECT another_func(123)", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Create a prepared statement to test sqlite3_value_numeric_type
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT id, val FROM t1", -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the second column value
            sqlite3_value *value = sqlite3_column_value(stmt, 1);
            if (value) {
                // Test sqlite3_value_numeric_type
                int numeric_type = sqlite3_value_numeric_type(value);
                (void)numeric_type; // Use result to avoid unused warning
                
                // Test sqlite3_context_db_handle by creating a dummy context
                // Note: We need to create a function context to test this
                // For fuzzing purposes, we'll create a minimal context
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    // Clean up
    sqlite3_update_hook(db, NULL, NULL); // Disable hook
    if (hook_arg) {
        free(hook_arg);
    }
    
    // The app_data will be freed by dummy_destroy when function is removed
    // Remove the function to trigger destroy callback
    sqlite3_create_function_v2(db, func_name, nArg, encodings[encoding_idx], NULL, NULL, NULL, NULL, NULL);
    
    sqlite3_close(db);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}