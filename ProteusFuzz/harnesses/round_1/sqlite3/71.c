// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to safely create a database connection */
static sqlite3 *create_db(void) {
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return NULL;
    }
    return db;
}

/* Static helper to destroy a database connection */
static void destroy_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Collation Compare Callback
 * Returns -1, 0, or 1 based on simple memcmp.
 * Ensures consistency for the fuzzer.
 */
static int collation_compare(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    int result = memcmp(pLeft, pRight, (nLeft < nRight) ? nLeft : nRight);
    if (result != 0) {
        return result;
    }
    if (nLeft < nRight) return -1;
    if (nLeft > nRight) return 1;
    return 0;
}

/*
 * Scalar Function Callback
 * Simple implementation that returns a constant or length.
 */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_TEXT) {
        sqlite3_result_text(ctx, (const char *)sqlite3_value_text(argv[0]), -1, SQLITE_STATIC);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

/*
 * Aggregate Step Callback
 */
static void agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_int64 *pSum = (sqlite3_int64 *)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum && argc > 0) {
        *pSum += sqlite3_value_int(argv[0]);
    }
}

/*
 * Aggregate Final Callback
 */
static void agg_final(sqlite3_context *ctx) {
    sqlite3_int64 *pSum = (sqlite3_int64 *)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum) {
        sqlite3_result_int64(ctx, *pSum);
    } else {
        sqlite3_result_null(ctx);
    }
}

/*
 * Window Function Step Callback
 */
static void win_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    agg_step(ctx, argc, argv);
}

/*
 * Window Function Final Callback
 */
static void win_final(sqlite3_context *ctx) {
    agg_final(ctx);
}

/*
 * Window Function Value Callback
 */
static void win_value(sqlite3_context *ctx) {
    agg_final(ctx);
}

/*
 * Window Function Inverse Callback
 */
static void win_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_int64 *pSum = (sqlite3_int64 *)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum && argc > 0) {
        *pSum -= sqlite3_value_int(argv[0]);
    }
}

/*
 * Destructor for application data
 */
