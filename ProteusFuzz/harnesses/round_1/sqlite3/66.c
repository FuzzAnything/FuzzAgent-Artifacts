// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <fcntl.h>

/* Static helper to create a dummy file if needed */
static int create_dummy_file(void) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT, 0644);
    if (fd < 0) return -1;
    /* Write some dummy data to ensure the file is not empty */
    const char *dummy = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
    write(fd, dummy, strlen(dummy));
    close(fd);
    return 0;
}

/* Static helper to cleanup the dummy file */
static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

/* Profile callback for sqlite3_profile */
static void dummy_profile_callback(void *pArg, const char *zSql, sqlite3_uint64 ns) {
    (void)pArg;
    (void)zSql;
    (void)ns;
    /* Do nothing */
}

/* Application-defined SQL function for testing sqlite3_context_db_handle */
static void dummy_sql_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Retrieve the database handle from the context to exercise the API */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    (void)db;
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    void *prev_profile_arg = NULL;
    const void *err_msg = NULL;
    int auto_commit = 0;
    sqlite3 *db_from_stmt = NULL;
    
    /* Ensure we have a file to work with */
    if (create_dummy_file() != 0) {
        return 0;
    }

    /* Initialize database connection */
    if (sqlite3_open("./dummy_file", &db) != SQLITE_OK) {
        cleanup_dummy_file();
        return 0;
    }

    /* 1. Test sqlite3_errmsg16 with a valid handle (should return NULL on success) */
    err_msg = sqlite3_errmsg16(db);
    (void)err_msg;

    /* 2. Test sqlite3_errmsg16 with NULL handle */
    err_msg = sqlite3_errmsg16(NULL);
    (void)err_msg;

    /* 3. Test sqlite3_get_autocommit with valid handle */
    auto_commit = sqlite3_get_autocommit(db);
    (void)auto_commit;

    /* 4. Test sqlite3_get_autocommit with NULL handle.
       The documentation says: "If the connection is invalid... returns 0".
       However, the internal implementation in some builds may dereference the pointer
       before checking for NULL if API armor is not fully enabled or in specific configurations.
       To prevent the ASAN crash observed, we skip calling with NULL if the implementation
       is known to be unsafe, or we rely on the fact that the fuzzing environment might
       have strict checks. Given the crash log, the function dereferences 'db' immediately.
       We will skip the NULL call to avoid the crash, as the behavior is undefined or 
       implementation-specific regarding safety checks on NULL input for this specific API 
       in this build.
    */
    /* auto_commit = sqlite3_get_autocommit(NULL); */ 

    /* 5. Test sqlite3_profile */
    prev_profile_arg = sqlite3_profile(db, dummy_profile_callback, (void *)0x1234);
    (void)prev_profile_arg;
    
    /* Disable profiling */
    prev_profile_arg = sqlite3_profile(db, NULL, NULL);
    (void)prev_profile_arg;

    /* 6. Prepare a statement to test sqlite3_db_handle and sqlite3_next_stmt */
    const char *sql = "SELECT 1;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        /* Test sqlite3_db_handle with valid statement */
        db_from_stmt = sqlite3_db_handle(stmt);
        (void)db_from_stmt;

        /* Test sqlite3_db_handle with NULL statement */
        db_from_stmt = sqlite3_db_handle(NULL);
        (void)db_from_stmt;

        /* Test sqlite3_next_stmt starting from NULL (first statement) */
        next_stmt = sqlite3_next_stmt(db, NULL);
        (void)next_stmt;

        /* Test sqlite3_next_stmt starting from current statement */
        next_stmt = sqlite3_next_stmt(db, stmt);
        (void)next_stmt;

        /* Test sqlite3_next_stmt with invalid db (NULL) - should handle safely */
        next_stmt = sqlite3_next_stmt(NULL, stmt);
        (void)next_stmt;

        sqlite3_finalize(stmt);
    }

    /* 7. Test sqlite3_context_db_handle */
    /* Register a function and call it via SQL to create a valid context */
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_sql_function, NULL, NULL);
    
    const char *sql_func = "SELECT dummy_func();";
    sqlite3_stmt *stmt_func = NULL;
    if (sqlite3_prepare_v2(db, sql_func, -1, &stmt_func, NULL) == SQLITE_OK) {
        /* Step to execute the function. This triggers dummy_sql_function 
           which calls sqlite3_context_db_handle internally. */
        sqlite3_step(stmt_func);
        sqlite3_finalize(stmt_func);
    }

    /* Cleanup */
    sqlite3_close(db);
    cleanup_dummy_file();

    return 0;
}