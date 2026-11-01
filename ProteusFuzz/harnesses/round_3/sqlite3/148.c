// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzzing target for sqlite3_value_* family of functions.
 * These functions require a protected sqlite3_value object, which is typically
 * obtained inside a user-defined SQL function callback.
 */

static void custom_func_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /*
     * Iterate over all arguments and call the target APIs.
     * We expect argc to be at least 1 for meaningful testing.
     */
    for (int i = 0; i < argc; i++) {
        sqlite3_value *val = argv[i];
        if (val == NULL) continue;

        /*
         * Call sqlite3_value_text16
         * Returns const void* to UTF-16 text in native byte order.
         */
        const void *text16 = sqlite3_value_text16(val);
        (void)text16; /* Suppress unused variable warning */

        /*
         * Call sqlite3_value_bytes
         * Returns byte length of value as UTF-8.
         */
        int bytes = sqlite3_value_bytes(val);
        (void)bytes;

        /*
         * Call sqlite3_value_text
         * Returns UTF-8 encoded C-string.
         */
        const unsigned char *text = sqlite3_value_text(val);
        (void)text;

        /*
         * Call sqlite3_value_text16le
         * Returns UTF-16 little-endian string.
         */
        const void *text16le = sqlite3_value_text16le(val);
        (void)text16le;

        /*
         * Call sqlite3_value_subtype
         * Returns custom 32-bit context tag.
         */
        unsigned int subtype = sqlite3_value_subtype(val);
        (void)subtype;

        /*
         * Call sqlite3_value_blob
         * Returns BLOB data pointer.
         */
        const void *blob = sqlite3_value_blob(val);
        (void)blob;
    }

    /* Return a simple result to keep the function valid */
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Create a custom scalar function that will be called with arguments
     * derived from the fuzz input. We use the function to trigger the
     * sqlite3_value_* APIs inside a callback context.
     */
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8 | SQLITE_SUBTYPE,
                                  NULL, custom_func_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Prepare a SQL statement that uses the custom function with multiple arguments.
     * We bind parameters from the fuzz input to test various value types.
     */
    const char *sql = "SELECT fuzz_func(?1, ?2, ?3, ?4, ?5)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /*
     * Bind parameters from the fuzz input.
     * We try to bind different types: integer, text, blob, etc.
     */
    if (Size >= 1) {
        int64_t int_val = (int64_t)Data[0];
        sqlite3_bind_int64(stmt, 1, int_val);
    }
    if (Size >= 2) {
        double double_val = (double)Data[1];
        sqlite3_bind_double(stmt, 2, double_val);
    }
    if (Size >= 3) {
        /* Bind text: use a portion of the input as text */
        size_t text_len = Size - 2;
        if (text_len > 100) text_len = 100; /* Limit text length */
        sqlite3_bind_text(stmt, 3, (const char *)(Data + 2), text_len, SQLITE_TRANSIENT);
    }
    if (Size >= 4) {
        /* Bind blob: use a portion of the input as blob */
        size_t blob_len = Size - 3;
        if (blob_len > 100) blob_len = 100; /* Limit blob length */
        sqlite3_bind_blob(stmt, 4, (const void *)(Data + 3), blob_len, SQLITE_TRANSIENT);
    }
    if (Size >= 5) {
        /* Bind another integer */
        int64_t int_val2 = (int64_t)Data[4];
        sqlite3_bind_int64(stmt, 5, int_val2);
    }

    /* Execute the statement and step through results */
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        /*
         * The custom function has already been called and tested the APIs.
         * We can also test sqlite3_column_value on the result column.
         * Note: sqlite3_column_value returns an unprotected value, which is
         * not suitable for the target APIs. However, we can still call it
         * to ensure the statement execution is valid.
         */
        sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
        (void)col_val;
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}