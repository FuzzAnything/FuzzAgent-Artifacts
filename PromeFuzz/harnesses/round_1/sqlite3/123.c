// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
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
    char* err_msg = NULL;
    sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
}

static void setup_database(sqlite3** db) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    execute_sql(*db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);");
    execute_sql(*db, "CREATE TABLE IF NOT EXISTS test2 (id INTEGER, value INTEGER);");
}

static void cleanup_database(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

static void perform_operations(sqlite3* db, const uint8_t* data, size_t size) {
    if (!db) return;
    
    if (size > 0) {
        int op_type = data[0] % 6;
        
        switch (op_type) {
            case 0:
                execute_sql(db, "INSERT INTO test (data) VALUES ('fuzz');");
                break;
            case 1:
                execute_sql(db, "UPDATE test SET data = 'updated' WHERE id = 1;");
                break;
            case 2:
                execute_sql(db, "DELETE FROM test WHERE id = 1;");
                break;
            case 3:
                execute_sql(db, "INSERT INTO test2 (id, value) VALUES (1, 100);");
                break;
            case 4:
                execute_sql(db, "UPDATE test2 SET value = 200 WHERE id = 1;");
                break;
            case 5:
                execute_sql(db, "DELETE FROM test2 WHERE id = 1;");
                break;
        }
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *data, size_t size) {
    sqlite3* db = NULL;
    
    setup_database(&db);
    if (!db) {
        return 0;
    }
    
    perform_operations(db, data, size);
    
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    int changes = sqlite3_changes(db);
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    int total_changes = sqlite3_total_changes(db);
    
    if (size > 8) {
        sqlite3_int64 set_rowid;
        memcpy(&set_rowid, data, sizeof(sqlite3_int64));
        sqlite3_set_last_insert_rowid(db, set_rowid);
        
        sqlite3_int64 new_last_rowid = sqlite3_last_insert_rowid(db);
        (void)new_last_rowid;
    }
    
    (void)changes64;
    (void)total_changes64;
    (void)changes;
    (void)last_rowid;
    (void)total_changes;
    
    cleanup_database(db);
    
    return 0;
}