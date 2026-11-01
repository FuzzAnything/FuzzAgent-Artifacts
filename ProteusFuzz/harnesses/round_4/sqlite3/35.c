// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* Callback for sqlite3_exec */
static int exec_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

/* Scalar function for sqlite3_create_function */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

/* Busy handler for sqlite3_busy_handler */
static int busy_handler(void *arg, int count) {
    (void)arg;
    (void)count;
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;
    const char *filename = "./dummy_file";

    if (Size == 0) {
        return 0;
    }

    /* Create a safe SQL string from the fuzz input */
    char *safe_sql = (char *)malloc(Size + 1);
    if (!safe_sql) {
        return 0;
    }
    memcpy(safe_sql, Data, Size);
    safe_sql[Size] = '\0';

    /* 1. sqlite3_open_v2 */
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        free(safe_sql);
        return 0;
    }

    /* 2. sqlite3_close */
    rc = sqlite3_close(db);
    /* After close, db pointer is invalid for further operations on this handle.
       The prompt requires the order: open -> close -> create -> busy -> exec.
       To avoid segfaults on a closed handle (which is NULL or invalid),
       we must re-open the database to satisfy the subsequent calls safely,
       while maintaining the logical sequence of API invocations requested.
       We re-open to a valid state to allow the subsequent functions to execute without crashing.
    */
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        free(safe_sql);
        return 0;
    }

    /* 3. sqlite3_create_function */
    rc = sqlite3_create_function(
        db, 
        "fuzz_func", 
        0, 
        SQLITE_UTF8, 
        NULL, 
        scalar_func, 
        NULL, 
        NULL
    );

    /* 4. sqlite3_busy_handler */
    rc = sqlite3_busy_handler(db, busy_handler, NULL);

    /* 5. sqlite3_exec */
    rc = sqlite3_exec(db, safe_sql, exec_callback, NULL, &errmsg);

    /* Cleanup */
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    sqlite3_close(db);
    free(safe_sql);

    return 0;
}