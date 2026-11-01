// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a safe SQL string from input data
    // Ensure null-termination and limit size to prevent buffer overflows
    const char *sql;
    size_t sql_len = Size;
    
    // If input is too large, truncate it safely
    if (sql_len > 4095) {
        sql_len = 4095;
    }

    // Allocate buffer for SQL string with null terminator
    char *safe_sql = (char *)malloc(sql_len + 1);
    if (!safe_sql) {
        sqlite3_close(db);
        return 0;
    }

    // Copy data and ensure null termination
    memcpy(safe_sql, Data, sql_len);
    safe_sql[sql_len] = '\0';
    
    // Basic validation: if the string contains null bytes in the middle, 
    // we should stop at the first null byte to ensure valid C string
    size_t actual_len = strlen(safe_sql);
    if (actual_len != sql_len) {
        // There was a null byte inside, use the truncated length
        sql_len = actual_len;
    }
    
    // If the resulting string is empty, use a default safe query
    if (sql_len == 0) {
        sql = "SELECT 1;";
    } else {
        sql = safe_sql;
    }

    // Prepare the statement with explicit length to avoid parsing beyond buffer
    rc = sqlite3_prepare_v2(db, sql, (int)sql_len, &stmt, NULL);
    
    // Free the allocated buffer
    free(safe_sql);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Execute the statement
    rc = sqlite3_step(stmt);

    // Fuzz sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);

    // Fuzz sqlite3_data_count
    int data_count = sqlite3_data_count(stmt);

    // Fuzz sqlite3_expired
    int expired = sqlite3_expired(stmt);

    // Iterate through columns to fuzz column-specific functions
    // Ensure column index is within valid range
    for (int i = 0; i < col_count; i++) {
        // Fuzz sqlite3_column_bytes16
        int bytes16 = sqlite3_column_bytes16(stmt, i);
        
        // Fuzz sqlite3_column_name16
        const void *name16 = sqlite3_column_name16(stmt, i);
        
        // Fuzz sqlite3_column_text16
        const void *text16 = sqlite3_column_text16(stmt, i);
        
        // Additional step to explore more states
        if (rc == SQLITE_ROW) {
            rc = sqlite3_step(stmt);
        }
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Call the fuzzing function
    fuzz_sqlite3(Data, Size);

    return 0;
}