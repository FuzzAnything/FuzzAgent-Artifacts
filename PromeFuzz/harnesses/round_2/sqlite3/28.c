// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Create a simple table and insert some data using the fuzzer input
    const char *create_table = "CREATE TABLE IF NOT EXISTS t1 (id INTEGER, text TEXT);";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);

    // Prepare an INSERT statement with the fuzzer data
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO t1 (id, text) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer data as parameters
        sqlite3_bind_int(stmt, 1, (int)(Size % 100));
        sqlite3_bind_text(stmt, 2, (const char*)Data, (int)(Size % 1024), SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Prepare a SELECT statement for the subsequent API calls
    const char *select_sql = "SELECT * FROM t1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_realloc
    void *realloc_ptr = NULL;
    if (Size > 0) {
        // Allocate initial memory with first byte of data
        void *initial_ptr = sqlite3_malloc(1);
        if (initial_ptr) {
            *(char*)initial_ptr = (char)Data[0];
            // Reallocate to a size based on input
            size_t realloc_size = Size % 1024;
            if (realloc_size == 0) realloc_size = 1;
            realloc_ptr = sqlite3_realloc(initial_ptr, (int)realloc_size);
            if (realloc_ptr && realloc_size > 1) {
                // Copy some data into the reallocated buffer
                size_t copy_size = (Size < realloc_size) ? Size : realloc_size;
                memcpy(realloc_ptr, Data, copy_size);
            }
        }
    }

    // 2. sqlite3_step
    int step_result = sqlite3_step(stmt);

    // 3. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);

    // 4. sqlite3_mprintf
    char *mprintf_str = NULL;
    if (Size > 0) {
        // Use a format string and the fuzzer data as argument
        mprintf_str = sqlite3_mprintf("Data: %.*s", (int)(Size % 256), Data);
    } else {
        mprintf_str = sqlite3_mprintf("Empty input");
    }

    // 5. sqlite3_column_text
    const unsigned char *column_text = NULL;
    if (step_result == SQLITE_ROW && col_count > 0) {
        column_text = sqlite3_column_text(stmt, 0);
    }

    // 6. sqlite3_free
    if (mprintf_str) {
        sqlite3_free(mprintf_str);
    }

    // 7. sqlite3_reset
    int reset_result = sqlite3_reset(stmt);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    if (realloc_ptr) {
        sqlite3_free(realloc_ptr);
    }

    return 0;
}