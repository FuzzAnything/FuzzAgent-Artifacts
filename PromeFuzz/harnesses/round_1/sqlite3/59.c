// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void initialize_database(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        *db = NULL;
    } else {
        // Create a simple table to have something to query
        sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);", NULL, NULL, NULL);
        sqlite3_exec(*db, "INSERT OR IGNORE INTO test VALUES (1, 'test1');", NULL, NULL, NULL);
        sqlite3_exec(*db, "INSERT OR IGNORE INTO test VALUES (2, 'test2');", NULL, NULL, NULL);
    }
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3 *db = NULL;
    initialize_database(&db);
    if (!db) return 0;
    
    // Prepare a dummy file if needed (not used in this case)
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Ensure null termination for SQL string
    char *sql = malloc(Size + 1);
    if (!sql) {
        cleanup_database(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';
    
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Test sqlite3_prepare (legacy)
    int rc = sqlite3_prepare(db, sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        // Step through results if it's a SELECT statement
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < col_count; i++) {
                // Test sqlite3_column_name
                const char *col_name = sqlite3_column_name(stmt, i);
                (void)col_name; // Use to avoid unused variable warning
                
                // Test sqlite3_column_table_name
                const char *table_name = sqlite3_column_table_name(stmt, i);
                (void)table_name;
                
                // Test sqlite3_column_type
                int col_type = sqlite3_column_type(stmt, i);
                (void)col_type;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test sqlite3_prepare_v3 with different flags
    unsigned int prep_flags = 0;
    if (Size > 0) {
        // Use first byte to determine some flags (simplified)
        prep_flags = Data[0] % 4; // Limited to available flags
    }
    
    stmt = NULL;
    tail = NULL;
    rc = sqlite3_prepare_v3(db, sql, -1, prep_flags, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Test the same functions with v3 prepared statement
        int col_count = sqlite3_column_count(stmt);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < col_count; i++) {
                const char *col_name = sqlite3_column_name(stmt, i);
                (void)col_name;
                
                const char *table_name = sqlite3_column_table_name(stmt, i);
                (void)table_name;
                
                int col_type = sqlite3_column_type(stmt, i);
                (void)col_type;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(sql);
    cleanup_database(db);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}