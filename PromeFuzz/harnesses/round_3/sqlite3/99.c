// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
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

static void setup_database(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER);";
    sqlite3_exec(*db, create_table, NULL, NULL, NULL);
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static void perform_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db) return;
    
    char *err_msg = NULL;
    
    if (size > 0) {
        int op_type = data[0] % 4;
        
        switch (op_type) {
            case 0: {
                char insert_sql[256];
                int id_val = (size > 1) ? data[1] : 1;
                int value_val = (size > 2) ? data[2] : 100;
                snprintf(insert_sql, sizeof(insert_sql),
                         "INSERT INTO test_table (name, value) VALUES ('test%d', %d);",
                         id_val, value_val);
                sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg);
                break;
            }
            case 1: {
                char update_sql[256];
                int value_val = (size > 1) ? data[1] : 200;
                snprintf(update_sql, sizeof(update_sql),
                         "UPDATE test_table SET value = %d WHERE id = 1;",
                         value_val);
                sqlite3_exec(db, update_sql, NULL, NULL, &err_msg);
                break;
            }
            case 2: {
                sqlite3_exec(db, "DELETE FROM test_table WHERE id = 1;", 
                            NULL, NULL, &err_msg);
                break;
            }
            case 3: {
                char insert_replace_sql[256];
                int id_val = (size > 1) ? data[1] : 2;
                int value_val = (size > 2) ? data[2] : 300;
                snprintf(insert_replace_sql, sizeof(insert_replace_sql),
                         "INSERT OR REPLACE INTO test_table (id, name, value) "
                         "VALUES (%d, 'replace%d', %d);",
                         id_val, id_val, value_val);
                sqlite3_exec(db, insert_replace_sql, NULL, NULL, &err_msg);
                break;
            }
        }
        
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    
    setup_database(&db);
    if (!db) {
        return 0;
    }
    
    perform_operations(db, Data, Size);
    
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid;
    
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64;
    
    int changes = sqlite3_changes(db);
    (void)changes;
    
    if (Size > 0) {
        sqlite3_int64 set_rowid = 0;
        for (size_t i = 0; i < (Size < 8 ? Size : 8); i++) {
            set_rowid = (set_rowid << 8) | Data[i];
        }
        sqlite3_set_last_insert_rowid(db, set_rowid);
        
        sqlite3_int64 verify_rowid = sqlite3_last_insert_rowid(db);
        (void)verify_rowid;
    }
    
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    (void)total_changes64;
    
    cleanup_database(db);
    
    return 0;
}