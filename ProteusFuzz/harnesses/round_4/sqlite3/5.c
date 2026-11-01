// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Callback function for sqlite3_exec.
 * Returns 0 to continue, non-zero to abort.
 */
static int exec_callback(void *arg, int argc, char **argv, char **colv) {
    (void)arg;
    (void)argc;
    (void)argv;
    (void)colv;
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;
    int status_current, status_highwater;
    int db_status_cur, db_status_hiwtr;
    char *mprintf_result = NULL;

    /* 1. sqlite3_config */
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);

    /* 2. sqlite3_open */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If open fails, we can't proceed with most DB operations, but we can still test errmsg */
        sqlite3_errmsg(db);
        sqlite3_close(db);
        return 0;
    }

    /* 3. sqlite3_errmsg */
    const char *err = sqlite3_errmsg(db);
    (void)err; /* Suppress unused warning */

    /* 4. sqlite3_exec */
    if (Size > 0) {
        char *sql_buf = (char *)malloc(Size + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, Size);
            sql_buf[Size] = '\0';
            sqlite3_exec(db, sql_buf, exec_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql_buf);
        }
    }

    /* 5. sqlite3_free */
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* 6-16. sqlite3_db_status (11 times) */
    /* Using valid SQLITE_DBSTATUS options. Removed undefined ones. */
    int db_options[] = {
        SQLITE_DBSTATUS_LOOKASIDE_USED,
        SQLITE_DBSTATUS_CACHE_USED,
        SQLITE_DBSTATUS_SCHEMA_USED,
        SQLITE_DBSTATUS_STMT_USED,
        SQLITE_DBSTATUS_LOOKASIDE_HIT,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
        SQLITE_DBSTATUS_CACHE_HIT,
        SQLITE_DBSTATUS_CACHE_MISS,
        SQLITE_DBSTATUS_CACHE_WRITE,
        SQLITE_DBSTATUS_DEFERRED_FKS
    };
    for (int i = 0; i < 11; i++) {
        if (db) {
            sqlite3_db_status(db, db_options[i % 11], &db_status_cur, &db_status_hiwtr, 0);
            sqlite3_db_status(db, db_options[i % 11], &db_status_cur, &db_status_hiwtr, 1);
        }
    }

    /* 17. sqlite3_close */
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }

    /* 18-22. sqlite3_status (5 times) */
    int global_options[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_MALLOC_SIZE,
        SQLITE_STATUS_PARSER_STACK
    };
    for (int i = 0; i < 5; i++) {
        sqlite3_status(global_options[i % 5], &status_current, &status_highwater, 0);
        sqlite3_status(global_options[i % 5], &status_current, &status_highwater, 1);
    }

    /* 23. sqlite3_mprintf */
    if (Size > 0) {
        char safe_format[] = "%s";
        char *safe_arg = (char *)malloc(Size + 1);
        if (safe_arg) {
            memcpy(safe_arg, Data, Size);
            safe_arg[Size] = '\0';
            mprintf_result = sqlite3_mprintf(safe_format, safe_arg);
            free(safe_arg);
        }
    } else {
        mprintf_result = sqlite3_mprintf("test");
    }

    /* 24. sqlite3_free */
    if (mprintf_result) {
        sqlite3_free(mprintf_result);
        mprintf_result = NULL;
    }

    /* 25. sqlite3_mprintf */
    mprintf_result = sqlite3_mprintf("another_test_%d", 12345);

    /* 26. sqlite3_free */
    if (mprintf_result) {
        sqlite3_free(mprintf_result);
        mprintf_result = NULL;
    }

    /* 27. sqlite3_open (second time) */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_errmsg(db);
        sqlite3_close(db);
        return 0;
    }

    /* 28. sqlite3_errmsg */
    err = sqlite3_errmsg(db);
    (void)err;

    /* 29-31. sqlite3_exec (3 times) */
    for (int i = 0; i < 3; i++) {
        if (Size > 0) {
            char *sql_buf = (char *)malloc(Size + 1);
            if (sql_buf) {
                memcpy(sql_buf, Data, Size);
                sql_buf[Size] = '\0';
                sqlite3_exec(db, sql_buf, exec_callback, NULL, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
                free(sql_buf);
            }
        } else {
            sqlite3_exec(db, "SELECT 1;", exec_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }
    }

    /* 32. sqlite3_free */
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* 33. sqlite3_close */
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }

    return 0;
}