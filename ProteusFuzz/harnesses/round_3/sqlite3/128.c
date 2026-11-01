// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to allow statement preparation
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a safe default statement first to ensure we have a valid stmt
    // We will use the input data to modify the SQL or as a fallback if we can safely construct one
    const char *safe_sql = "SELECT 1;";
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Try to prepare a statement using the input data as SQL, but ensure it's null-terminated
    if (Size > 0 && Size < 10000) {
        // Create a buffer to hold the SQL string with a guaranteed null terminator
        char *safe_input = (char *)malloc(Size + 1);
        if (safe_input) {
            memcpy(safe_input, Data, Size);
            safe_input[Size] = '\0'; // Ensure null termination

            sqlite3_stmt *stmt_input = NULL;
            rc = sqlite3_prepare_v2(db, safe_input, Size, &stmt_input, NULL);
            
            if (rc == SQLITE_OK) {
                // If input SQL is valid, use it for fuzzing
                sqlite3_finalize(stmt);
                stmt = stmt_input;
            } else {
                // If invalid, keep the safe statement and ignore the input SQL
                sqlite3_finalize(stmt_input);
            }
            free(safe_input);
        }
    }

    if (stmt) {
        // Fuzz sqlite3_step
        int step_result = sqlite3_step(stmt);
        
        // Fuzz sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        (void)col_count;

        // Fuzz sqlite3_stmt_status with various opcodes
        int status_vals[] = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9
        };
        
        for (int i = 0; i < 10; i++) {
            int val = sqlite3_stmt_status(stmt, status_vals[i], 0);
            (void)val;
            int val_reset = sqlite3_stmt_status(stmt, status_vals[i], 1);
            (void)val_reset;
        }

        // Fuzz sqlite3_stmt_isexplain
        int is_explain = sqlite3_stmt_isexplain(stmt);
        (void)is_explain;

        // Fuzz sqlite3_stmt_explain
        sqlite3_reset(stmt);
        
        int explain_modes[] = {0, 1, 2};
        for (int i = 0; i < 3; i++) {
            rc = sqlite3_stmt_explain(stmt, explain_modes[i]);
            if (rc == SQLITE_OK) {
                int current_mode = sqlite3_stmt_isexplain(stmt);
                (void)current_mode;
                
                step_result = sqlite3_step(stmt);
                while (step_result == SQLITE_ROW) {
                    step_result = sqlite3_step(stmt);
                }
                sqlite3_reset(stmt);
            }
        }

        // Final reset and cleanup
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    sqlite3_close(db);
    return 0;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_sqlite3(Data, Size);
    
    return 0;
}