// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
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
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db1 = NULL, *db2 = NULL, *db3 = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *tail = NULL;
    const unsigned char *text_result = NULL;
    
    // Create dummy file for database
    const char *filename = "./dummy_file";
    
    // Write fuzzer data to file if needed for database content
    // (In real fuzzing, we might use this data as the database file)
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // 1. sqlite3_blob_close - safe with NULL
    sqlite3_blob *blob = NULL;
    sqlite3_blob_close(blob);
    
    // 2. sqlite3_exec - needs a database, but we don't have one yet
    // We'll open a temporary database for this
    int rc = sqlite3_open(":memory:", &db1);
    if (rc == SQLITE_OK) {
        // Use fuzzer data as SQL command (truncated to null-terminated string)
        char *sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            sqlite3_exec(db1, sql, callback, 0, &errmsg);
            if (errmsg) sqlite3_free(errmsg);
            free(sql);
        }
    }
    
    // 3. sqlite3_close - close db1
    if (db1) sqlite3_close(db1);
    
    // 4. sqlite3_close - close another (NULL is safe)
    sqlite3_close(NULL);
    
    // 5. sqlite3_libversion - always available
    const char *version = sqlite3_libversion();
    (void)version; // Use version to avoid unused variable warning
    
    // 6. sqlite3_open - open database with fuzzer-provided filename or data
    rc = sqlite3_open(filename, &db2);
    
    // 7. sqlite3_prepare_v2 - prepare a statement
    if (rc == SQLITE_OK && db2) {
        // Use fuzzer data as SQL statement
        char *sql2 = (char *)malloc(Size + 1);
        if (sql2) {
            memcpy(sql2, Data, Size);
            sql2[Size] = '\0';
            // Limit SQL length to avoid excessive memory usage
            int sql_len = Size > 1000 ? 1000 : (int)Size;
            sqlite3_prepare_v2(db2, sql2, sql_len, &stmt, &tail);
            free(sql2);
        }
    }
    
    // 8. sqlite3_step - execute prepared statement
    if (stmt) {
        sqlite3_step(stmt);
    }
    
    // 9. sqlite3_column_text - get column text if available
    if (stmt) {
        text_result = sqlite3_column_text(stmt, 0);
        (void)text_result; // Use to avoid unused variable warning
    }
    
    // 10. sqlite3_finalize - finalize statement
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // 11. sqlite3_close - close database
    if (db2) {
        sqlite3_close(db2);
    }
    
    // Open and close another database to explore more states
    sqlite3_open(":memory:", &db3);
    if (db3) {
        // Try to use fuzzer data as table name or SQL fragment
        if (Size > 0) {
            char create_table[256];
            snprintf(create_table, sizeof(create_table), 
                    "CREATE TABLE t%d (a INT, b TEXT)", Data[0] % 100);
            sqlite3_exec(db3, create_table, NULL, NULL, NULL);
        }
        sqlite3_close(db3);
    }
    
    // Clean up dummy file
    remove(filename);
    
    return 0;
}