// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *not_used, int argc, char **argv, char **azColName) {
    (void)not_used;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

static void custom_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    if (argc < 1) return;

    sqlite3_value *val = argv[0];
    
    // Fuzz sqlite3_value_type
    int type = sqlite3_value_type(val);
    (void)type;

    // Fuzz sqlite3_value_frombind
    int from_bind = sqlite3_value_frombind(val);
    (void)from_bind;

    // Fuzz sqlite3_value_bytes16
    int bytes16 = sqlite3_value_bytes16(val);
    (void)bytes16;

    // Fuzz sqlite3_value_nochange
    int nochange = sqlite3_value_nochange(val);
    (void)nochange;

    // Fuzz sqlite3_value_int
    int int_val = sqlite3_value_int(val);
    (void)int_val;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a dummy table to allow binding and execution
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, val TEXT);";
    rc = sqlite3_exec(db, create_sql, callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with bound parameters to generate sqlite3_value objects
    const char *insert_sql = "INSERT INTO test (id, val) VALUES (?, ?);";
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Fuzzing logic: Bind data from input to the statement
    int bind_count = 2;
    for (int i = 0; i < bind_count; i++) {
        if (Size > 0) {
            int32_t int_val = 0;
            if (Size >= 4) {
                memcpy(&int_val, Data, 4);
            }
            sqlite3_bind_int(stmt, i + 1, int_val);
            
            if (Size > 4) {
                size_t text_len = Size - 4;
                if (text_len > 100) text_len = 100;
                sqlite3_bind_text(stmt, i + 1, (const char *)(Data + 4), text_len, SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_text(stmt, i + 1, "", 0, SQLITE_TRANSIENT);
            }
        } else {
            sqlite3_bind_null(stmt, i + 1);
        }
    }

    rc = sqlite3_step(stmt);
    (void)rc;

    // Register custom function to exercise value APIs
    sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, custom_func, NULL, NULL);

    // Execute a query that uses the custom function with bound parameters
    const char *select_sql = "SELECT fuzz_func(?), fuzz_func(?), fuzz_func(?);";
    sqlite3_stmt *stmt2 = NULL;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, NULL);
    if (rc == SQLITE_OK) {
        for (int i = 0; i < 3; i++) {
            if (Size > 0) {
                int32_t val = 0;
                if (Size >= 4) memcpy(&val, Data + (i * 4), 4);
                sqlite3_bind_int(stmt2, i + 1, val);
            } else {
                sqlite3_bind_null(stmt2, i + 1);
            }
        }
        
        rc = sqlite3_step(stmt2);
        (void)rc;
    }

    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}