// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy scalar function for testing */
static void dummy_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Try to get auxdata for argument 0 */
    void *auxdata = sqlite3_get_auxdata(ctx, 0);
    
    /* Set a text result */
    const char *result = auxdata ? "Has auxdata" : "No auxdata";
    sqlite3_result_text(ctx, result, -1, SQLITE_TRANSIENT);
}

/* Dummy step function for aggregate */
static void dummy_step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Do nothing */
}

/* Dummy final function for aggregate */
static void dummy_final_func(sqlite3_context *ctx) {
    sqlite3_result_text(ctx, "Aggregate result", -1, SQLITE_TRANSIENT);
}

/* Destructor for application data */
static void dummy_destructor(void *ptr) {
    /* Do nothing - just for testing */
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use fuzzer input to generate function name */
    char func_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
    } else {
        strcpy(func_name, "fuzz_func");
    }
    
    /* Use fuzzer input to determine parameters */
    int nArg = (Size > 0) ? (Data[0] % 10) : 1; /* Limit to 0-9 args */
    int eTextRep = SQLITE_UTF8;
    if (Size > 1) {
        switch (Data[1] & 0x07) {
            case 0: eTextRep = SQLITE_UTF8; break;
            case 1: eTextRep = SQLITE_UTF16; break;
            case 2: eTextRep = SQLITE_UTF16LE; break;
            case 3: eTextRep = SQLITE_UTF16BE; break;
            case 4: eTextRep = SQLITE_UTF8 | SQLITE_DETERMINISTIC; break;
            case 5: eTextRep = SQLITE_UTF8 | SQLITE_DIRECTONLY; break;
            case 6: eTextRep = SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY; break;
            default: eTextRep = SQLITE_UTF8; break;
        }
    }
    
    /* Create some dummy application data */
    void *app_data = (void *)(Data + (Size > 10 ? 10 : 0));
    
    /* Test sqlite3_create_function */
    rc = sqlite3_create_function(
        db,
        func_name,
        nArg,
        eTextRep,
        app_data,
        dummy_scalar_func,
        NULL,
        NULL
    );
    
    /* Test sqlite3_create_function_v2 */
    char func_name2[256];
    snprintf(func_name2, sizeof(func_name2), "%s_v2", func_name);
    rc = sqlite3_create_function_v2(
        db,
        func_name2,
        nArg,
        eTextRep,
        app_data,
        dummy_scalar_func,
        NULL,
        NULL,
        dummy_destructor
    );
    
    /* Test creating an aggregate function */
    char func_name3[256];
    snprintf(func_name3, sizeof(func_name3), "%s_agg", func_name);
    rc = sqlite3_create_function(
        db,
        func_name3,
        nArg,
        eTextRep,
        app_data,
        NULL,
        dummy_step_func,
        dummy_final_func
    );
    
    /* Test sqlite3_set_clientdata and sqlite3_get_clientdata */
    if (Size > 20) {
        char key_name[256];
        size_t key_len = (Size - 20) < 255 ? (Size - 20) : 255;
        memcpy(key_name, Data + 20, key_len);
        key_name[key_len] = '\0';
        
        void *client_data = (void *)(Data + (Size > 50 ? 50 : 0));
        rc = sqlite3_set_clientdata(db, key_name, client_data, dummy_destructor);
        
        /* Retrieve the client data */
        void *retrieved = sqlite3_get_clientdata(db, key_name);
        
        /* Try with a non-existent key */
        retrieved = sqlite3_get_clientdata(db, "non_existent_key");
    }
    
    /* Execute a simple query to trigger the functions */
    sqlite3_exec(db, "SELECT 1", NULL, NULL, NULL);
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}