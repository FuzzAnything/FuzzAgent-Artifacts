// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static void test_with_null_db(void) {
    // Test functions with NULL database handle
    sqlite3_extended_errcode(NULL);
    sqlite3_system_errno(NULL);
    sqlite3_errmsg(NULL);
}

static void test_open_functions(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Test sqlite3_open with UTF-8 filename
    if (size > 0) {
        char *filename = malloc(size + 1);
        if (filename) {
            memcpy(filename, data, size);
            filename[size] = '\0';
            rc = sqlite3_open(filename, &db);
            
            // Test error functions on opened database
            if (db) {
                sqlite3_extended_errcode(db);
                sqlite3_system_errno(db);
                sqlite3_errmsg(db);
                sqlite3_close(db);
                db = NULL;
            }
            free(filename);
        }
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    if (size > 0) {
        // Convert input to UTF-16 (simplified - just copy bytes as UTF-16LE)
        size_t utf16_size = (size + 1) * 2;
        uint16_t *filename16 = malloc(utf16_size);
        if (filename16) {
            for (size_t i = 0; i < size; i++) {
                filename16[i] = data[i];
            }
            filename16[size] = 0; // Null terminator
            
            rc = sqlite3_open16(filename16, &db);
            
            // Test error functions on opened database
            if (db) {
                sqlite3_extended_errcode(db);
                sqlite3_system_errno(db);
                sqlite3_errmsg(db);
                sqlite3_close(db);
                db = NULL;
            }
            free(filename16);
        }
    }
    
    // Test sqlite3_open_v2 with various flags
    if (size > 0) {
        char *filename = malloc(size + 1);
        if (filename) {
            memcpy(filename, data, size);
            filename[size] = '\0';
            
            // Try different flag combinations
            int flags[] = {
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                SQLITE_OPEN_READONLY,
                SQLITE_OPEN_READWRITE,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY
            };
            
            for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
                rc = sqlite3_open_v2(filename, &db, flags[i], NULL);
                
                if (db) {
                    sqlite3_extended_errcode(db);
                    sqlite3_system_errno(db);
                    sqlite3_errmsg(db);
                    sqlite3_close(db);
                    db = NULL;
                }
            }
            free(filename);
        }
    }
    
    // Test with dummy file
    write_dummy_file(data, size);
    
    rc = sqlite3_open("./dummy_file", &db);
    if (db) {
        sqlite3_extended_errcode(db);
        sqlite3_system_errno(db);
        sqlite3_errmsg(db);
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test sqlite3_open16 with dummy file
    {
        const char *dummy_file = "./dummy_file";
        size_t len = strlen(dummy_file);
        uint16_t *filename16 = malloc((len + 1) * 2);
        if (filename16) {
            // Simple ASCII to UTF-16 conversion
            for (size_t i = 0; i < len; i++) {
                filename16[i] = dummy_file[i];
            }
            filename16[len] = 0;
            
            rc = sqlite3_open16(filename16, &db);
            
            if (db) {
                sqlite3_extended_errcode(db);
                sqlite3_system_errno(db);
                sqlite3_errmsg(db);
                sqlite3_close(db);
                db = NULL;
            }
            free(filename16);
        }
    }
    
    remove_dummy_file();
}

int LLVMFuzzerTestOneInput_65(const uint8_t *data, size_t size) {
    // Test with NULL database handle
    test_with_null_db();
    
    // Test various open functions and error functions
    test_open_functions(data, size);
    
    // Test edge cases
    if (size == 0) {
        // Test with empty filename
        sqlite3 *db = NULL;
        int rc = sqlite3_open("", &db);
        if (db) {
            sqlite3_extended_errcode(db);
            sqlite3_system_errno(db);
            sqlite3_errmsg(db);
            sqlite3_close(db);
        }
        
        // Test sqlite3_open16 with empty filename (NULL pointer)
        rc = sqlite3_open16(NULL, &db);
        if (db) {
            sqlite3_extended_errcode(db);
            sqlite3_system_errno(db);
            sqlite3_errmsg(db);
            sqlite3_close(db);
        }
    }
    
    // Test with special filenames
    {
        sqlite3 *db = NULL;
        const char *special_names[] = {
            ":memory:",
            "file::memory:",
            "file:data.db?mode=memory",
            NULL
        };
        
        for (int i = 0; special_names[i] != NULL; i++) {
            int rc = sqlite3_open(special_names[i], &db);
            if (db) {
                sqlite3_extended_errcode(db);
                sqlite3_system_errno(db);
                sqlite3_errmsg(db);
                sqlite3_close(db);
                db = NULL;
            }
        }
    }
    
    return 0;
}