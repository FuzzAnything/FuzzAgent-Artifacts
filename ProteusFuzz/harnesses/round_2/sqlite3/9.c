// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * Static helper to create a valid sqlite3_value from input data.
 */
static sqlite3_value* create_protected_value_from_input(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *val = NULL;

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }

    const char *sql = "SELECT ?1;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }

    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 1);
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }

    val = sqlite3_column_value(stmt, 0);
    
    if (val) {
        val = sqlite3_value_dup(val);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return val;
}

/* 
 * Callback function for the fuzz target.
 * Defined outside LLVMFuzzerTestOneInput to avoid compilation errors.
 */
static void fuzz_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_value *v = (sqlite3_value*)sqlite3_user_data(ctx);
    if (!v) return;

    /* 1. sqlite3_value_text */
    const unsigned char *text_val = sqlite3_value_text(v);
    (void)text_val;

    /* 2. sqlite3_value_blob */
    const void *blob_val = sqlite3_value_blob(v);
    (void)blob_val;

    /* 3. sqlite3_value_bytes */
    int bytes_val = sqlite3_value_bytes(v);
    (void)bytes_val;

    /* 4. sqlite3_result_int64 */
    sqlite3_int64 result_val = (bytes_val > 0) ? bytes_val : 0;
    sqlite3_result_int64(ctx, result_val);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    /* Step 1: Prepare environment */
    sqlite3_value *value = create_protected_value_from_input(Data, Size);
    if (!value) {
        return 0;
    }

    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_value_free(value);
        return 0;
    }

    /* Register the function with our value as user_data */
    sqlite3_create_function(db, "fuzz_target", 0, SQLITE_UTF8, value, fuzz_callback, NULL, NULL);

    sqlite3_stmt *stmt = NULL;
    /* Execute the function to trigger the callback */
    if (sqlite3_prepare_v2(db, "SELECT fuzz_target();", -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_close(db);
    sqlite3_value_free(value);

    return 0;
}