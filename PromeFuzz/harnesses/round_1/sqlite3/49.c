// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

static void init_dummy_vfs(sqlite3_vfs *vfs) {
    static sqlite3_vfs base_vfs;
    
    memset(vfs, 0, sizeof(*vfs));
    vfs->iVersion = 3;
    vfs->szOsFile = sizeof(sqlite3_file);
    vfs->mxPathname = 1024;
    vfs->zName = "fuzz_vfs";
    
    sqlite3_vfs *default_vfs = sqlite3_vfs_find(NULL);
    if (default_vfs) {
        vfs->xRandomness = default_vfs->xRandomness;
        vfs->xSleep = default_vfs->xSleep;
        vfs->xCurrentTime = default_vfs->xCurrentTime;
        vfs->xCurrentTimeInt64 = default_vfs->xCurrentTimeInt64;
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    uint8_t selector = Data[0];
    
    if (Size >= sizeof(int)) {
        int sleep_ms;
        memcpy(&sleep_ms, Data, sizeof(int));
        sqlite3_sleep(sleep_ms);
    }
    
    sqlite3_vfs custom_vfs;
    init_dummy_vfs(&custom_vfs);
    
    int make_default = (selector & 1) ? 1 : 0;
    sqlite3_vfs_register(&custom_vfs, make_default);
    
    const char *vfs_name = (selector & 2) ? "fuzz_vfs" : NULL;
    sqlite3_vfs *found_vfs = sqlite3_vfs_find(vfs_name);
    
    sqlite3_vfs_unregister(&custom_vfs);
    
    sqlite3 *db = NULL;
    int rc;
    
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            size_t write_size = Size > 1024 ? 1024 : Size;
            write(fd, Data, write_size);
        }
        close(fd);
        
        rc = sqlite3_open(filename, &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
            db = NULL;
        }
        
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (selector & 4) {
            flags |= SQLITE_OPEN_MEMORY;
        }
        if (selector & 8) {
            flags |= SQLITE_OPEN_NOMUTEX;
        }
        
        const char *vfs_param = (selector & 16) ? "unix" : NULL;
        rc = sqlite3_open_v2(filename, &db, flags, vfs_param);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
        
        unlink(filename);
    }
    
    found_vfs = sqlite3_vfs_find("fuzz_vfs");
    
    if (Size >= 2 * sizeof(int)) {
        int sleep_ms2;
        memcpy(&sleep_ms2, Data + sizeof(int), sizeof(int));
        sqlite3_sleep(sleep_ms2);
    }
    
    for (int i = 0; i < 3 && i * 8 < Size; i++) {
        sqlite3_vfs_register(&custom_vfs, (selector >> i) & 1);
        sqlite3_vfs_unregister(&custom_vfs);
    }
    
    if (selector & 32) {
        rc = sqlite3_open("", &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
    }
    
    if (selector & 64) {
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif