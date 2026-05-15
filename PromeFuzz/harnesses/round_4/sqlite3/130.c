// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {}
static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {}
static void dummy_xFinal(sqlite3_context *ctx) {}
static void dummy_xValue(sqlite3_context *ctx) {}
static void dummy_xInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {}
static void dummy_xDestroy(void *p) {}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    char func_name[256] = {0};
    size_t name_len = 0;
    if (Size - offset > 0) {
        name_len = (Size - offset) > 255 ? 255 : (Size - offset);
        memcpy(func_name, Data + offset, name_len);
        func_name[name_len] = '\0';
        offset += name_len;
    } else {
        strcpy(func_name, "fuzz_func");
    }
    
    int nArg = -1;
    if (Size - offset >= 1) {
        nArg = (int)Data[offset] % 128;
        if (nArg < -1) nArg = -1;
        offset++;
    }
    
    int eTextRep = SQLITE_UTF8;
    if (Size - offset >= 1) {
        uint8_t enc = Data[offset] % 8;
        switch (enc) {
            case 0: eTextRep = SQLITE_UTF8; break;
            case 1: eTextRep = SQLITE_UTF16LE; break;
            case 2: eTextRep = SQLITE_UTF16BE; break;
            case 3: eTextRep = SQLITE_UTF16; break;
            case 4: eTextRep = SQLITE_UTF8 | SQLITE_DETERMINISTIC; break;
            case 5: eTextRep = SQLITE_UTF8 | SQLITE_DIRECTONLY; break;
            case 6: eTextRep = SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY; break;
            default: eTextRep = SQLITE_UTF8; break;
        }
        offset++;
    }
    
    void *pApp = NULL;
    static int dummy_app_data = 42;
    pApp = &dummy_app_data;
    
    switch (selector) {
        case 0:
            sqlite3_create_window_function(
                db, func_name, nArg, eTextRep, pApp,
                dummy_xStep, dummy_xFinal, dummy_xValue, dummy_xInverse, dummy_xDestroy);
            break;
        case 1:
            sqlite3_create_function_v2(
                db, func_name, nArg, eTextRep, pApp,
                dummy_xFunc, dummy_xStep, dummy_xFinal, dummy_xDestroy);
            break;
        case 2:
            sqlite3_overload_function(db, func_name, nArg);
            break;
        case 3: {
            char key_name[256] = {0};
            size_t key_len = 0;
            if (Size - offset > 0) {
                key_len = (Size - offset) > 255 ? 255 : (Size - offset);
                memcpy(key_name, Data + offset, key_len);
                key_name[key_len] = '\0';
            } else {
                strcpy(key_name, "fuzz_key");
            }
            sqlite3_set_clientdata(db, key_name, pApp, dummy_xDestroy);
            break;
        }
        case 4:
            sqlite3_create_function(
                db, func_name, nArg, eTextRep, pApp,
                dummy_xFunc, dummy_xStep, dummy_xFinal);
            break;
        case 5: {
            sqlite3_create_function16(
                db, (const void*)func_name, nArg, eTextRep, pApp,
                dummy_xFunc, dummy_xStep, dummy_xFinal);
            break;
        }
    }
    
    sqlite3_close(db);
    return 0;
}