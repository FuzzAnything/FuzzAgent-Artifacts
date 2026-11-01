// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value related APIs.
 * Target APIs:
 * - sqlite3_value_text16be
 * - sqlite3_value_dup
 * - sqlite3_value_free
 * - sqlite3_value_text16
 * - sqlite3_value_text16le
 * - sqlite3_value_subtype
 */

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    /* This function is registered to ensure the environment is set up correctly */
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    sqlite3_value *val = NULL;
    sqlite3_value *dup_val = NULL;
    const void *text16 = NULL;
    const void *text16be = NULL;
    const void *text16le = NULL;
    unsigned int subtype = 0;

    (void)Data;
    (void)Size;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table and insert data to generate sqlite3_value objects */
    const char *sql = "CREATE TABLE test (id INTEGER, data TEXT); INSERT INTO test VALUES (1, 'hello');";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register a dummy function to ensure the environment is set up correctly */
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);

    /* Prepare a statement to fetch values */
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test WHERE id = 1;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step to get a result */
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        /* Get an unprotected value from the column */
        val = sqlite3_column_value(stmt, 0);
        
        if (val != NULL) {
            /* Test sqlite3_value_subtype */
            subtype = sqlite3_value_subtype(val);
            (void)subtype; /* Use to avoid unused warning */

            /* Test sqlite3_value_dup to create a protected copy */
            dup_val = sqlite3_value_dup(val);
            
            if (dup_val != NULL) {
                /* Test sqlite3_value_text16 on the duplicated (protected) value */
                text16 = sqlite3_value_text16(dup_val);
                (void)text16;

                /* Test sqlite3_value_text16be on the duplicated (protected) value */
                text16be = sqlite3_value_text16be(dup_val);
                (void)text16be;

                /* Test sqlite3_value_text16le on the duplicated (protected) value */
                text16le = sqlite3_value_text16le(dup_val);
                (void)text16le;

                /* Free the duplicated value */
                sqlite3_value_free(dup_val);
            }
        }
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}