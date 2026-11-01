// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 string and prepare APIs.
 * Target sequence:
 * 1. sqlite3_str_appendf
 * 2. sqlite3_str_append
 * 3. sqlite3_str_append
 * 4. sqlite3_prepare_v2
 * 5. sqlite3_str_value
 * 6. sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *zTail = NULL;
    char *resultStr = NULL;
    int rc;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a dynamic string object */
    str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        return 0;
    }

    /* Ensure we have enough data to work with */
    if (Size < 2) {
        sqlite3_str_finish(str);
        sqlite3_close(db);
        return 0;
    }

    /* 2. Invoke sqlite3_str_appendf
     * We use a simple format string to avoid complex parsing issues in fuzzing.
     */
    sqlite3_str_appendf(str, "prefix_%d", (int)Data[0]);

    /* 3. Invoke sqlite3_str_append (First call)
     * Append a chunk of data. We limit the length to avoid huge allocations.
     */
    int len1 = (Size > 100) ? 100 : (int)Size - 1;
    if (len1 > 0) {
        sqlite3_str_append(str, (const char *)&Data[1], len1);
    }

    /* 4. Invoke sqlite3_str_append (Second call)
     * Append another chunk, potentially overlapping or different.
     */
    int offset2 = (Size > 10) ? 10 : 1;
    int len2 = (Size > offset2 + 50) ? 50 : (int)(Size - offset2);
    if (len2 > 0) {
        sqlite3_str_append(str, (const char *)&Data[offset2], len2);
    }

    /* 5. Retrieve the constructed string
     * sqlite3_str_value returns a pointer to the internal buffer.
     * The buffer is owned by the sqlite3_str object.
     */
    resultStr = sqlite3_str_value(str);

    /* 6. Invoke sqlite3_prepare_v2
     * We attempt to prepare the constructed string as SQL.
     */
    if (resultStr) {
        rc = sqlite3_prepare_v2(db, resultStr, (int)strlen(resultStr), &stmt, &zTail);
    } else {
        /* Fallback to a simple query if the string is empty/invalid */
        rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, &zTail);
    }

    /* 7. Invoke sqlite3_finalize
     * Clean up the prepared statement.
     */
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* 8. Cleanup: Finish the string object and close the database
     * sqlite3_str_finish frees the internal buffer and the struct itself.
     * This is crucial to prevent the memory leak reported by LeakSanitizer.
     */
    if (str) {
        sqlite3_str_finish(str);
        str = NULL;
    }

    if (db) {
        sqlite3_close(db);
        db = NULL;
    }

    return 0;
}