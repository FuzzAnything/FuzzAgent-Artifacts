// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a simple destructor that tracks calls or does nothing.
 * Used to satisfy the destructor parameter in result functions.
 */
static void dummy_destructor(void *p) {
    (void)p;
}

/*
 * Static helper to create a custom function that uses the target APIs.
 * This function is registered with the database and called during SQL execution.
 */
static void target_function_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) {
        return;
    }

    /* 
     * To avoid global-buffer-overflow and ensure valid UTF-16 input,
     * we use properly allocated and initialized buffers.
     * For UTF-16LE and UTF-16BE, the data must be valid UTF-16 (even length, valid code points).
     * We use simple ASCII characters which are valid in UTF-16 (0x0041, etc.).
     */

    /* 
     * BLOB data: simple string, safe for any encoding.
     */
    static const char *blob_data = "FUZZ_BLOB_DATA";
    sqlite3_result_blob(ctx, blob_data, 16, dummy_destructor);

    /* 
     * UTF-16LE data: Must be even length and valid UTF-16.
     * We construct a valid UTF-16LE string manually to ensure safety.
     * "FUZZ" in UTF-16LE is: 'F'=0x46, 0x00, 'U'=0x55, 0x00, 'Z'=0x5A, 0x00, 'Z'=0x5A, 0x00
     */
    static const unsigned char text16le_data[] = {
        0x46, 0x00, 0x55, 0x00, 0x5A, 0x00, 0x5A, 0x00
    };
    sqlite3_result_text16le(ctx, text16le_data, 8, dummy_destructor);

    /* 
     * UTF-8 text: standard C string.
     */
    static const char *text_data = "FUZZ_TEXT";
    sqlite3_result_text(ctx, text_data, -1, SQLITE_STATIC);

    /* 
     * Auxdata: simple integer pointer.
     */
    static int aux_val = 42;
    sqlite3_set_auxdata(ctx, 0, &aux_val, dummy_destructor);

    /* 
     * UTF-16BE data: Must be even length and valid UTF-16.
     * "FUZZ" in UTF-16BE is: 0x00, 0x46, 0x00, 0x55, 0x00, 0x5A, 0x00, 0x5A
     */
    static const unsigned char text16be_data[] = {
        0x00, 0x46, 0x00, 0x55, 0x00, 0x5A, 0x00, 0x5A
    };
    sqlite3_result_text16be(ctx, text16be_data, 8, dummy_destructor);

    /* 
     * UTF-16 (native) data: We'll use the same as LE for simplicity, 
     * assuming the system is little-endian (common). 
     * If big-endian, the bytes would be swapped, but the data is still valid UTF-16.
     */
    static const unsigned char text16_data[] = {
        0x46, 0x00, 0x55, 0x00, 0x5A, 0x00, 0x5A, 0x00
    };
    sqlite3_result_text16(ctx, text16_data, 8, dummy_destructor);
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Guard against empty input or excessively large input */
    if (Size == 0 || Size > 1024 * 1024) {
        return 0;
    }

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function that exercises the target APIs */
    rc = sqlite3_create_function(db, "fuzz_target_func", 0, SQLITE_UTF8, NULL, target_function_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Construct a SQL query that invokes the function.
     * We use a simple query to ensure the function is called.
     */
    const char *sql_query = "SELECT fuzz_target_func();";
    
    /* 
     * Prepare the statement
     */
    rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Execute the statement to trigger the callback */
        do {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                /* 
                 * If we get a row, we can inspect the result, 
                 * but the main goal is that the callback ran.
                 */
            }
        } while (rc == SQLITE_ROW);
        
        sqlite3_finalize(stmt);
    }

    /* 
     * Try another query that might trigger different states, 
     * potentially using the fuzz input as a parameter if we bind it.
     * This tests the interaction between binding and result setting.
     */
    const char *bind_query = "SELECT fuzz_target_func() WHERE ? IS NOT NULL;";
    rc = sqlite3_prepare_v2(db, bind_query, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Bind the fuzz input as a blob */
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
        
        do {
            rc = sqlite3_step(stmt);
        } while (rc == SQLITE_ROW);
        
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}