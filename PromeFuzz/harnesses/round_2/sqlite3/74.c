// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *dummy_file = "./dummy_file";
    
    // Open an in-memory database for testing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table and perform some operations to generate changes
    char *err_msg = NULL;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Use fuzzer input to generate various SQL operations
    if (Size > 0) {
        // Insert some data
        sqlite3_exec(db, "INSERT INTO test_table (data) VALUES ('test1');", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO test_table (data) VALUES ('test2');", NULL, NULL, NULL);
        
        // Update some data
        sqlite3_exec(db, "UPDATE test_table SET data = 'updated' WHERE id = 1;", NULL, NULL, NULL);
        
        // Delete some data
        sqlite3_exec(db, "DELETE FROM test_table WHERE id = 2;", NULL, NULL, NULL);
    }
    
    // Test sqlite3_total_changes
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    // Test sqlite3_total_changes64
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    (void)total_changes64;
    
    // Test sqlite3_changes64
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64;
    
    // Test sqlite3_changes
    int changes = sqlite3_changes(db);
    (void)changes;
    
    // Test sqlite3_set_last_insert_rowid and sqlite3_last_insert_rowid
    if (Size > 0) {
        // Set a custom rowid value
        sqlite3_int64 custom_rowid = (sqlite3_int64)Data[0];
        if (Size > 8) {
            memcpy(&custom_rowid, Data, sizeof(custom_rowid) > Size ? Size : sizeof(custom_rowid));
        }
        sqlite3_set_last_insert_rowid(db, custom_rowid);
        
        // Get the last insert rowid
        sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
        (void)last_rowid;
        
        // Perform another insert to get a real rowid
        sqlite3_exec(db, "INSERT INTO test_table (data) VALUES ('test3');", NULL, NULL, NULL);
        
        // Get the real last insert rowid
        last_rowid = sqlite3_last_insert_rowid(db);
        (void)last_rowid;
        
        // Set another custom value
        sqlite3_set_last_insert_rowid(db, custom_rowid + 1);
        last_rowid = sqlite3_last_insert_rowid(db);
        (void)last_rowid;
    }
    
    // Create a file-based database if we have enough data
    if (Size > 10) {
        FILE *fp = fopen(dummy_file, "wb");
        if (fp) {
            fwrite(Data, 1, Size > 100 ? 100 : Size, fp);
            fclose(fp);
            
            // Open the file database
            sqlite3 *file_db = NULL;
            rc = sqlite3_open(dummy_file, &file_db);
            if (rc == SQLITE_OK && file_db) {
                // Test the functions on file database
                int file_total_changes = sqlite3_total_changes(file_db);
                (void)file_total_changes;
                
                sqlite3_int64 file_total_changes64 = sqlite3_total_changes64(file_db);
                (void)file_total_changes64;
                
                sqlite3_int64 file_changes64 = sqlite3_changes64(file_db);
                (void)file_changes64;
                
                int file_changes = sqlite3_changes(file_db);
                (void)file_changes;
                
                sqlite3_int64 file_last_rowid = sqlite3_last_insert_rowid(file_db);
                (void)file_last_rowid;
                
                sqlite3_set_last_insert_rowid(file_db, 999);
                file_last_rowid = sqlite3_last_insert_rowid(file_db);
                (void)file_last_rowid;
                
                sqlite3_close(file_db);
            }
            
            // Remove the dummy file
            remove(dummy_file);
        }
    }
    
    // Perform more operations to test changes after additional modifications
    if (Size > 1) {
        // Insert more data
        sqlite3_exec(db, "INSERT INTO test_table (data) VALUES ('more_data');", NULL, NULL, NULL);
        
        // Test changes after insert
        changes = sqlite3_changes(db);
        (void)changes;
        changes64 = sqlite3_changes64(db);
        (void)changes64;
        
        // Test total changes after more operations
        total_changes = sqlite3_total_changes(db);
        (void)total_changes;
        total_changes64 = sqlite3_total_changes64(db);
        (void)total_changes64;
        
        // Update with WHERE clause that may affect 0 rows
        sqlite3_exec(db, "UPDATE test_table SET data = 'no_change' WHERE id = 999;", NULL, NULL, NULL);
        
        // Test changes after update with no matching rows
        changes = sqlite3_changes(db);
        (void)changes;
        changes64 = sqlite3_changes64(db);
        (void)changes64;
    }
    
    // Test with triggers if we have enough data
    if (Size > 20) {
        // Create a table with trigger
        sqlite3_exec(db, 
            "CREATE TABLE IF NOT EXISTS main_table (id INTEGER PRIMARY KEY, value INTEGER);"
            "CREATE TABLE IF NOT EXISTS log_table (id INTEGER PRIMARY KEY, operation TEXT, old_value INTEGER, new_value INTEGER);"
            "CREATE TRIGGER IF NOT EXISTS log_update AFTER UPDATE ON main_table "
            "BEGIN "
            "  INSERT INTO log_table (operation, old_value, new_value) VALUES ('UPDATE', OLD.value, NEW.value);"
            "END;", NULL, NULL, NULL);
        
        // Insert into main table
        sqlite3_exec(db, "INSERT INTO main_table (value) VALUES (100);", NULL, NULL, NULL);
        
        // Update to trigger the trigger
        sqlite3_exec(db, "UPDATE main_table SET value = 200 WHERE id = 1;", NULL, NULL, NULL);
        
        // Test changes and total changes after trigger execution
        changes = sqlite3_changes(db);
        (void)changes;
        total_changes = sqlite3_total_changes(db);
        (void)total_changes;
        
        // Test the 64-bit versions
        changes64 = sqlite3_changes64(db);
        (void)changes64;
        total_changes64 = sqlite3_total_changes64(db);
        (void)total_changes64;
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}