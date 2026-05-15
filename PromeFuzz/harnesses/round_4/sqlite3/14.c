// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(void *ptr) {
    // Do nothing - just a placeholder for sqlite3_bind_text
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Initialize database and statements
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table for testing
    const char *create_table = "CREATE TABLE test (id INTEGER, name TEXT);";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);
    
    // Prepare some statements for testing
    const char *sql1 = "SELECT name FROM test WHERE id = ?";
    const char *sql2 = "INSERT INTO test VALUES (?, ?)";
    const char *sql3 = "SELECT * FROM test";
    
    // Prepare first statement
    if (sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare third statement
    if (sqlite3_prepare_v2(db, sql3, -1, &stmt3, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input to create a null-terminated string
    char *input_str = NULL;
    if (Size > 0) {
        input_str = (char*)malloc(Size + 1);
        if (input_str) {
            memcpy(input_str, Data, Size);
            input_str[Size] = '\0';
        }
    }
    
    // 1. sqlite3_column_text - try to get column text from stmt1 (no rows yet)
    if (stmt1) {
        const unsigned char *col_text = sqlite3_column_text(stmt1, 0);
        (void)col_text; // Use result to avoid unused variable warning
    }
    
    // 2. sqlite3_snprintf - format a string using fuzzer input
    char buffer1[256];
    if (input_str) {
        sqlite3_snprintf(sizeof(buffer1), buffer1, "Input: %s", input_str);
    } else {
        sqlite3_snprintf(sizeof(buffer1), buffer1, "No input");
    }
    
    // 3. sqlite3_snprintf - another format operation
    char buffer2[256];
    sqlite3_snprintf(sizeof(buffer2), buffer2, "Buffer1: %s", buffer1);
    
    // 4. sqlite3_bind_text - bind text to prepared statement
    if (stmt2 && input_str) {
        sqlite3_bind_text(stmt2, 2, input_str, -1, dummy_callback);
        sqlite3_bind_int(stmt2, 1, Size % 100); // Bind an integer parameter
    }
    
    // 5. sqlite3_step - execute the prepared statement
    if (stmt2) {
        sqlite3_step(stmt2);
    }
    
    // 6. sqlite3_reset - reset the prepared statement
    if (stmt2) {
        sqlite3_reset(stmt2);
    }
    
    // 7. sqlite3_sql - get the SQL text of prepared statements
    if (stmt1) {
        const char *sql_text1 = sqlite3_sql(stmt1);
        (void)sql_text1;
    }
    if (stmt2) {
        const char *sql_text2 = sqlite3_sql(stmt2);
        (void)sql_text2;
    }
    if (stmt3) {
        const char *sql_text3 = sqlite3_sql(stmt3);
        (void)sql_text3;
    }
    
    // 8. sqlite3_finalize - finalize first statement
    if (stmt1) {
        sqlite3_finalize(stmt1);
    }
    
    // 9. sqlite3_finalize - finalize second statement
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    
    // 10. sqlite3_finalize - finalize third statement
    if (stmt3) {
        sqlite3_finalize(stmt3);
    }
    
    // Cleanup
    if (input_str) {
        free(input_str);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}