// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:127555:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    /* Test sqlite3_open and sqlite3_open_v2 with various flags */
    write_dummy_file(Data, Size);
    
    /* Use first byte to determine open flags */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_URI;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_byte & 0x20) flags |= SQLITE_OPEN_PRIVATECACHE;
        if (flag_byte & 0x40) flags |= SQLITE_OPEN_EXRESCODE;
        if (flag_byte & 0x80) flags |= SQLITE_OPEN_NOFOLLOW;
    }
    
    /* Test sqlite3_open */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        /* Test sqlite3_db_filename */
        const char *zDbName = "main";
        if (Size > 1) {
            /* Use second byte to select database name */
            uint8_t db_selector = Data[1];
            if (db_selector == 0) zDbName = "main";
            else if (db_selector == 1) zDbName = "temp";
            else if (db_selector == 2) zDbName = NULL;
            else {
                /* Try using a string from the data */
                size_t name_len = Size > 2 ? Data[2] % (Size - 2) : 0;
                if (name_len > 0) {
                    char *temp_name = malloc(name_len + 1);
                    if (temp_name) {
                        memcpy(temp_name, Data + 3, name_len);
                        temp_name[name_len] = '\0';
                        sqlite3_db_filename(db, temp_name);
                        free(temp_name);
                    }
                }
            }
        }
        sqlite3_filename filename = sqlite3_db_filename(db, zDbName);
        (void)filename; /* Use result to avoid unused variable warning */
        
        /* Test sqlite3_db_name */
        int N = 0;
        if (Size > 3) {
            N = Data[3] % 10; /* Test with N from 0 to 9 */
        }
        const char *db_name = sqlite3_db_name(db, N);
        (void)db_name; /* Use result to avoid unused variable warning */
        
        /* Test sqlite3_load_extension - but first enable it */
        sqlite3_enable_load_extension(db, 1);
        
        const char *zFile = "./dummy_file";
        const char *zProc = NULL;
        if (Size > 4) {
            /* Use fourth byte to decide whether to specify proc name */
            if (Data[4] & 0x01) {
                zProc = "sqlite3_extension_init";
            }
        }
        
        rc = sqlite3_load_extension(db, zFile, zProc, &zErrMsg);
        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
        
        /* Test sqlite3_file_control */
        if (Size > 5) {
            int op = Data[5];
            void *pArg = NULL;
            rc = sqlite3_file_control(db, zDbName, op, pArg);
        }
        
        sqlite3_close(db);
        db = NULL;
    }
    
    /* Test sqlite3_open_v2 */
    const char *zVfs = NULL;
    if (Size > 6) {
        /* Use sixth byte to decide VFS */
        if (Data[6] & 0x01) {
            zVfs = "unix"; /* Common VFS name */
        }
    }
    
    rc = sqlite3_open_v2("./dummy_file", &db, flags, zVfs);
    if (rc == SQLITE_OK && db) {
        /* Test the same functions with the v2-opened database */
        const char *zDbName = "main";
        sqlite3_filename filename2 = sqlite3_db_filename(db, zDbName);
        (void)filename2;
        
        const char *db_name2 = sqlite3_db_name(db, 0);
        (void)db_name2;
        
        sqlite3_close(db);
    }
    
    /* Clean up */
    remove_dummy_file();
    
    return 0;
}