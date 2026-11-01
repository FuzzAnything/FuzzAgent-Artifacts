// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:92807:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
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

static void custom_func_nomem(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_error_nomem(ctx);
}

static void custom_func_null(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

static void custom_func_toobig(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_error_toobig(ctx);
}

static void custom_func_value(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0 && argv[0] != NULL) {
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

static void custom_func_subtype(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    unsigned int subtype = 0;
    if (argc > 0) {
        subtype = (unsigned int)sqlite3_value_int(argv[0]);
    }
    sqlite3_result_subtype(ctx, subtype);
    sqlite3_result_null(ctx);
}

static void custom_func_userdata(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    void *user_data = sqlite3_user_data(ctx);
    if (user_data != NULL) {
        sqlite3_result_int64(ctx, (sqlite3_int64)(intptr_t)user_data);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Register custom functions
    rc = sqlite3_create_function(db, "fuzz_nomem", 0, SQLITE_UTF8, NULL, custom_func_nomem, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_null", 0, SQLITE_UTF8, NULL, custom_func_null, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_toobig", 0, SQLITE_UTF8, NULL, custom_func_toobig, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_value", 1, SQLITE_UTF8, NULL, custom_func_value, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_subtype", 1, SQLITE_UTF8, NULL, custom_func_subtype, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Use the input data as user data for the userdata function
    void *user_data_ptr = (void *)(intptr_t)(Data[0] & 0xFF);
    rc = sqlite3_create_function(db, "fuzz_userdata", 0, SQLITE_UTF8, user_data_ptr, custom_func_userdata, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a dummy table to ensure the database is in a valid state
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare and execute various SQL statements to trigger the custom functions
    const char *sql_queries[] = {
        "SELECT fuzz_nomem();",
        "SELECT fuzz_null();",
        "SELECT fuzz_toobig();",
        "SELECT fuzz_value('test');",
        "SELECT fuzz_subtype(1);",
        "SELECT fuzz_userdata();",
        "INSERT INTO fuzz_test (data) VALUES ('test_data');",
        "SELECT * FROM fuzz_test;",
        "UPDATE fuzz_test SET data = 'updated' WHERE id = 1;",
        "DELETE FROM fuzz_test WHERE id = 1;"
    };

    int num_queries = sizeof(sql_queries) / sizeof(sql_queries[0]);
    for (int i = 0; i < num_queries; i++) {
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql_queries[i], -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Consume the result
                int col_count = sqlite3_column_count(stmt);
                for (int j = 0; j < col_count; j++) {
                    sqlite3_value *val = sqlite3_column_value(stmt, j);
                    if (val != NULL) {
                        (void)sqlite3_value_type(val);
                    }
                }
            }
            sqlite3_finalize(stmt);
        }
    }

    // Close the database connection
    sqlite3_close(db);

    return 0;
}