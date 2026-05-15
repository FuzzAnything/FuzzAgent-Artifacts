// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void convert_to_utf16(const uint8_t *data, size_t size, uint16_t **utf16_buf, size_t *utf16_size) {
    // Simple conversion: treat input as ASCII/UTF-8 and convert to UTF-16LE
    // This is a simplified approach for fuzzing
    *utf16_size = size * 2 + 2;  // Each char becomes 2 bytes + null terminator
    *utf16_buf = (uint16_t *)malloc(*utf16_size);
    if (*utf16_buf) {
        for (size_t i = 0; i < size; i++) {
            (*utf16_buf)[i] = data[i];  // Simple zero-extend to 16-bit
        }
        (*utf16_buf)[size] = 0;  // Null terminator
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    int rc;
    
    // Test sqlite3_open with UTF-8 filename
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_system_errno
        int sys_err = sqlite3_system_errno(db);
        (void)sys_err;
        
        // Test sqlite3_errmsg
        const char *err_msg = sqlite3_errmsg(db);
        (void)err_msg;
        
        // Test sqlite3_errmsg16
        const void *err_msg16 = sqlite3_errmsg16(db);
        (void)err_msg16;
        
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    uint16_t *utf16_filename = NULL;
    size_t utf16_size = 0;
    convert_to_utf16(Data, Size > 100 ? 100 : Size, &utf16_filename, &utf16_size);
    
    if (utf16_filename) {
        rc = sqlite3_open16(utf16_filename, &db);
        if (rc == SQLITE_OK && db) {
            int sys_err = sqlite3_system_errno(db);
            (void)sys_err;
            
            const char *err_msg = sqlite3_errmsg(db);
            (void)err_msg;
            
            const void *err_msg16 = sqlite3_errmsg16(db);
            (void)err_msg16;
            
            sqlite3_close(db);
            db = NULL;
        }
        free(utf16_filename);
    }
    
    // Test sqlite3_open_v2 with various flags
    int flags_list[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_FULLMUTEX,
        SQLITE_OPEN_SHAREDCACHE,
        SQLITE_OPEN_PRIVATECACHE,
    };
    
    for (int i = 0; i < sizeof(flags_list)/sizeof(flags_list[0]); i++) {
        rc = sqlite3_open_v2("./dummy_file", &db, flags_list[i], NULL);
        if (rc == SQLITE_OK && db) {
            int sys_err = sqlite3_system_errno(db);
            (void)sys_err;
            
            const char *err_msg = sqlite3_errmsg(db);
            (void)err_msg;
            
            const void *err_msg16 = sqlite3_errmsg16(db);
            (void)err_msg16;
            
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test with NULL database handle
    int sys_err_null = sqlite3_system_errno(NULL);
    (void)sys_err_null;
    
    const char *err_msg_null = sqlite3_errmsg(NULL);
    (void)err_msg_null;
    
    const void *err_msg16_null = sqlite3_errmsg16(NULL);
    (void)err_msg16_null;
    
    // Test with empty string filename
    rc = sqlite3_open("", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test with ":memory:" filename
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_close(db);
        db = NULL;
    }
    
    // Test with NULL filename for sqlite3_open16
    rc = sqlite3_open16(NULL, &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_close(db);
        db = NULL;
    }
    
    return 0;
}