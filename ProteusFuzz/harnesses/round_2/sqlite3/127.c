// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Global variable to control the behavior of the fuzzing callback.
 * This allows the main fuzzing loop to dictate which API is called inside the callback.
 */
static int g_fuzz_mode = 0;

/*
 * User-defined function implementation.
 * This function is called by SQLite when the SQL statement executes.
 * It receives sqlite3_value* arguments which we can inspect using the target APIs.
 */
static void fuzz_value_func_impl(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    if (argc < 1) return;

    sqlite3_value *val = argv[0];

    /* 
     * Call the target APIs based on g_fuzz_mode.
     * We call them to exercise the code paths.
     * We don't care about the return values for the fuzzing logic, 
     * just that the function is called with valid/invalid inputs.
     */
    switch (g_fuzz_mode % 6) {
        case 0:
            (void)sqlite3_value_text16be(val);
            break;
        case 1:
            (void)sqlite3_value_subtype(val);
            break;
        case 2:
            (void)sqlite3_value_text16(val);
            break;
        case 3:
            (void)sqlite3_value_text16le(val);
            break;
        case 4:
            (void)sqlite3_value_blob(val);
            break;
        case 5:
            (void)sqlite3_value_text(val);
            break;
    }
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Register the fuzz function with SQLITE_SUBTYPE to enable subtype testing.
     */
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8 | SQLITE_SUBTYPE, NULL, fuzz_value_func_impl, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Prepare a table to hold data if needed.
     */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_data (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    /* 
     * Parse the fuzz input to determine behavior.
     * We'll use the first byte to select the operation mode.
     */
    uint8_t mode = Data[0];
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    if (payload_size == 0) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Insert a row with the fuzz payload to use as a source for our function.
     */
    sqlite3_stmt *stmt_insert = NULL;
    const char *insert_sql = "INSERT INTO fuzz_data (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_blob(stmt_insert, 1, payload, payload_size, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt_insert);
    sqlite3_finalize(stmt_insert);

    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Set the mode based on the first byte of input.
     */
    g_fuzz_mode = mode;

    /* 
     * Execute the function call.
     * We select the data from the table and pass it to the function.
     */
    sqlite3_stmt *stmt_exec = NULL;
    const char *exec_sql = "SELECT fuzz_func(data) FROM fuzz_data;";
    rc = sqlite3_prepare_v2(db, exec_sql, -1, &stmt_exec, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt_exec);
        sqlite3_finalize(stmt_exec);
    }

    /* 
     * Also try to invoke the function with NULL or different types if possible.
     */
    if (Size > 2) {
        uint8_t type_mode = Data[1];
        sqlite3_stmt *stmt_exec2 = NULL;
        const char *exec_sql2 = "SELECT fuzz_func(?);";
        rc = sqlite3_prepare_v2(db, exec_sql2, -1, &stmt_exec2, NULL);
        if (rc == SQLITE_OK) {
            switch (type_mode % 5) {
                case 0:
                    sqlite3_bind_null(stmt_exec2, 1);
                    break;
                case 1:
                    sqlite3_bind_int(stmt_exec2, 1, 42);
                    break;
                case 2:
                    sqlite3_bind_double(stmt_exec2, 1, 3.14);
                    break;
                case 3:
                    sqlite3_bind_text(stmt_exec2, 1, "hello", 5, SQLITE_TRANSIENT);
                    break;
                case 4:
                    sqlite3_bind_blob(stmt_exec2, 1, payload, payload_size, SQLITE_TRANSIENT);
                    break;
            }
            rc = sqlite3_step(stmt_exec2);
            sqlite3_finalize(stmt_exec2);
        }
    }

    sqlite3_close(db);
    return 0;
}