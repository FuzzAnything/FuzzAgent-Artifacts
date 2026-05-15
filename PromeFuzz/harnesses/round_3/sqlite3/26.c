// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:170987:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }
    
    // Create a dummy file for database operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize variables
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    const char *tail = NULL;
    int rc;
    
    // 1. sqlite3_blob_close - safe with NULL
    sqlite3_blob_close(blob);
    
    // 2. sqlite3_exec - try with NULL database first
    char *sql_exec = NULL;
    if (Size > 0) {
        sql_exec = (char *)malloc(Size + 1);
        if (sql_exec) {
            memcpy(sql_exec, Data, Size);
            sql_exec[Size] = '\0';
            sqlite3_exec(NULL, sql_exec, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }
    }
    
    // 3. sqlite3_close - safe with NULL
    sqlite3_close(NULL);
    
    // 4. sqlite3_close - another NULL close
    sqlite3_close(NULL);
    
    // 5. sqlite3_libversion - always safe
    const char *version = sqlite3_libversion();
    (void)version;
    
    // 6. sqlite3_open - open the dummy file
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        // If open fails, we still need to try other operations
        db = NULL;
    }
    
    // 7. sqlite3_prepare_v2 - prepare a statement
    char *sql_prepare = NULL;
    if (Size > 0) {
        sql_prepare = (char *)malloc(Size + 1);
        if (sql_prepare && db) {
            memcpy(sql_prepare, Data, Size);
            sql_prepare[Size] = '\0';
            sqlite3_prepare_v2(db, sql_prepare, Size, &stmt, &tail);
        }
    }
    
    // 8. sqlite3_step - execute the prepared statement
    if (stmt) {
        sqlite3_step(stmt);
    }
    
    // 9. sqlite3_column_text - get column text if available
    if (stmt) {
        sqlite3_column_text(stmt, 0);
    }
    
    // 10. sqlite3_finalize - finalize the statement
    sqlite3_finalize(stmt);
    
    // 11. sqlite3_close - close the database
    sqlite3_close(db);
    
    // Try another sequence with different parameters
    // Open a second database
    rc = sqlite3_open(":memory:", &db2);
    if (rc == SQLITE_OK && db2) {
        // Try sqlite3_exec with valid database
        if (sql_exec) {
            sqlite3_exec(db2, sql_exec, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }
        
        // Try preparing a different statement
        sqlite3_prepare_v2(db2, "CREATE TABLE t1(a,b);", -1, &stmt, &tail);
        if (stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        // Close the second database
        sqlite3_close(db2);
    }
    
    // Clean up
    if (sql_exec) {
        free(sql_exec);
    }
    if (sql_prepare) {
        free(sql_prepare);
    }
    
    // Remove the dummy file
    remove("./dummy_file");
    
    return 0;
}