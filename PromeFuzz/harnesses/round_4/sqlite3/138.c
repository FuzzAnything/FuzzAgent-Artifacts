// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
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

/* Dummy callback functions for function registration */
static void dummy_xFunc(sqlite3_context* ctx, int argc, sqlite3_value** argv) {}
static void dummy_xStep(sqlite3_context* ctx, int argc, sqlite3_value** argv) {}
static void dummy_xFinal(sqlite3_context* ctx) {}
static void dummy_xValue(sqlite3_context* ctx) {}
static void dummy_xInverse(sqlite3_context* ctx, int argc, sqlite3_value** argv) {}
static void dummy_xDestroy(void* p) {}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use the fuzzer input to generate function names and parameters */
    if (Size > 0) {
        /* Extract function name from input (max 255 bytes as per SQLite limit) */
        size_t name_len = Size > 255 ? 255 : Size;
        char* func_name = (char*)malloc(name_len + 1);
        if (!func_name) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
        
        /* Extract parameters from input */
        int nArg = (Size > 0) ? (Data[0] % 128) : 0; /* nArg between 0-127 */
        int eTextRep = (Size > 1) ? (Data[1] % 8) : SQLITE_UTF8;
        void* pApp = NULL;
        
        /* Try different API functions with the generated parameters */
        
        /* 1. sqlite3_create_window_function */
        sqlite3_create_window_function(
            db, func_name, nArg, eTextRep, pApp,
            dummy_xStep, dummy_xFinal, dummy_xValue, dummy_xInverse, dummy_xDestroy);
        
        /* 2. sqlite3_create_function_v2 */
        sqlite3_create_function_v2(
            db, func_name, nArg, eTextRep, pApp,
            dummy_xFunc, dummy_xStep, dummy_xFinal, dummy_xDestroy);
        
        /* 3. sqlite3_overload_function */
        sqlite3_overload_function(db, func_name, nArg);
        
        /* 4. sqlite3_set_clientdata */
        char* key_name = (char*)malloc(name_len + 1);
        if (key_name) {
            memcpy(key_name, Data, name_len);
            key_name[name_len] = '\0';
            sqlite3_set_clientdata(db, key_name, NULL, NULL);
            free(key_name);
        }
        
        /* 5. sqlite3_create_function */
        sqlite3_create_function(
            db, func_name, nArg, eTextRep, pApp,
            dummy_xFunc, dummy_xStep, dummy_xFinal);
        
        /* 6. sqlite3_create_function16 (UTF-16 version) */
        /* Convert function name to UTF-16 */
        void* func_name16 = malloc((name_len + 1) * 2);
        if (func_name16) {
            /* Simple conversion: each byte becomes a 16-bit char */
            uint16_t* dest = (uint16_t*)func_name16;
            for (size_t i = 0; i < name_len; i++) {
                dest[i] = (uint16_t)func_name[i];
            }
            dest[name_len] = 0;
            
            sqlite3_create_function16(
                db, func_name16, nArg, eTextRep, pApp,
                dummy_xFunc, dummy_xStep, dummy_xFinal);
            
            free(func_name16);
        }
        
        free(func_name);
    }
    
    /* Try to create a file-based database if input suggests it */
    if (Size > 10) {
        FILE* fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1000 ? 1000 : Size, fp);
            fclose(fp);
            
            /* Open the file database */
            sqlite3* file_db = NULL;
            rc = sqlite3_open("./dummy_file", &file_db);
            if (rc == SQLITE_OK && file_db) {
                /* Try some operations on file database */
                char* func_name2 = "fuzz_func";
                sqlite3_create_function(file_db, func_name2, 1, SQLITE_UTF8, 
                                       NULL, dummy_xFunc, NULL, NULL);
                sqlite3_close(file_db);
            }
            remove("./dummy_file");
        }
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}