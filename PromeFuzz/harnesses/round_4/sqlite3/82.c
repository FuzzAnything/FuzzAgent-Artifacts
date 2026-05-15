// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzz_prepare_and_test(sqlite3 *db, const uint8_t *sql_data, size_t sql_size) {
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    /* Prepare statement using fuzzer-provided SQL */
    int rc = sqlite3_prepare_v2(db, (const char*)sql_data, (int)sql_size, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Test sqlite3_sql */
        const char *original_sql = sqlite3_sql(stmt);
        (void)original_sql; /* Use result to avoid unused variable warning */
        
        /* Test sqlite3_db_handle */
        sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
        (void)db_from_stmt;
        
        /* Test sqlite3_column_count */
        int col_count = sqlite3_column_count(stmt);
        (void)col_count;
        
        /* Test sqlite3_reset */
        sqlite3_reset(stmt);
        
        /* Test sqlite3_next_stmt iteration */
        sqlite3_stmt *first_stmt = sqlite3_next_stmt(db, NULL);
        sqlite3_stmt *next_stmt = sqlite3_next_stmt(db, first_stmt);
        (void)next_stmt;
        
        /* Finalize the statement */
        sqlite3_finalize(stmt);
    }
}

static void fuzz_with_different_sql_statements(sqlite3 *db, const uint8_t *data, size_t size) {
    /* Try different SQL statement types */
    const char* sql_statements[] = {
        "SELECT 1;",
        "CREATE TABLE t1(a,b);",
        "INSERT INTO t1 VALUES(1,2);",
        "UPDATE t1 SET a=3 WHERE b=2;",
        "DELETE FROM t1 WHERE a=1;",
        "DROP TABLE t1;",
        "BEGIN;",
        "COMMIT;",
        "ROLLBACK;"
    };
    
    for (size_t i = 0; i < sizeof(sql_statements)/sizeof(sql_statements[0]); i++) {
        sqlite3_stmt *stmt = NULL;
        const char *tail = NULL;
        
        /* Prepare each statement type */
        int rc = sqlite3_prepare_v2(db, sql_statements[i], -1, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            /* Test all target functions on this statement */
            const char *sql_text = sqlite3_sql(stmt);
            (void)sql_text;
            
            sqlite3 *db_handle = sqlite3_db_handle(stmt);
            (void)db_handle;
            
            int cols = sqlite3_column_count(stmt);
            (void)cols;
            
            sqlite3_reset(stmt);
            
            /* Clean up */
            sqlite3_finalize(stmt);
        }
    }
    
    /* Also test with fuzzer data as SQL */
    fuzz_prepare_and_test(db, data, size);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open an in-memory database for fuzzing */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Test with fuzzer-provided SQL data */
    fuzz_prepare_and_test(db, Data, Size);
    
    /* Test with various SQL statement types */
    fuzz_with_different_sql_statements(db, Data, Size);
    
    /* Additional test: create a dummy file and use it */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
        
        /* Open the dummy file as a database */
        sqlite3 *file_db = NULL;
        rc = sqlite3_open("./dummy_file", &file_db);
        if (rc == SQLITE_OK && file_db != NULL) {
            fuzz_prepare_and_test(file_db, Data, Size);
            sqlite3_close(file_db);
        }
        
        remove("./dummy_file");
    }
    
    /* Test sqlite3_next_stmt with multiple prepared statements */
    sqlite3_stmt *stmt1 = NULL, *stmt2 = NULL, *stmt3 = NULL;
    const char *sql1 = "SELECT 1;";
    const char *sql2 = "SELECT 2;";
    const char *sql3 = "SELECT 3;";
    
    sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL);
    sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    sqlite3_prepare_v2(db, sql3, -1, &stmt3, NULL);
    
    /* Iterate through all statements */
    sqlite3_stmt *current = sqlite3_next_stmt(db, NULL);
    while (current != NULL) {
        /* Test functions on each statement */
        const char *sql = sqlite3_sql(current);
        (void)sql;
        
        sqlite3 *db_h = sqlite3_db_handle(current);
        (void)db_h;
        
        int cols = sqlite3_column_count(current);
        (void)cols;
        
        sqlite3_reset(current);
        
        current = sqlite3_next_stmt(db, current);
    }
    
    /* Clean up prepared statements */
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt3) sqlite3_finalize(stmt3);
    
    /* Close database */
    sqlite3_close(db);
    
    return 0;
}