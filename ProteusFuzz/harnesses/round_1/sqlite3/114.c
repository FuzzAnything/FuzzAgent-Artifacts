// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const void *pzTail = NULL;
    uint16_t *utf16_buf = NULL;
    size_t utf16_len = 0;
    size_t input_utf16_chars = 0;

    // Initialize database connection in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Convert input data to UTF-16 for prepare functions
    if (Size > 0) {
        // Calculate number of full 2-byte units we can safely read
        input_utf16_chars = Size / 2;
        
        // Allocate buffer for UTF-16 characters + 1 null terminator
        // Size is (chars + 1) * sizeof(uint16_t)
        size_t alloc_size = (input_utf16_chars + 1) * sizeof(uint16_t);
        utf16_buf = (uint16_t *)malloc(alloc_size);
        if (!utf16_buf) {
            sqlite3_close(db);
            return 0;
        }

        // Copy input data into UTF-16 buffer
        // We copy exactly input_utf16_chars * 2 bytes
        size_t bytes_to_copy = input_utf16_chars * sizeof(uint16_t);
        memcpy(utf16_buf, Data, bytes_to_copy);
        
        // Ensure null termination
        utf16_buf[input_utf16_chars] = 0;
        
        // Calculate byte length for sqlite3_prepare functions
        utf16_len = bytes_to_copy;

        // Fuzzing: Randomly inject valid SQL keywords or malformed strings
        // to trigger different code paths in preparation
        // We overwrite the first few characters with a simple SELECT if it looks random
        // This helps ensure we actually get a prepared statement sometimes
        if (input_utf16_chars >= 6) {
            // Try to make it look like SQL occasionally
            // 0x53 = 'S', 0x45 = 'E', 0x4C = 'L', 0x43 = 'C', 0x54 = 'T'
            // "SELECT" in UTF-16 LE
            utf16_buf[0] = 0x53; 
            utf16_buf[1] = 0x45;
            utf16_buf[2] = 0x4C;
            utf16_buf[3] = 0x43;
            utf16_buf[4] = 0x54;
            utf16_buf[5] = 0x00; // Null terminate early for safety if needed
        }
    }

    // 1. Test sqlite3_prepare16
    if (utf16_buf && db) {
        rc = sqlite3_prepare16(db, utf16_buf, (int)utf16_len, &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_column_name16
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                const void *name = sqlite3_column_name16(stmt, i);
                (void)name; // Use the result
            }
            // Test out of bounds column name
            // Ensure we pass a valid stmt pointer even if index is out of bounds
            const void *bad_name = sqlite3_column_name16(stmt, col_count + 10);
            (void)bad_name;

            // Test sqlite3_stmt_busy before stepping
            int is_busy = sqlite3_stmt_busy(stmt);
            (void)is_busy;

            // Step the statement to potentially make it busy
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
                // Test sqlite3_stmt_busy after stepping
                is_busy = sqlite3_stmt_busy(stmt);
                (void)is_busy;
            }

            // Test sqlite3_expired
            int is_expired = sqlite3_expired(stmt);
            (void)is_expired;

            // Finalize statement
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    // 2. Test sqlite3_prepare16_v2
    if (utf16_buf && db) {
        rc = sqlite3_prepare16_v2(db, utf16_buf, (int)utf16_len, &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_column_name16
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                const void *name = sqlite3_column_name16(stmt, i);
                (void)name;
            }
            
            // Test sqlite3_expired
            int is_expired = sqlite3_expired(stmt);
            (void)is_expired;

            // Test sqlite3_stmt_busy
            int is_busy = sqlite3_stmt_busy(stmt);
            (void)is_busy;

            // Step to change state
            sqlite3_step(stmt);
            is_busy = sqlite3_stmt_busy(stmt);
            (void)is_busy;

            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    // 3. Test sqlite3_prepare16_v3
    if (utf16_buf && db) {
        unsigned int prepFlags = 0;
        // Fuzz flags slightly
        if (Size > 0) prepFlags = Data[0] & 0xFF;
        
        rc = sqlite3_prepare16_v3(db, utf16_buf, (int)utf16_len, prepFlags, &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_column_name16
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                const void *name = sqlite3_column_name16(stmt, i);
                (void)name;
            }

            // Test sqlite3_expired
            int is_expired = sqlite3_expired(stmt);
            (void)is_expired;

            // Test sqlite3_stmt_busy
            int is_busy = sqlite3_stmt_busy(stmt);
            (void)is_busy;

            sqlite3_step(stmt);
            is_busy = sqlite3_stmt_busy(stmt);
            (void)is_busy;

            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    // 4. Test with NULL statement pointer for sqlite3_expired and sqlite3_stmt_busy
    // These functions are designed to handle NULL safely
    int expired_null = sqlite3_expired(NULL);
    (void)expired_null;
    int busy_null = sqlite3_stmt_busy(NULL);
    (void)busy_null;

    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    if (utf16_buf) {
        free(utf16_buf);
    }

    return 0;
}