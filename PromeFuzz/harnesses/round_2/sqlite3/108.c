// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

/* Global database connection for fuzzing */
static sqlite3 *g_db = NULL;
static sqlite3_context *g_dummy_context = NULL;

/* Dummy callback functions for sqlite3_create_function */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Use the fuzzer-provided data to decide what to do */
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(ctx);
    if (data && *data % 3 == 0) {
        sqlite3_result_error(ctx, "fuzzer-generated error", -1);
    } else {
        sqlite3_result_int(ctx, 42);
    }
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Do nothing */
}

static void dummy_final(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 99);
}

static void dummy_destroy(void *p) {
    /* Do nothing */
}

/* Initialize database connection */
static int init_db(void) {
    if (g_db == NULL) {
        int rc = sqlite3_open(":memory:", &g_db);
        if (rc != SQLITE_OK) {
            return 0;
        }
    }
    return 1;
}

/* Clean up */
static void cleanup_db(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}

/* Create a dummy context for testing */
static int init_dummy_context(void) {
    if (g_dummy_context == NULL) {
        /* We need to create a minimal context. Since sqlite3_context is opaque,
         * we can't allocate it directly. Instead, we'll create a function and
         * call it to get a context. */
        if (!init_db()) return 0;
        
        /* Create a temporary function to get a context */
        sqlite3_create_function(g_db, "dummy", 0, SQLITE_UTF8, NULL, 
                                dummy_func, NULL, NULL);
        
        /* Execute a query that calls the function */
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(g_db, "SELECT dummy()", -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Context is created internally during execution */
            }
            sqlite3_finalize(stmt);
        }
        
        /* Note: We can't actually get the context pointer legally.
         * For fuzzing purposes, we'll use a NULL context and rely on
         * the fuzzer to explore other code paths. */
    }
    return 1;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    /* Initialize database if needed */
    if (!init_db()) {
        return 0;
    }
    
    /* Use the first few bytes to control behavior */
    size_t offset = 0;
    if (Size < 1) return 0;
    
    /* Extract parameters from fuzz data */
    int action = Data[offset] % 6;
    offset = (offset + 1) % Size;
    
    /* Function name from data (limited to 255 bytes as per SQLite limit) */
    char func_name[256];
    size_t name_len = Size - offset;
    if (name_len > 255) name_len = 255;
    if (name_len > 0) {
        memcpy(func_name, Data + offset, name_len);
        func_name[name_len] = '\0';
        offset = (offset + name_len) % Size;
    } else {
        strcpy(func_name, "fuzz_func");
    }
    
    /* Additional parameters */
    int nArg = (Size > offset) ? (Data[offset] % 128) : 1;
    offset = (offset + 1) % Size;
    
    int eTextRep = (Size > offset) ? (Data[offset] % 8) : SQLITE_UTF8;
    offset = (offset + 1) % Size;
    
    /* Add flags */
    if (Size > offset && (Data[offset] & 1)) eTextRep |= SQLITE_DETERMINISTIC;
    if (Size > offset && (Data[offset] & 2)) eTextRep |= SQLITE_DIRECTONLY;
    offset = (offset + 1) % Size;
    
    /* Application data pointer */
    void *pApp = (void *)(Data + (offset % Size));
    
    /* Perform the selected action */
    switch (action) {
        case 0: /* sqlite3_create_function */
            sqlite3_create_function(g_db, func_name, nArg, eTextRep, 
                                   pApp, dummy_func, NULL, NULL);
            break;
            
        case 1: /* sqlite3_set_clientdata */
            if (Size > offset + 10) {
                char key[256];
                size_t key_len = (Size - offset > 255) ? 255 : Size - offset;
                memcpy(key, Data + offset, key_len);
                key[key_len] = '\0';
                sqlite3_set_clientdata(g_db, key, pApp, dummy_destroy);
            }
            break;
            
        case 2: /* sqlite3_result_error */
            /* Need a context - try to create one */
            init_dummy_context();
            if (Size > offset) {
                char err_msg[256];
                size_t err_len = (Size - offset > 255) ? 255 : Size - offset;
                memcpy(err_msg, Data + offset, err_len);
                err_msg[err_len] = '\0';
                /* Note: g_dummy_context is NULL, but we call anyway to explore code */
                sqlite3_result_error(g_dummy_context, err_msg, -1);
            }
            break;
            
        case 3: /* sqlite3_get_clientdata */
            if (Size > offset) {
                char key[256];
                size_t key_len = (Size - offset > 255) ? 255 : Size - offset;
                memcpy(key, Data + offset, key_len);
                key[key_len] = '\0';
                void *result = sqlite3_get_clientdata(g_db, key);
                (void)result; /* Ignore result */
            }
            break;
            
        case 4: /* sqlite3_create_function_v2 */
            sqlite3_create_function_v2(g_db, func_name, nArg, eTextRep,
                                      pApp, dummy_func, dummy_step, dummy_final,
                                      dummy_destroy);
            break;
            
        case 5: /* sqlite3_get_auxdata */
            /* Need a context - try to create one */
            init_dummy_context();
            if (g_dummy_context) {
                int idx = (Size > offset) ? (Data[offset] % 10) : 0;
                void *aux = sqlite3_get_auxdata(g_dummy_context, idx);
                (void)aux; /* Ignore result */
            }
            break;
    }
    
    /* Clean up some resources periodically */
    static int call_count = 0;
    call_count++;
    if (call_count % 100 == 0) {
        cleanup_db();
    }
    
    return 0;
}