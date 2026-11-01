// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to avoid name conflicts */
static int dummy_compare(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    if (nLeft == 0 && nRight == 0) return 0;
    if (nLeft == 0) return -1;
    if (nRight == 0) return 1;
    return memcmp(pLeft, pRight, nLeft < nRight ? nLeft : nRight);
}

static void dummy_step(sqlite3_context *ctx, int nVal, sqlite3_value **val) {
    /* Do nothing */
}

static void dummy_final(sqlite3_context *ctx) {
    sqlite3_result_null(ctx);
}

static void dummy_value(sqlite3_context *ctx) {
    sqlite3_result_null(ctx);
}

static void dummy_inverse(sqlite3_context *ctx, int nVal, sqlite3_value **val) {
    /* Do nothing */
}

static void dummy_destroy(void *pApp) {
    /* Do nothing */
}

static void dummy_func(sqlite3_context *ctx, int nVal, sqlite3_value **val) {
    sqlite3_result_null(ctx);
}

/* Function specifically for testing sqlite3_value_bytes16 */
static void func_using_bytes16(sqlite3_context *ctx, int nVal, sqlite3_value **val) {
    if (nVal > 0 && val != NULL) {
        int len = sqlite3_value_bytes16(val[0]);
        (void)len;
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Prepare environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have some data to work with */
    if (Size < 1) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy table to ensure the DB is in a valid state for function calls */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    /*
     * Fuzzing Logic:
     * We interpret the input buffer to drive the parameters of the target APIs.
     * We focus on boundary conditions, NULL pointers, and varied encodings.
     */

    /* --- 1. Fuzz sqlite3_create_collation16 --- */
    /* zName must be a valid UTF-16 string. We construct a safe one from input if possible. */
    if (Size >= 5) {
        uint16_t name_utf16[4] = {0};
        name_utf16[0] = Data[0];
        name_utf16[1] = Data[1];
        name_utf16[2] = Data[2];
        name_utf16[3] = 0; /* Null terminator */
        
        int enc = (Data[3] % 5); /* 0 to 4 mapping to SQLITE_UTF8, UTF16LE, UTF16BE, UTF16, UTF16_ALIGNED */
        if (enc == 0) enc = SQLITE_UTF8;
        else if (enc == 1) enc = SQLITE_UTF16LE;
        else if (enc == 2) enc = SQLITE_UTF16BE;
        else if (enc == 3) enc = SQLITE_UTF16;
        else enc = SQLITE_UTF16_ALIGNED;

        rc = sqlite3_create_collation16(db, name_utf16, enc, (void*)(uintptr_t)(Data[4] % 256), dummy_compare);
        (void)rc;
    }

    /* --- 2. Fuzz sqlite3_create_window_function --- */
    if (Size >= 18) {
        /* Create a safe ASCII name from input */
        char name[16] = {0};
        int len = Size < 15 ? Size : 15;
        for (int i = 0; i < len; i++) {
            char c = Data[i];
            if (c >= 32 && c < 127) name[i] = c;
            else name[i] = 'a';
        }
        name[len] = '\0';

        int nArg = (Data[15] % 10) - 5; /* -5 to 4 */
        int enc = (Data[16] % 5);
        if (enc == 0) enc = SQLITE_UTF8;
        else if (enc == 1) enc = SQLITE_UTF16LE;
        else if (enc == 2) enc = SQLITE_UTF16BE;
        else if (enc == 3) enc = SQLITE_UTF16;
        else enc = SQLITE_UTF16_ALIGNED;

        /* Register with all callbacks */
        rc = sqlite3_create_window_function(db, name, nArg, enc, (void*)(uintptr_t)(Data[17] % 256),
                                            dummy_step, dummy_final, dummy_value, dummy_inverse, dummy_destroy);
        (void)rc;
    }

    /* --- 3. Fuzz sqlite3_create_collation --- */
    if (Size >= 17) {
        char name[16] = {0};
        int len = Size < 15 ? Size : 15;
        for (int i = 0; i < len; i++) {
            char c = Data[i];
            if (c >= 32 && c < 127) name[i] = c;
            else name[i] = 'b';
        }
        name[len] = '\0';

        int enc = (Data[15] % 5);
        if (enc == 0) enc = SQLITE_UTF8;
        else if (enc == 1) enc = SQLITE_UTF16LE;
        else if (enc == 2) enc = SQLITE_UTF16BE;
        else if (enc == 3) enc = SQLITE_UTF16;
        else enc = SQLITE_UTF16_ALIGNED;

        rc = sqlite3_create_collation(db, name, enc, (void*)(uintptr_t)(Data[16] % 256), dummy_compare);
        (void)rc;
        
        /* Test deletion by passing NULL compare */
        rc = sqlite3_create_collation(db, name, enc, NULL, NULL);
        (void)rc;
    }

    /* --- 4. Fuzz sqlite3_value_bytes16 --- */
    /* This function requires a valid sqlite3_value from a callback. 
       We simulate a call by creating a function that uses it. */
    static int bytes16_called = 0;

    if (!bytes16_called && Size >= 2) {
        char fname[16] = "fuzz_bytes16";
        rc = sqlite3_create_function(db, fname, 1, SQLITE_UTF8, NULL, func_using_bytes16, NULL, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_exec(db, "SELECT fuzz_bytes16('test')", NULL, NULL, NULL);
            bytes16_called = 1;
        }
    }

    /* --- 5. Fuzz sqlite3_create_function16 --- */
    if (Size >= 18) {
        uint16_t name_utf16[16] = {0};
        int len = Size < 15 ? Size : 15;
        for (int i = 0; i < len; i++) {
            name_utf16[i] = Data[i];
        }
        name_utf16[len] = 0;

        int nArg = (Data[15] % 10) - 5;
        int enc = (Data[16] % 5);
        if (enc == 0) enc = SQLITE_UTF8;
        else if (enc == 1) enc = SQLITE_UTF16LE;
        else if (enc == 2) enc = SQLITE_UTF16BE;
        else if (enc == 3) enc = SQLITE_UTF16;
        else enc = SQLITE_UTF16_ALIGNED;

        rc = sqlite3_create_function16(db, name_utf16, nArg, enc, (void*)(uintptr_t)(Data[17] % 256),
                                       dummy_func, dummy_step, dummy_final);
        (void)rc;
    }

    /* --- 6. Fuzz sqlite3_create_function --- */
    if (Size >= 18) {
        char name[16] = {0};
        int len = Size < 15 ? Size : 15;
        for (int i = 0; i < len; i++) {
            char c = Data[i];
            if (c >= 32 && c < 127) name[i] = c;
            else name[i] = 'c';
        }
        name[len] = '\0';

        int nArg = (Data[15] % 10) - 5;
        int enc = (Data[16] % 5);
        if (enc == 0) enc = SQLITE_UTF8;
        else if (enc == 1) enc = SQLITE_UTF16LE;
        else if (enc == 2) enc = SQLITE_UTF16BE;
        else if (enc == 3) enc = SQLITE_UTF16;
        else enc = SQLITE_UTF16_ALIGNED;

        /* Test scalar function */
        rc = sqlite3_create_function(db, name, nArg, enc, (void*)(uintptr_t)(Data[17] % 256),
                                     dummy_func, NULL, NULL);
        (void)rc;

        /* Test aggregate function */
        rc = sqlite3_create_function(db, name, nArg, enc, (void*)(uintptr_t)(Data[17] % 256),
                                     NULL, dummy_step, dummy_final);
        (void)rc;

        /* Test deletion */
        rc = sqlite3_create_function(db, name, nArg, enc, NULL, NULL, NULL, NULL);
        (void)rc;
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}