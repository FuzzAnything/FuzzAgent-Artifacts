// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
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

static void execute_sql(sqlite3* db, const char* sql) {
    sqlite3_exec(db, sql, NULL, NULL, NULL);
}

static void create_test_table(sqlite3* db) {
    execute_sql(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, value TEXT);");
}

static void insert_test_data(sqlite3* db, int count) {
    for (int i = 0; i < count; i++) {
        execute_sql(db, "INSERT INTO test (value) VALUES ('test');");
    }
}

static void update_test_data(sqlite3* db, int count) {
    for (int i = 0; i < count; i++) {
        execute_sql(db, "UPDATE test SET value = 'updated' WHERE id = 1;");
    }
}

static void delete_test_data(sqlite3* db, int count) {
    for (int i = 0; i < count; i++) {
        execute_sql(db, "DELETE FROM test WHERE id = 1;");
    }
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create test table
    create_test_table(db);
    
    // Use fuzzer input to determine operations
    if (Size > 0) {
        // Use first byte to determine number of operations
        int num_ops = Data[0] % 10 + 1;
        
        // Perform INSERT operations
        insert_test_data(db, num_ops);
        
        // Call target functions after INSERT
        sqlite3_int64 changes64 = sqlite3_changes64(db);
        sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
        int changes = sqlite3_changes(db);
        sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
        int total_changes = sqlite3_total_changes(db);
        
        // Use fuzzer data to set last insert rowid
        if (Size > 8) {
            sqlite3_int64 custom_rowid;
            memcpy(&custom_rowid, Data + 1, sizeof(custom_rowid));
            sqlite3_set_last_insert_rowid(db, custom_rowid);
            
            // Verify the set value
            sqlite3_int64 verify_rowid = sqlite3_last_insert_rowid(db);
        }
        
        // Perform UPDATE operations
        update_test_data(db, num_ops);
        
        // Call target functions after UPDATE
        changes64 = sqlite3_changes64(db);
        total_changes64 = sqlite3_total_changes64(db);
        changes = sqlite3_changes(db);
        total_changes = sqlite3_total_changes(db);
        
        // Perform DELETE operations
        delete_test_data(db, num_ops);
        
        // Call target functions after DELETE
        changes64 = sqlite3_changes64(db);
        total_changes64 = sqlite3_total_changes64(db);
        changes = sqlite3_changes(db);
        total_changes = sqlite3_total_changes(db);
        
        // Test with empty table operations
        execute_sql(db, "DELETE FROM test;");
        
        // Call target functions on empty table
        changes64 = sqlite3_changes64(db);
        total_changes64 = sqlite3_total_changes64(db);
        changes = sqlite3_changes(db);
        last_rowid = sqlite3_last_insert_rowid(db);
        total_changes = sqlite3_total_changes(db);
        
        // Test with various SQL statements that shouldn't affect changes
        execute_sql(db, "SELECT * FROM test;");
        execute_sql(db, "BEGIN TRANSACTION;");
        execute_sql(db, "COMMIT;");
        
        // Call target functions after non-modifying statements
        changes64 = sqlite3_changes64(db);
        total_changes64 = sqlite3_total_changes64(db);
        changes = sqlite3_changes(db);
        total_changes = sqlite3_total_changes(db);
        
        // Test with CREATE TABLE AS SELECT (shouldn't count as changes)
        execute_sql(db, "CREATE TABLE test2 AS SELECT * FROM test;");
        
        // Call target functions after CREATE TABLE AS SELECT
        changes64 = sqlite3_changes64(db);
        total_changes64 = sqlite3_total_changes64(db);
        changes = sqlite3_changes(db);
        total_changes = sqlite3_total_changes(db);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}