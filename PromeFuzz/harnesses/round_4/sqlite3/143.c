// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for database operations
    write_dummy_file(Data, Size);
    
    // Test with NULL database handle
    const void *err16_null = sqlite3_errmsg16(NULL);
    int sys_errno_null = sqlite3_system_errno(NULL);
    const char *err_null = sqlite3_errmsg(NULL);
    
    // Test database operations
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    sqlite3 *db3 = NULL;
    
    // Test sqlite3_open with dummy file
    int rc1 = sqlite3_open("./dummy_file", &db);
    
    if (rc1 == SQLITE_OK && db) {
        // Test error message functions with valid handle
        const void *err16 = sqlite3_errmsg16(db);
        int sys_errno = sqlite3_system_errno(db);
        const char *err = sqlite3_errmsg(db);
        
        // Close the database
        sqlite3_close(db);
    }
    
    // Test sqlite3_open_v2 with various flags
    // Use first byte to determine flags (if available)
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_byte & 0x20) flags |= SQLITE_OPEN_PRIVATECACHE;
    }
    
    int rc2 = sqlite3_open_v2("./dummy_file", &db2, flags, NULL);
    
    if (rc2 == SQLITE_OK && db2) {
        // Test error functions again
        const void *err16_2 = sqlite3_errmsg16(db2);
        int sys_errno_2 = sqlite3_system_errno(db2);
        const char *err_2 = sqlite3_errmsg(db2);
        
        sqlite3_close(db2);
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    // Convert ASCII "./dummy_file" to UTF-16LE
    static const unsigned short utf16_filename[] = 
        {'.', '/', 'd', 'u', 'm', 'm', 'y', '_', 'f', 'i', 'l', 'e', 0};
    
    int rc3 = sqlite3_open16(utf16_filename, &db3);
    
    if (rc3 == SQLITE_OK && db3) {
        // Test error functions with UTF-16 opened database
        const void *err16_3 = sqlite3_errmsg16(db3);
        int sys_errno_3 = sqlite3_system_errno(db3);
        const char *err_3 = sqlite3_errmsg(db3);
        
        sqlite3_close(db3);
    }
    
    // Test with invalid filename (using input data as filename if possible)
    if (Size > 0 && Size < 256) {
        char temp_filename[256];
        memcpy(temp_filename, Data, Size);
        temp_filename[Size] = '\0';
        
        sqlite3 *db4 = NULL;
        int rc4 = sqlite3_open(temp_filename, &db4);
        
        if (rc4 == SQLITE_OK && db4) {
            const void *err16_4 = sqlite3_errmsg16(db4);
            int sys_errno_4 = sqlite3_system_errno(db4);
            const char *err_4 = sqlite3_errmsg(db4);
            
            sqlite3_close(db4);
        }
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}