// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
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

/* Dummy destructor for client data */
static void dummy_destructor(void *ptr) {
    /* Do nothing */
}

/* Dummy file path */
static const char* dummy_file = "./dummy_file";

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    const char *err_msg;
    void *client_data_ptr;
    void *retrieved_data;
    
    /* Use first byte to determine flags for open */
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0xFF;
    }
    
    /* Use second byte to determine VFS selection */
    const char *zVfs = NULL;
    if (Size > 1 && (Data[1] & 0x01)) {
        zVfs = "unix"; /* Example VFS name */
    }
    
    /* Create dummy file with fuzzer data */
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    /* Test sqlite3_open_v2 */
    rc = sqlite3_open_v2(dummy_file, &db, flags, zVfs);
    
    if (rc == SQLITE_OK && db) {
        /* Test sqlite3_errmsg */
        err_msg = sqlite3_errmsg(db);
        
        /* Test sqlite3_set_clientdata and sqlite3_get_clientdata */
        if (Size > 2) {
            /* Use third byte as client data name length */
            size_t name_len = (Data[2] % 32) + 1;
            if (name_len > Size - 3) {
                name_len = Size > 3 ? Size - 3 : 1;
            }
            
            /* Create client data name from fuzzer input */
            char *name = (char*)malloc(name_len + 1);
            if (name) {
                size_t copy_len = name_len;
                if (copy_len > Size - 3) {
                    copy_len = Size > 3 ? Size - 3 : 0;
                }
                if (copy_len > 0) {
                    memcpy(name, Data + 3, copy_len);
                }
                name[copy_len] = '\0';
                
                /* Set client data */
                client_data_ptr = (void*)(uintptr_t)Data[0];
                rc = sqlite3_set_clientdata(db, name, client_data_ptr, dummy_destructor);
                
                /* Get client data */
                retrieved_data = sqlite3_get_clientdata(db, name);
                
                free(name);
            }
        }
        
        /* Test with different client data names */
        if (Size > 10) {
            char name2[2] = {0};
            name2[0] = Data[3] ? Data[3] : 'A';
            void *data2 = (void*)(uintptr_t)Data[4];
            sqlite3_set_clientdata(db, name2, data2, NULL);
            sqlite3_get_clientdata(db, name2);
        }
        
        /* Test sqlite3_close */
        rc = sqlite3_close(db);
        
        /* Test sqlite3_close with NULL */
        sqlite3_close(NULL);
    }
    
    /* Test with in-memory database */
    rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc == SQLITE_OK && db2) {
        /* Test client data operations on in-memory db */
        if (Size > 5) {
            char name3[2] = {Data[5], '\0'};
            void *data3 = (void*)(uintptr_t)Data[6];
            sqlite3_set_clientdata(db2, name3, data3, dummy_destructor);
            sqlite3_get_clientdata(db2, name3);
        }
        
        sqlite3_close(db2);
    }
    
    /* Test sqlite3_get_auxdata - requires sqlite3_context */
    /* We can't easily create a valid sqlite3_context without implementing
       a custom SQL function, but we can test with NULL context */
    if (Size > 7) {
        int N = Data[7] % 10; /* Limit N to small values */
        sqlite3_context *ctx = NULL;
        sqlite3_get_auxdata(ctx, N);
    }
    
    /* Clean up dummy file */
    remove(dummy_file);
    
    return 0;
}