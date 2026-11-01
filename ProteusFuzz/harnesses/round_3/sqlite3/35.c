// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
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

/*
 * Custom SQL function callback that demonstrates the usage of the target APIs.
 * This function is invoked by SQLite during SQL execution.
 */
static void fuzz_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Step 1: sqlite3_value_text
     * Retrieve text from the first argument if available.
     * Note: The returned pointer is valid only within this callback.
     */
    const unsigned char *text_data = NULL;
    if (argc > 0 && argv[0] != NULL) {
        text_data = sqlite3_value_text(argv[0]);
    }

    /* Step 2: sqlite3_malloc64
     * Allocate memory based on the length of the text or a fuzzed size.
     * We use a safe size derived from the input to avoid OOM, but allow fuzzing.
     */
    sqlite3_uint64 alloc_size = 0;
    if (text_data != NULL) {
        alloc_size = strlen((const char *)text_data) + 1;
    } else {
        /* Fallback to a small allocation if no text is available */
        alloc_size = 10;
    }

    /* Limit allocation size to prevent excessive memory usage during fuzzing */
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    void *ptr = sqlite3_malloc64(alloc_size);
    if (ptr == NULL) {
        /* Allocation failed; return early to avoid undefined behavior */
        sqlite3_result_error_nomem(context);
        return;
    }

    /* Step 3: sqlite3_result_text
     * Set the result of the function.
     * If we have text_data, use it; otherwise, use the allocated buffer.
     * We pass NULL as the destructor because sqlite3_result_text will
     * copy the string if the destructor is NULL, or manage it if provided.
     * Here we just copy the data we have.
     */
    if (text_data != NULL) {
        sqlite3_result_text(context, (const char *)text_data, -1, SQLITE_TRANSIENT);
    } else {
        /* Write some dummy data to the allocated buffer to exercise the path */
        memset(ptr, 'A', alloc_size);
        /* Ensure null termination if we treat it as a string, though size is passed */
        if (alloc_size > 0) {
            ((char *)ptr)[alloc_size - 1] = '\0';
        }
        sqlite3_result_text(context, (const char *)ptr, (int)alloc_size - 1, SQLITE_TRANSIENT);
    }

    /* Step 4: sqlite3_free
     * Free the allocated memory.
     * Ensure we only free if allocation succeeded.
     */
    sqlite3_free(ptr);
    ptr = NULL; /* Prevent double-free */
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Prepare environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a dummy table to allow SQL execution */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function that exercises the target APIs */
    rc = sqlite3_create_function(db, "fuzz_test_func", 1, SQLITE_UTF8, NULL, fuzz_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement that uses the custom function */
    /* We use the fuzzed data as input to the function */
    const char *sql_template = "SELECT fuzz_test_func(?) FROM test_table LIMIT 1;";
    
    /* Ensure the fuzzed data is treated as a string for the SQL parameter */
    /* We need to escape or handle the data safely. For fuzzing, we can try to bind it directly.
     * If the data contains null bytes, sqlite3_bind_text handles it if length is provided.
     */
    rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzzed input as text to the first parameter */
    /* We pass Size to ensure null bytes are handled correctly */
    sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);

    /* Execute the statement to trigger the callback */
    rc = sqlite3_step(stmt);
    /* We don't care about the result of the step, just that it executed without crashing */
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}