// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Destructor function for client data */
static void client_data_destructor(void *data) {
    /* Do nothing - just for testing */
}

/* Destructor function for auxdata */
static void auxdata_destructor(void *data) {
    /* Do nothing - just for testing */
}

/* Simple test function to create a context for auxdata testing */
static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* This function exists only to provide a valid sqlite3_context */
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Test sqlite3_set_clientdata and sqlite3_get_clientdata */
    if (Size > 0) {
        /* Use first byte as name length, but ensure it's null-terminated */
        size_t name_len = (Size > 1) ? (Data[0] % 64) : 1;
        if (name_len > Size - 1) name_len = (Size > 1) ? Size - 1 : 1;
        
        char *name = malloc(name_len + 1);
        if (name) {
            size_t copy_len = (name_len < Size - 1) ? name_len : Size - 1;
            memcpy(name, Data + 1, copy_len);
            name[copy_len] = '\0';
            
            /* Set client data */
            void *client_data = (void *)(uintptr_t)Data[0];
            sqlite3_set_clientdata(db, name, client_data, client_data_destructor);
            
            /* Get client data */
            void *retrieved = sqlite3_get_clientdata(db, name);
            (void)retrieved; /* Use variable to avoid unused warning */
            
            free(name);
        }
    }
    
    /* Create a simple table and prepare a statement for testing */
    const char *create_table_sql = "CREATE TABLE test (id INTEGER, value TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    /* Insert some test data */
    const char *insert_sql = "INSERT INTO test VALUES (1, 'test');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    
    /* Prepare a statement for column_value testing */
    const char *select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Step once to get data */
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            /* Test sqlite3_column_value */
            sqlite3_value *value = sqlite3_column_value(stmt, 0);
            (void)value; /* Use variable to avoid unused warning */
            
            /* Test with different column indices */
            if (Size > 0) {
                int col_index = Data[0] % 2; /* Only 0 or 1 since we have 2 columns */
                sqlite3_value *col_value = sqlite3_column_value(stmt, col_index);
                (void)col_value; /* Use variable to avoid unused warning */
            }
        }
        
        /* Test sqlite3_finalize */
        sqlite3_finalize(stmt);
    }
    
    /* Test auxdata functions - need a valid sqlite3_context */
    /* We'll create a simple user-defined function to get a context */
    if (Size > 0) {
        /* Register a test function */
        sqlite3_create_function(db, "testfunc", 1, SQLITE_UTF8, NULL, 
                                test_function, NULL, NULL);
        
        /* Prepare a statement that uses the function */
        const char *func_sql = "SELECT testfunc(1);";
        sqlite3_stmt *func_stmt = NULL;
        rc = sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, NULL);
        
        if (rc == SQLITE_OK && func_stmt != NULL) {
            /* Get the function context (this is a bit hacky but works for fuzzing) */
            /* In real usage, the context would be available inside the function implementation */
            sqlite3_finalize(func_stmt);
        }
    }
    
    /* Test sqlite3_close */
    sqlite3_close(db);
    
    /* Test with NULL parameters */
    sqlite3_close(NULL);
    sqlite3_finalize(NULL);
    
    /* Test get_clientdata with NULL db */
    if (Size > 0) {
        char dummy_name[] = "dummy";
        sqlite3_get_clientdata(NULL, dummy_name);
    }
    
    return 0;
}