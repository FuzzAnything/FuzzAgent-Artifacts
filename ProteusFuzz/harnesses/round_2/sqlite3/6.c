// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API sequence.
 * Invokes: sqlite3_config -> sqlite3_open -> sqlite3_errmsg -> sqlite3_exec -> sqlite3_free
 *          -> sqlite3_db_status (x14) -> sqlite3_close -> sqlite3_status (x5)
 *          -> sqlite3_mprintf -> sqlite3_free -> sqlite3_mprintf -> sqlite3_free
 *          -> sqlite3_open -> sqlite3_errmsg -> sqlite3_exec (x3) -> sqlite3_free -> sqlite3_close
 */

static int callback(void *NotUsed, int argc, char **argv, char **col_name) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;
    int cur, hiwtr;
    char *fmt_str = NULL;

    /* 1. sqlite3_config */
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0);
    sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);

    /* 2. sqlite3_open */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
        rc = sqlite3_open("./dummy_file", &db);
    }

    if (!db) {
        return 0;
    }

    /* 3. sqlite3_errmsg */
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    /* 4. sqlite3_exec */
    if (Size > 0) {
        char *sql_buf = (char *)malloc(Size + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, Size);
            sql_buf[Size] = '\0';
            rc = sqlite3_exec(db, sql_buf, callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql_buf);
        }
    } else {
        rc = sqlite3_exec(db, "SELECT 1;", callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }

    /* 5. sqlite3_free */
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* 6-19. sqlite3_db_status (14 times) */
    int db_status_opts[] = {
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
        SQLITE_DBSTATUS_DEFERRED_FKS,
        SQLITE_DBSTATUS_CACHE_USED_SHARED,
        SQLITE_DBSTATUS_LOOKASIDE_USED
    };

    for (int i = 0; i < 14; i++) {
        int opt = db_status_opts[i % 13];
        sqlite3_db_status(db, opt, &cur, &hiwtr, 0);
        sqlite3_db_status(db, opt, &cur, &hiwtr, 1);
    }

    /* 20. sqlite3_close */
    rc = sqlite3_close(db);
    db = NULL;

    /* 21-25. sqlite3_status (5 times) */
    int status_opts[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_MALLOC_SIZE,
        SQLITE_STATUS_PARSER_STACK
    };

    for (int i = 0; i < 5; i++) {
        int opt = status_opts[i];
        sqlite3_status(opt, &cur, &hiwtr, 0);
    }

    /* 26. sqlite3_mprintf */
    if (Size > 0) {
        /* Allocate extra space to safely handle potential doubling of '%' characters */
        size_t max_size = Size * 2 + 1;
        char *safe_input = (char *)malloc(max_size);
        if (safe_input) {
            size_t write_idx = 0;
            for (size_t j = 0; j < Size; j++) {
                if (Data[j] == '%') {
                    safe_input[write_idx++] = '%';
                    safe_input[write_idx++] = '%';
                } else {
                    safe_input[write_idx++] = Data[j];
                }
            }
            safe_input[write_idx] = '\0';

            fmt_str = sqlite3_mprintf("Test: %s", safe_input);
            free(safe_input);
        }
    } else {
        fmt_str = sqlite3_mprintf("Test: Empty Input");
    }

    /* 27. sqlite3_free */
    if (fmt_str) {
        sqlite3_free(fmt_str);
        fmt_str = NULL;
    }

    /* 28. sqlite3_mprintf */
    fmt_str = sqlite3_mprintf("Second Test");

    /* 29. sqlite3_free */
    if (fmt_str) {
        sqlite3_free(fmt_str);
        fmt_str = NULL;
    }

    /* 30. sqlite3_open */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK && db) {
        sqlite3_close(db);
        db = NULL;
    }

    if (!db) {
        return 0;
    }

    /* 31. sqlite3_errmsg */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    /* 32-34. sqlite3_exec (3 times) */
    for (int i = 0; i < 3; i++) {
        const char *sql = (i == 0) ? "SELECT 1;" : (i == 1) ? "CREATE TABLE IF NOT EXISTS test (id INT);" : "INSERT INTO test VALUES (1);";
        rc = sqlite3_exec(db, sql, callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }

    /* 35. sqlite3_free */
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* 36. sqlite3_close */
    rc = sqlite3_close(db);
    db = NULL;

    return 0;
}