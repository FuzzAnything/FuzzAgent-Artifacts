// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3 extended result codes and error handling APIs.
 * Target APIs:
 * 1. sqlite3_extended_result_codes
 * 2. sqlite3_is_interrupted
 * 3. sqlite3_extended_errcode
 * 4. sqlite3_close_v2
 * 5. sqlite3_errcode
 * 6. sqlite3_wal_checkpoint_v2
 */

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";

    /* Write input data to dummy file if size is reasonable to avoid huge files */
    if (Size < 1024 * 1024) {
        write_dummy_file(filename, Data, Size);
    } else {
        /* Write a small header if data is too large */
        write_dummy_file(filename, Data, 1024);
    }

    /* Open database in read-write mode, creating if necessary */
    int rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        /* If open fails, we can still test some APIs with NULL or invalid db */
        /* Test sqlite3_is_interrupted with NULL */
        (void)sqlite3_is_interrupted(NULL);
        /* Test sqlite3_extended_errcode with NULL */
        (void)sqlite3_extended_errcode(NULL);
        /* Test sqlite3_errcode with NULL */
        (void)sqlite3_errcode(NULL);
        /* Test sqlite3_extended_result_codes with NULL */
        (void)sqlite3_extended_result_codes(NULL, 1);
        (void)sqlite3_extended_result_codes(NULL, 0);
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    uint8_t *ptr = (uint8_t *)Data;
    size_t idx = 0;

    /* 1. sqlite3_extended_result_codes: enable/disable based on input */
    int enable_extended = (idx < Size) ? (ptr[idx++] % 2) : 0;
    (void)sqlite3_extended_result_codes(db, enable_extended);

    /* 2. sqlite3_is_interrupted: check interruption status */
    int is_interrupted = sqlite3_is_interrupted(db);
    (void)is_interrupted;

    /* 3. sqlite3_extended_errcode: retrieve extended error code */
    int ext_errcode = sqlite3_extended_errcode(db);
    (void)ext_errcode;

    /* 4. sqlite3_errcode: retrieve standard error code */
    int errcode = sqlite3_errcode(db);
    (void)errcode;

    /* 5. sqlite3_wal_checkpoint_v2: checkpoint with various modes */
    int mode = (idx < Size) ? (ptr[idx++] % 5) : 0; /* 0-4: PASSIVE, FULL, RESTART, TRUNCATE, NOOP */
    int *pnLog = NULL;
    int *pnCkpt = NULL;
    int log_val = 0, ckpt_val = 0;
    if (idx < Size && ptr[idx] == 1) {
        pnLog = &log_val;
        idx++;
    }
    if (idx < Size && ptr[idx] == 1) {
        pnCkpt = &ckpt_val;
        idx++;
    }

    const char *zDb = NULL;
    if (idx < Size) {
        /* Use a small portion of input as database name if it's a valid string */
        size_t name_len = 0;
        while (idx + name_len < Size && ptr[idx + name_len] != 0 && name_len < 100) {
            name_len++;
        }
        if (name_len > 0) {
            char *zDb_buf = malloc(name_len + 1);
            if (zDb_buf) {
                memcpy(zDb_buf, ptr + idx, name_len);
                zDb_buf[name_len] = '\0';
                zDb = zDb_buf;
                idx += name_len;
            }
        }
    }

    (void)sqlite3_wal_checkpoint_v2(db, zDb, mode, pnLog, pnCkpt);

    if (zDb) {
        free((void *)zDb);
    }

    /* Try to induce errors by executing invalid SQL */
    const char *invalid_sql = "SELECT * FROM nonexistent_table_xyz;";
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, invalid_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        /* Check error codes after failure */
        (void)sqlite3_errcode(db);
        (void)sqlite3_extended_errcode(db);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    /* 6. sqlite3_close_v2: close the database */
    rc = sqlite3_close_v2(db);
    (void)rc;

    /* Cleanup: remove dummy file */
    remove(filename);

    return 0;
}