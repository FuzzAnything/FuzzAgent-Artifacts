// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
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
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; // Use the result to avoid unused variable warning
    
    // Test sqlite3_errmsg16
    const void *errmsg16 = sqlite3_errmsg16(db);
    (void)errmsg16; // Use the result to avoid unused variable warning
    
    // Test sqlite3_system_errno
    int sys_errno = sqlite3_system_errno(db);
    (void)sys_errno; // Use the result to avoid unused variable warning
}

static void test_with_null_handle(void) {
    // Test error functions with NULL handle
    const char *errmsg = sqlite3_errmsg(NULL);
    (void)errmsg;
    
    const void *errmsg16 = sqlite3_errmsg16(NULL);
    (void)errmsg16;
    
    int sys_errno = sqlite3_system_errno(NULL);
    (void)sys_errno;
}

static void test_open_functions(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Use first byte to determine which open function to test
    uint8_t func_selector = size > 0 ? data[0] : 0;
    
    // Use "./dummy_file" as the database file
    const char *filename = "./dummy_file";
    
    // Write fuzzer data to the file (truncate if needed)
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (size > 1) {
            fwrite(data + 1, 1, size - 1, fp);
        }
        fclose(fp);
    }
    
    switch (func_selector % 3) {
        case 0:
            // Test sqlite3_open
            rc = sqlite3_open(filename, &db);
            (void)rc; // Use the result
            break;
            
        case 1:
            // Test sqlite3_open_v2 with various flags
            {
                int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
                if (size > 1) {
                    // Use second byte to modify flags
                    uint8_t flag_mod = data[1];
                    if (flag_mod & 0x01) flags |= SQLITE_OPEN_READONLY;
                    if (flag_mod & 0x02) flags |= SQLITE_OPEN_MEMORY;
                    if (flag_mod & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
                    if (flag_mod & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
                }
                rc = sqlite3_open_v2(filename, &db, flags, NULL);
                (void)rc; // Use the result
            }
            break;
            
        case 2:
            // Test sqlite3_open16 with UTF-16 filename
            {
                // Convert ASCII filename to UTF-16 (simple conversion)
                size_t len = strlen(filename);
                uint16_t *utf16_filename = malloc((len + 1) * sizeof(uint16_t));
                if (utf16_filename) {
                    for (size_t i = 0; i < len; i++) {
                        utf16_filename[i] = (uint16_t)filename[i];
                    }
                    utf16_filename[len] = 0;
                    
                    rc = sqlite3_open16(utf16_filename, &db);
                    (void)rc; // Use the result
                    
                    free(utf16_filename);
                }
            }
            break;
    }
    
    // Test error functions with the opened database handle
    if (db) {
        test_error_functions(db);
        sqlite3_close(db);
    } else {
        // Also test with NULL handle
        test_error_functions(NULL);
    }
    
    // Clean up the dummy file
    remove(filename);
}

int LLVMFuzzerTestOneInput_146(const uint8_t *data, size_t size) {
    // Test error functions with NULL handle
    test_with_null_handle();
    
    // Test open functions with fuzzer data
    test_open_functions(data, size);
    
    return 0;
}