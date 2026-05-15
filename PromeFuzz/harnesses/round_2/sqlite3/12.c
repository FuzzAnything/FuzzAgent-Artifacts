// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void dummy_callback(void *ptr) {
    // Do nothing - used for sqlite3_bind_text
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    char buffer1[256];
    char buffer2[256];
    const char *sql_text = NULL;
    int rc;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a simple table and insert some data
    const char *create_sql = "CREATE TABLE test (id INTEGER, name TEXT);"
                             "INSERT INTO test VALUES (1, 'test1');"
                             "INSERT INTO test VALUES (2, 'test2');";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    
    // Prepare a statement for SELECT
    const char *select_sql = "SELECT name FROM test WHERE id = ?";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt1, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare a statement for INSERT
    const char *insert_sql = "INSERT INTO test VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt2, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare another statement (for finalize testing)
    const char *dummy_sql = "SELECT 1";
    if (sqlite3_prepare_v2(db, dummy_sql, -1, &stmt3, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // 1. sqlite3_column_text - try to get column text (should fail since we haven't stepped yet)
    (void)sqlite3_column_text(stmt1, 0);
    
    // 2. sqlite3_snprintf - format a string using fuzzer data
    if (Size > 0) {
        // Use first part of data for format string
        size_t format_len = Size > 100 ? 100 : Size;
        char format[101];
        memcpy(format, Data, format_len);
        format[format_len] = '\0';
        
        sqlite3_snprintf(sizeof(buffer1), buffer1, format, "test");
    } else {
        sqlite3_snprintf(sizeof(buffer1), buffer1, "Default: %s", "fuzz");
    }
    
    // 3. sqlite3_snprintf - another format operation
    if (Size > 10) {
        // Use different part of data
        size_t offset = Size > 50 ? Size - 50 : 0;
        size_t len = Size - offset > 100 ? 100 : Size - offset;
        char format2[101];
        memcpy(format2, Data + offset, len);
        format2[len] = '\0';
        
        sqlite3_snprintf(sizeof(buffer2), buffer2, format2, buffer1);
    } else {
        sqlite3_snprintf(sizeof(buffer2), buffer2, "Buffer: %s", buffer1);
    }
    
    // 4. sqlite3_bind_text - bind text parameter to prepared statement
    if (Size > 0) {
        // Use fuzzer data for binding (truncate to reasonable length)
        size_t bind_len = Size > 100 ? 100 : Size;
        char *bind_data = (char *)malloc(bind_len + 1);
        if (bind_data) {
            memcpy(bind_data, Data, bind_len);
            bind_data[bind_len] = '\0';
            sqlite3_bind_text(stmt1, 1, bind_data, -1, SQLITE_STATIC);
            free(bind_data);
        } else {
            sqlite3_bind_text(stmt1, 1, "default", -1, SQLITE_STATIC);
        }
    } else {
        sqlite3_bind_text(stmt1, 1, "1", -1, SQLITE_STATIC);
    }
    
    // 5. sqlite3_step - execute the prepared statement
    rc = sqlite3_step(stmt1);
    if (rc == SQLITE_ROW) {
        // Now we can get column text successfully
        (void)sqlite3_column_text(stmt1, 0);
    }
    
    // 6. sqlite3_reset - reset the prepared statement
    sqlite3_reset(stmt1);
    
    // 7. sqlite3_sql - get the SQL text of prepared statements
    sql_text = sqlite3_sql(stmt1);
    (void)sql_text;
    sql_text = sqlite3_sql(stmt2);
    (void)sql_text;
    sql_text = sqlite3_sql(stmt3);
    (void)sql_text;
    
    // 8. sqlite3_finalize - finalize first statement
    sqlite3_finalize(stmt1);
    stmt1 = NULL;
    
    // 9. sqlite3_finalize - finalize second statement
    sqlite3_finalize(stmt2);
    stmt2 = NULL;
    
    // 10. sqlite3_finalize - finalize third statement
    sqlite3_finalize(stmt3);
    stmt3 = NULL;
    
cleanup:
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt3) sqlite3_finalize(stmt3);
    if (db) sqlite3_close(db);
    
    return 0;
}