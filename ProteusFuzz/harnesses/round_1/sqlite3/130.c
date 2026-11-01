// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to test bindings */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB, text TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an INSERT statement with multiple parameters */
    const char *insert_sql = "INSERT INTO test (id, data, text) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Fuzzing logic */
    if (Size > 0) {
        /* Use input data to determine parameter count for binding */
        int param_count = sqlite3_bind_parameter_count(stmt);
        if (param_count > 0) {
            /* Determine how many parameters we can bind based on input size */
            int bind_count = (Size < (size_t)param_count) ? (int)Size : param_count;
            if (bind_count > 3) bind_count = 3; /* Limit to available placeholders */

            for (int i = 1; i <= bind_count; i++) {
                uint8_t type = (i > 0 && i <= Size) ? Data[i - 1] % 10 : 0;
                uint8_t len_byte = (i > 0 && i <= Size) ? Data[i - 1] : 0;
                sqlite3_uint64 len = (sqlite3_uint64)len_byte * 10 + 1;
                
                /* Ensure we don't exceed input bounds for data pointer */
                size_t available = (Size > i) ? (Size - i) : 0;
                size_t data_len = (len > available) ? available : (size_t)len;

                switch (type) {
                    case 0: {
                        /* Bind BLOB using sqlite3_bind_blob64 */
                        const void *blob_data = (data_len > 0) ? (const void*)&Data[i] : NULL;
                        rc = sqlite3_bind_blob64(stmt, i, blob_data, data_len, SQLITE_STATIC);
                        break;
                    }
                    case 1: {
                        /* Bind Text using sqlite3_bind_text64 */
                        const char *text_data = (data_len > 0) ? (const char*)&Data[i] : "";
                        rc = sqlite3_bind_text64(stmt, i, text_data, data_len, SQLITE_STATIC, SQLITE_UTF8);
                        break;
                    }
                    case 2: {
                        /* Bind ZeroBlob using sqlite3_bind_zeroblob64 */
                        rc = sqlite3_bind_zeroblob64(stmt, i, len);
                        break;
                    }
                    case 3: {
                        /* Bind Value using sqlite3_bind_value */
                        sqlite3_value *val = NULL;
                        /* Create a temporary value to bind */
                        if (data_len > 0) {
                            /* We need a valid sqlite3_value* to bind. 
                               We can get one from a column if we had data, but here we are inserting.
                               Alternative: Create a dummy SELECT statement to get a value.
                            */
                            sqlite3_stmt *tmp_stmt = NULL;
                            const char *tmp_sql = "SELECT 'fuzz';";
                            if (sqlite3_prepare_v2(db, tmp_sql, -1, &tmp_stmt, NULL) == SQLITE_OK) {
                                if (sqlite3_step(tmp_stmt) == SQLITE_ROW) {
                                    val = sqlite3_column_value(tmp_stmt, 0);
                                    if (val) {
                                        rc = sqlite3_bind_value(stmt, i, val);
                                    }
                                }
                                sqlite3_finalize(tmp_stmt);
                            }
                        }
                        break;
                    }
                    default:
                        /* Bind NULL or Integer */
                        rc = sqlite3_bind_null(stmt, i);
                        break;
                }
                
                if (rc != SQLITE_OK) {
                    /* Reset statement to allow re-binding or just continue */
                    sqlite3_reset(stmt);
                    sqlite3_clear_bindings(stmt);
                }
            }
        }
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW && rc != SQLITE_OK) {
        /* Error occurred, reset and clear */
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }

    /* Test sqlite3_reset explicitly */
    rc = sqlite3_reset(stmt);
    
    /* Finalize statement */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}