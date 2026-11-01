// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:83994:28 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:84168:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Helper to safely close database connections */
static void safe_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/* Helper to safely finish backup operations */
static void safe_backup_finish(sqlite3_backup *backup) {
    if (backup) {
        sqlite3_backup_finish(backup);
    }
}

/* Helper to create a dummy database file with some content */
static int create_dummy_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        return -1;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        safe_close(db);
        return -1;
    }

    sql = "INSERT INTO test (data) VALUES ('fuzz_test_data');";
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        safe_close(db);
        return -1;
    }

    safe_close(db);
    return 0;
}

/* Helper to parse input data into parameters */
static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset, int *out_val) {
    if (*offset + sizeof(int) > size) {
        *out_val = 0;
        return 0;
    }
    memcpy(out_val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return 1;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure we have enough data for basic operations */
    if (Size < 4) return 0;

    size_t offset = 0;
    int nPage = 1;
    int flags = 0;

    /* Extract parameters from fuzz input */
    if (get_int_from_data(Data, Size, &offset, &nPage)) {
        /* Normalize nPage to avoid extreme values that might cause hangs */
        if (nPage > 1000) nPage = 1000;
        if (nPage < -1000) nPage = -1000;
    }

    if (offset + sizeof(int) <= Size) {
        memcpy(&flags, Data + offset, sizeof(int));
        offset += sizeof(int);
    }

    /* Create source and destination database files */
    const char *src_file = "./dummy_src.db";
    const char *dst_file = "./dummy_dst.db";

    /* Initialize source database */
    if (create_dummy_db(src_file) != 0) {
        return 0;
    }

    /* Initialize destination database (empty or with schema) */
    if (create_dummy_db(dst_file) != 0) {
        return 0;
    }

    sqlite3 *src_db = NULL;
    sqlite3 *dst_db = NULL;
    sqlite3_backup *backup = NULL;

    int rc = sqlite3_open(src_file, &src_db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    rc = sqlite3_open(dst_file, &dst_db);
    if (rc != SQLITE_OK) {
        safe_close(src_db);
        return 0;
    }

    /* Test sqlite3_backup_init */
    /* We use "main" for both source and destination names */
    backup = sqlite3_backup_init(dst_db, "main", src_db, "main");

    if (backup) {
        /* Test sqlite3_backup_step with varying nPage */
        int step_rc;
        int step_count = 0;
        int max_steps = 10; /* Limit steps to prevent infinite loops */

        while (step_count < max_steps) {
            step_rc = sqlite3_backup_step(backup, nPage);
            step_count++;

            if (step_rc == SQLITE_DONE) {
                break;
            }
            if (step_rc == SQLITE_BUSY || step_rc == SQLITE_LOCKED) {
                /* Retry a few times or break */
                if (step_count > 3) break;
                continue;
            }
            if (step_rc != SQLITE_OK) {
                break;
            }
        }

        /* Test sqlite3_backup_remaining and sqlite3_backup_pagecount */
        int remaining = sqlite3_backup_remaining(backup);
        int pagecount = sqlite3_backup_pagecount(backup);

        /* Use values to trigger different code paths if needed */
        (void)remaining;
        (void)pagecount;

        /* Test sqlite3_backup_finish */
        int finish_rc = sqlite3_backup_finish(backup);
        (void)finish_rc;
    } else {
        /* Test with NULL backup handle */
        int remaining = sqlite3_backup_remaining(NULL);
        int pagecount = sqlite3_backup_pagecount(NULL);
        int finish_rc = sqlite3_backup_finish(NULL);
        (void)remaining;
        (void)pagecount;
        (void)finish_rc;
    }

    /* Test sqlite3_deserialize */
    /* Create a simple serialized database buffer */
    unsigned char *buffer = NULL;
    sqlite3_int64 szDb = 0;
    sqlite3_int64 szBuf = 0;

    /* Try to get a serialized version of the source database */
    if (src_db) {
        buffer = (unsigned char *)sqlite3_serialize(src_db, "main", &szDb, 0);
        if (buffer && szDb > 0) {
            szBuf = szDb;
            /* Try to deserialize into destination */
            int deser_rc = sqlite3_deserialize(dst_db, "main", buffer, szDb, szBuf, flags);
            (void)deser_rc;

            /* Free the buffer if we allocated it (sqlite3_serialize allocates) */
            sqlite3_free(buffer);
        }
    }

    /* Cleanup */
    safe_close(src_db);
    safe_close(dst_db);

    /* Remove dummy files */
    remove(src_file);
    remove(dst_file);

    return 0;
}