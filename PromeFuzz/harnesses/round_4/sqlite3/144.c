// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    // Always test with NULL database handle
    sqlite3_errmsg16(NULL);
    sqlite3_system_errno(NULL);
    sqlite3_errmsg(NULL);
    
    // Create a dummy filename from fuzzer input
    char filename[256];
    if (Size > 0 && Size < sizeof(filename)) {
        memcpy(filename, Data, Size);
        filename[Size] = '\0';
    } else {
        // Use default filename if input is too large or empty
        strcpy(filename, "./dummy_file");
    }
    
    sqlite3 *db = NULL;
    
    // Test sqlite3_open with various inputs
    int rc_open = sqlite3_open(filename, &db);
    if (rc_open == SQLITE_OK && db != NULL) {
        // Test error functions on valid database handle
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        
        // Close the database
        sqlite3_close(db);
        db = NULL;
    } else if (db != NULL) {
        // Database handle was created but open failed
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test sqlite3_open_v2 with various flags
    // Use first byte of input to determine flags (if available)
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        // Use some bits from input to vary flags
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_URI;
    }
    
    int rc_open_v2 = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc_open_v2 == SQLITE_OK && db != NULL) {
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    } else if (db != NULL) {
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    // Convert filename to UTF-16 if we have enough data
    if (Size > 1) {
        // Simple conversion: treat input as UTF-16LE
        // Allocate buffer with space for null terminator
        size_t utf16_len = Size / 2;
        if (utf16_len > 0) {
            uint16_t *utf16_filename = malloc((utf16_len + 1) * sizeof(uint16_t));
            if (utf16_filename) {
                // Copy data as UTF-16 words
                for (size_t i = 0; i < utf16_len; i++) {
                    utf16_filename[i] = (Data[i*2] << 8) | Data[i*2 + 1];
                }
                utf16_filename[utf16_len] = 0; // Null terminator
                
                int rc_open16 = sqlite3_open16(utf16_filename, &db);
                if (rc_open16 == SQLITE_OK && db != NULL) {
                    sqlite3_errmsg16(db);
                    sqlite3_system_errno(db);
                    sqlite3_errmsg(db);
                    sqlite3_close(db);
                    db = NULL;
                } else if (db != NULL) {
                    sqlite3_errmsg16(db);
                    sqlite3_system_errno(db);
                    sqlite3_errmsg(db);
                    sqlite3_close(db);
                    db = NULL;
                }
                
                free(utf16_filename);
            }
        }
    }
    
    // Test with empty filename
    sqlite3_open("", &db);
    if (db != NULL) {
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test with special filenames
    sqlite3_open(":memory:", &db);
    if (db != NULL) {
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test with NULL filename (should create temporary database)
    sqlite3_open(NULL, &db);
    if (db != NULL) {
        sqlite3_errmsg16(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    }
    
    return 0;
}