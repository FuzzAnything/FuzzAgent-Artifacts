// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Global variables to maintain state across function calls */
static sqlite3 *g_db = NULL;
static sqlite3_context *g_context = NULL;
static int g_initialized = 0;

/* Helper functions */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Dummy function for sqlite3_create_function_v2 */
    sqlite3_result_int(ctx, 42);
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Dummy step function for aggregate */
}

static void dummy_xFinal(sqlite3_context *ctx) {
    /* Dummy final function for aggregate */
    sqlite3_result_int(ctx, 100);
}

static void dummy_xDestroy(void *p) {
    /* Dummy destroy function */
    if (p) free(p);
}

/* Initialize database and context once */
static int initialize_fuzzing(void) {
    if (g_initialized) return 1;
    
    /* Open in-memory database */
    if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
        return 0;
    }
    
    /* Create a dummy table to enable function context creation */
    sqlite3_exec(g_db, "CREATE TABLE IF NOT EXISTS dummy(x);", NULL, NULL, NULL);
    
    /* Prepare a statement to get a context */
    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM dummy";
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        /* Get context from the statement */
        g_context = (sqlite3_context *)stmt; /* Note: This is a hacky cast */
        sqlite3_finalize(stmt);
    }
    
    g_initialized = 1;
    return 1;
}

/* Clean up resources */
static void cleanup_fuzzing(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
    g_context = NULL;
    g_initialized = 0;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    /* Initialize if not already done */
    if (!initialize_fuzzing()) {
        return 0;
    }
    
    /* Use data to determine which functions to call and with what parameters */
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0] % 10;
    size_t data_offset = 1;
    
    switch (selector) {
        case 0: {
            /* Test sqlite3_aggregate_count */
            if (g_context) {
                int count = sqlite3_aggregate_count(g_context);
                (void)count; /* Use result to avoid unused variable warning */
            }
            break;
        }
        
        case 1: {
            /* Test sqlite3_result_zeroblob */
            if (g_context && Size - data_offset >= sizeof(int)) {
                int n;
                memcpy(&n, Data + data_offset, sizeof(int));
                sqlite3_result_zeroblob(g_context, n);
                data_offset += sizeof(int);
            }
            break;
        }
        
        case 2: {
            /* Test sqlite3_create_function_v2 */
            if (g_db && Size - data_offset >= 1) {
                /* Extract function name from data */
                size_t name_len = (Data[data_offset] % 32) + 1;
                if (name_len > Size - data_offset - 1) {
                    name_len = Size - data_offset - 1;
                }
                
                char *func_name = malloc(name_len + 1);
                if (func_name) {
                    memcpy(func_name, Data + data_offset + 1, name_len);
                    func_name[name_len] = '\0';
                    
                    int nArg = (Data[data_offset] % 5);
                    int eTextRep = SQLITE_UTF8;
                    
                    /* Create dummy app data */
                    void *pApp = malloc(16);
                    
                    sqlite3_create_function_v2(
                        g_db,
                        func_name,
                        nArg,
                        eTextRep,
                        pApp,
                        dummy_xFunc,
                        dummy_xStep,
                        dummy_xFinal,
                        dummy_xDestroy
                    );
                    
                    free(func_name);
                }
                data_offset += 1 + name_len;
            }
            break;
        }
        
        case 3: {
            /* Test sqlite3_result_int */
            if (g_context && Size - data_offset >= sizeof(int)) {
                int value;
                memcpy(&value, Data + data_offset, sizeof(int));
                sqlite3_result_int(g_context, value);
                data_offset += sizeof(int);
            }
            break;
        }
        
        case 4: {
            /* Test sqlite3_result_null */
            if (g_context) {
                sqlite3_result_null(g_context);
            }
            break;
        }
        
        case 5: {
            /* Test sqlite3_result_text64 */
            if (g_context && Size - data_offset >= 1) {
                size_t text_len = (Data[data_offset] % 64);
                if (text_len > Size - data_offset - 1) {
                    text_len = Size - data_offset - 1;
                }
                
                if (text_len > 0) {
                    char *text = malloc(text_len + 1);
                    if (text) {
                        memcpy(text, Data + data_offset + 1, text_len);
                        text[text_len] = '\0';
                        
                        unsigned char encoding = SQLITE_UTF8;
                        if (Size - data_offset - 1 - text_len >= 1) {
                            encoding = Data[data_offset + 1 + text_len] % 3;
                        }
                        
                        sqlite3_result_text64(
                            g_context,
                            text,
                            (sqlite3_uint64)text_len,
                            SQLITE_TRANSIENT,
                            encoding
                        );
                        
                        free(text);
                    }
                }
            }
            break;
        }
        
        default: {
            /* Mix of multiple functions */
            if (g_context && Size - data_offset >= sizeof(int)) {
                int n;
                memcpy(&n, Data + data_offset, sizeof(int));
                
                /* Call multiple result functions */
                sqlite3_result_int(g_context, n);
                sqlite3_result_zeroblob(g_context, n % 100);
                sqlite3_result_null(g_context);
                
                data_offset += sizeof(int);
            }
            break;
        }
    }
    
    /* Periodically clean up and reinitialize to explore different states */
    static int call_count = 0;
    call_count++;
    
    if (call_count % 100 == 0) {
        cleanup_fuzzing();
        initialize_fuzzing();
    }
    
    return 0;
}