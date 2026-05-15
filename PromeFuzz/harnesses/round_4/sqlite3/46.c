// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_xClose(sqlite3_file* file) { return SQLITE_OK; }
static int dummy_xRead(sqlite3_file* file, void* buf, int amt, sqlite3_int64 offset) { return SQLITE_IOERR; }
static int dummy_xWrite(sqlite3_file* file, const void* buf, int amt, sqlite3_int64 offset) { return SQLITE_IOERR; }
static int dummy_xTruncate(sqlite3_file* file, sqlite3_int64 size) { return SQLITE_IOERR; }
static int dummy_xSync(sqlite3_file* file, int flags) { return SQLITE_IOERR; }
static int dummy_xFileSize(sqlite3_file* file, sqlite3_int64* size) { *size = 0; return SQLITE_OK; }
static int dummy_xLock(sqlite3_file* file, int lock) { return SQLITE_IOERR; }
static int dummy_xUnlock(sqlite3_file* file, int lock) { return SQLITE_IOERR; }
static int dummy_xCheckReservedLock(sqlite3_file* file, int* out) { *out = 0; return SQLITE_OK; }
static int dummy_xFileControl(sqlite3_file* file, int op, void* arg) { return SQLITE_NOTFOUND; }
static int dummy_xSectorSize(sqlite3_file* file) { return 512; }
static int dummy_xDeviceCharacteristics(sqlite3_file* file) { return 0; }
static int dummy_xShmMap(sqlite3_file* file, int region, int size, int write, void volatile** out) { return SQLITE_IOERR; }
static int dummy_xShmLock(sqlite3_file* file, int offset, int n, int flags) { return SQLITE_IOERR; }
static void dummy_xShmBarrier(sqlite3_file* file) {}
static int dummy_xShmUnmap(sqlite3_file* file, int deleteFlag) { return SQLITE_IOERR; }
static int dummy_xFetch(sqlite3_file* file, sqlite3_int64 offset, int amt, void** out) { return SQLITE_IOERR; }
static int dummy_xUnfetch(sqlite3_file* file, sqlite3_int64 offset, void* ptr) { return SQLITE_IOERR; }

static sqlite3_io_methods dummy_io_methods = {
    3, /* iVersion */
    dummy_xClose,
    dummy_xRead,
    dummy_xWrite,
    dummy_xTruncate,
    dummy_xSync,
    dummy_xFileSize,
    dummy_xLock,
    dummy_xUnlock,
    dummy_xCheckReservedLock,
    dummy_xFileControl,
    dummy_xSectorSize,
    dummy_xDeviceCharacteristics,
    dummy_xShmMap,
    dummy_xShmLock,
    dummy_xShmBarrier,
    dummy_xShmUnmap,
    dummy_xFetch,
    dummy_xUnfetch
};

static int dummy_xOpen(sqlite3_vfs* vfs, const char* name, sqlite3_file* file, int flags, int* outFlags) {
    file->pMethods = &dummy_io_methods;
    return SQLITE_OK;
}
static int dummy_xDelete(sqlite3_vfs* vfs, const char* name, int syncDir) { return SQLITE_OK; }
static int dummy_xAccess(sqlite3_vfs* vfs, const char* name, int flags, int* out) { *out = 0; return SQLITE_OK; }
static int dummy_xFullPathname(sqlite3_vfs* vfs, const char* name, int outLen, char* out) {
    strncpy(out, name, outLen);
    return SQLITE_OK;
}
static void* dummy_xDlOpen(sqlite3_vfs* vfs, const char* filename) { return NULL; }
static void dummy_xDlError(sqlite3_vfs* vfs, int size, char* msg) {}
static void (*dummy_xDlSym(sqlite3_vfs* vfs, void* handle, const char* symbol))(void) { return NULL; }
static void dummy_xDlClose(sqlite3_vfs* vfs, void* handle) {}
static int dummy_xRandomness(sqlite3_vfs* vfs, int size, char* out) { return 0; }
static int dummy_xSleep(sqlite3_vfs* vfs, int microseconds) { return microseconds; }
static int dummy_xCurrentTime(sqlite3_vfs* vfs, double* out) { *out = 0.0; return SQLITE_OK; }
static int dummy_xGetLastError(sqlite3_vfs* vfs, int size, char* msg) { return 0; }
static int dummy_xCurrentTimeInt64(sqlite3_vfs* vfs, sqlite3_int64* out) { *out = 0; return SQLITE_OK; }

