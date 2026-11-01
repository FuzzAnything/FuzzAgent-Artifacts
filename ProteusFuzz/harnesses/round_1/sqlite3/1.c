// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence:
 * sqlite3_prepare_v2 -> sqlite3_errmsg -> sqlite3_step -> sqlite3_step ->
 * sqlite3_column_count -> sqlite3_column_type -> sqlite3_column_name ->
 * sqlite3_column_text (x3) -> sqlite3_column_bytes -> sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql;
    int rc;
    int i;

    if (Size == 0) {
        return 0;
    }

    /* Ensure the input has a null terminator for safety if used as string */
    /* We will use the raw data as SQL, but ensure it's treated safely */
    /* sqlite3_prepare_v2 takes a length, so we can pass Size directly */
    sql = (const char *)Data;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 1. sqlite3_prepare_v2 */
    rc = sqlite3_prepare_v2(db, sql, (int)Size, &stmt, NULL);
    
    /* 2. sqlite3_errmsg */
    /* Always safe to call, even if prepare failed */
    const char *err = sqlite3_errmsg(db);
    (void)err; /* Use to suppress unused warning */

    /* If prepare failed, finalize and exit early */
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 3. sqlite3_step (First call) */
    rc = sqlite3_step(stmt);

    /* 4. sqlite3_step (Second call) */
    /* Even if the first step returned SQLITE_DONE, calling step again is valid 
       (it will return SQLITE_ROW if data exists, or SQLITE_DONE/MISUSE depending on state) */
    rc = sqlite3_step(stmt);

    /* 5. sqlite3_column_count */
    int colCount = sqlite3_column_count(stmt);

    /* 6. sqlite3_column_type */
    /* Check column 0 if available */
    if (colCount > 0) {
        int type = sqlite3_column_type(stmt, 0);
        (void)type;
    }

    /* 7. sqlite3_column_name */
    /* Check column 0 if available */
    if (colCount > 0) {
        const char *name = sqlite3_column_name(stmt, 0);
        (void)name;
    }

    /* 8. sqlite3_column_text (x3) */
    /* Attempt to read text from first 3 columns if they exist */
    for (i = 0; i < 3 && i < colCount; i++) {
        const unsigned char *text = sqlite3_column_text(stmt, i);
        (void)text;
    }

    /* 9. sqlite3_column_bytes */
    /* Check column 0 if available */
    if (colCount > 0) {
        int bytes = sqlite3_column_bytes(stmt, 0);
        (void)bytes;
    }

    /* 10. sqlite3_finalize */
    sqlite3_finalize(stmt);

    /* Cleanup database */
    sqlite3_close(db);

    return 0;
}