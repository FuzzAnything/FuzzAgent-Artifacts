// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
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

static void test_error_functions(sqlite3 *db) {
    // Test sqlite3_errmsg
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg; // Use result to avoid unused variable warning
    
    // Test sqlite3_errmsg16
    const void *err_msg16 = sqlite3_errmsg16(db);
    (void)err_msg16; // Use result to avoid unused variable warning
    
    // Test sqlite3_system_errno
    int sys_errno = sqlite3_system_errno(db);
    (void)sys_errno; // Use result to avoid unused variable warning
}

static void test_with_null_handle(void) {
    // Test error functions with NULL handle
    const char *err_msg = sqlite3_errmsg(NULL);
    (void)err_msg;
    
    const void *err_msg16 = sqlite3_errmsg16(NULL);
    (void)err_msg16;
    
    int sys_errno = sqlite3_system_errno(NULL);
    (void)sys_errno;
}

static int open_and_test(const uint8_t *data, size_t size, int use_v2, int use_utf16) {
    sqlite3 *db = NULL;
    int rc = SQLITE_OK;
    
    // Use first few bytes to determine flags for open_v2
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (size > 0) {
        // Use data to influence flags
        if (data[0] & 0x01) flags |= SQLITE_OPEN_URI;
        if (data[0] & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (data[0] & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (data[0] & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        if (data[0] & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (data[0] & 0x20) flags |= SQLITE_OPEN_PRIVATECACHE;
    }
    
    // Determine filename based on input
    const char *filename = "./dummy_file";
    
    if (use_utf16) {
        // For sqlite3_open16, we need UTF-16 filename
        // Create UTF-16 version of filename manually
        static const unsigned short utf16_filename[] = 
            {'.', '/', 'd', 'u', 'm', 'm', 'y', '_', 'f', 'i', 'l', 'e', 0};
        rc = sqlite3_open16(utf16_filename, &db);
    } else if (use_v2) {
        // For sqlite3_open_v2
        rc = sqlite3_open_v2(filename, &db, flags, NULL);
    } else {
        // For sqlite3_open
        rc = sqlite3_open(filename, &db);
    }
    
    // Test error functions regardless of open result
    if (db) {
        test_error_functions(db);
        sqlite3_close(db);
    } else if (rc == SQLITE_OK) {
        // Handle case where db is NULL but rc is OK (memory allocation failure)
        test_with_null_handle();
    }
    
    return rc;
}

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write some data to make it a non-empty file
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, fp);
        } else {
            // Write minimal SQLite header
            const char header[] = "SQLite format 3";
            fwrite(header, 1, sizeof(header), fp);
        }
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_147(const uint8_t *data, size_t size) {
    // Test with NULL handle first
    test_with_null_handle();
    
    // Create dummy file with fuzzer data
    create_dummy_file(data, size);
    
    // Test different open functions with various configurations
    // Use input data to determine which combinations to test
    
    if (size == 0) {
        // Test all variations with minimal input
        open_and_test(data, size, 0, 0); // sqlite3_open
        open_and_test(data, size, 1, 0); // sqlite3_open_v2
        open_and_test(data, size, 0, 1); // sqlite3_open16
    } else {
        // Use first byte to determine which functions to test
        uint8_t selector = data[0];
        
        if (selector & 0x01) {
            open_and_test(data, size, 0, 0); // sqlite3_open
        }
        
        if (selector & 0x02) {
            open_and_test(data, size, 1, 0); // sqlite3_open_v2
        }
        
        if (selector & 0x04) {
            open_and_test(data, size, 0, 1); // sqlite3_open16
        }
        
        // Test multiple opens in sequence to explore more states
        if (selector & 0x08) {
            sqlite3 *db1 = NULL, *db2 = NULL;
            sqlite3_open("./dummy_file", &db1);
            sqlite3_open_v2("./dummy_file", &db2, 
                           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
            
            if (db1) {
                test_error_functions(db1);
                sqlite3_close(db1);
            }
            if (db2) {
                test_error_functions(db2);
                sqlite3_close(db2);
            }
        }
    }
    
    // Clean up
    cleanup_dummy_file();
    
    return 0;
}