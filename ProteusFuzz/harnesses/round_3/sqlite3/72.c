// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to test against
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a SELECT statement with parameters
    const char *select_sql = "SELECT id, name, data FROM test_table WHERE id = ?1 AND name = ?2;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Use input data to construct bind values
    // First byte determines if we try to bind as UTF-16 or UTF-8
    int use_utf16 = (Data[0] % 2);
    int param1_val = (Data[1] % 100) + 1; // Ensure non-zero for id

    // Bind first parameter (integer)
    rc = sqlite3_bind_int(stmt, 1, param1_val);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Bind second parameter based on input
    if (use_utf16) {
        // Construct a simple UTF-16 string from input
        // Ensure even length for UTF-16
        size_t len = (Size > 2) ? (Size - 2) : 0;
        if (len % 2 != 0) len--;
        
        if (len > 0) {
            // Allocate buffer for UTF-16 data. 
            // len is the number of bytes we want to copy from Data.
            // We need to copy 'len' bytes, so we allocate 'len' bytes.
            // However, we must ensure we don't read past Data bounds.
            // Data has Size bytes. We are reading from Data[2] to Data[2+len-1].
            // So we need 2 + len <= Size.
            // Since len was calculated as Size - 2 (or less), this is safe.
            
            uint16_t *utf16_data = (uint16_t *)malloc(len);
            if (utf16_data) {
                // Copy raw bytes from Data to utf16_data
                // We treat the input bytes as UTF-16 code units (little endian or big endian depending on system)
                memcpy(utf16_data, Data + 2, len);
                
                // Bind as UTF-16. The length passed is in bytes.
                rc = sqlite3_bind_text16(stmt, 2, utf16_data, (int)len, SQLITE_TRANSIENT);
                free(utf16_data);
            }
        } else {
            // Bind empty string
            rc = sqlite3_bind_text16(stmt, 2, "", 0, SQLITE_STATIC);
        }
    } else {
        // Bind as UTF-8
        size_t len = (Size > 2) ? (Size - 2) : 0;
        if (len > 0) {
            rc = sqlite3_bind_text(stmt, 2, (const char *)(Data + 2), (int)len, SQLITE_TRANSIENT);
        } else {
            rc = sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
        }
    }

    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step through results
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            // Test sqlite3_column_type
            int type = sqlite3_column_type(stmt, i);
            (void)type; // Use to avoid unused warning

            // Test sqlite3_column_bytes16
            int bytes16 = sqlite3_column_bytes16(stmt, i);
            (void)bytes16;

            // Test sqlite3_column_text16
            const void *text16 = sqlite3_column_text16(stmt, i);
            (void)text16;

            // Test sqlite3_column_name16
            const void *name16 = sqlite3_column_name16(stmt, i);
            (void)name16;

            // Test sqlite3_column_text
            const unsigned char *text = sqlite3_column_text(stmt, i);
            (void)text;
        }
        rc = sqlite3_step(stmt);
    }

    // Reset and try again with different bindings to explore more states
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    // Bind with different values
    rc = sqlite3_bind_int(stmt, 1, 999);
    if (rc == SQLITE_OK) {
        rc = sqlite3_bind_text(stmt, 2, "fuzz_test", 9, SQLITE_STATIC);
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(stmt);
            while (rc == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count; i++) {
                    sqlite3_column_type(stmt, i);
                    sqlite3_column_bytes16(stmt, i);
                    sqlite3_column_text16(stmt, i);
                    sqlite3_column_name16(stmt, i);
                    sqlite3_column_text(stmt, i);
                }
                rc = sqlite3_step(stmt);
            }
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}