static void app_data_destroy(void *pApp) {
    if (pApp) {
        free(pApp);
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    sqlite3 *db = create_db();
    if (!db) return 0;

    /*
     * We interpret the input data to drive the fuzzing logic.
     * We use a simple state machine based on the first byte.
     */
    uint8_t mode = Data[0] % 6;

    /*
     * Prepare some dummy data for names and arguments.
     * We ensure null-termination for strings.
     */
    char name_buf[256];
    size_t name_len = (Size > 255) ? 255 : Size;
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';

    /* Ensure name is not empty for valid tests, or handle empty gracefully */
    if (name_len == 0) {
        name_buf[0] = 't';
        name_buf[1] = 'e';
        name_buf[2] = 's';
        name_buf[3] = 't';
        name_buf[4] = '\0';
    }

    /* Allocate some dummy app data */
    void *pApp = malloc(64);
    if (pApp) {
        memset(pApp, 0x42, 64);
    }

    int eTextRep = (Data[1] % 5); 
    /* Map to valid constants: SQLITE_UTF8(1), SQLITE_UTF16LE(2), SQLITE_UTF16BE(3), SQLITE_UTF16(4), SQLITE_UTF16_ALIGNED(5) */
    if (eTextRep == 0) eTextRep = SQLITE_UTF8;
    else if (eTextRep == 1) eTextRep = SQLITE_UTF16LE;
    else if (eTextRep == 2) eTextRep = SQLITE_UTF16BE;
    else if (eTextRep == 3) eTextRep = SQLITE_UTF16;
    else eTextRep = SQLITE_UTF16_ALIGNED;

    int nArg = (int)(Data[2] % 128) - 64; /* Range -64 to 63 */
    if (nArg == 0) nArg = -1; /* Allow variable args */

    /* Track if we registered a destructor for pApp in this iteration */
    int destructor_registered = 0;

    switch (mode) {
        case 0:
            /* Fuzz sqlite3_create_collation */
            /* Try with valid callback */
            sqlite3_create_collation(db, name_buf, eTextRep, pApp, collation_compare);
            /* Try with NULL callback to remove */
            sqlite3_create_collation(db, name_buf, eTextRep, NULL, NULL);
            break;

        case 1:
            /* Fuzz sqlite3_create_collation16 */
            /* Convert name to UTF-16 for testing */
            wchar_t *name_utf16 = (wchar_t *)malloc((name_len + 1) * sizeof(wchar_t));
            if (name_utf16) {
                for (size_t i = 0; i <= name_len; i++) {
                    name_utf16[i] = (wchar_t)name_buf[i];
                }
                sqlite3_create_collation16(db, name_utf16, eTextRep, pApp, collation_compare);
                sqlite3_create_collation16(db, name_utf16, eTextRep, NULL, NULL);
                free(name_utf16);
            }
            break;

        case 2:
            /* Fuzz sqlite3_create_function */
            /* Scalar function */
            sqlite3_create_function(db, name_buf, nArg, eTextRep, pApp, scalar_func, NULL, NULL);
            /* Aggregate function */
            sqlite3_create_function(db, name_buf, nArg, eTextRep, pApp, NULL, agg_step, agg_final);
            /* Remove function */
            sqlite3_create_function(db, name_buf, nArg, eTextRep, NULL, NULL, NULL, NULL);
            break;

        case 3:
            /* Fuzz sqlite3_create_function16 */
            wchar_t *func_name_utf16 = (wchar_t *)malloc((name_len + 1) * sizeof(wchar_t));
            if (func_name_utf16) {
                for (size_t i = 0; i <= name_len; i++) {
                    func_name_utf16[i] = (wchar_t)name_buf[i];
                }
                sqlite3_create_function16(db, func_name_utf16, nArg, eTextRep, pApp, scalar_func, NULL, NULL);
                sqlite3_create_function16(db, func_name_utf16, nArg, eTextRep, pApp, NULL, agg_step, agg_final);
                sqlite3_create_function16(db, func_name_utf16, nArg, eTextRep, NULL, NULL, NULL, NULL);
                free(func_name_utf16);
            }
            break;

        case 4:
            /* Fuzz sqlite3_create_function_v2 */
            /* 
             * Note: sqlite3_create_function_v2 calls xDestroy if registration fails.
             * If registration succeeds, xDestroy is called when the function is deleted or db closed.
             * To avoid double-free, we must ensure we don't free pApp manually if a destructor was registered
             * AND the registration succeeded.
             * However, tracking success for every call is complex. 
             * Safer approach: Only use xDestroy for the LAST registration in a sequence, 
             * or ensure pApp is not freed manually if xDestroy was passed.
             * 
             * Revised Strategy: 
             * If we pass xDestroy, we assume SQLite will manage the memory. 
             * We should NOT free pApp manually in the cleanup block if xDestroy was passed.
             * But if the call fails, SQLite calls xDestroy immediately. 
             * If we pass xDestroy, we must trust SQLite to handle it.
             * 
             * Correction: The error occurred because we called create_function_v2 twice with the same pApp 
             * and xDestroy. The first call registered it. The second call (overriding) triggered the 
             * destructor for the first pApp. Then we tried to free pApp again at the end.
             * 
             * Fix: If we pass xDestroy, we must set a flag to indicate we should NOT free pApp manually.
             * Even if the call fails, xDestroy is called, so pApp is freed.
             * If the call succeeds, xDestroy is called on close/override.
             * So in ALL cases where xDestroy is passed, we should NOT free pApp manually.
             */
            sqlite3_create_function_v2(db, name_buf, nArg, eTextRep, pApp, scalar_func, NULL, NULL, app_data_destroy);
            destructor_registered = 1;
            
            /* 
             * We cannot safely register the same pApp again with a destructor unless we want it freed again.
             * Let's use a different pointer or just skip the second registration with the same pApp for this mode.
             * Or, we can register an aggregate function with a NEW pApp.
             */
            void *pApp2 = malloc(64);
            if (pApp2) {
                memset(pApp2, 0x42, 64);
                sqlite3_create_function_v2(db, name_buf, nArg, eTextRep, pApp2, NULL, agg_step, agg_final, app_data_destroy);
                /* pApp2 is managed by SQLite now */
            }
            
            /* Remove function (triggers destructor for the last registered one) */
            sqlite3_create_function_v2(db, name_buf, nArg, eTextRep, NULL, NULL, NULL, NULL, NULL);
            break;

        case 5:
            /* Fuzz sqlite3_create_window_function */
            /* Requires xStep and xFinal to be non-NULL */
            sqlite3_create_window_function(
                db, name_buf, nArg, eTextRep, pApp,
                win_step, win_final,
                win_value, win_inverse,
                app_data_destroy
            );
            destructor_registered = 1;

            /* Try without value/inverse (regular aggregate behavior in window context if supported) */
            /* Use a new pointer to avoid double-free logic complexity */
            void *pApp3 = malloc(64);
            if (pApp3) {
                memset(pApp3, 0x42, 64);
                sqlite3_create_window_function(
                    db, name_buf, nArg, eTextRep, pApp3,
                    win_step, win_final,
                    NULL, NULL,
                    app_data_destroy
                );
            }
            break;
    }

    /* Cleanup */
    if (pApp) {
        /* If we registered a destructor (modes 4 and 5), sqlite3_close will call it.
         * We must NOT free it here to avoid double-free.
         * For modes 0-3, no destructor was registered, so we free manually.
         */
        if (!destructor_registered) {
            free(pApp);
        }
    }

    destroy_db(db);
    return 0;
}