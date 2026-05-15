// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void call_error_functions(sqlite3 *db) {
    // Call sqlite3_extended_errcode
    int extended_err = sqlite3_extended_errcode(db);
    (void)extended_err; // Use result to avoid unused variable warning
    
    // Call sqlite3_system_errno
    int system_err = sqlite3_system_errno(db);
    (void)system_err;
    
    // Call sqlite3_errmsg
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;
}

static void test_open_functions(const uint8_t *data, size_t size, int flags) {
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Write fuzzer data to dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
    
    // Test sqlite3_open
    int rc = sqlite3_open(filename, &db);
    if (rc == SQLITE_OK) {
        call_error_functions(db);
        sqlite3_close(db);
        db = NULL;
    } else {
        call_error_functions(db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test sqlite3_open_v2 with various flags
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc == SQLITE_OK) {
        call_error_functions(db);
        sqlite3_close(db);
        db = NULL;
    } else {
        call_error_functions(db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test sqlite3_open_v2 with different VFS
    rc = sqlite3_open_v2(filename, &db, flags, "unix");
    if (rc == SQLITE_OK) {
        call_error_functions(db);
        sqlite3_close(db);
        db = NULL;
    } else {
        call_error_functions(db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    static const wchar_t filename_utf16[] = L"./dummy_file";
    rc = sqlite3_open16(filename_utf16, &db);
    if (rc == SQLITE_OK) {
        call_error_functions(db);
        sqlite3_close(db);
        db = NULL;
    } else {
        call_error_functions(db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test with empty filename (creates temporary database)
    rc = sqlite3_open("", &db);
    if (rc == SQLITE_OK) {
        call_error_functions(db);
        sqlite3_close(db);
        db = NULL;
    } else {
        call_error_functions(db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test with :memory: filename (in-memory database)
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK) {
        call_error_functions(db);
        sqlite3_close(db);
        db = NULL;
    } else {
        call_error_functions(db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test with NULL database handle
    call_error_functions(NULL);
}

int LLVMFuzzerTestOneInput_86(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    // Use first byte to determine flags combination
    uint8_t flags_selector = data[0];
    
    // Define common flag combinations
    int flag_combinations[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_SHAREDCACHE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_PRIVATECACHE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXRESCODE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOFOLLOW
    };
    
    int num_combinations = sizeof(flag_combinations) / sizeof(flag_combinations[0]);
    int flags = flag_combinations[flags_selector % num_combinations];
    
    // Test with the selected flags
    test_open_functions(data + 1, size - 1, flags);
    
    // Also test with some random flags based on data
    if (size >= 4) {
        int random_flags = *(int*)(data + (size - 4));
        test_open_functions(data + 1, size - 1, random_flags);
    }
    
    return 0;
}