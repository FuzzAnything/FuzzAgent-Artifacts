// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_busy_handler(void *ptr, int count) { (void)ptr; (void)count; }
static int dummy_progress_handler(void *ptr) { (void)ptr; return 0; }

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_value *value = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;

    /* Prepare two statements with placeholders */
    const char *sql1 = "SELECT ?, ?, ?";
    const char *sql2 = "SELECT ?, ?, ?";
    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    /* Create a dummy sqlite3_value using a separate statement */
    sqlite3_stmt *dummy_stmt = NULL;
    const char *dummy_sql = "SELECT ?";
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &dummy_stmt, NULL);
    if (rc == SQLITE_OK && dummy_stmt) {
        /* Bind some data to the dummy statement */
        if (Size > 0) {
            sqlite3_bind_blob(dummy_stmt, 1, Data, (int)(Size > 100 ? 100 : Size), SQLITE_TRANSIENT);
        }
        /* Step once to get a value */
        if (sqlite3_step(dummy_stmt) == SQLITE_ROW) {
            value = sqlite3_column_value(dummy_stmt, 0);
        }
        sqlite3_reset(dummy_stmt);
        sqlite3_finalize(dummy_stmt);
    }

    /* Use the fuzz input to decide which operations to perform */
    size_t index = 0;
    while (index < Size) {
        uint8_t op = Data[index] % 6;
        index++;

        switch (op) {
            case 0: {
                /* sqlite3_bind_parameter_count */
                int count = sqlite3_bind_parameter_count(stmt1);
                (void)count;
                break;
            }
            case 1: {
                /* sqlite3_clear_bindings */
                sqlite3_clear_bindings(stmt1);
                break;
            }
            case 2: {
                /* sqlite3_transfer_bindings */
                if (stmt1 && stmt2) {
                    sqlite3_transfer_bindings(stmt1, stmt2);
                }
                break;
            }
            case 3: {
                /* sqlite3_bind_value */
                if (value && stmt1) {
                    int param_index = (Data[index % Size] % 3) + 1;
                    index = (index + 1) % Size;
                    sqlite3_bind_value(stmt1, param_index, value);
                }
                break;
            }
            case 4: {
                /* sqlite3_bind_null */
                if (stmt1) {
                    int param_index = (Data[index % Size] % 3) + 1;
                    index = (index + 1) % Size;
                    sqlite3_bind_null(stmt1, param_index);
                }
                break;
            }
            case 5: {
                /* sqlite3_reset */
                sqlite3_reset(stmt1);
                break;
            }
        }
    }

cleanup:
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close(db);
    return 0;
}