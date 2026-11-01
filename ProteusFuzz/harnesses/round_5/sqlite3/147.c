// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_* family of functions.
 * 
 * Strategy:
 * 1. Create an in-memory database.
 * 2. Prepare a statement with bound parameters of various types.
 * 3. Step through the statement to get column values.
 * 4. Use sqlite3_column_value() to get unprotected values.
 * 5. Protect them using sqlite3_value_dup().
 * 6. Call the target functions on the protected values.
 * 7. Clean up resources carefully to avoid use-after-free or double-free.
 */

static void invoke_target_functions(sqlite3_value *val) {
    if (!val) return;

    /* Target 1: sqlite3_value_text16be */
    const void *res1 = sqlite3_value_text16be(val);
    (void)res1;

    /* Target 2: sqlite3_value_text16 */
    const void *res2 = sqlite3_value_text16(val);
    (void)res2;

    /* Target 3: sqlite3_value_blob */
    const void *res3 = sqlite3_value_blob(val);
    (void)res3;

    /* Target 4: sqlite3_value_pointer */
    void *res4 = sqlite3_value_pointer(val, "fuzz_type");
    (void)res4;

    /* Target 5: sqlite3_value_text16le */
    const void *res5 = sqlite3_value_text16le(val);
    (void)res5;

    /* Target 6: sqlite3_value_text */
    const unsigned char *res6 = sqlite3_value_text(val);
    (void)res6;
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to hold data */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Prepare an INSERT statement to bind fuzzed data */
    const char *insert_sql = "INSERT INTO fuzz_table (data) VALUES (?1);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 3. Bind the fuzzed input as a BLOB */
    rc = sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Execute the insert */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    /* Finalize insert statement before reusing db for select */
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* 4. Prepare a SELECT statement to retrieve the data */
    const char *select_sql = "SELECT data FROM fuzz_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step to get the row */
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        /* Get the column value (unprotected) */
        sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
        
        if (col_val) {
            /* Protect the value */
            sqlite3_value *prot_val = sqlite3_value_dup(col_val);
            
            if (prot_val) {
                /* Invoke target functions */
                invoke_target_functions(prot_val);
                
                /* Cleanup protected value */
                sqlite3_value_free(prot_val);
            }
        }
    }

    /* Finalize select statement */
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* 5. Additional exploration: Bind different types to test type conversions */
    
    /* Test with Integer binding */
    rc = sqlite3_prepare_v2(db, "INSERT INTO fuzz_table (data) VALUES (?1);", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Bind an integer derived from the input */
        int64_t int_val = 0;
        if (Size >= 8) {
            memcpy(&int_val, Data, 8);
        } else if (Size > 0) {
            int_val = (int64_t)Data[0];
        }
        
        sqlite3_bind_int64(stmt, 1, int_val);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;

        /* Retrieve and test */
        rc = sqlite3_prepare_v2(db, "SELECT data FROM fuzz_table LIMIT 1;", -1, &stmt, NULL);
        if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
            if (col_val) {
                sqlite3_value *prot_val = sqlite3_value_dup(col_val);
                if (prot_val) {
                    invoke_target_functions(prot_val);
                    sqlite3_value_free(prot_val);
                }
            }
        }
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /* Test with Text binding (if input looks like text) */
    rc = sqlite3_prepare_v2(db, "INSERT INTO fuzz_table (data) VALUES (?1);", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Bind as text, ensuring null termination */
        char *text_buf = (char *)malloc(Size + 1);
        if (text_buf) {
            memcpy(text_buf, Data, Size);
            text_buf[Size] = '\0';
            sqlite3_bind_text(stmt, 1, text_buf, Size, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;

            rc = sqlite3_prepare_v2(db, "SELECT data FROM fuzz_table LIMIT 1;", -1, &stmt, NULL);
            if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
                if (col_val) {
                    sqlite3_value *prot_val = sqlite3_value_dup(col_val);
                    if (prot_val) {
                        invoke_target_functions(prot_val);
                        sqlite3_value_free(prot_val);
                    }
                }
            }
            if (stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            free(text_buf);
        }
    }

    /* 6. Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    sqlite3_close(db);

    return 0;
}