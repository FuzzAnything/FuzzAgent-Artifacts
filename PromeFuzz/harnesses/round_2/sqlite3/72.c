// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

static void fuzz_open_functions(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    const char *errmsg = NULL;
    int extended_err = 0;
    int system_err = 0;
    
    // Test sqlite3_open with fuzzer data as filename
    if (size > 0) {
        char *filename = malloc(size + 1);
        if (filename) {
            memcpy(filename, data, size);
            filename[size] = '\0';
            
            int rc = sqlite3_open(filename, &db);
            if (rc == SQLITE_OK) {
                // Test error functions on valid handle
                errmsg = sqlite3_errmsg(db);
                extended_err = sqlite3_extended_errcode(db);
                system_err = sqlite3_system_errno(db);
                
                // Test sqlite3_open_v2 with various flags
                sqlite3 *db2 = NULL;
                int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
                int rc2 = sqlite3_open_v2(filename, &db2, flags, NULL);
                if (rc2 == SQLITE_OK) {
                    sqlite3_close(db2);
                }
            }
            if (db) {
                sqlite3_close(db);
                db = NULL;
            }
            free(filename);
        }
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    if (size >= 2) {
        // Ensure we have at least 2 bytes for UTF-16 null terminator
        size_t utf16_size = (size / 2) * 2;
        void *utf16_data = malloc(utf16_size + 2);
        if (utf16_data) {
            memcpy(utf16_data, data, utf16_size);
            // Add UTF-16 null terminator (two zero bytes)
            ((uint8_t*)utf16_data)[utf16_size] = 0;
            ((uint8_t*)utf16_data)[utf16_size + 1] = 0;
            
            int rc = sqlite3_open16(utf16_data, &db);
            if (rc == SQLITE_OK) {
                errmsg = sqlite3_errmsg(db);
                extended_err = sqlite3_extended_errcode(db);
                system_err = sqlite3_system_errno(db);
            }
            if (db) {
                sqlite3_close(db);
                db = NULL;
            }
            free(utf16_data);
        }
    }
    
    // Test with NULL filename (should open empty string database)
    int rc = sqlite3_open16(NULL, &db);
    if (rc == SQLITE_OK) {
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test sqlite3_open with empty string
    rc = sqlite3_open("", &db);
    if (rc == SQLITE_OK) {
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test sqlite3_open_v2 with various flag combinations
    if (size > 0) {
        char *filename = malloc(size + 1);
        if (filename) {
            memcpy(filename, data, size);
            filename[size] = '\0';
            
            // Try different flag combinations
            int flag_combinations[] = {
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                SQLITE_OPEN_READONLY,
                SQLITE_OPEN_READWRITE,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
            };
            
            for (int i = 0; i < sizeof(flag_combinations)/sizeof(flag_combinations[0]); i++) {
                sqlite3 *db2 = NULL;
                int rc2 = sqlite3_open_v2(filename, &db2, flag_combinations[i], NULL);
                if (rc2 == SQLITE_OK) {
                    // Test error functions
                    errmsg = sqlite3_errmsg(db2);
                    extended_err = sqlite3_extended_errcode(db2);
                    system_err = sqlite3_system_errno(db2);
                    sqlite3_close(db2);
                }
            }
            
            // Test with URI filename if data looks like it could be a URI
            if (size >= 5 && memcmp(filename, "file:", 5) == 0) {
                sqlite3 *db2 = NULL;
                int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI;
                int rc2 = sqlite3_open_v2(filename, &db2, flags, NULL);
                if (rc2 == SQLITE_OK) {
                    sqlite3_close(db2);
                }
            }
            
            free(filename);
        }
    }
    
    // Test error functions on NULL handle (should be safe)
    errmsg = sqlite3_errmsg(NULL);
    extended_err = sqlite3_extended_errcode(NULL);
    system_err = sqlite3_system_errno(NULL);
}

static void fuzz_with_file(const uint8_t *data, size_t size) {
    // Write fuzzer data to a file and test opening it
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
        
        sqlite3 *db = NULL;
        int rc = sqlite3_open("./dummy_file", &db);
        if (rc == SQLITE_OK) {
            // Test all error functions
            const char *errmsg = sqlite3_errmsg(db);
            int extended_err = sqlite3_extended_errcode(db);
            int system_err = sqlite3_system_errno(db);
            
            // Also test sqlite3_open_v2 on the same file
            sqlite3 *db2 = NULL;
            int rc2 = sqlite3_open_v2("./dummy_file", &db2, SQLITE_OPEN_READWRITE, NULL);
            if (rc2 == SQLITE_OK) {
                sqlite3_close(db2);
            }
            
            sqlite3_close(db);
        }
        
        // Test sqlite3_open16 on the file
        const wchar_t *wfilename = L"./dummy_file";
        db = NULL;
        rc = sqlite3_open16(wfilename, &db);
        if (rc == SQLITE_OK) {
            sqlite3_close(db);
        }
        
        remove("./dummy_file");
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *data, size_t size) {
    // Initialize SQLite if needed (not required for basic operations)
    
    // Test all API functions with fuzzer data
    fuzz_open_functions(data, size);
    
    // Test with file operations
    fuzz_with_file(data, size);
    
    return 0;
}