static sqlite3_vfs dummy_vfs = {
    3, /* iVersion */
    0, /* szOsFile */
    1024, /* mxPathname */
    NULL, /* pNext */
    "dummy", /* zName */
    NULL, /* pAppData */
    dummy_xOpen,
    dummy_xDelete,
    dummy_xAccess,
    dummy_xFullPathname,
    dummy_xDlOpen,
    dummy_xDlError,
    dummy_xDlSym,
    dummy_xDlClose,
    dummy_xRandomness,
    dummy_xSleep,
    dummy_xCurrentTime,
    dummy_xGetLastError,
    dummy_xCurrentTimeInt64,
    NULL, /* xSetSystemCall */
    NULL, /* xGetSystemCall */
    NULL  /* xNextSystemCall */
};

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Write data to dummy file */
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    /* Test sqlite3_vfs_find */
    const char* vfs_name = NULL;
    if (Size > 0) {
        /* Use first byte to decide: 0 = NULL, 1 = "dummy", 2 = random string */
        uint8_t choice = Data[0] % 3;
        if (choice == 1) {
            vfs_name = "dummy";
        } else if (choice == 2 && Size > 1) {
            /* Use up to 32 bytes as VFS name */
            size_t name_len = Size > 33 ? 32 : Size - 1;
            char* temp_name = (char*)malloc(name_len + 1);
            if (temp_name) {
                memcpy(temp_name, Data + 1, name_len);
                temp_name[name_len] = 0;
                sqlite3_vfs_find(temp_name);
                free(temp_name);
            }
        } else {
            vfs_name = NULL;
        }
    }
    sqlite3_vfs* vfs = sqlite3_vfs_find(vfs_name);
    
    /* Test sqlite3_sleep */
    int sleep_ms = 0;
    if (Size > 4) {
        memcpy(&sleep_ms, Data, sizeof(int));
    } else if (Size > 0) {
        sleep_ms = Data[0];
    }
    sqlite3_sleep(sleep_ms);
    
    /* Test sqlite3_vfs_register */
    int make_default = 0;
    if (Size > 0) {
        make_default = Data[0] & 1;
    }
    sqlite3_vfs_register(&dummy_vfs, make_default);
    
    /* Test sqlite3_open and sqlite3_open_v2 */
    sqlite3* db = NULL;
    const char* filename = "./dummy_file";
    
    /* Try different open modes based on input data */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        uint8_t mode = Data[0] % 4;
        switch (mode) {
            case 0: flags = SQLITE_OPEN_READONLY; break;
            case 1: flags = SQLITE_OPEN_READWRITE; break;
            case 2: flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE; break;
            case 3: flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_MEMORY; break;
        }
    }
    
    /* Test sqlite3_open */
    int rc = sqlite3_open(filename, &db);
    if (db) {
        /* Test sqlite3_file_control */
        const char* db_name = NULL;
        if (Size > 0) {
            uint8_t db_choice = Data[0] % 3;
            if (db_choice == 0) {
                db_name = NULL;
            } else if (db_choice == 1) {
                db_name = "main";
            } else {
                db_name = "temp";
            }
        }
        
        int op = 0;
        if (Size > 4) {
            memcpy(&op, Data, sizeof(int));
        }
        sqlite3_file_control(db, db_name, op, NULL);
        
        sqlite3_close(db);
    }
    
    /* Test sqlite3_open_v2 */
    const char* vfs_name_for_open = NULL;
    if (Size > 0 && (Data[0] & 0x80)) {
        vfs_name_for_open = "dummy";
    }
    
    rc = sqlite3_open_v2(filename, &db, flags, vfs_name_for_open);
    if (db) {
        sqlite3_close(db);
    }
    
    /* Unregister the dummy VFS to clean up */
    sqlite3_vfs_unregister(&dummy_vfs);
    
    /* Cleanup */
    remove("./dummy_file");
    
    return 0;
}