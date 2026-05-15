// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Use first byte to determine which functions to call
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // 1. Test sqlite3_sleep
    if (selector & 0x01) {
        int sleep_ms = 0;
        if (Size - offset >= sizeof(int)) {
            memcpy(&sleep_ms, Data + offset, sizeof(int));
            offset += sizeof(int);
        } else if (Size - offset > 0) {
            sleep_ms = Data[offset];
            offset++;
        }
        sqlite3_sleep(sleep_ms);
    }
    
    // 2. Test sqlite3_vfs_find
    sqlite3_vfs* vfs = NULL;
    if (selector & 0x02) {
        const char* vfs_name = NULL;
        if (Size - offset > 0) {
            // Use up to 64 bytes for VFS name
            size_t name_len = Size - offset;
            if (name_len > 64) name_len = 64;
            
            // Ensure null termination
            char* temp_name = malloc(name_len + 1);
            if (temp_name) {
                memcpy(temp_name, Data + offset, name_len);
                temp_name[name_len] = '\0';
                vfs_name = temp_name;
                offset += name_len;
            }
        }
        vfs = sqlite3_vfs_find(vfs_name);
        if (vfs_name) free((void*)vfs_name);
    }
    
    // 3. Test sqlite3_vfs_unregister (only if we found a non-default VFS)
    if ((selector & 0x04) && vfs && vfs != sqlite3_vfs_find(NULL)) {
        sqlite3_vfs_unregister(vfs);
    }
    
    // 4. Test sqlite3_open and sqlite3_open_v2
    if (selector & 0x08) {
        sqlite3* db = NULL;
        int flags = 0;
        
        // Generate filename from data
        char filename[256] = "./dummy_file";
        
        // Write data to file for database operations
        FILE* fp = fopen(filename, "wb");
        if (fp) {
            if (Size > offset) {
                fwrite(Data + offset, 1, Size - offset, fp);
            }
            fclose(fp);
        }
        
        // Try sqlite3_open
        int rc1 = sqlite3_open(filename, &db);
        if (rc1 == SQLITE_OK && db) {
            sqlite3_close(db);
            db = NULL;
        }
        
        // Try sqlite3_open_v2 with various flags
        if (Size - offset >= sizeof(int)) {
            memcpy(&flags, Data + offset, sizeof(int));
            offset += sizeof(int);
        }
        
        // Ensure at least one valid flag combination
        if (!(flags & (SQLITE_OPEN_READONLY | SQLITE_OPEN_READWRITE))) {
            flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        }
        
        const char* vfs_name_for_open = NULL;
        if (vfs) {
            vfs_name_for_open = vfs->zName;
        }
        
        int rc2 = sqlite3_open_v2(filename, &db, flags, vfs_name_for_open);
        if (rc2 == SQLITE_OK && db) {
            sqlite3_close(db);
        }
        
        // Clean up file
        remove(filename);
    }
    
    // 5. Test sqlite3_vfs_register with a dummy VFS
    if (selector & 0x10) {
        // Create a minimal dummy VFS structure
        static sqlite3_vfs dummy_vfs = {
            3,                          // iVersion
            sizeof(sqlite3_file),       // szOsFile
            1024,                       // mxPathname
            NULL,                       // pNext
            "dummy_vfs",                // zName
            NULL,                       // pAppData
            NULL,                       // xOpen
            NULL,                       // xDelete
            NULL,                       // xAccess
            NULL,                       // xFullPathname
            NULL,                       // xDlOpen
            NULL,                       // xDlError
            NULL,                       // xDlSym
            NULL,                       // xDlClose
            NULL,                       // xRandomness
            NULL,                       // xSleep
            NULL,                       // xCurrentTime
            NULL,                       // xGetLastError
            NULL,                       // xCurrentTimeInt64
            NULL,                       // xSetSystemCall
            NULL,                       // xGetSystemCall
            NULL                        // xNextSystemCall
        };
        
        int make_default = (selector & 0x20) ? 1 : 0;
        sqlite3_vfs_register(&dummy_vfs, make_default);
        
        // Unregister it immediately to avoid accumulation
        sqlite3_vfs_unregister(&dummy_vfs);
    }
    
    return 0;
}