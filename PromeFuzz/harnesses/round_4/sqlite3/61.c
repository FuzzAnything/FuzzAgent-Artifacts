// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzz_operations(sqlite3* db, const uint8_t* data, size_t size) {
    // Use fuzz data to influence operations
    if (size == 0) return;
    
    // Create a simple table if it doesn't exist
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS fuzz_table ("
        "id INTEGER PRIMARY KEY,"
        "value TEXT"
        ");";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);
    
    // Use first byte to decide operation type
    uint8_t op_type = data[0] % 6;
    sqlite3_int64 rowid_val = 0;
    
    if (size > 1) {
        // Use some bytes to form a rowid value
        for (size_t i = 0; i < 8 && i + 1 < size; i++) {
            rowid_val = (rowid_val << 8) | data[i + 1];
        }
    }
    
    switch (op_type) {
        case 0: {
            // Insert operation
            const char* insert = "INSERT INTO fuzz_table (value) VALUES ('fuzz_data');";
            sqlite3_exec(db, insert, NULL, NULL, NULL);
            break;
        }
        case 1: {
            // Update operation
            const char* update = "UPDATE fuzz_table SET value = 'updated' WHERE id = 1;";
            sqlite3_exec(db, update, NULL, NULL, NULL);
            break;
        }
        case 2: {
            // Delete operation
            const char* delete = "DELETE FROM fuzz_table WHERE id = 1;";
            sqlite3_exec(db, delete, NULL, NULL, NULL);
            break;
        }
        case 3: {
            // Multiple operations
            const char* multi = 
                "BEGIN;"
                "INSERT INTO fuzz_table (value) VALUES ('multi1');"
                "INSERT INTO fuzz_table (value) VALUES ('multi2');"
                "COMMIT;";
            sqlite3_exec(db, multi, NULL, NULL, NULL);
            break;
        }
        case 4: {
            // Replace operation
            const char* replace = 
                "CREATE TABLE IF NOT EXISTS fuzz_table2 ("
                "id INTEGER PRIMARY KEY,"
                "value TEXT UNIQUE"
                ");"
                "INSERT OR REPLACE INTO fuzz_table2 (id, value) VALUES (1, 'test');"
                "INSERT OR REPLACE INTO fuzz_table2 (id, value) VALUES (1, 'test2');";
            sqlite3_exec(db, replace, NULL, NULL, NULL);
            break;
        }
        case 5: {
            // Trigger operations (if we want to test trigger effects)
            const char* trigger = 
                "CREATE TABLE IF NOT EXISTS log (msg TEXT);"
                "CREATE TRIGGER IF NOT EXISTS fuzz_trigger AFTER INSERT ON fuzz_table "
                "BEGIN "
                "  INSERT INTO log VALUES ('inserted');"
                "END;"
                "INSERT INTO fuzz_table (value) VALUES ('trigger_test');";
            sqlite3_exec(db, trigger, NULL, NULL, NULL);
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Perform some database operations based on fuzz input
    fuzz_operations(db, Data, Size);
    
    // Call all target API functions in various combinations
    // 1. sqlite3_total_changes64
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    (void)total_changes64; // Use result to avoid unused variable warning
    
    // 2. sqlite3_last_insert_rowid
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid;
    
    // 3. sqlite3_total_changes
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    // 4. sqlite3_changes64
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64;
    
    // 5. sqlite3_set_last_insert_rowid
    // Use fuzz data to set a rowid value
    sqlite3_int64 set_rowid = 0;
    if (Size > 0) {
        for (size_t i = 0; i < 8 && i < Size; i++) {
            set_rowid = (set_rowid << 8) | Data[i];
        }
        sqlite3_set_last_insert_rowid(db, set_rowid);
        
        // Verify it was set
        sqlite3_int64 verify_rowid = sqlite3_last_insert_rowid(db);
        (void)verify_rowid;
    }
    
    // 6. sqlite3_changes
    int changes = sqlite3_changes(db);
    (void)changes;
    
    // Perform more operations and call APIs again
    if (Size > 1) {
        // Use second byte to decide additional operations
        uint8_t extra_op = Data[1] % 4;
        
        switch (extra_op) {
            case 0: {
                // Insert and check changes
                const char* insert = "INSERT INTO fuzz_table (value) VALUES ('extra');";
                sqlite3_exec(db, insert, NULL, NULL, NULL);
                int changes_after = sqlite3_changes(db);
                (void)changes_after;
                sqlite3_int64 changes64_after = sqlite3_changes64(db);
                (void)changes64_after;
                break;
            }
            case 1: {
                // Update and check total changes
                const char* update = "UPDATE fuzz_table SET value = 'changed' WHERE 1=1;";
                sqlite3_exec(db, update, NULL, NULL, NULL);
                int total_after = sqlite3_total_changes(db);
                (void)total_after;
                sqlite3_int64 total64_after = sqlite3_total_changes64(db);
                (void)total64_after;
                break;
            }
            case 2: {
                // Set rowid and verify
                sqlite3_int64 new_rowid = 999;
                if (Size > 2) {
                    new_rowid = Data[2];
                }
                sqlite3_set_last_insert_rowid(db, new_rowid);
                sqlite3_int64 check = sqlite3_last_insert_rowid(db);
                (void)check;
                break;
            }
            case 3: {
                // Multiple API calls in sequence
                sqlite3_int64 c1 = sqlite3_changes64(db);
                sqlite3_int64 c2 = sqlite3_total_changes64(db);
                sqlite3_int64 c3 = sqlite3_last_insert_rowid(db);
                (void)c1; (void)c2; (void)c3;
                
                const char* insert = "INSERT INTO fuzz_table (value) VALUES ('seq');";
                sqlite3_exec(db, insert, NULL, NULL, NULL);
                
                sqlite3_int64 c4 = sqlite3_changes64(db);
                sqlite3_int64 c5 = sqlite3_total_changes64(db);
                sqlite3_int64 c6 = sqlite3_last_insert_rowid(db);
                (void)c4; (void)c5; (void)c6;
                break;
            }
        }
    }
    
    // Test edge cases
    // Call APIs on empty/clean database state
    sqlite3* db2 = NULL;
    if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
        sqlite3_int64 clean_changes = sqlite3_changes64(db2);
        sqlite3_int64 clean_total = sqlite3_total_changes64(db2);
        sqlite3_int64 clean_rowid = sqlite3_last_insert_rowid(db2);
        (void)clean_changes; (void)clean_total; (void)clean_rowid;
        
        sqlite3_close(db2);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}