// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int simple_callback(void *data, int argc, char **argv, char **colnames) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)colnames;
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int cur, hiwtr;
    int rc;

    // 1. sqlite3_config
    int config_op = (Size > 0) ? (Data[0] % 3) : 0;
    if (config_op == 0) rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    else if (config_op == 1) rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    else rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    (void)rc;

    // 2. sqlite3_open
    const char *filename = (Size > 0 && Data[0] == ':') ? ":memory:" : "./dummy_file";
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        // db might be non-NULL even on error
    }

    // 3. sqlite3_errmsg
    const char *msg = sqlite3_errmsg(db);
    (void)msg;

    // 4. sqlite3_exec
    size_t sql_len = (Size > 1000) ? 1000 : Size;
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (sql_buf) {
        memcpy(sql_buf, Data, sql_len);
        sql_buf[sql_len] = '\0';
        rc = sqlite3_exec(db, sql_buf, simple_callback, NULL, &errmsg);
        if (errmsg != NULL) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        free(sql_buf);
    }

    // 5. sqlite3_free
    sqlite3_free(errmsg);

    // 6-15. sqlite3_db_status (10 times)
    for (int i = 0; i < 10; i++) {
        int op = (Size > (size_t)(i + 1)) ? (Data[i + 1] % 5) : 0;
        rc = sqlite3_db_status(db, op, &cur, &hiwtr, 0);
        (void)rc;
    }

    // 16. sqlite3_close
    rc = sqlite3_close(db);
    db = NULL;

    // 17-21. sqlite3_status (5 times)
    for (int i = 0; i < 5; i++) {
        int op = (Size > (size_t)(i + 11)) ? (Data[i + 11] % 5) : 0;
        rc = sqlite3_status(op, &cur, &hiwtr, 0);
        (void)rc;
    }

    // 22. sqlite3_mprintf
    char *fmt_str = NULL;
    if (Size > 0) {
        fmt_str = sqlite3_mprintf("Test: %s", (Size > 10 && Data[0] != '%') ? "safe" : "input");
    } else {
        fmt_str = sqlite3_mprintf("Test: empty");
    }

    // 23. sqlite3_free
    sqlite3_free(fmt_str);

    // 24. sqlite3_mprintf
    fmt_str = sqlite3_mprintf("Second: %d", (Size > 0) ? Data[0] : 0);

    // 25. sqlite3_free
    sqlite3_free(fmt_str);

    // 26. sqlite3_open (second time)
    const char *filename2 = (Size > 1 && Data[1] == ':') ? ":memory:" : "./dummy_file";
    rc = sqlite3_open(filename2, &db);
    if (rc != SQLITE_OK) {
        // db might still be valid for errmsg
    }

    // 27. sqlite3_errmsg
    msg = sqlite3_errmsg(db);
    (void)msg;

    // 28. sqlite3_exec
    if (Size > 0) {
        size_t sql_len2 = (Size > 500) ? 500 : Size;
        char *sql_buf2 = (char *)malloc(sql_len2 + 1);
        if (sql_buf2) {
            memcpy(sql_buf2, Data, sql_len2);
            sql_buf2[sql_len2] = '\0';
            rc = sqlite3_exec(db, sql_buf2, simple_callback, NULL, &errmsg);
            if (errmsg != NULL) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql_buf2);
        }
    }

    // 29. sqlite3_exec
    if (Size > 1) {
        size_t sql_len3 = (Size > 500) ? 500 : (Size - 1);
        char *sql_buf3 = (char *)malloc(sql_len3 + 1);
        if (sql_buf3) {
            memcpy(sql_buf3, Data + 1, sql_len3);
            sql_buf3[sql_len3] = '\0';
            rc = sqlite3_exec(db, sql_buf3, simple_callback, NULL, &errmsg);
            if (errmsg != NULL) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql_buf3);
        }
    }

    // 30. sqlite3_exec
    if (Size > 2) {
        size_t sql_len4 = (Size > 500) ? 500 : (Size - 2);
        char *sql_buf4 = (char *)malloc(sql_len4 + 1);
        if (sql_buf4) {
            memcpy(sql_buf4, Data + 2, sql_len4);
            sql_buf4[sql_len4] = '\0';
            rc = sqlite3_exec(db, sql_buf4, simple_callback, NULL, &errmsg);
            if (errmsg != NULL) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql_buf4);
        }
    }

    // 31. sqlite3_free
    sqlite3_free(errmsg);

    // 32. sqlite3_close
    if (db) {
        rc = sqlite3_close(db);
        (void)rc;
    }

    return 0;
}