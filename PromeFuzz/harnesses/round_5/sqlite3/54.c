// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *errmsg;
    const void *errmsg16;
    int sys_errno;
    
    // Use first byte to determine which function combination to test
    uint8_t test_case = 0;
    if (Size > 0) {
        test_case = Data[0] % 6;  // 6 different test cases
        Data++;
        Size--;
    }
    
    // Create a null-terminated filename from input data
    char filename[256] = "./dummy_file";
    if (Size > 0) {
        size_t filename_len = Size < 255 ? Size : 255;
        memcpy(filename, Data, filename_len);
        filename[filename_len] = '\0';
    }
    
    // Try different open functions based on test case
    switch (test_case) {
        case 0:
            // Test sqlite3_open
            rc = sqlite3_open(filename, &db);
            break;
        case 1:
            // Test sqlite3_open_v2 with random flags
            {
                int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
                if (Size > 4) {
                    // Use some bytes for flags
                    uint32_t flag_bytes = 0;
                    memcpy(&flag_bytes, Data + (Size > 8 ? 8 : 0), sizeof(uint32_t));
                    flags = flag_bytes;
                }
                rc = sqlite3_open_v2(filename, &db, flags, NULL);
            }
            break;
        case 2:
            // Test sqlite3_open16 (UTF-16 filename)
            {
                // Convert input to UTF-16 (simplified - just duplicate bytes)
                size_t utf16_len = (Size < 127 ? Size : 127) * 2 + 2;
                uint16_t *utf16_filename = malloc(utf16_len);
                if (utf16_filename) {
                    for (size_t i = 0; i < Size && i < 127; i++) {
                        utf16_filename[i] = Data[i];
                    }
                    utf16_filename[Size < 127 ? Size : 127] = 0;
                    rc = sqlite3_open16(utf16_filename, &db);
                    free(utf16_filename);
                } else {
                    rc = SQLITE_NOMEM;
                }
            }
            break;
        case 3:
            // Test sqlite3_open with empty string (temporary database)
            rc = sqlite3_open("", &db);
            break;
        case 4:
            // Test sqlite3_open with :memory: database
            rc = sqlite3_open(":memory:", &db);
            break;
        case 5:
            // Test sqlite3_open_v2 with URI filename
            {
                char uri_filename[300] = "file:";
                size_t uri_len = Size < 295 ? Size : 295;
                if (Size > 0) {
                    memcpy(uri_filename + 5, Data, uri_len);
                    uri_filename[5 + uri_len] = '\0';
                }
                rc = sqlite3_open_v2(uri_filename, &db, 
                                     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, 
                                     NULL);
            }
            break;
        default:
            rc = SQLITE_ERROR;
            break;
    }
    
    // Always call error functions to test them
    if (db) {
        errmsg = sqlite3_errmsg(db);
        sys_errno = sqlite3_system_errno(db);
        errmsg16 = sqlite3_errmsg16(db);
        
        // Close the database if it was opened
        sqlite3_close(db);
    } else {
        // Test error functions with NULL db
        errmsg = sqlite3_errmsg(NULL);
        sys_errno = sqlite3_system_errno(NULL);
        errmsg16 = sqlite3_errmsg16(NULL);
    }
    
    // Use the results to avoid optimization
    (void)rc;
    (void)errmsg;
    (void)sys_errno;
    (void)errmsg16;
    
    return 0;
}

#ifdef __cplusplus
}
#endif