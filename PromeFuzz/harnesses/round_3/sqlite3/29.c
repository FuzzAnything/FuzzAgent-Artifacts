// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql_text = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Create a simple table and insert some data */
    const char *create_sql = "CREATE TABLE test (id INTEGER, value INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    const char *insert_sql = "INSERT INTO test VALUES (1, 100), (2, 200), (3, 300);";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Prepare first statement using fuzzer input if possible, otherwise use default */
    const char *default_sql = "SELECT * FROM test WHERE id > ?";
    const char *sql_to_prepare = default_sql;
    
    /* Try to use fuzzer input as SQL if it looks reasonable */
    if (Size > 0 && Size < 1000) {
        char *fuzz_sql = (char *)malloc(Size + 1);
        if (fuzz_sql) {
            memcpy(fuzz_sql, Data, Size);
            fuzz_sql[Size] = '\0';
            
            /* Try to prepare with fuzzer SQL */
            if (sqlite3_prepare_v2(db, fuzz_sql, -1, &stmt1, NULL) == SQLITE_OK) {
                sql_to_prepare = fuzz_sql;
            } else {
                /* If fuzzer SQL fails, prepare with default */
                sqlite3_finalize(stmt1);
                stmt1 = NULL;
                if (sqlite3_prepare_v2(db, default_sql, -1, &stmt1, NULL) != SQLITE_OK) {
                    free(fuzz_sql);
                    sqlite3_close(db);
                    return 0;
                }
            }
            free(fuzz_sql);
        } else {
            /* Memory allocation failed, use default */
            if (sqlite3_prepare_v2(db, default_sql, -1, &stmt1, NULL) != SQLITE_OK) {
                sqlite3_close(db);
                return 0;
            }
        }
    } else {
        /* Use default SQL */
        if (sqlite3_prepare_v2(db, default_sql, -1, &stmt1, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
    }
    
    /* Bind a parameter if the statement has parameters */
    if (sqlite3_bind_parameter_count(stmt1) > 0) {
        /* Use fuzzer data to determine binding value */
        int bind_value = 0;
        if (Size > 0) {
            bind_value = Data[0] % 10; /* Use first byte for binding */
        }
        sqlite3_bind_int(stmt1, 1, bind_value);
    }
    
    /* First sqlite3_sql call */
    sql_text = sqlite3_sql(stmt1);
    
    /* First sqlite3_step call */
    rc = sqlite3_step(stmt1);
    
    /* Second sqlite3_sql call */
    sql_text = sqlite3_sql(stmt1);
    
    /* First sqlite3_column_int call */
    if (rc == SQLITE_ROW && sqlite3_column_count(stmt1) > 0) {
        int col0 = sqlite3_column_int(stmt1, 0);
        (void)col0; /* Suppress unused variable warning */
    }
    
    /* Second sqlite3_column_int call */
    if (rc == SQLITE_ROW && sqlite3_column_count(stmt1) > 1) {
        int col1 = sqlite3_column_int(stmt1, 1);
        (void)col1; /* Suppress unused variable warning */
    }
    
    /* Continue stepping through results */
    while (rc == SQLITE_ROW) {
        rc = sqlite3_step(stmt1);
    }
    
    /* First sqlite3_finalize call */
    sqlite3_finalize(stmt1);
    stmt1 = NULL;
    
    /* sqlite3_db_release_memory call */
    sqlite3_db_release_memory(db);
    
    /* Prepare second statement */
    const char *second_sql = "SELECT COUNT(*) FROM test";
    if (sqlite3_prepare_v2(db, second_sql, -1, &stmt2, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Third sqlite3_sql call */
    sql_text = sqlite3_sql(stmt2);
    
    /* Second sqlite3_step call */
    rc = sqlite3_step(stmt2);
    
    /* Fourth sqlite3_sql call */
    sql_text = sqlite3_sql(stmt2);
    
    /* Second sqlite3_finalize call */
    sqlite3_finalize(stmt2);
    stmt2 = NULL;
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}