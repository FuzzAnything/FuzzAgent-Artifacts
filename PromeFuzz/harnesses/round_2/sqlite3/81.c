// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:174586:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:173656:24 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:173716:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
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

/* Dummy callback for collation_needed16 */
static void dummy_collation_callback(void* pArg, sqlite3* db, int eTextRep, const void* pName) {
    /* Do nothing */
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    const char *err_msg = NULL;
    const void *err_msg16 = NULL;
    int rc, rc2;
    int system_errno;
    
    /* Use first byte to determine behavior variations */
    uint8_t mode = Size > 0 ? Data[0] : 0;
    
    /* Prepare filename for database */
    const char *filename_utf8 = "./dummy_file";
    
    /* Write data to dummy file (skip first byte if used for mode) */
    FILE *fp = fopen(filename_utf8, "wb");
    if (fp) {
        size_t write_size = Size > 1 ? Size - 1 : 0;
        const uint8_t *write_data = Size > 1 ? Data + 1 : Data;
        if (write_size > 0) {
            fwrite(write_data, 1, write_size, fp);
        }
        fclose(fp);
    }
    
    /* Test sqlite3_open with various modes based on fuzz input */
    rc = sqlite3_open(filename_utf8, &db);
    
    /* Test sqlite3_errmsg on the database handle */
    if (db) {
        err_msg = sqlite3_errmsg(db);
        (void)err_msg; /* Use result to avoid unused variable warning */
    }
    
    /* Test sqlite3_system_errno */
    system_errno = sqlite3_system_errno(db);
    (void)system_errno;
    
    /* Test sqlite3_errmsg16 */
    if (db) {
        err_msg16 = sqlite3_errmsg16(db);
        (void)err_msg16;
    }
    
    /* Test sqlite3_collation_needed16 */
    if (db) {
        rc2 = sqlite3_collation_needed16(db, NULL, dummy_collation_callback);
        (void)rc2;
    }
    
    /* Close first database if opened */
    if (db) {
        sqlite3_close(db);
    }
    
    /* Test sqlite3_open16 with UTF-16 filename */
    /* Convert ASCII filename to UTF-16 (simple conversion for ASCII chars) */
    size_t utf16_len = strlen(filename_utf8) * 2 + 2;
    uint16_t *filename_utf16 = (uint16_t*)malloc(utf16_len);
    if (filename_utf16) {
        /* Simple ASCII to UTF-16LE conversion */
        for (size_t i = 0; i < strlen(filename_utf8); i++) {
            filename_utf16[i] = (uint16_t)filename_utf8[i];
        }
        filename_utf16[strlen(filename_utf8)] = 0; /* Null terminator */
        
        /* Open with UTF-16 filename */
        rc = sqlite3_open16(filename_utf16, &db2);
        
        /* Test error functions on second database */
        if (db2) {
            err_msg = sqlite3_errmsg(db2);
            (void)err_msg;
            
            err_msg16 = sqlite3_errmsg16(db2);
            (void)err_msg16;
            
            system_errno = sqlite3_system_errno(db2);
            (void)system_errno;
            
            rc2 = sqlite3_collation_needed16(db2, NULL, dummy_collation_callback);
            (void)rc2;
        }
        
        /* Close second database if opened */
        if (db2) {
            sqlite3_close(db2);
        }
        
        free(filename_utf16);
    }
    
    /* Test with NULL filename (should create temporary database) */
    if ((mode & 0x1) && Size > 10) {
        rc = sqlite3_open(NULL, &db);
        if (db) {
            err_msg = sqlite3_errmsg(db);
            (void)err_msg;
            sqlite3_close(db);
        }
    }
    
    /* Test with empty string filename (should create temporary database) */
    if ((mode & 0x2) && Size > 20) {
        rc = sqlite3_open("", &db);
        if (db) {
            err_msg = sqlite3_errmsg(db);
            (void)err_msg;
            sqlite3_close(db);
        }
    }
    
    /* Test sqlite3_open16 with NULL (should create temporary database) */
    if ((mode & 0x4) && Size > 30) {
        rc = sqlite3_open16(NULL, &db);
        if (db) {
            err_msg16 = sqlite3_errmsg16(db);
            (void)err_msg16;
            sqlite3_close(db);
        }
    }
    
    /* Clean up dummy file */
    remove(filename_utf8);
    
    return 0;
}