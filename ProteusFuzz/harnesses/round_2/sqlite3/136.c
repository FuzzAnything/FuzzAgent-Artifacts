// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target functions:
 * 1. sqlite3_db_name
 * 2. sqlite3_db_filename
 * 3. sqlite3_open
 * 4. sqlite3_open_v2
 * 5. sqlite3_file_control
 * 6. sqlite3_close_v2
 */

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close_v2(db);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;
    int flags;
    int n;
    int op;
    /* Allocate pArg on heap with sufficient size (8 bytes) to prevent stack overflow
     * when sqlite3_file_control casts pArg to i64* or other large types. */
    sqlite3_int64 *pArg = (sqlite3_int64 *)sqlite3_malloc(sizeof(sqlite3_int64));
    if (!pArg) {
        return 0;
    }
    *pArg = 0;

    /* Safely derive parameters from input data with bounds checking */
    flags = (int)Data[0];
    n = (Size > 1) ? (int)Data[1] : 0;
    op = (Size > 2) ? (int)Data[2] : 0;

    /* Use a dummy file for on-disk database operations */
    const char *filename = "./dummy_file";
    const char *vfs_name = NULL;

    /* Try to open database with sqlite3_open */
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        /* Even on error, db might be non-NULL, so clean up */
        cleanup_db(db);
        db = NULL;
        /* Try opening with sqlite3_open_v2 with different flags */
        rc = sqlite3_open_v2(filename, &db, flags, vfs_name);
        if (rc != SQLITE_OK && db != NULL) {
            cleanup_db(db);
            db = NULL;
        }
    }

    if (db == NULL) {
        /* If we couldn't open a database, try with in-memory or empty string */
        rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK) {
            cleanup_db(db);
            db = NULL;
        }
    }

    if (db == NULL) {
        /* Last resort: try opening with sqlite3_open_v2 and various flags */
        rc = sqlite3_open_v2("", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc != SQLITE_OK && db != NULL) {
            cleanup_db(db);
            db = NULL;
        }
    }

    if (db != NULL) {
        /* Test sqlite3_db_name */
        const char *schema_name = sqlite3_db_name(db, n);
        (void)schema_name; /* Suppress unused variable warning */

        /* Test sqlite3_db_filename with various database names */
        const char *db_name = "main";
        sqlite3_filename filename_main = sqlite3_db_filename(db, db_name);
        (void)filename_main;

        db_name = "temp";
        sqlite3_filename filename_temp = sqlite3_db_filename(db, db_name);
        (void)filename_temp;

        /* Test with a potentially invalid database name derived from input */
        if (Size > 10) {
            char invalid_name[16];
            size_t len = (Size > 15) ? 15 : Size - 10;
            memcpy(invalid_name, Data + 10, len);
            invalid_name[len] = '\0';
            sqlite3_filename filename_invalid = sqlite3_db_filename(db, invalid_name);
            (void)filename_invalid;
        }

        /* Test sqlite3_file_control with various opcodes */
        rc = sqlite3_file_control(db, "main", op, pArg);
        (void)rc;

        rc = sqlite3_file_control(db, "temp", op, pArg);
        (void)rc;

        /* Test with NULL db name (refers to main) */
        rc = sqlite3_file_control(db, NULL, op, pArg);
        (void)rc;

        /* Test with invalid db name */
        if (Size > 20) {
            char invalid_db_name[16];
            size_t len = (Size > 29) ? 15 : Size - 20;
            memcpy(invalid_db_name, Data + 20, len);
            invalid_db_name[len] = '\0';
            rc = sqlite3_file_control(db, invalid_db_name, op, pArg);
            (void)rc;
        }

        /* Try to attach a database to test more states */
        if (Size > 30) {
            char attach_name[16];
            size_t len = (Size > 39) ? 15 : Size - 30;
            memcpy(attach_name, Data + 30, len);
            attach_name[len] = '\0';
            
            char attach_sql[64];
            snprintf(attach_sql, sizeof(attach_sql), "ATTACH DATABASE '%s' AS fuzzdb", attach_name);
            char *err_msg = NULL;
            rc = sqlite3_exec(db, attach_sql, NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            (void)rc;

            /* Now test db_name and db_filename with the attached database */
            const char *attached_schema = sqlite3_db_name(db, 2);
            (void)attached_schema;

            sqlite3_filename attached_filename = sqlite3_db_filename(db, "fuzzdb");
            (void)attached_filename;

            /* Test file_control on attached database */
            rc = sqlite3_file_control(db, "fuzzdb", op, pArg);
            (void)rc;
        }

        /* Close the database */
        cleanup_db(db);
        db = NULL;
    }

    /* Test sqlite3_close_v2 with NULL (should be safe) */
    rc = sqlite3_close_v2(NULL);
    (void)rc;

    /* Free the allocated argument buffer */
    sqlite3_free(pArg);

    return 0;
}