// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#define MAX_FUZZ_SIZE 1024
#define DUMMY_FILE "./dummy_file"

static sqlite3 *g_db = NULL;
static sqlite3_context *g_ctx = NULL;
static sqlite3_value *g_test_values[10] = {0};

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, sqlite3_aggregate_count(ctx));
}

static void dummy_final(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, sqlite3_aggregate_count(ctx));
}

static void init_database(void) {
    if (!g_db) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            g_db = NULL;
        }
    }
}

static void cleanup_database(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}

static void create_test_context(void) {
    if (!g_ctx) {
        g_ctx = (sqlite3_context *)malloc(1024);
        if (g_ctx) {
            memset(g_ctx, 0, 1024);
        }
    }
}

static void create_test_values(const uint8_t *data, size_t size) {
    for (int i = 0; i < 10 && i * 256 < size; i++) {
        g_test_values[i] = (sqlite3_value *)malloc(256);
        if (g_test_values[i]) {
            memset(g_test_values[i], 0, 256);
        }
    }
}

static void cleanup_test_values(void) {
    for (int i = 0; i < 10; i++) {
        if (g_test_values[i]) {
            free(g_test_values[i]);
            g_test_values[i] = NULL;
        }
    }
}

static void cleanup_test_context(void) {
    if (g_ctx) {
        free(g_ctx);
        g_ctx = NULL;
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    init_database();
    create_test_context();
    create_test_values(data, size);
    
    if (!g_db) {
        cleanup_test_values();
        cleanup_test_context();
        cleanup_database();
        return 0;
    }
    
    FILE *f = fopen(DUMMY_FILE, "wb");
    if (f) {
        fwrite(data, 1, size > MAX_FUZZ_SIZE ? MAX_FUZZ_SIZE : size, f);
        fclose(f);
    }
    
    int func_type = data[0] % 6;
    size_t offset = 1;
    
    switch (func_type) {
        case 0:
            if (g_ctx) {
                sqlite3_result_null(g_ctx);
            }
            break;
            
        case 1: {
            const char *func_name = "fuzz_func";
            int n_arg = (offset < size) ? (data[offset] % 10) - 1 : -1;
            offset++;
            int e_text_rep = (offset < size) ? (data[offset] % 5) + 1 : SQLITE_UTF8;
            offset++;
            
            void *p_app = (offset < size) ? (void *)(uintptr_t)data[offset] : NULL;
            offset++;
            
            int x_func_type = (offset < size) ? data[offset] % 3 : 0;
            offset++;
            
            void (*x_func)(sqlite3_context*, int, sqlite3_value**) = NULL;
            void (*x_step)(sqlite3_context*, int, sqlite3_value**) = NULL;
            void (*x_final)(sqlite3_context*) = NULL;
            
            if (x_func_type == 0) {
                x_func = dummy_func;
            } else if (x_func_type == 1) {
                x_step = dummy_step;
                x_final = dummy_final;
            }
            
            sqlite3_create_function(g_db, func_name, n_arg, e_text_rep, p_app, x_func, x_step, x_final);
            break;
        }
            
        case 2: {
            if (g_ctx) {
                int val = (offset < size) ? (int)data[offset] : 0;
                sqlite3_result_int(g_ctx, val);
            }
            break;
        }
            
        case 3: {
            if (g_ctx) {
                int count = sqlite3_aggregate_count(g_ctx);
                sqlite3_result_int(g_ctx, count);
            }
            break;
        }
            
        case 4: {
            if (g_ctx) {
                const char *text = "fuzz_text";
                sqlite3_uint64 len = (offset < size) ? data[offset] % 100 : 10;
                offset++;
                unsigned char encoding = (offset < size) ? data[offset] % 4 + 1 : SQLITE_UTF8;
                sqlite3_result_text64(g_ctx, text, len, SQLITE_TRANSIENT, encoding);
            }
            break;
        }
            
        case 5: {
            if (g_ctx) {
                int n = (offset < size) ? (int)data[offset] % 1000 : 100;
                sqlite3_result_zeroblob(g_ctx, n);
            }
            break;
        }
    }
    
    if (g_db) {
        sqlite3_exec(g_db, "DROP TABLE IF EXISTS test", NULL, NULL, NULL);
        sqlite3_exec(g_db, "CREATE TABLE test(id INTEGER)", NULL, NULL, NULL);
        
        char *err_msg = NULL;
        sqlite3_exec(g_db, "SELECT * FROM test", NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
    
    cleanup_test_values();
    cleanup_test_context();
    cleanup_database();
    
    remove(DUMMY_FILE);
    
    return 0;
}