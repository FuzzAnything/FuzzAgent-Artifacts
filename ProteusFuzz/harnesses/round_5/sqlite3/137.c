// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Helper function to safely open a database connection.
 * Uses "./dummy_file" as the database file as per instructions.
 */
static sqlite3 *open_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Dummy function implementations for callbacks.
 * These are required to register functions/collations but do not perform logic.
 */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
    sqlite3_result_null(ctx);
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
}

static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_null(ctx);
}

static void dummy_value(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_null(ctx);
}

static void dummy_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
}

static void dummy_destroy(void *pApp) {
    (void)pApp;
    /* No cleanup needed for dummy data */
}

static int dummy_compare(void *pArg, int len1, const void *s1, int len2, const void *s2) {
    (void)pArg;
    /* Simple byte comparison */
    int cmp = memcmp(s1, s2, (len1 < len2) ? len1 : len2);
    if (cmp != 0) return cmp;
    return len1 - len2;
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = open_db();
    if (!db) {
        return 0;
    }

    /* 
     * Use input data to drive fuzzing parameters.
     * We ensure bounds checking to prevent crashes from invalid indices.
     */
    const char *utf8_name = (const char *)Data;
    size_t name_len = Size;
    
    /* Ensure null termination for string functions */
    char *safe_name = malloc(name_len + 1);
    if (!safe_name) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(safe_name, Data, name_len);
    safe_name[name_len] = '\0';

    /* Extract parameters from data if available */
    int nArg = (Size > 0) ? (Data[0] % 128) - 1 : 0;
    int eTextRep = (Size > 1) ? (Data[1] % 5) : SQLITE_UTF8;
    
    /* Create a dummy app data pointer based on input */
    void *pApp = malloc(1);
    if (!pApp) {
        free(safe_name);
        sqlite3_close(db);
        return 0;
    }
    if (Size > 2) {
        *(uint8_t *)pApp = Data[2];
    }

    /*
     * 1. Fuzz sqlite3_create_function_v2
     * Registers a custom function with a destructor.
     */
    sqlite3_create_function_v2(
        db,
        safe_name,
        nArg,
        eTextRep,
        pApp,
        dummy_func,
        dummy_step,
        dummy_final,
        dummy_destroy
    );

    /*
     * 2. Fuzz sqlite3_create_function16
     * Requires UTF-16 name. We convert the input string or use a static fallback.
     * Since we don't have a UTF-8 to UTF-16 converter easily available without extra deps,
     * we simulate the fuzzing by passing the raw pointer (which might be invalid UTF-16)
     * or a constructed UTF-16 string. Here we construct a minimal valid UTF-16 string.
     */
    {
        wchar_t wname[256];
        size_t wlen = 0;
        /* Simple conversion for fuzzing purposes (assumes ASCII subset for safety) */
        if (name_len < 256) {
            for (size_t i = 0; i < name_len; i++) {
                wname[i] = (wchar_t)Data[i];
                wlen++;
            }
            wname[wlen] = 0;
        } else {
            wname[0] = 0;
        }
        sqlite3_create_function16(
            db,
            (const void *)wname,
            nArg,
            eTextRep,
            pApp,
            dummy_func,
            dummy_step,
            dummy_final
        );
    }

    /*
     * 3. Fuzz sqlite3_create_collation16
     * Creates a UTF-16 named collation.
     */
    {
        wchar_t wcoll[256];
        size_t wlen = 0;
        if (name_len < 256) {
            for (size_t i = 0; i < name_len; i++) {
                wcoll[i] = (wchar_t)Data[i];
                wlen++;
            }
            wcoll[wlen] = 0;
        } else {
            wcoll[0] = 0;
        }
        sqlite3_create_collation16(
            db,
            (const void *)wcoll,
            eTextRep,
            pApp,
            dummy_compare
        );
    }

    /*
     * 4. Fuzz sqlite3_create_collation
     * Creates a UTF-8 named collation.
     */
    sqlite3_create_collation(
        db,
        safe_name,
        eTextRep,
        pApp,
        dummy_compare
    );

    /*
     * 5. Fuzz sqlite3_create_window_function
     * Registers a window function.
     */
    sqlite3_create_window_function(
        db,
        safe_name,
        nArg,
        eTextRep,
        pApp,
        dummy_step,
        dummy_final,
        dummy_value,
        dummy_inverse,
        dummy_destroy
    );

    /*
     * 6. Fuzz sqlite3_create_function
     * Standard function registration.
     */
    sqlite3_create_function(
        db,
        safe_name,
        nArg,
        eTextRep,
        pApp,
        dummy_func,
        dummy_step,
        dummy_final
    );

    /* Cleanup */
    free(safe_name);
    free(pApp);
    sqlite3_close(db);

    return 0;
}