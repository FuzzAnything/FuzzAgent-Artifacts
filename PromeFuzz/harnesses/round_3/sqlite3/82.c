// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy user data for testing */
static int dummy_user_data = 0xDEADBEEF;

/* Dummy auxiliary data for testing */
static int dummy_auxdata = 0xCAFEBABE;

/* Destructor for auxiliary data */
static void dummy_destructor(void *ptr) {
    /* Do nothing, just for testing */
}

/* Test function that will be registered with SQLite */
static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Use fuzzer input to decide which operations to perform */
    if (argc < 1) return;
    
    /* Get the fuzzer input from the first argument */
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int data_size = sqlite3_value_bytes(argv[0]);
    
    if (data_size < 1) return;
    
    /* Use first byte to determine which operations to perform */
    uint8_t op_code = data[0];
    
    /* Test sqlite3_user_data */
    void *user_data = sqlite3_user_data(context);
    
    /* Test sqlite3_get_auxdata with different indices */
    int aux_index = (data_size > 1) ? (data[1] % 4) : 0;
    void *aux_data = sqlite3_get_auxdata(context, aux_index);
    
    /* Test different result functions based on op_code */
    switch (op_code % 6) {
        case 0:
            /* Test sqlite3_result_error */
            if (data_size > 2) {
                /* Use part of fuzzer data as error message */
                int err_len = (data_size - 2) > 255 ? 255 : (data_size - 2);
                char *err_msg = malloc(err_len + 1);
                if (err_msg) {
                    memcpy(err_msg, data + 2, err_len);
                    err_msg[err_len] = '\0';
                    sqlite3_result_error(context, err_msg, err_len);
                    free(err_msg);
                } else {
                    sqlite3_result_error(context, "Memory allocation failed", -1);
                }
            } else {
                sqlite3_result_error(context, "Test error", -1);
            }
            break;
            
        case 1:
            /* Test sqlite3_result_int */
            if (data_size > 1) {
                sqlite3_result_int(context, (int)data[1]);
            } else {
                sqlite3_result_int(context, 42);
            }
            break;
            
        case 2:
            /* Test sqlite3_result_null */
            sqlite3_result_null(context);
            break;
            
        case 3:
            /* Test sqlite3_result_subtype */
            if (data_size > 1) {
                sqlite3_result_int(context, 100);
                sqlite3_result_subtype(context, (unsigned int)data[1]);
            } else {
                sqlite3_result_int(context, 100);
                sqlite3_result_subtype(context, 0x55);
            }
            break;
            
        case 4:
            /* Test combination of operations */
            sqlite3_result_int(context, 200);
            if (data_size > 1 && (data[1] & 1)) {
                sqlite3_result_subtype(context, 0xAA);
            }
            break;
            
        case 5:
            /* Test with auxdata operations */
            if (data_size > 2) {
                /* Set auxdata first, then get it */
                sqlite3_set_auxdata(context, aux_index, &dummy_auxdata, dummy_destructor);
                aux_data = sqlite3_get_auxdata(context, aux_index);
                sqlite3_result_int(context, (aux_data == &dummy_auxdata) ? 1 : 0);
            } else {
                sqlite3_result_int(context, 300);
            }
            break;
    }
}

/* Aggregate step function (required for aggregate registration) */
static void test_aggregate_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Do nothing for testing */
}

/* Aggregate final function (required for aggregate registration) */
static void test_aggregate_final(sqlite3_context *context) {
    sqlite3_result_int(context, 999);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    FILE *fp = NULL;
    
    /* Create a dummy file with fuzzer data */
    fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            /* Write minimal data if input is empty */
            uint8_t dummy = 0;
            fwrite(&dummy, 1, 1, fp);
        }
        fclose(fp);
    }
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Register a scalar function */
    rc = sqlite3_create_function(
        db,
        "test_func",
        1,  /* Number of arguments */
        SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_RESULT_SUBTYPE,
        &dummy_user_data,
        test_function,
        NULL,
        NULL
    );
    
    /* Register an aggregate function */
    rc = sqlite3_create_function(
        db,
        "test_agg",
        1,  /* Number of arguments */
        SQLITE_UTF8,
        &dummy_user_data,
        NULL,
        test_aggregate_step,
        test_aggregate_final
    );
    
    /* Test the functions with various queries */
    if (Size > 0) {
        /* Use fuzzer data to create different test scenarios */
        
        /* Test 1: Call scalar function with blob parameter */
        rc = sqlite3_prepare_v2(db, "SELECT test_func(?)", -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            if (Size > 0) {
                sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
            } else {
                uint8_t dummy = 0;
                sqlite3_bind_blob(stmt, 1, &dummy, 1, SQLITE_STATIC);
            }
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Function executed, result available */
                sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        
        /* Test 2: Call aggregate function */
        rc = sqlite3_prepare_v2(db, "SELECT test_agg(1)", -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        
        /* Test 3: Create a table and use functions with it */
        rc = sqlite3_exec(db, "CREATE TABLE t1(x BLOB)", NULL, NULL, NULL);
        if (rc == SQLITE_OK) {
            /* Insert fuzzer data into table */
            rc = sqlite3_prepare_v2(db, "INSERT INTO t1 VALUES(?)", -1, &stmt, NULL);
            if (rc == SQLITE_OK && stmt) {
                if (Size > 0) {
                    sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 512 ? 512 : Size), SQLITE_STATIC);
                } else {
                    uint8_t dummy = 0;
                    sqlite3_bind_blob(stmt, 1, &dummy, 1, SQLITE_STATIC);
                }
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            /* Query using our test function */
            rc = sqlite3_prepare_v2(db, "SELECT test_func(x) FROM t1", -1, &stmt, NULL);
            if (rc == SQLITE_OK && stmt) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    /* Process results */
                }
                sqlite3_finalize(stmt);
            }
        }
        
        /* Test 4: Use the dummy file */
        rc = sqlite3_prepare_v2(db, "SELECT test_func(readfile('./dummy_file'))", -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    /* Remove dummy file */
    remove("./dummy_file");
    
    return 0;
}