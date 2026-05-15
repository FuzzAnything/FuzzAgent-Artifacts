// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
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

/* Dummy busy handler callback */
static int dummy_busy_handler(void *data, int count) {
    /* Return non-zero to retry, zero to abort */
    return (count < 3) ? 1 : 0;
}

/* Dummy SQL function callback */
static void dummy_sql_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_int(context, 42);
}

/* Dummy exec callback */
static int dummy_exec_callback(void *data, int argc, char **argv, char **col_names) {
    return 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    
    /* Use fixed database filename as instructed */
    const char *filename = "./dummy_file";
    
    /* Open database with various flags based on fuzzed data */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        /* Use first byte to influence flags */
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_PRIVATECACHE;
    }
    
    /* 1. sqlite3_open_v2 */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        /* If open fails, we can't proceed with other operations */
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    /* 2. sqlite3_close - but we need to keep db open for subsequent calls */
    /* We'll call close at the end instead */
    
    /* 3. sqlite3_create_function */
    /* Create a dummy function with parameters influenced by fuzzed data */
    int nArg = -1; /* Variable number of arguments */
    if (Size > 1) {
        /* Use second byte to influence number of arguments */
        nArg = Data[1] % 10; /* Limit to reasonable number */
    }
    
    int eTextRep = SQLITE_UTF8;
    if (Size > 2) {
        /* Use third byte to influence text encoding */
        uint8_t enc_byte = Data[2];
        if (enc_byte & 0x01) eTextRep = SQLITE_UTF16;
        if (enc_byte & 0x02) eTextRep = SQLITE_UTF16LE;
        if (enc_byte & 0x04) eTextRep = SQLITE_UTF16BE;
        if (enc_byte & 0x08) eTextRep |= SQLITE_DETERMINISTIC;
    }
    
    rc = sqlite3_create_function(db, "dummy_func", nArg, eTextRep, 
                                 NULL, dummy_sql_function, NULL, NULL);
    
    /* 4. sqlite3_busy_handler */
    void *busy_data = NULL;
    if (Size > 3) {
        /* Use fourth byte as busy handler data */
        busy_data = (void *)(uintptr_t)Data[3];
    }
    sqlite3_busy_handler(db, dummy_busy_handler, busy_data);
    
    /* 5. sqlite3_exec */
    /* Create SQL statement from fuzzed data */
    char *sql = NULL;
    if (Size > 4) {
        /* Use remaining data for SQL statement, but ensure it's null-terminated */
        size_t sql_len = Size - 4;
        if (sql_len > 1024) sql_len = 1024; /* Limit size */
        
        sql = (char *)malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data + 4, sql_len);
            sql[sql_len] = '\0';
            
            /* Execute the SQL */
            rc = sqlite3_exec(db, sql, dummy_exec_callback, NULL, &errmsg);
            
            /* Free error message if any */
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            
            free(sql);
        }
    } else {
        /* Use a simple default SQL statement */
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t(x);", 
                         dummy_exec_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
        }
    }
    
    /* Now close the database */
    sqlite3_close(db);
    
    return 0;
}