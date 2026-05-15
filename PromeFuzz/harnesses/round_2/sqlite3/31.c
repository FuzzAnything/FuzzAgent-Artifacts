// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

static int busy_handler_callback(void* data, int count) {
    return count < 5 ? 1 : 0;
}

static void dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int(ctx, 123);
}

static void dummy_step(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
}

static void dummy_final(sqlite3_context* ctx) {
    sqlite3_result_int(ctx, 456);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    char* errmsg = NULL;
    
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    const char* vfs = NULL;
    
    rc = sqlite3_open_v2(filename, &db, flags, vfs);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    rc = sqlite3_close(db);
    if (rc == SQLITE_BUSY) {
        sqlite3_close(db);
    }
    
    db = NULL;
    rc = sqlite3_open_v2(filename, &db, flags, vfs);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    const char* func_name = "fuzz_func";
    int nArg = 1;
    int eTextRep = SQLITE_UTF8;
    void* pApp = NULL;
    
    rc = sqlite3_create_function(db, func_name, nArg, eTextRep, pApp, 
                                 dummy_func, dummy_step, dummy_final);
    
    rc = sqlite3_busy_handler(db, busy_handler_callback, NULL);
    
    const char* sql = "SELECT fuzz_func(1); CREATE TABLE IF NOT EXISTS t(x);";
    rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    sqlite3_close(db);
    
    return 0;
}