// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql;
    char *sql_buf = NULL;

    if (Size < 1) {
        return 0;
    }

    // Initialize database in memory
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Use input data as SQL query, ensuring it's null-terminated
    if (Data[Size - 1] != '\0') {
        sql_buf = (char *)malloc(Size + 1);
        if (!sql_buf) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql_buf, Data, Size);
        sql_buf[Size] = '\0';
        sql = sql_buf;
    } else {
        sql = (const char *)Data;
    }

    // Prepare statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        // If preparation fails, try a simple valid query to test column functions
        sqlite3_finalize(stmt);
        stmt = NULL;
        if (sqlite3_prepare_v2(db, "SELECT 1, 'test', 3.14, NULL, X'0102'", -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            if (sql_buf) free(sql_buf);
            return 0;
        }
    }

    // Execute statement to get to first row if possible
    sqlite3_step(stmt);

    // Get column count
    int col_count = sqlite3_column_count(stmt);
    if (col_count > 0) {
        // Fuzz column index within bounds and slightly out of bounds
        for (int i = -1; i <= col_count; i++) {
            // Test sqlite3_column_name16
            const void *name16 = sqlite3_column_name16(stmt, i);
            (void)name16; // Suppress unused warning

            // Test sqlite3_column_type
            int col_type = sqlite3_column_type(stmt, i);
            (void)col_type;

            // Test sqlite3_column_text16
            const void *text16 = sqlite3_column_text16(stmt, i);
            (void)text16;

            // Test sqlite3_column_decltype16
            const void *decltype16 = sqlite3_column_decltype16(stmt, i);
            (void)decltype16;

            // Test sqlite3_column_double
            double col_double = sqlite3_column_double(stmt, i);
            (void)col_double;
        }
    }

    // Reset and step again to test state changes
    sqlite3_reset(stmt);
    sqlite3_step(stmt);

    // Test again after reset
    col_count = sqlite3_column_count(stmt);
    if (col_count > 0) {
        for (int i = 0; i < col_count; i++) {
            sqlite3_column_name16(stmt, i);
            sqlite3_column_type(stmt, i);
            sqlite3_column_text16(stmt, i);
            sqlite3_column_decltype16(stmt, i);
            sqlite3_column_double(stmt, i);
        }
    }

    // Finalize statement
    sqlite3_finalize(stmt);

    // Close database
    sqlite3_close(db);

    // Free allocated buffer if we created one
    if (sql_buf) {
        free(sql_buf);
    }

    return 0;
}