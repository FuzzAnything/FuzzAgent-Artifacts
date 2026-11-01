// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_database(sqlite3 **db_out) {
    int rc = sqlite3_open(":memory:", db_out);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table for testing
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(*db_out, sql, NULL, NULL, NULL);
    
    return 1;
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static int fuzz_stmt_status(sqlite3_stmt *stmt, const uint8_t *data, size_t size) {
    if (!stmt) return 0;
    
    // Try different counter codes based on input data
    int op = 0;
    int resetFlg = 0;
    
    if (size >= 1) {
        op = data[0] % 10; // Limit to reasonable range for counter codes
    }
    if (size >= 2) {
        resetFlg = data[1] % 2;
    }
    
    sqlite3_stmt_status(stmt, op, resetFlg);
    return 1;
}

static int fuzz_reset(sqlite3_stmt *stmt) {
    if (!stmt) return 0;
    
    sqlite3_reset(stmt);
    return 1;
}

static int fuzz_readonly(sqlite3_stmt *stmt) {
    if (!stmt) return 0;
    
    sqlite3_stmt_readonly(stmt);
    return 1;
}

static int fuzz_explain(sqlite3_stmt *stmt, const uint8_t *data, size_t size) {
    if (!stmt) return 0;
    
    // Reset first to ensure statement is not active
    sqlite3_reset(stmt);
    
    int eMode = 0;
    if (size >= 1) {
        eMode = data[0] % 3; // 0, 1, or 2
    }
    
    sqlite3_stmt_explain(stmt, eMode);
    return 1;
}

static int fuzz_busy(sqlite3_stmt *stmt) {
    if (!stmt) return 0;
    
    sqlite3_stmt_busy(stmt);
    return 1;
}

static int fuzz_isexplain(sqlite3_stmt *stmt) {
    if (!stmt) return 0;
    
    sqlite3_stmt_isexplain(stmt);
    return 1;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Need minimum size to proceed
    if (Size < 1) {
        return 0;
    }
    
    // Create database
    if (!create_database(&db)) {
        return 0;
    }
    
    // Prepare a statement based on input
    // Use first few bytes to determine SQL type
    const char *sql;
    int sql_len;
    
    if (Size >= 5) {
        int type = Data[0] % 4;
        switch (type) {
            case 0:
                sql = "SELECT * FROM test_table;";
                break;
            case 1:
                sql = "INSERT INTO test_table (data) VALUES ('test');";
                break;
            case 2:
                sql = "UPDATE test_table SET data = 'updated' WHERE id = 1;";
                break;
            default:
                sql = "DELETE FROM test_table WHERE id = 1;";
                break;
        }
        sql_len = strlen(sql);
    } else {
        sql = "SELECT * FROM test_table;";
        sql_len = strlen(sql);
    }
    
    int rc = sqlite3_prepare_v2(db, sql, sql_len, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        cleanup_database(db);
        return 0;
    }
    
    // Execute the statement once to put it in a "busy" state
    sqlite3_step(stmt);
    
    // Fuzz all target functions
    fuzz_stmt_status(stmt, Data, Size);
    fuzz_reset(stmt);
    fuzz_readonly(stmt);
    fuzz_explain(stmt, Data, Size);
    fuzz_busy(stmt);
    fuzz_isexplain(stmt);
    
    // Try stepping again to explore more states
    sqlite3_step(stmt);
    
    // Fuzz again after stepping
    fuzz_stmt_status(stmt, Data, Size);
    fuzz_reset(stmt);
    fuzz_readonly(stmt);
    fuzz_explain(stmt, Data, Size);
    fuzz_busy(stmt);
    fuzz_isexplain(stmt);
    
    // Finalize statement
    sqlite3_finalize(stmt);
    
    // Cleanup
    cleanup_database(db);
    
    return 0;
}