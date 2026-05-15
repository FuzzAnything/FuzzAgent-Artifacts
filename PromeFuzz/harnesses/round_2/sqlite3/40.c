// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Call sqlite3_compileoption_used with various option names
    if (Size > 0) {
        // Use first byte to determine which option to check
        int opt_index = Data[0] % 8;
        const char* options[] = {
            "SQLITE_ENABLE_COLUMN_METADATA",
            "SQLITE_ENABLE_FTS3",
            "SQLITE_ENABLE_FTS4",
            "SQLITE_ENABLE_FTS5",
            "SQLITE_ENABLE_JSON1",
            "SQLITE_ENABLE_RTREE",
            "SQLITE_ENABLE_GEOPOLY",
            "SQLITE_THREADSAFE"
        };
        
        if (opt_index < 8) {
            sqlite3_compileoption_used(options[opt_index]);
        }
        
        // Also try with a string from the input data if we have enough bytes
        if (Size > 1) {
            // Create a null-terminated string from input data
            size_t str_len = Size < 100 ? Size : 100;
            char *opt_str = (char*)malloc(str_len + 1);
            if (opt_str) {
                memcpy(opt_str, Data, str_len);
                opt_str[str_len] = '\0';
                sqlite3_compileoption_used(opt_str);
                free(opt_str);
            }
        }
    }
    
    // Call sqlite3_open_v2 with different flags
    sqlite3 *db = NULL;
    int rc;
    
    // Write input data to dummy file
    write_dummy_file(Data, Size);
    
    // Try opening with different flags based on input data
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 1) {
        // Use second byte to modify flags
        uint8_t flag_bits = Data[1 % Size];
        if (flag_bits & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (flag_bits & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (flag_bits & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_bits & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_bits & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_bits & 0x20) flags |= SQLITE_OPEN_PRIVATECACHE;
    }
    
    // Try opening database
    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    
    // Try with different VFS if we have enough data
    if (Size > 2 && db) {
        sqlite3_close(db);
        db = NULL;
        
        // Use a substring as VFS name
        size_t vfs_len = Size < 50 ? Size : 50;
        char *vfs_name = (char*)malloc(vfs_len + 1);
        if (vfs_name) {
            memcpy(vfs_name, Data + (Size/2), vfs_len > Size/2 ? Size/2 : vfs_len);
            vfs_name[vfs_len] = '\0';
            rc = sqlite3_open_v2("./dummy_file", &db, flags, vfs_name);
            free(vfs_name);
        }
    }
    
    // Call sqlite3_close
    if (db) {
        sqlite3_close(db);
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}