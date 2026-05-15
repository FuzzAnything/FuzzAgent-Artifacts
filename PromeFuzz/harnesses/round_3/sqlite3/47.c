// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    sqlite3 *db3 = NULL;
    sqlite3 *db4 = NULL;
    const char *errmsg = NULL;
    int rc, rc2, rc3, rc4;
    int sys_errno, extended_err;
    
    // Prepare a dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Test sqlite3_open with UTF-8 filename
    // Use fuzzer data to create filename or use dummy file
    if (Size > 0) {
        // Try using first few bytes as filename (null-terminated)
        size_t len = Size < 100 ? Size : 100;
        char *filename = (char*)malloc(len + 1);
        if (filename) {
            memcpy(filename, Data, len);
            filename[len] = '\0';
            rc = sqlite3_open(filename, &db);
            free(filename);
        } else {
            rc = sqlite3_open("./dummy_file", &db);
        }
    } else {
        rc = sqlite3_open("./dummy_file", &db);
    }
    
    // Test sqlite3_open16 with UTF-16 filename
    // Convert dummy filename to UTF-16
    const char *utf8_filename = "./dummy_file";
    size_t utf8_len = strlen(utf8_filename);
    // Simple conversion: ASCII to UTF-16LE (not proper conversion, but works for ASCII)
    size_t utf16_len = (utf8_len + 1) * 2;
    uint8_t *utf16_buf = (uint8_t*)malloc(utf16_len);
    if (utf16_buf) {
        for (size_t i = 0; i <= utf8_len; i++) {
            utf16_buf[i*2] = utf8_filename[i];
            utf16_buf[i*2 + 1] = 0;
        }
        rc2 = sqlite3_open16(utf16_buf, &db2);
        free(utf16_buf);
    } else {
        rc2 = SQLITE_NOMEM;
    }
    
    // Test sqlite3_open_v2 with various flags
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        // Use first byte to modify flags
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_byte & 0x20) flags |= SQLITE_OPEN_PRIVATECACHE;
        if (flag_byte & 0x40) flags |= SQLITE_OPEN_URI;
    }
    
    const char *vfs_name = NULL;
    if (Size > 1 && (Data[1] & 0x01)) {
        // Try with NULL VFS
        vfs_name = NULL;
    }
    
    rc3 = sqlite3_open_v2("./dummy_file", &db3, flags, vfs_name);
    
    // Test sqlite3_system_errno on opened databases
    if (db) {
        sys_errno = sqlite3_system_errno(db);
        (void)sys_errno; // Use result to avoid unused variable warning
    }
    
    if (db2) {
        sys_errno = sqlite3_system_errno(db2);
        (void)sys_errno;
    }
    
    if (db3) {
        sys_errno = sqlite3_system_errno(db3);
        (void)sys_errno;
    }
    
    // Test sqlite3_extended_errcode on opened databases
    if (db) {
        extended_err = sqlite3_extended_errcode(db);
        (void)extended_err;
    }
    
    if (db2) {
        extended_err = sqlite3_extended_errcode(db2);
        (void)extended_err;
    }
    
    if (db3) {
        extended_err = sqlite3_extended_errcode(db3);
        (void)extended_err;
    }
    
    // Test sqlite3_errmsg on opened databases
    if (db) {
        errmsg = sqlite3_errmsg(db);
        (void)errmsg;
    }
    
    if (db2) {
        errmsg = sqlite3_errmsg(db2);
        (void)errmsg;
    }
    
    if (db3) {
        errmsg = sqlite3_errmsg(db3);
        (void)errmsg;
    }
    
    // Test with NULL database handle
    sys_errno = sqlite3_system_errno(NULL);
    extended_err = sqlite3_extended_errcode(NULL);
    errmsg = sqlite3_errmsg(NULL);
    (void)sys_errno;
    (void)extended_err;
    (void)errmsg;
    
    // Test sqlite3_open with NULL filename (should create temporary database)
    if (Size > 0 && (Data[0] & 0x80)) {
        rc4 = sqlite3_open(NULL, &db4);
        if (db4) {
            sys_errno = sqlite3_system_errno(db4);
            extended_err = sqlite3_extended_errcode(db4);
            errmsg = sqlite3_errmsg(db4);
            (void)sys_errno;
            (void)extended_err;
            (void)errmsg;
            sqlite3_close(db4);
        }
    }
    
    // Test sqlite3_open16 with NULL filename (should create temporary database)
    if (Size > 1 && (Data[1] & 0x02)) {
        rc4 = sqlite3_open16(NULL, &db4);
        if (db4) {
            sys_errno = sqlite3_system_errno(db4);
            extended_err = sqlite3_extended_errcode(db4);
            errmsg = sqlite3_errmsg(db4);
            (void)sys_errno;
            (void)extended_err;
            (void)errmsg;
            sqlite3_close(db4);
        }
    }
    
    // Test sqlite3_open with empty string filename (should create temporary database)
    if (Size > 2 && (Data[2] & 0x01)) {
        rc4 = sqlite3_open("", &db4);
        if (db4) {
            sys_errno = sqlite3_system_errno(db4);
            extended_err = sqlite3_extended_errcode(db4);
            errmsg = sqlite3_errmsg(db4);
            (void)sys_errno;
            (void)extended_err;
            (void)errmsg;
            sqlite3_close(db4);
        }
    }
    
    // Test sqlite3_open with ":memory:" filename
    if (Size > 2 && (Data[2] & 0x02)) {
        rc4 = sqlite3_open(":memory:", &db4);
        if (db4) {
            sys_errno = sqlite3_system_errno(db4);
            extended_err = sqlite3_extended_errcode(db4);
            errmsg = sqlite3_errmsg(db4);
            (void)sys_errno;
            (void)extended_err;
            (void)errmsg;
            sqlite3_close(db4);
        }
    }
    
    // Clean up
    if (db) sqlite3_close(db);
    if (db2) sqlite3_close(db2);
    if (db3) sqlite3_close(db3);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}