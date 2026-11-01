// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
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
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Callback function for sqlite3_exec.
 * Returns 0 to continue execution.
 */
static int exec_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int cur, hiwtr;
    int rc;

    if (Size == 0) {
        return 0;
    }

    /* 1. sqlite3_config */
    /* Use data to fuzz the option code, though many are invalid. 
       We try SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, etc.
       Since config is global, we are careful not to break subsequent calls.
       We pass a dummy option derived from input to trigger different paths.
    */
    int config_opt = (Size > 0) ? (Data[0] & 0xFF) : 0;
    /* Only call config with known safe options or let it fail gracefully.
       SQLITE_CONFIG_LOG is an "anytime" option, but we don't have a callback.
       We'll just call it with a potentially invalid option to test error handling.
    */
    sqlite3_config(config_opt);

    /* 2. sqlite3_open */
    /* Use "./dummy_file" as requested */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        /* Check error message even if open fails, though db might be NULL or valid */
        if (db) {
            sqlite3_errmsg(db);
            sqlite3_close(db);
            db = NULL;
        }
        return 0;
    }

    /* 3. sqlite3_errmsg */
    const char *err = sqlite3_errmsg(db);
    (void)err; /* Suppress unused warning */

    /* 4. sqlite3_exec */
    /* Construct a SQL string from the fuzz input. 
       Ensure it is null-terminated and safe-ish (though exec handles errors).
       We limit the length to avoid huge allocations.
    */
    size_t sql_len = (Size > 1024) ? 1024 : Size;
    char *sql = (char *)malloc(sql_len + 1);
    if (sql) {
        memcpy(sql, Data, sql_len);
        sql[sql_len] = '\0';
        
        /* Execute the SQL */
        rc = sqlite3_exec(db, sql, exec_callback, NULL, &errmsg);
        
        /* 5. sqlite3_free (for errmsg) */
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        free(sql);
    }

    /* 6-16. sqlite3_db_status (11 times) */
    /* Fuzz the operation code using different bytes from input */
    for (int i = 0; i < 11; i++) {
        int op = (Size > i + 1) ? (Data[i + 1] & 0xFF) : 0;
        sqlite3_db_status(db, op, &cur, &hiwtr, 0);
        sqlite3_db_status(db, op, &cur, &hiwtr, 1); /* Try reset flag */
    }

    /* 17. sqlite3_close */
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }

    /* 18-22. sqlite3_status (5 times) */
    for (int i = 0; i < 5; i++) {
        int op = (Size > i + 2) ? (Data[i + 2] & 0xFF) : 0;
        sqlite3_status(op, &cur, &hiwtr, 0);
        sqlite3_status(op, &cur, &hiwtr, 1);
    }

    /* 23. sqlite3_mprintf */
    /* Use input as format string if possible, but be careful. 
       We'll use a safe format string and pass input as argument if it looks like a string.
       Or just use a fixed format to test allocation.
    */
    char *fmt = (Size > 0) ? "%" : "%s";
    char *p1 = sqlite3_mprintf(fmt);
    if (p1) {
        /* 24. sqlite3_free */
        sqlite3_free(p1);
    }

    /* 25. sqlite3_mprintf */
    char *p2 = sqlite3_mprintf("test");
    if (p2) {
        /* 26. sqlite3_free */
        sqlite3_free(p2);
    }

    /* 27. sqlite3_open (again) */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_errmsg(db);
            sqlite3_close(db);
            db = NULL;
        }
        return 0;
    }

    /* 28. sqlite3_errmsg */
    err = sqlite3_errmsg(db);
    (void)err;

    /* 29. sqlite3_exec */
    if (sql_len > 0) {
        /* Reuse the sql buffer logic or create a new one */
        char *sql2 = (char *)malloc(sql_len + 1);
        if (sql2) {
            memcpy(sql2, Data, sql_len);
            sql2[sql_len] = '\0';
            rc = sqlite3_exec(db, sql2, exec_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql2);
        }
    }

    /* 30. sqlite3_exec */
    if (sql_len > 0) {
        char *sql3 = (char *)malloc(sql_len + 1);
        if (sql3) {
            memcpy(sql3, Data, sql_len);
            sql3[sql_len] = '\0';
            rc = sqlite3_exec(db, sql3, exec_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql3);
        }
    }

    /* 31. sqlite3_exec */
    if (sql_len > 0) {
        char *sql4 = (char *)malloc(sql_len + 1);
        if (sql4) {
            memcpy(sql4, Data, sql_len);
            sql4[sql_len] = '\0';
            rc = sqlite3_exec(db, sql4, exec_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql4);
        }
    }

    /* 32. sqlite3_free (cleanup any remaining errmsg if somehow set, though loop handles it) */
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