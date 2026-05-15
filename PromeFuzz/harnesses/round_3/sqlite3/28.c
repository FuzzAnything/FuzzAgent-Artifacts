// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int dummy_busy_handler(void *data, int count) {
    (void)data;
    (void)count;
    return 0; /* Abort on busy */
}

static void dummy_scalar_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

static int dummy_callback(void *data, int argc, char **argv, char **col_names) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    const char *filename = "./dummy_file";
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    int rc;
    
    /* Write fuzzer data to dummy file */
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    /* 1. sqlite3_open_v2 */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* 2. sqlite3_close (early close to test error paths) */
    sqlite3_close(db);
    
    /* Re-open for remaining functions */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* 3. sqlite3_create_function */
    /* Use fuzzer data to determine function name and parameters */
    const char *func_name = "fuzz_func";
    int nArg = -1; /* Variable number of arguments */
    int eTextRep = SQLITE_UTF8;
    void *pApp = NULL;
    
    if (Size > 0) {
        /* Use first byte to determine nArg (limit to -1..127) */
        nArg = (int8_t)Data[0];
        if (nArg < -1) nArg = -1;
        if (nArg > 127) nArg = 127;
        
        /* Use second byte to determine encoding */
        if (Size > 1) {
            switch (Data[1] & 0x03) {
                case 0: eTextRep = SQLITE_UTF8; break;
                case 1: eTextRep = SQLITE_UTF16; break;
                case 2: eTextRep = SQLITE_UTF16LE; break;
                case 3: eTextRep = SQLITE_UTF16BE; break;
            }
            if (Data[1] & 0x04) eTextRep |= SQLITE_DETERMINISTIC;
            if (Data[1] & 0x08) eTextRep |= SQLITE_DIRECTONLY;
        }
    }
    
    sqlite3_create_function(db, func_name, nArg, eTextRep, pApp,
                           dummy_scalar_function, NULL, NULL);
    
    /* 4. sqlite3_busy_handler */
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    /* 5. sqlite3_exec */
    /* Create SQL from fuzzer data (ensure null termination) */
    char *sql = NULL;
    if (Size > 0) {
        sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            /* Try different SQL statements */
            rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            
            /* Try with CREATE TABLE */
            const char *create_sql = "CREATE TABLE IF NOT EXISTS t1(a,b,c);";
            rc = sqlite3_exec(db, create_sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            
            /* Try with INSERT using fuzzer data */
            char insert_sql[256];
            if (Size > 0) {
                snprintf(insert_sql, sizeof(insert_sql),
                        "INSERT INTO t1 VALUES(%d,'test',x'%02x');",
                        (int)Size, Size > 0 ? Data[0] : 0);
                rc = sqlite3_exec(db, insert_sql, dummy_callback, NULL, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
            }
            
            free(sql);
        }
    } else {
        /* Empty input - test with simple SQL */
        rc = sqlite3_exec(db, "SELECT 1;", dummy_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    /* Remove dummy file */
    remove(filename);
    
    return 0;
}