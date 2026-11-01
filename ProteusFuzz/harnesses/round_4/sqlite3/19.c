// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *tail = NULL;
    char *expanded_sql = NULL;
    int rc;

    // Initialize database connection in memory
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Ensure we have enough data to create a valid SQL string
    if (Size < 2) {
        sqlite3_close(db);
        return 0;
    }

    // Create a safe SQL string from input data
    size_t sql_len = Size;
    char *sql_buf = (char *)sqlite3_malloc(sql_len + 100);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }

    // Construct a SQL statement. We try to make it valid but allow fuzzing the content.
    // We copy the input data into the buffer, ensuring it's null-terminated.
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    // Step 1: sqlite3_prepare_v2 (Required before sqlite3_step)
    rc = sqlite3_prepare_v2(db, sql_buf, (int)sql_len, &stmt1, &tail);
    if (rc != SQLITE_OK) {
        // If prepare fails, stmt1 is NULL.
    }

    // Step 2: sqlite3_step
    if (stmt1) {
        sqlite3_step(stmt1);
    }

    // Step 3: sqlite3_finalize
    if (stmt1) {
        sqlite3_finalize(stmt1);
        stmt1 = NULL;
    }

    // Step 4: sqlite3_finalize (Second call as per requirement)
    sqlite3_finalize(stmt1);

    // Step 5: sqlite3_prepare_v2 (Again to get a new statement for subsequent steps)
    rc = sqlite3_prepare_v2(db, sql_buf, (int)sql_len, &stmt2, &tail);
    if (rc != SQLITE_OK) {
        // If preparation fails, stmt2 is NULL.
    }

    // Step 6: sqlite3_expanded_sql
    if (stmt2) {
        expanded_sql = sqlite3_expanded_sql(stmt2);
    }

    // Step 7: sqlite3_free
    if (expanded_sql) {
        sqlite3_free(expanded_sql);
        expanded_sql = NULL;
    }

    // Step 8: sqlite3_bind_pointer
    if (stmt2) {
        void *dummy_ptr = (void *)Data;
        sqlite3_bind_pointer(stmt2, 1, dummy_ptr, "dummy_type", NULL);
    }

    // Step 9: sqlite3_step
    if (stmt2) {
        sqlite3_step(stmt2);
    }

    // Step 10: sqlite3_finalize
    if (stmt2) {
        sqlite3_finalize(stmt2);
        stmt2 = NULL;
    }

    // Step 11: sqlite3_finalize (Second call as per requirement)
    sqlite3_finalize(stmt2);

    // Cleanup
    sqlite3_free(sql_buf);
    sqlite3_close(db);

    return 0;
}