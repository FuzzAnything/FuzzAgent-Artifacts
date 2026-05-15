// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *dummy_file = "./dummy_file";
    FILE *fp = NULL;
    
    // Write fuzzer data to dummy file
    fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table for testing
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Test sqlite3_exec with fuzzer data as SQL
    if (Size > 0) {
        char *sql = (char*)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql);
        }
    }
    
    // Prepare a statement for sqlite3_step testing
    const char *insert_sql = "INSERT INTO test VALUES(?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind some values
        sqlite3_bind_int(stmt, 1, 1);
        sqlite3_bind_text(stmt, 2, "test", -1, SQLITE_STATIC);
        
        // Test sqlite3_step
        int step_result = sqlite3_step(stmt);
        while (step_result == SQLITE_ROW) {
            step_result = sqlite3_step(stmt);
        }
        
        sqlite3_reset(stmt);
        
        // Bind different values for another test
        if (Size > 0) {
            sqlite3_bind_int(stmt, 1, (int)(Data[0] % 100));
            if (Size > 1) {
                char text_buf[256];
                size_t text_len = Size > 255 ? 255 : Size;
                memcpy(text_buf, Data, text_len);
                text_buf[text_len] = '\0';
                sqlite3_bind_text(stmt, 2, text_buf, -1, SQLITE_STATIC);
            } else {
                sqlite3_bind_text(stmt, 2, "fuzz", -1, SQLITE_STATIC);
            }
            
            step_result = sqlite3_step(stmt);
            while (step_result == SQLITE_ROW) {
                step_result = sqlite3_step(stmt);
            }
        }
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Test changes functions after INSERT
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    int changes = sqlite3_changes(db);
    
    // Execute an UPDATE to test changes again
    sqlite3_exec(db, "UPDATE test SET name = 'updated' WHERE id = 1;", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    changes64 = sqlite3_changes64(db);
    changes = sqlite3_changes(db);
    
    // Execute a DELETE
    sqlite3_exec(db, "DELETE FROM test WHERE id = 1;", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    changes64 = sqlite3_changes64(db);
    changes = sqlite3_changes(db);
    
    // Test total changes functions
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    int total_changes = sqlite3_total_changes(db);
    
    // Prepare and step through a SELECT statement
    const char *select_sql = "SELECT * FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        int step_result = sqlite3_step(stmt);
        while (step_result == SQLITE_ROW) {
            // Access column data to ensure full execution
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *name = sqlite3_column_text(stmt, 1);
            step_result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with empty or NULL SQL in sqlite3_exec
    sqlite3_exec(db, "", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    sqlite3_exec(db, NULL, NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Test changes functions with no recent changes
    changes64 = sqlite3_changes64(db);
    changes = sqlite3_changes(db);
    total_changes64 = sqlite3_total_changes64(db);
    total_changes = sqlite3_total_changes(db);
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    // Remove dummy file
    remove(dummy_file);
    
    return 0;
}