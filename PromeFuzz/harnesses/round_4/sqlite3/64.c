// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:78864:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:78882:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:78873:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Custom destructor for testing */
static void custom_destructor(void *ptr) {
    free(ptr);
}

/* Global variables to pass fuzz data to the callback */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

/* Fuzz function that will be called by SQLite */
static void fuzz_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Use the global fuzz data */
    const uint8_t *Data = g_fuzz_data;
    size_t Size = g_fuzz_size;
    
    if (Size > 0) {
        /* Determine which function to test based on first byte */
        uint8_t selector = Data[0];
        
        /* Adjust data pointer and size for the actual data */
        const uint8_t *actual_data = (Size > 1) ? Data + 1 : Data;
        size_t actual_size = (Size > 1) ? Size - 1 : 1;
        
        /* Ensure we have at least some data */
        if (actual_size == 0) {
            actual_data = Data;
            actual_size = 1;
        }
        
        /* Limit size to avoid excessive memory usage */
        if (actual_size > 1024) {
            actual_size = 1024;
        }
        
        /* Test different functions based on selector */
        switch (selector % 7) {
            case 0:
                sqlite3_result_blob(context, actual_data, (int)actual_size, SQLITE_STATIC);
                break;
            case 1:
                sqlite3_result_blob(context, actual_data, (int)actual_size, SQLITE_TRANSIENT);
                break;
            case 2:
                sqlite3_result_blob(context, actual_data, (int)actual_size, custom_destructor);
                break;
            case 3:
                sqlite3_result_text(context, (const char*)actual_data, (int)actual_size, SQLITE_TRANSIENT);
                break;
            case 4:
                sqlite3_result_text16(context, actual_data, (int)actual_size, SQLITE_TRANSIENT);
                break;
            case 5:
                sqlite3_result_text16le(context, actual_data, (int)actual_size, SQLITE_TRANSIENT);
                break;
            case 6:
                sqlite3_result_text16be(context, actual_data, (int)actual_size, SQLITE_TRANSIENT);
                break;
        }
        
        /* Also test sqlite3_set_auxdata with different destructor types */
        if (Size > 2) {
            int aux_index = Data[1] % 10;  /* Use second byte for index */
            void *aux_data = malloc(actual_size);
            if (aux_data) {
                memcpy(aux_data, actual_data, actual_size);
                
                switch (selector % 3) {
                    case 0:
                        /* For SQLITE_STATIC, we need to free the memory ourselves */
                        sqlite3_set_auxdata(context, aux_index, aux_data, SQLITE_STATIC);
                        free(aux_data);
                        break;
                    case 1:
                        /* For SQLITE_TRANSIENT, SQLite makes a copy and frees it */
                        sqlite3_set_auxdata(context, aux_index, aux_data, SQLITE_TRANSIENT);
                        free(aux_data);
                        break;
                    case 2:
                        /* For custom destructor, it will free the memory */
                        sqlite3_set_auxdata(context, aux_index, aux_data, custom_destructor);
                        break;
                }
            }
        }
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = 0;
    
    /* Store fuzz data in global variables for the callback */
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Create and call the fuzz function */
    rc = sqlite3_create_function(
        db, 
        "fuzz_func", 
        0,  /* number of arguments */
        SQLITE_UTF8, 
        NULL,  /* pAppData */
        fuzz_func, 
        NULL,  /* xStep */
        NULL   /* xFinal */
    );
    
    if (rc == SQLITE_OK) {
        /* Call the function to trigger the fuzzing */
        sqlite3_exec(db, "SELECT fuzz_func()", NULL, NULL, &zErrMsg);
    }
    
    /* Clean up */
    if (zErrMsg) {
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    
    return 0;
}