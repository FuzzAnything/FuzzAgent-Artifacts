// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * The fuzz driver entry point.
 * Note: The function must be visible to the linker. 
 * If building as a shared library or specific fuzzer target, 
 * ensure the symbol is exported correctly. 
 * Standard AFL/LLVMFuzzer expects this exact signature.
 */
int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str = NULL;
    const char *zStr = NULL;
    const char *zPattern = NULL;
    unsigned int cEsc = 0;
    int rc;
    int iCol;
    const char *colName;
    int colType;
    int colCount;
    int strLikeRes;
    int strIcmpRes;
    char *finishedStr;

    if (Size < 2) {
        return 0;
    }

    // Initialize database connection
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table and insert data to ensure stmt is valid for column operations
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, name TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    rc = sqlite3_exec(db, "INSERT INTO test VALUES (1, 'hello');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement to get column info
    rc = sqlite3_prepare_v2(db, "SELECT id, name FROM test WHERE id = 1;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Execute the statement to position the cursor
    rc = sqlite3_step(stmt);

    // 1. sqlite3_strlike
    // We use fixed safe strings to avoid undefined behavior with non-null-terminated fuzz data
    // as sqlite3_strlike expects null-terminated strings.
    strLikeRes = sqlite3_strlike("a%", "abc", 0);

    // 2. sqlite3_column_count
    colCount = sqlite3_column_count(stmt);

    // 3. sqlite3_column_name (first call)
    iCol = (Size % 2); // 0 or 1
    colName = sqlite3_column_name(stmt, iCol);

    // 4. sqlite3_column_name (second call)
    iCol = ((Size + 1) % 2);
    colName = sqlite3_column_name(stmt, iCol);

    // 5. sqlite3_stricmp
    // Use fixed strings for safety
    strIcmpRes = sqlite3_stricmp("Hello", "hello");

    // 6. sqlite3_column_type
    iCol = (Size % 2);
    colType = sqlite3_column_type(stmt, iCol);

    // 7. sqlite3_str_appendf (first call)
    str = sqlite3_str_new(db);
    if (str) {
        int val = (Size > 0) ? Data[0] : 0;
        sqlite3_str_appendf(str, "Value: %d", val);
    }

    // 8. sqlite3_str_appendf (second call)
    if (str) {
        int val2 = (Size > 1) ? Data[1] : 0;
        sqlite3_str_appendf(str, " | Extra: %d", val2);
    }

    // 9. sqlite3_finalize
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    // 10. sqlite3_str_appendf (third call)
    if (str) {
        sqlite3_str_appendf(str, " | Done");
    }

    // 11. sqlite3_str_finish
    if (str) {
        finishedStr = sqlite3_str_finish(str);
        if (finishedStr) {
            sqlite3_free(finishedStr);
        }
    }

    // Cleanup database
    sqlite3_close(db);

    return 0;
}