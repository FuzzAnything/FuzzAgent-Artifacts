// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Static helper to destroy allocated memory if needed.
 */
static void my_destructor(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

/*
 * Global variable to store the current fuzz input size.
 * This allows the callback to know the valid range of the input data.
 */
static size_t g_fuzz_input_size = 0;

/*
 * Static helper to create a simple user-defined function that exercises
 * the target APIs based on the fuzz input.
 */
static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(ctx);
    int size = 0;
    
    /* 
     * Get size from the first argument if available.
     * We rely on the fuzzer to pass a reasonable integer in the first argument.
     * We clamp the size to a safe maximum to prevent huge allocations.
     */
    if (argc > 0) {
        size = sqlite3_value_int(argv[0]);
    }

    /* Clamp size to reasonable bounds */
    if (size < 0) size = 0;
    if (size > 1000000) size = 1000000;

    /* 
     * CRITICAL FIX: Ensure we never read beyond the actual fuzz input buffer.
     * The 'size' variable comes from the SQL argument, which might be larger
     * than the actual 'g_fuzz_input_size'. We must cap the copy size.
     */
    int safe_copy_size = (size > (int)g_fuzz_input_size) ? (int)g_fuzz_input_size : size;
    
    /* 1. sqlite3_result_blob */
    if (size > 0) {
        /* Allocate memory for the blob */
        void *blob_data = malloc(size);
        if (blob_data) {
            if (safe_copy_size > 0 && data != NULL) {
                memcpy(blob_data, data, safe_copy_size);
                /* Pad the rest with zeros if needed */
                if (safe_copy_size < size) {
                    memset((char*)blob_data + safe_copy_size, 0, size - safe_copy_size);
                }
            } else {
                memset(blob_data, 0, size);
            }
            sqlite3_result_blob(ctx, blob_data, size, my_destructor);
            return;
        }
    }

    /* 2. sqlite3_set_auxdata */
    /* Store some data in auxdata for argument 0 */
    if (size > 0) {
        void *aux_data = malloc(size);
        if (aux_data) {
            if (safe_copy_size > 0 && data != NULL) {
                memcpy(aux_data, data, safe_copy_size);
                if (safe_copy_size < size) {
                    memset((char*)aux_data + safe_copy_size, 0, size - safe_copy_size);
                }
            } else {
                memset(aux_data, 0, size);
            }
            sqlite3_set_auxdata(ctx, 0, aux_data, my_destructor);
        }
    }

    /* 3. sqlite3_result_text16be */
    /* Ensure even length for UTF-16 */
    int len16be = (size > 0) ? (size & ~1) : 0;
    if (len16be > 0) {
        int safe_len16be = (len16be > (int)g_fuzz_input_size) ? (int)g_fuzz_input_size : len16be;
        /* Ensure the safe length is also even */
        safe_len16be &= ~1;
        
        if (safe_len16be > 0 && data != NULL) {
            sqlite3_result_text16be(ctx, data, safe_len16be, SQLITE_TRANSIENT);
        } else {
            /* Create a zero-filled buffer */
            void *buf = malloc(safe_len16be);
            if (buf) {
                memset(buf, 0, safe_len16be);
                sqlite3_result_text16be(ctx, buf, safe_len16be, my_destructor);
            }
        }
        return;
    }

    /* 4. sqlite3_result_text16le */
    int len16le = (size > 0) ? (size & ~1) : 0;
    if (len16le > 0) {
        int safe_len16le = (len16le > (int)g_fuzz_input_size) ? (int)g_fuzz_input_size : len16le;
        safe_len16le &= ~1;
        
        if (safe_len16le > 0 && data != NULL) {
            sqlite3_result_text16le(ctx, data, safe_len16le, SQLITE_TRANSIENT);
        } else {
            void *buf = malloc(safe_len16le);
            if (buf) {
                memset(buf, 0, safe_len16le);
                sqlite3_result_text16le(ctx, buf, safe_len16le, my_destructor);
            }
        }
        return;
    }

    /* 5. sqlite3_result_text16 */
    int len16 = (size > 0) ? (size & ~1) : 0;
    if (len16 > 0) {
        int safe_len16 = (len16 > (int)g_fuzz_input_size) ? (int)g_fuzz_input_size : len16;
        safe_len16 &= ~1;
        
        if (safe_len16 > 0 && data != NULL) {
            sqlite3_result_text16(ctx, data, safe_len16, SQLITE_TRANSIENT);
        } else {
            void *buf = malloc(safe_len16);
            if (buf) {
                memset(buf, 0, safe_len16);
                sqlite3_result_text16(ctx, buf, safe_len16, my_destructor);
            }
        }
        return;
    }

    /* 6. sqlite3_result_text */
    if (size > 0) {
        int safe_text_size = (size > (int)g_fuzz_input_size) ? (int)g_fuzz_input_size : size;
        if (safe_text_size > 0 && data != NULL) {
            sqlite3_result_text(ctx, (const char *)data, safe_text_size, SQLITE_TRANSIENT);
        } else {
            void *buf = malloc(safe_text_size + 1);
            if (buf) {
                memset(buf, 0, safe_text_size + 1);
                sqlite3_result_text(ctx, (const char *)buf, safe_text_size, my_destructor);
            }
        }
        return;
    }

    /* Fallback: return NULL if no valid data */
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    sqlite3_stmt *stmt = NULL;

    /* Set the global size for the callback to use */
    g_fuzz_input_size = Size;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 
     * Create a user-defined function that uses the fuzz input.
     * We pass the fuzz data as user_data to the function.
     */
    rc = sqlite3_create_function(
        db, 
        "fuzz_func", 
        -1, /* nArg: -1 means variable arguments */
        SQLITE_UTF8, 
        (void *)Data, /* pApp: pass the fuzz data */
        fuzz_function, 
        NULL, 
        NULL
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Prepare a statement that calls our function with varying arguments.
     */
    const char *sql = "SELECT fuzz_func(?, ?, ?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind arguments from the fuzz input if available */
    if (Size >= 4) {
        /* Use the first byte as the size argument */
        sqlite3_bind_int(stmt, 1, (int)Data[0]);
        sqlite3_bind_int(stmt, 2, (int)Data[1]);
        sqlite3_bind_int(stmt, 3, (int)Data[2]);
    } else if (Size >= 1) {
        sqlite3_bind_int(stmt, 1, (int)Data[0]);
        /* Leave others unbound (NULL) */
    } else {
        /* No data, bind 0 */
        sqlite3_bind_int(stmt, 1, 0);
    }

    /* Execute the statement to trigger the function */
    rc = sqlite3_step(stmt);
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}