// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int is_open = 0;

    // 1. Prepare the environment: Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    is_open = 1;

    // Create a simple table to allow valid SQL execution
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, init_sql, NULL, NULL, NULL);

    // Interpret input data as SQL string
    // Ensure null termination for safety
    size_t sql_len = Size;
    if (sql_len > 0 && Data[sql_len - 1] == 0) {
        sql_len--;
    }
    
    char *sql_buf = NULL;
    if (sql_len > 0) {
        sql_buf = (char *)malloc(sql_len + 1);
        if (!sql_buf) {
            goto cleanup;
        }
        memcpy(sql_buf, Data, sql_len);
        sql_buf[sql_len] = '\0';
    } else {
        // Fallback to a safe SQL if input is empty
        sql_buf = (char *)malloc(100);
        if (!sql_buf) goto cleanup;
        strcpy(sql_buf, "SELECT 1;");
    }

    // 2. Prepare the statement
    rc = sqlite3_prepare_v2(db, sql_buf, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If preparation fails, we cannot safely call statement-specific APIs
        goto cleanup;
    }

    // 3. Explore program states by invoking target functions diversely
    
    // Fuzz sqlite3_stmt_isexplain
    int explain_mode = sqlite3_stmt_isexplain(stmt);
    (void)explain_mode;

    // Fuzz sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;

    // Fuzz sqlite3_step first to ensure the statement is in a valid state
    // and to potentially initialize internal counters used by stmt_status
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // If we have a row, step again or reset
        rc = sqlite3_step(stmt);
    }
    // Reset the statement to a clean state before querying status
    // This ensures counters are accessible and valid
    sqlite3_reset(stmt);

    // Fuzz sqlite3_stmt_status with various op codes and reset flags
    // We try a few common op codes to trigger different code paths
    // Only call this if stmt is valid (which it is here)
    int op_codes[] = {
        SQLITE_STMTSTATUS_FULLSCAN_STEP,
        SQLITE_STMTSTATUS_SORT,
        SQLITE_STMTSTATUS_AUTOINDEX,
        SQLITE_STMTSTATUS_MEMUSED,
        0
    };
    int num_op_codes = 5;
    
    for (int i = 0; i < num_op_codes; i++) {
        int reset_flags[] = {0, 1};
        for (int j = 0; j < 2; j++) {
            // Ensure stmt is not NULL before calling API
            if (stmt) {
                int status = sqlite3_stmt_status(stmt, op_codes[i], reset_flags[j]);
                (void)status;
            }
        }
    }

    // Fuzz sqlite3_step again to explore execution paths
    for (int i = 0; i < 5; i++) {
        if (!stmt) break; // Safety check
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            continue;
        } else if (rc == SQLITE_DONE) {
            // Reset to allow re-execution
            sqlite3_reset(stmt);
        } else {
            // Error or other, break loop to avoid infinite loops on errors
            break;
        }
    }

    // Fuzz sqlite3_stmt_explain
    // Try toggling explain modes (0, 1, 2)
    // Note: Must reset before changing explain mode
    if (stmt) {
        sqlite3_reset(stmt);
        int explain_modes[] = {0, 1, 2};
        int num_modes = 3;
        
        for (int i = 0; i < num_modes; i++) {
            rc = sqlite3_stmt_explain(stmt, explain_modes[i]);
            (void)rc;
            // Check new explain mode
            (void)sqlite3_stmt_isexplain(stmt);
        }
    }

    // Final cleanup of statement
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

cleanup:
    if (sql_buf) {
        free(sql_buf);
    }
    if (is_open && db) {
        sqlite3_close(db);
    }
    return 0;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    // Handle edge case of zero size
    if (Size == 0) {
        return 0;
    }
    
    return fuzz_sqlite3(Data, Size);
}