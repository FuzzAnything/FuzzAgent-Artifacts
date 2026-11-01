// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Global variable to store the context captured during function execution */
static sqlite3_context *g_captured_ctx = NULL;

/* Custom SQL function that captures the context */
static void context_capturing_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    g_captured_ctx = ctx;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the function to capture context */
    rc = sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, context_capturing_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func()";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step 1: Invoke sqlite3_value_text */
    /* We pass NULL to test the NULL handling path safely */
    sqlite3_value *dummyVal = NULL;
    const unsigned char *textResult = sqlite3_value_text(dummyVal);
    (void)textResult; /* Expected to be NULL */

    /* Step 2: Execute statement to capture context for Step 4 */
    rc = sqlite3_step(stmt);
    /* rc should be SQLITE_DONE */

    /* Step 3: Invoke sqlite3_malloc64 */
    sqlite3_uint64 allocSize = 1;
    if (Size >= 8) {
        allocSize = *(const sqlite3_uint64 *)(Data);
        if (allocSize == 0) allocSize = 1;
        if (allocSize > 1024 * 1024) allocSize = 1024 * 1024;
    } else if (Size > 0) {
        allocSize = Data[0];
        if (allocSize == 0) allocSize = 1;
        if (allocSize > 1024 * 1024) allocSize = 1024 * 1024;
    }

    void *mem = sqlite3_malloc64(allocSize);
    if (mem == NULL) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Copy input data to allocated memory */
    size_t copyLen = (allocSize > Size) ? Size : allocSize;
    if (copyLen > 0) {
        memcpy(mem, Data, copyLen);
        if (copyLen < allocSize) {
            ((char *)mem)[copyLen] = '\0';
        }
    } else {
        if (allocSize > 0) ((char *)mem)[0] = '\0';
    }

    /* Step 4: Invoke sqlite3_result_text */
    /* Only if we successfully captured a context */
    if (g_captured_ctx != NULL) {
        sqlite3_result_text(g_captured_ctx, (const char *)mem, (int)copyLen, NULL);
    }

    /* Step 5: Cleanup */
    sqlite3_free(mem);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}