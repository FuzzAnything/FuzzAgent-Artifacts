// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15, ?16, ?17, ?18, ?19, ?20;";
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind values from fuzz input if available */
    size_t col_idx = 0;
    int param_idx = 1;
    
    while (param_idx <= 20 && col_idx < Size) {
        int type = (Size > col_idx) ? (Data[col_idx] % 6) : 0;
        int len = 0;
        
        if (col_idx + 1 < Size) {
            len = Data[col_idx + 1];
        }
        
        if (type == 0) {
            sqlite3_bind_null(stmt, param_idx);
            col_idx += 2; /* Skip type and len byte */
        } else if (type == 1) {
            int val = 0;
            if (col_idx + 2 + sizeof(int) <= Size) {
                memcpy(&val, &Data[col_idx + 2], sizeof(int));
                col_idx += 2 + sizeof(int);
            } else {
                col_idx = Size; /* Not enough data, stop */
            }
            sqlite3_bind_int(stmt, param_idx, val);
        } else if (type == 2) {
            double val = 0.0;
            if (col_idx + 2 + sizeof(double) <= Size) {
                memcpy(&val, &Data[col_idx + 2], sizeof(double));
                col_idx += 2 + sizeof(double);
            } else {
                col_idx = Size;
            }
            sqlite3_bind_double(stmt, param_idx, val);
        } else if (type == 3 || type == 4) {
            int blob_len = (len > 0 && col_idx + 2 + len <= Size) ? len : 0;
            if (blob_len > 0) {
                const void *blob_data = (const void *)&Data[col_idx + 2];
                if (type == 3) {
                    sqlite3_bind_text(stmt, param_idx, blob_data, blob_len, SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_blob(stmt, param_idx, blob_data, blob_len, SQLITE_TRANSIENT);
                }
                col_idx += 2 + blob_len;
            } else {
                sqlite3_bind_text(stmt, param_idx, "", 0, SQLITE_TRANSIENT);
                col_idx += 2;
            }
        } else {
            sqlite3_bind_null(stmt, param_idx);
            col_idx += 2;
        }
        param_idx++;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Invoke the target API functions in the specified order */
    
    /* 1. sqlite3_column_text */
    const unsigned char *text_val = sqlite3_column_text(stmt, 0);
    (void)text_val;

    /* 2. sqlite3_bind_value */
    sqlite3_value *val1 = sqlite3_column_value(stmt, 1);
    if (val1) {
        sqlite3_bind_value(stmt, 1, val1);
    }

    /* 3. sqlite3_column_value */
    sqlite3_value *val2 = sqlite3_column_value(stmt, 2);
    (void)val2;

    /* 4. sqlite3_bind_value */
    if (val2) {
        sqlite3_bind_value(stmt, 2, val2);
    }

    /* 5. sqlite3_column_value */
    sqlite3_value *val3 = sqlite3_column_value(stmt, 3);
    (void)val3;

    /* 6. sqlite3_step */
    rc = sqlite3_step(stmt);
    (void)rc;

    /* 7. sqlite3_column_int */
    int int_val = sqlite3_column_int(stmt, 4);
    (void)int_val;

    /* 8. sqlite3_reset */
    rc = sqlite3_reset(stmt);
    (void)rc;

    /* Re-step to ensure valid row for subsequent calls */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 9. sqlite3_column_bytes16 */
    int bytes16_1 = sqlite3_column_bytes16(stmt, 5);
    (void)bytes16_1;

    /* 10. sqlite3_column_blob */
    const void *blob1 = sqlite3_column_blob(stmt, 6);
    (void)blob1;

    /* 11. sqlite3_column_bytes16 */
    int bytes16_2 = sqlite3_column_bytes16(stmt, 7);
    (void)bytes16_2;

    /* 12. sqlite3_column_blob */
    const void *blob2 = sqlite3_column_blob(stmt, 8);
    (void)blob2;

    /* 13. sqlite3_column_bytes */
    int bytes1 = sqlite3_column_bytes(stmt, 9);
    (void)bytes1;

    /* 14. sqlite3_column_blob */
    const void *blob3 = sqlite3_column_blob(stmt, 10);
    (void)blob3;

    /* 15. sqlite3_column_bytes */
    int bytes2 = sqlite3_column_bytes(stmt, 11);
    (void)bytes2;

    /* 16. sqlite3_column_blob */
    const void *blob4 = sqlite3_column_blob(stmt, 12);
    (void)blob4;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}