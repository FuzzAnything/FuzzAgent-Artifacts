// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static int fuzz_sqlite3_step(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to ensure valid schema for testing */
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, init_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement using fuzzed input as SQL */
    /* Ensure the input is null-terminated for sqlite3_prepare_v2 */
    if (Size > 0 && Data[Size - 1] != '\0') {
        /* We cannot easily append a null terminator to const uint8_t*, 
           so we limit the length to Size-1 if the last byte is not null,
           or just pass Size and hope the API handles it, but standard
           requires null termination. We will use a safe substring approach
           or just pass the buffer if it's small enough, but strictly speaking
           we need a null terminator.
           
           Better approach: Use the data as is, but sqlite3_prepare_v2 expects
           a null-terminated string. If the fuzzer provides binary data without
           a null terminator, we might get a crash or undefined behavior.
           To be safe, we will only pass the data up to the first null byte
           or the full size if we assume the fuzzer might provide valid strings.
           However, for fuzzing, we want to test invalid inputs too.
           If we pass a non-null-terminated string to sqlite3_prepare_v2,
           it will read past the buffer. This is a bug in the driver if we do that.
           
           Correct approach for fuzzing:
           1. If the data contains a null byte, use the length up to that byte.
           2. If not, we cannot safely pass it to sqlite3_prepare_v2 without
              allocating a new buffer with a null terminator.
           
           Let's allocate a buffer with a null terminator to be safe and
              allow testing of the full buffer content as SQL text.
        */
        char *safe_sql = malloc(Size + 1);
        if (!safe_sql) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(safe_sql, Data, Size);
        safe_sql[Size] = '\0';
        
        rc = sqlite3_prepare_v2(db, safe_sql, Size, &stmt, NULL);
        free(safe_sql);
    } else {
        /* If the last byte is null, we can pass directly, but Size includes the null.
           sqlite3_prepare_v2 takes the length. If the string is null-terminated
           at Size-1, passing Size is fine as long as the buffer has a null at Size-1.
           Actually, if Data[Size-1] is '\0', then the string length is Size-1.
           But sqlite3_prepare_v2 takes the byte length. It will stop at the first null.
           So passing Size is safe if the buffer is valid up to Size.
        */
        rc = sqlite3_prepare_v2(db, (const char *)Data, Size, &stmt, NULL);
    }

    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Fuzz sqlite3_step */
    int step_result = sqlite3_step(stmt);
    
    /* Fuzz sqlite3_column_count */
    int col_count = sqlite3_column_count(stmt);
    (void)col_count; /* Use the value to avoid unused variable warning */

    /* Fuzz sqlite3_stmt_isexplain */
    int is_explain = sqlite3_stmt_isexplain(stmt);
    (void)is_explain;

    /* Fuzz sqlite3_stmt_status */
    int status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 0);
    (void)status;
    /* Try resetting the status */
    status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
    (void)status;

    /* Fuzz sqlite3_reset */
    int reset_result = sqlite3_reset(stmt);
    (void)reset_result;

    /* Fuzz sqlite3_stmt_explain with different modes */
    /* Mode 0: Normal */
    int explain_result = sqlite3_stmt_explain(stmt, 0);
    (void)explain_result;
    
    /* Mode 1: EXPLAIN */
    explain_result = sqlite3_stmt_explain(stmt, 1);
    (void)explain_result;
    
    /* Mode 2: EXPLAIN QUERY PLAN */
    explain_result = sqlite3_stmt_explain(stmt, 2);
    (void)explain_result;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_sqlite3_step(Data, Size);

    return 0;
}