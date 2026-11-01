// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return -1;
    }
    // Create a simple table to ensure we have something to query
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(*db, sql, NULL, NULL, NULL);
    return 0;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    // Initialize database
    if (create_db(&db) != 0) {
        return 0;
    }

    // Use input data to generate a SQL query
    // We ensure the SQL is valid by using a simple template and sanitizing the input
    // or by limiting the input to a safe subset. Here we use a fixed query structure
    // and bind the data as parameters to avoid SQL injection issues in the fuzzer logic itself.
    // The fuzzer's job is to test the API behavior with various bindings and states.
    
    const char *sql_template = "SELECT * FROM test_table WHERE id = ?1;";
    
    // Prepare the first statement
    rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare the second statement for transfer_bindings test
    rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }

    // Bind some data from the input to stmt1
    // We use the first byte to determine the type of binding to test diversity
    int bind_type = Data[0] % 4;
    int bind_len = (Size > 1) ? (Data[1] % 100) : 0;
    
    if (bind_type == 0) {
        // Bind integer
        int val = (Size > 2) ? (Data[2] * 256 + (Size > 3 ? Data[3] : 0)) : 0;
        sqlite3_bind_int(stmt1, 1, val);
    } else if (bind_type == 1) {
        // Bind text
        if (bind_len > 0 && Size > bind_len + 2) {
            sqlite3_bind_text(stmt1, 1, (const char*)&Data[2], bind_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_text(stmt1, 1, "fuzz_text", -1, SQLITE_TRANSIENT);
        }
    } else if (bind_type == 2) {
        // Bind blob
        if (bind_len > 0 && Size > bind_len + 2) {
            sqlite3_bind_blob(stmt1, 1, &Data[2], bind_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_blob(stmt1, 1, "blob", 4, SQLITE_TRANSIENT);
        }
    } else {
        // Bind null
        sqlite3_bind_null(stmt1, 1);
    }

    // Test sqlite3_step
    // Loop to get multiple rows or until done/error
    int step_rc;
    do {
        step_rc = sqlite3_step(stmt1);
        // No specific action on rows, just stepping to trigger internal logic
    } while (step_rc == SQLITE_ROW);

    // Test sqlite3_reset
    sqlite3_reset(stmt1);

    // Test sqlite3_clear_bindings
    sqlite3_clear_bindings(stmt1);

    // Test sqlite3_transfer_bindings
    // Re-bind to stmt1 to have something to transfer
    if (bind_type == 0) {
        int val = (Size > 2) ? (Data[2] * 256 + (Size > 3 ? Data[3] : 0)) : 0;
        sqlite3_bind_int(stmt1, 1, val);
    } else if (bind_type == 1) {
        if (bind_len > 0 && Size > bind_len + 2) {
            sqlite3_bind_text(stmt1, 1, (const char*)&Data[2], bind_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_text(stmt1, 1, "fuzz_text", -1, SQLITE_TRANSIENT);
        }
    } else if (bind_type == 2) {
        if (bind_len > 0 && Size > bind_len + 2) {
            sqlite3_bind_blob(stmt1, 1, &Data[2], bind_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_blob(stmt1, 1, "blob", 4, SQLITE_TRANSIENT);
        }
    } else {
        sqlite3_bind_null(stmt1, 1);
    }

    // Reset stmt2 before transfer
    sqlite3_reset(stmt2);
    sqlite3_clear_bindings(stmt2);

    // Attempt transfer
    sqlite3_transfer_bindings(stmt1, stmt2);

    // Test sqlite3_stmt_explain
    // Modes: 0 (normal), 1 (EXPLAIN), 2 (EXPLAIN QUERY PLAN)
    int explain_mode = (Size > 4) ? (Data[4] % 3) : 0;
    sqlite3_reset(stmt1);
    sqlite3_stmt_explain(stmt1, explain_mode);

    // Test sqlite3_stmt_readonly
    sqlite3_stmt_readonly(stmt1);
    sqlite3_stmt_readonly(stmt2);

    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);

    return 0;
}