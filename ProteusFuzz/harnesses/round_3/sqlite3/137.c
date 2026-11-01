// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Ensure input is treated as valid UTF-16LE for fuzzing purposes
    // We interpret the raw input as a stream of 16-bit words.
    // If Size is odd, we ignore the last byte to maintain alignment.
    size_t utf16Len = Size;
    if (utf16Len % 2 != 0) {
        utf16Len--;
    }
    
    // Ensure we have at least one character (2 bytes)
    if (utf16Len < 2) {
        sqlite3_close(db);
        return 0;
    }

    const uint16_t *zSql = (const uint16_t *)Data;

    // Fuzz sqlite3_prepare16
    rc = sqlite3_prepare16(db, zSql, (int)utf16Len, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        // Check statement properties
        (void)sqlite3_expired(stmt);
        (void)sqlite3_stmt_busy(stmt);
        (void)sqlite3_stmt_readonly(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // Fuzz sqlite3_prepare16_v2
    rc = sqlite3_prepare16_v2(db, zSql, (int)utf16Len, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        (void)sqlite3_expired(stmt);
        (void)sqlite3_stmt_busy(stmt);
        (void)sqlite3_stmt_readonly(stmt);
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // Fuzz sqlite3_prepare16_v3 with various flags
    // We use the first byte of input to determine flags, masked to valid constants
    unsigned int flags = 0;
    if (Size > 0) {
        // SQLITE_PREPARE_PERSISTENT (0x10), SQLITE_PREPARE_NO_VTAB (0x20), SQLITE_PREPARE_NORMALIZE (0x02)
        // We construct a safe subset of flags based on input to avoid undefined behavior
        flags = (Data[0] & 0x32) | (Data[0] & 0x04 ? SQLITE_PREPARE_NORMALIZE : 0);
        // Ensure we don't pass invalid combinations if strict, but SQLite usually handles unknown flags gracefully
    }

    rc = sqlite3_prepare16_v3(db, zSql, (int)utf16Len, flags, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        (void)sqlite3_expired(stmt);
        (void)sqlite3_stmt_busy(stmt);
        (void)sqlite3_stmt_readonly(stmt);
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // Test sqlite3_expired and sqlite3_stmt_readonly with NULL pointer
    // According to docs: sqlite3_expired(NULL) returns 1, sqlite3_stmt_readonly(NULL) returns 1
    (void)sqlite3_expired(NULL);
    (void)sqlite3_stmt_readonly(NULL);
    (void)sqlite3_stmt_busy(NULL);

    // Cleanup
    sqlite3_close(db);

    return 0;
}