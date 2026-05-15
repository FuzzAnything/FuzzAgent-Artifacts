// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void dummy_free(void *ptr) {
    // Do nothing - used for sqlite3_bind_text
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Initialize database and statements
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare some statements for testing
    const char *sql1 = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT)";
    const char *sql2 = "INSERT INTO test VALUES (?, ?)";
    const char *sql3 = "SELECT * FROM test";
    
    // Prepare three statements
    sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL);
    sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    sqlite3_prepare_v2(db, sql3, -1, &stmt3, NULL);
    
    // Use fuzzer data for various operations
    char *input_copy = NULL;
    if (Size > 0) {
        input_copy = (char*)malloc(Size + 1);
        if (input_copy) {
            memcpy(input_copy, Data, Size);
            input_copy[Size] = '\0';
        }
    }
    
    // 1. sqlite3_column_text - call on stmt3 (SELECT statement)
    // Need to step first to get a row, but we'll call it anyway
    const unsigned char *col_text = sqlite3_column_text(stmt3, 0);
    (void)col_text; // Ignore result
    
    // 2. sqlite3_snprintf - first call
    char buffer1[256];
    const char *format1 = "Test: %s";
    if (input_copy) {
        sqlite3_snprintf(sizeof(buffer1), buffer1, format1, input_copy);
    } else {
        sqlite3_snprintf(sizeof(buffer1), buffer1, "Test: no data");
    }
    
    // 3. sqlite3_snprintf - second call
    char buffer2[256];
    int int_value = (Size > 0) ? (int)Data[0] : 0;
    sqlite3_snprintf(sizeof(buffer2), buffer2, "Size: %d, First: %d", (int)Size, int_value);
    
    // 4. sqlite3_bind_text - bind to stmt2 (INSERT statement)
    if (input_copy && Size > 0) {
        // Use first 100 bytes or less for binding
        int bind_size = (Size > 100) ? 100 : (int)Size;
        sqlite3_bind_text(stmt2, 2, input_copy, bind_size, dummy_free);
    } else {
        sqlite3_bind_text(stmt2, 2, "fuzz", 4, SQLITE_STATIC);
    }
    sqlite3_bind_int(stmt2, 1, (int)Size);
    
    // 5. sqlite3_step - execute stmt1 (CREATE TABLE)
    sqlite3_step(stmt1);
    
    // 6. sqlite3_reset - reset stmt2
    sqlite3_reset(stmt2);
    
    // 7. sqlite3_sql - get SQL text
    const char *sql_text1 = sqlite3_sql(stmt1);
    const char *sql_text2 = sqlite3_sql(stmt2);
    const char *sql_text3 = sqlite3_sql(stmt3);
    (void)sql_text1;
    (void)sql_text2;
    (void)sql_text3;
    
    // 8, 9, 10. sqlite3_finalize - finalize all three statements
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmt3);
    
    // Cleanup
    if (input_copy) free(input_copy);
    if (db) sqlite3_close(db);
    
    return 0;
}