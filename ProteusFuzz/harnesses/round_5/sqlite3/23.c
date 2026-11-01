// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:185731:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
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

static int dummy_callback(void *data, int argc, char **argv, char **colz) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)colz;
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    const char *sql;
    const char *tail;
    int rc;

    /* Ensure we have enough data to form a minimal SQL string if needed */
    if (Size < 1) {
        return 0;
    }

    /* Step 1: sqlite3_blob_close (No-op on NULL or invalid handle) */
    rc = sqlite3_blob_close(blob);
    (void)rc;

    /* Step 2: sqlite3_exec (Requires a valid db, but we call it before open to test error path) */
    /* We pass a potentially malformed SQL string derived from fuzz input */
    rc = sqlite3_exec(NULL, (const char *)Data, dummy_callback, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* Step 3: sqlite3_close (No-op on NULL) */
    rc = sqlite3_close(db);
    (void)rc;

    /* Step 4: sqlite3_close (No-op on NULL) */
    rc = sqlite3_close(db);
    (void)rc;

    /* Step 5: sqlite3_libversion */
    const char *version = sqlite3_libversion();
    (void)version;

    /* Step 6: sqlite3_open */
    /* Use a dummy file name as requested */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        /* If open fails, we might not have a valid db for subsequent steps,
           but we continue to test error handling paths. */
        sqlite3_close(db);
        db = NULL;
    }

    if (db) {
        /* Step 7: sqlite3_prepare_v2 */
        /* Use fuzz input as SQL, ensuring it's null-terminated for safety if possible,
           though sqlite3_prepare_v2 takes nByte. We use Size or a safe limit. */
        int nByte = (Size < 10000) ? (int)Size : 10000;
        rc = sqlite3_prepare_v2(db, (const char *)Data, nByte, &stmt, &tail);
        (void)tail;

        if (rc == SQLITE_OK && stmt) {
            /* Step 8: sqlite3_step */
            rc = sqlite3_step(stmt);

            /* Step 9: sqlite3_column_text */
            /* Only call if we got a row, otherwise it might return NULL safely */
            if (rc == SQLITE_ROW) {
                const unsigned char *text = sqlite3_column_text(stmt, 0);
                (void)text;
            }
        }

        /* Step 10: sqlite3_finalize */
        if (stmt) {
            rc = sqlite3_finalize(stmt);
            (void)rc;
            stmt = NULL;
        }

        /* Step 11: sqlite3_close */
        rc = sqlite3_close(db);
        (void)rc;
        db = NULL;
    }

    return 0;
}