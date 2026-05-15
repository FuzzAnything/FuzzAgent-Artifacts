// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    void *arg = NULL;
    
    /* Use first byte to determine which operations to perform */
    uint8_t selector = Data[0];
    
    /* Write data to dummy file if needed */
    if (Size > 1) {
        write_dummy_file(Data + 1, Size - 1);
    }
    
    /* Test sqlite3_open */
    if (selector & 0x01) {
        rc = sqlite3_open("./dummy_file", &db);
        if (rc == SQLITE_OK && db) {
            /* Test sqlite3_file_control with various opcodes */
            int op = (int)(Size > 2 ? Data[2] : 0);
            const char *zDbName = (selector & 0x02) ? "main" : NULL;
            sqlite3_file_control(db, zDbName, op, &arg);
            
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    /* Test sqlite3_open_v2 with various flags */
    if (selector & 0x04) {
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (Size > 3) {
            flags = (int)Data[3];
        }
        
        const char *zVfs = NULL;
        if (selector & 0x08 && Size > 10) {
            /* Use a portion of data as VFS name (null-terminated) */
            static char vfs_name[32];
            size_t len = Size - 10 < 31 ? Size - 10 : 31;
            memcpy(vfs_name, Data + 10, len);
            vfs_name[len] = '\0';
            zVfs = vfs_name;
        }
        
        rc = sqlite3_open_v2("./dummy_file", &db, flags, zVfs);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    /* Test VFS operations */
    if (selector & 0x10) {
        sqlite3_vfs *vfs = NULL;
        
        /* Find default VFS */
        vfs = sqlite3_vfs_find(NULL);
        
        if (vfs && (selector & 0x20)) {
            /* Try to unregister and re-register */
            sqlite3_vfs_unregister(vfs);
            sqlite3_vfs_register(vfs, (selector & 0x40) ? 1 : 0);
        }
        
        /* Try to find a VFS by name if we have enough data */
        if (Size > 20) {
            static char vfs_name[64];
            size_t len = Size - 20 < 63 ? Size - 20 : 63;
            memcpy(vfs_name, Data + 20, len);
            vfs_name[len] = '\0';
            sqlite3_vfs_find(vfs_name);
        }
    }
    
    /* Open in-memory database for additional testing */
    if (selector & 0x80) {
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db) {
            /* Test file_control on in-memory database */
            int op = (int)(Size > 1 ? Data[1] : SQLITE_FCNTL_LOCKSTATE);
            sqlite3_file_control(db, "main", op, &arg);
            
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    /* Clean up */
    if (db) {
        sqlite3_close(db);
    }
    
    cleanup_dummy_file();
    
    return 0;
}