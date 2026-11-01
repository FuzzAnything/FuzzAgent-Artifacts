// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Global state for fuzzing */
static sqlite3 *g_db = NULL;
static uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;
static size_t g_fuzz_idx = 0;

/* Destructor for blob/text memory if needed */
/* Only free if we allocated it ourselves. 
 * Since we pass pointers into g_fuzz_data which is malloc'd, 
 * we must NOT free them here, as SQLite might call this destructor 
 * on data it thinks it owns or we need to handle carefully.
 * The safest approach for fuzzing with shared buffers is to use SQLITE_TRANSIENT
 * or SQLITE_STATIC and let SQLite handle the copy, or ensure we don't double free.
 * Here, we will use a flag or check to avoid freeing data that is part of the fuzz buffer.
 * However, the simplest fix for the ASan error is to NOT pass a destructor that frees
 * the fuzz buffer data, because SQLite might call it after we've already cleaned up 
 * or if the memory wasn't allocated via malloc in the way the destructor expects.
 * Instead, we will pass SQLITE_TRANSIENT to force a copy, or NULL.
 */

/* Callback function to test the target APIs */
static void fuzz_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (g_fuzz_idx >= g_fuzz_size) return;
    
    uint8_t op = g_fuzz_data[g_fuzz_idx++];
    
    switch (op % 6) {
        case 0: {
            /* sqlite3_result_error_code */
            int code = 0;
            if (g_fuzz_idx < g_fuzz_size) code = g_fuzz_data[g_fuzz_idx++];
            if (code == 0) code = 1; /* Non-zero required for error code */
            sqlite3_result_error_code(ctx, code);
            break;
        }
        case 1: {
            /* sqlite3_result_error16 */
            int len = 0;
            if (g_fuzz_idx < g_fuzz_size) len = g_fuzz_data[g_fuzz_idx++];
            /* Ensure len is reasonable */
            if (len > (int)(g_fuzz_size - g_fuzz_idx)) len = (int)(g_fuzz_size - g_fuzz_idx);
            if (len < 0) len = 0;
            if (len > 0 && g_fuzz_idx + len <= g_fuzz_size) {
                /* Pass NULL destructor to avoid freeing fuzz buffer data */
                sqlite3_result_error16(ctx, (const void*)&g_fuzz_data[g_fuzz_idx], len);
                g_fuzz_idx += len;
            } else {
                /* Fallback if not enough data */
                sqlite3_result_error16(ctx, "Error", 4);
            }
            break;
        }
        case 2: {
            /* sqlite3_result_zeroblob */
            int n = 0;
            if (g_fuzz_idx < g_fuzz_size) n = g_fuzz_data[g_fuzz_idx++];
            /* Clamp negative to 0 */
            if (n < 0) n = 0;
            sqlite3_result_zeroblob(ctx, n);
            break;
        }
        case 3: {
            /* sqlite3_aggregate_context */
            int nBytes = 0;
            if (g_fuzz_idx < g_fuzz_size) nBytes = g_fuzz_data[g_fuzz_idx++];
            if (nBytes < 0) nBytes = 0;
            void *p = sqlite3_aggregate_context(ctx, nBytes);
            (void)p;
            break;
        }
        case 4: {
            /* sqlite3_result_blob */
            int len = 0;
            if (g_fuzz_idx < g_fuzz_size) len = g_fuzz_data[g_fuzz_idx++];
            if (len < 0) len = 0;
            if (len > (int)(g_fuzz_size - g_fuzz_idx)) len = (int)(g_fuzz_size - g_fuzz_idx);
            if (len > 0 && g_fuzz_idx + len <= g_fuzz_size) {
                /* Use SQLITE_TRANSIENT to force SQLite to copy the data.
                 * This prevents SQLite from calling a destructor on our fuzz buffer. */
                sqlite3_result_blob(ctx, (const void*)&g_fuzz_data[g_fuzz_idx], len, SQLITE_TRANSIENT);
                g_fuzz_idx += len;
            } else {
                sqlite3_result_blob(ctx, "Blob", 4, SQLITE_STATIC);
            }
            break;
        }
        case 5: {
            /* sqlite3_result_text */
            int len = 0;
            if (g_fuzz_idx < g_fuzz_size) len = g_fuzz_data[g_fuzz_idx++];
            if (len < 0) len = 0;
            if (len > (int)(g_fuzz_size - g_fuzz_idx)) len = (int)(g_fuzz_size - g_fuzz_idx);
            if (len > 0 && g_fuzz_idx + len <= g_fuzz_size) {
                /* Use SQLITE_TRANSIENT to force SQLite to copy the data. */
                sqlite3_result_text(ctx, (const char*)&g_fuzz_data[g_fuzz_idx], len, SQLITE_TRANSIENT);
                g_fuzz_idx += len;
            } else {
                sqlite3_result_text(ctx, "Text", 4, SQLITE_STATIC);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize database if not already done */
    if (g_db == NULL) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            return 0;
        }
        
        /* Register the fuzz_callback function */
        sqlite3_create_function(g_db, "fuzz_test", 0, SQLITE_UTF8, NULL, fuzz_callback, NULL, NULL);
    }

    /* Update global fuzz data */
    if (g_fuzz_data) {
        free(g_fuzz_data);
    }
    g_fuzz_data = malloc(Size);
    if (!g_fuzz_data) return 0;
    memcpy(g_fuzz_data, Data, Size);
    g_fuzz_size = Size;
    g_fuzz_idx = 0;

    /* Execute the function to trigger the callbacks */
    sqlite3_stmt *stmt = NULL;
    const char *exec_sql = "SELECT fuzz_test()";
    
    if (sqlite3_prepare_v2(g_db, exec_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Cleanup fuzz data */
    if (g_fuzz_data) {
        free(g_fuzz_data);
        g_fuzz_data = NULL;
    }
    g_fuzz_size = 0;
    g_fuzz_idx = 0;

    return 0;
}