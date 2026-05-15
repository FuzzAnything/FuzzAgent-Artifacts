// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
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

static int dummy_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

static int dummy_busy_handler(void* data, int count) {
    return 0;
}

static void dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int(ctx, 0);
}

static void dummy_step(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
}

static void dummy_final(sqlite3_context* ctx) {
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char* db_file = "./dummy_file";
    sqlite3* db = NULL;
    char* errmsg = NULL;
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX;
    if (Size > 4) {
        flags = *((int*)Data) & 0xFF;
    }
    
    int rc = sqlite3_open_v2(db_file, &db, flags, NULL);
    
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        remove(db_file);
        return 0;
    }
    
    sqlite3_close(db);
    
    rc = sqlite3_open_v2(db_file, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        remove(db_file);
        return 0;
    }
    
    size_t func_name_len = Size > 100 ? 100 : Size;
    char* func_name = malloc(func_name_len + 1);
    if (!func_name) {
        sqlite3_close(db);
        remove(db_file);
        return 0;
    }
    memcpy(func_name, Data, func_name_len);
    func_name[func_name_len] = '\0';
    
    int nArg = (int)(Size % 128);
    if (nArg < -1) nArg = -1;
    
    int eTextRep = SQLITE_UTF8;
    if (Size > 1) {
        eTextRep = Data[1] & 0x0F;
        if (Size > 2 && (Data[2] & 0x01)) {
            eTextRep |= SQLITE_DETERMINISTIC;
        }
    }
    
    sqlite3_create_function(db, func_name, nArg, eTextRep, NULL, 
                           dummy_func, dummy_step, dummy_final);
    
    free(func_name);
    
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    size_t sql_len = Size > 1024 ? 1024 : Size;
    char* sql = malloc(sql_len + 1);
    if (!sql) {
        sqlite3_close(db);
        remove(db_file);
        return 0;
    }
    
    memcpy(sql, Data, sql_len);
    sql[sql_len] = '\0';
    
    sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    free(sql);
    sqlite3_close(db);
    remove(db_file);
    
    return 0;
}