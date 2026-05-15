// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pre-update hook callback
static void preupdate_hook_callback(
    void *arg,
    sqlite3 *db,
    int op,
    const char *dbname,
    const char *tablename,
    sqlite3_int64 rowid_old,
    sqlite3_int64 rowid_new
) {
    // We'll call preupdate functions from here
    sqlite3_value *pVal = NULL;
    int rc;
    
    // Try to access old values (for UPDATE/DELETE)
    if (op == SQLITE_UPDATE || op == SQLITE_DELETE) {
        for (int i = 0; i < 3; i++) {
            rc = sqlite3_preupdate_old(db, i, &pVal);
            if (rc == SQLITE_OK && pVal != NULL) {
                // Call target functions on the value
                int type = sqlite3_value_type(pVal);
                int int_val = sqlite3_value_int(pVal);
                int nochange = sqlite3_value_nochange(pVal);
                (void)type;
                (void)int_val;
                (void)nochange;
            }
        }
    }
    
    // Try to access new values (for INSERT/UPDATE)
    if (op == SQLITE_INSERT || op == SQLITE_UPDATE) {
        for (int i = 0; i < 3; i++) {
            rc = sqlite3_preupdate_new(db, i, &pVal);
            if (rc == SQLITE_OK && pVal != NULL) {
                // Call target functions on the value
                int type = sqlite3_value_type(pVal);
                int int_val = sqlite3_value_int(pVal);
                int nochange = sqlite3_value_nochange(pVal);
                (void)type;
                (void)int_val;
                (void)nochange;
            }
        }
    }
}

// Custom SQL function to test sqlite3_value_type and sqlite3_value_int
static void test_value_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_value *val = argv[0];
        int type = sqlite3_value_type(val);
        int int_val = sqlite3_value_int(val);
        int nochange = sqlite3_value_nochange(val);
        
        // Return the type as result
        sqlite3_result_int(context, type + int_val + nochange);
    } else {
        sqlite3_result_null(context);
    }
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Initialize database once
    if (!initialized) {
        rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK) {
            return 0;
        }
        
        // Register pre-update hook
        sqlite3_preupdate_hook(db, preupdate_hook_callback, NULL);
        
        // Register custom function
        sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                               test_value_func, NULL, NULL);
        
        // Create test table
        const char *create_sql = 
            "CREATE TABLE test (id INTEGER PRIMARY KEY, "
            "name TEXT, value INTEGER, data BLOB);";
        rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        // Create trigger to test sqlite3_value_nochange
        const char *trigger_sql =
            "CREATE TRIGGER test_trigger AFTER UPDATE ON test "
            "BEGIN "
            "  SELECT test_func(NEW.name); "
            "END;";
        sqlite3_exec(db, trigger_sql, NULL, NULL, NULL);
        
        initialized = 1;
    }
    
    if (!db) {
        return 0;
    }
    
    // Create a dummy file if needed (though we use in-memory db)
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        size_t write_size = Size < 1024 ? Size : 1024;
        if (write_size > 0) {
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }
    
    // Test 1: Insert data using prepared statement
    const char *insert_sql = "INSERT INTO test (name, value, data) VALUES (?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        // Bind parameters based on input data
        char name_buf[256];
        size_t name_len = Size < 255 ? Size : 255;
        if (name_len > 0) {
            memcpy(name_buf, Data, name_len);
        }
        name_buf[name_len] = '\0';
        
        sqlite3_bind_text(stmt, 1, name_buf, -1, SQLITE_TRANSIENT);
        
        // Safely compute integer value without buffer overflow
        int int_val = 0;
        if (Size >= 2) {
            int_val = (int)(Data[0] * 256 + Data[1]);
        } else if (Size == 1) {
            int_val = (int)Data[0];
        }
        sqlite3_bind_int(stmt, 2, int_val);
        
        if (Size > 2) {
            size_t blob_size = Size - 2;
            if (blob_size > 1000) blob_size = 1000;
            sqlite3_bind_blob(stmt, 3, Data + 2, blob_size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Test 2: Query data and test sqlite3_column_value
    const char *select_sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_value *col_val = sqlite3_column_value(stmt, i);
                if (col_val) {
                    int type = sqlite3_value_type(col_val);
                    int int_val = sqlite3_value_int(col_val);
                    int nochange = sqlite3_value_nochange(col_val);
                    (void)type;
                    (void)int_val;
                    (void)nochange;
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test 3: Update data to trigger hooks and triggers
    const char *update_sql = "UPDATE test SET value = value + 1 WHERE id = 1";
    sqlite3_exec(db, update_sql, NULL, NULL, NULL);
    
    // Test 4: Delete data to trigger pre-update hook
    const char *delete_sql = "DELETE FROM test WHERE id = 1";
    sqlite3_exec(db, delete_sql, NULL, NULL, NULL);
    
    // Test 5: Call custom function directly
    const char *func_sql = "SELECT test_func(name) FROM test";
    rc = sqlite3_prepare_v2(db, func_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Function already called in callback
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}