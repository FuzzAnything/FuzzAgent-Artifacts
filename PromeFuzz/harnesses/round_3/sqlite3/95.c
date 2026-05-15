// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback functions for testing */
static void dummy_xFunc(sqlite3_context* ctx, int argc, sqlite3_value** argv) {}
static void dummy_xStep(sqlite3_context* ctx, int argc, sqlite3_value** argv) {}
static void dummy_xFinal(sqlite3_context* ctx) {}
static void dummy_xValue(sqlite3_context* ctx) {}
static void dummy_xInverse(sqlite3_context* ctx, int argc, sqlite3_value** argv) {}
static void dummy_xDestroy(void* p) {}
static void dummy_destructor(void* p) {}

/* Helper to create UTF-16 string from fuzzer input */
static void* create_utf16_string(const uint8_t* data, size_t size, size_t* out_len) {
    if (size == 0) {
        *out_len = 2;
        uint16_t* empty = malloc(2);
        if (empty) empty[0] = 0;
        return empty;
    }
    
    /* Simple conversion: treat as UTF-16LE, ensure null termination */
    size_t utf16_len = (size / 2) + 1;
    uint16_t* utf16 = malloc(utf16_len * 2);
    if (!utf16) return NULL;
    
    size_t i;
    for (i = 0; i < size / 2 && i < utf16_len - 1; i++) {
        utf16[i] = (data[i*2] << 8) | data[i*2 + 1];
    }
    utf16[i] = 0;
    
    *out_len = (i + 1) * 2;
    return utf16;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Use fuzzer data to generate parameters */
    size_t data_idx = 0;
    
    /* Generate function name from fuzzer data */
    const char* func_name = "fuzz_func";
    char name_buf[256];
    if (Size > 0) {
        size_t name_len = Data[0] % 255;
        if (name_len > Size - 1) name_len = Size > 1 ? Size - 1 : 0;
        if (name_len > 0) {
            memcpy(name_buf, Data + 1, name_len);
            name_buf[name_len] = '\0';
            func_name = name_buf;
        }
        data_idx = 1 + name_len;
    }
    
    /* Generate parameters from remaining data */
    int nArg = -1;
    int eTextRep = SQLITE_UTF8;
    void* pApp = NULL;
    
    if (Size - data_idx >= 2) {
        nArg = Data[data_idx] % 128;
        eTextRep = Data[data_idx + 1] % 8;
        pApp = (void*)(uintptr_t)Data[data_idx];
        data_idx += 2;
    }
    
    /* Add deterministic flag sometimes */
    if (Size > 0 && (Data[0] & 0x80)) {
        eTextRep |= SQLITE_DETERMINISTIC;
    }
    
    /* Test sqlite3_set_clientdata */
    if (Size > 0) {
        char client_name[256];
        size_t client_len = Data[0] % 255;
        if (client_len > Size - 1) client_len = Size > 1 ? Size - 1 : 0;
        if (client_len > 0) {
            memcpy(client_name, Data, client_len);
            client_name[client_len] = '\0';
            void* client_data = (void*)(uintptr_t)Data[0];
            sqlite3_set_clientdata(db, client_name, client_data, dummy_destructor);
        }
    }
    
    /* Test sqlite3_create_function16 */
    if (Size > 0) {
        size_t utf16_len;
        void* utf16_name = create_utf16_string(Data, Size, &utf16_len);
        if (utf16_name) {
            sqlite3_create_function16(db, utf16_name, nArg, eTextRep, pApp,
                                     dummy_xFunc, dummy_xStep, dummy_xFinal);
            free(utf16_name);
        }
    }
    
    /* Test sqlite3_create_window_function */
    sqlite3_create_window_function(db, func_name, nArg, eTextRep, pApp,
                                  dummy_xStep, dummy_xFinal, dummy_xValue,
                                  dummy_xInverse, dummy_xDestroy);
    
    /* Test sqlite3_create_function */
    sqlite3_create_function(db, func_name, nArg, eTextRep, pApp,
                           dummy_xFunc, dummy_xStep, dummy_xFinal);
    
    /* Test sqlite3_overload_function */
    sqlite3_overload_function(db, func_name, nArg);
    
    /* Test sqlite3_create_function_v2 */
    sqlite3_create_function_v2(db, func_name, nArg, eTextRep, pApp,
                              dummy_xFunc, dummy_xStep, dummy_xFinal,
                              dummy_xDestroy);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}