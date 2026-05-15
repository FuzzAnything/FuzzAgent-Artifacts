// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:78864:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:78882:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:78873:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
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
#include <stdio.h>

static void dummy_destructor(void *ptr) {
    /* Do nothing - just for testing */
}

static void test_auxdata(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size > 0) {
        int N = data[0] % 8;  /* Use first byte for N parameter */
        void *aux_data = malloc(size);
        if (aux_data) {
            memcpy(aux_data, data, size);
            sqlite3_set_auxdata(ctx, N, aux_data, dummy_destructor);
        }
    }
}

static void test_result_functions(sqlite3_context *ctx, const uint8_t *data, size_t size) {
    if (size == 0) {
        sqlite3_result_null(ctx);
        return;
    }
    
    /* Use first byte to select which function to test */
    uint8_t selector = data[0];
    
    /* Prepare data pointer and length */
    const void *text_data = (size > 1) ? (data + 1) : data;
    int data_len = (size > 1) ? (size - 1) : size;
    
    switch (selector % 6) {
        case 0:
            sqlite3_result_text(ctx, (const char*)text_data, data_len, SQLITE_TRANSIENT);
            break;
        case 1:
            sqlite3_result_text16(ctx, text_data, data_len * 2, SQLITE_TRANSIENT);
            break;
        case 2:
            sqlite3_result_text16le(ctx, text_data, data_len * 2, SQLITE_TRANSIENT);
            break;
        case 3:
            sqlite3_result_text16be(ctx, text_data, data_len * 2, SQLITE_TRANSIENT);
            break;
        case 4:
            sqlite3_result_blob(ctx, text_data, data_len, SQLITE_TRANSIENT);
            break;
        case 5:
            sqlite3_result_blob(ctx, text_data, data_len, dummy_destructor);
            break;
    }
}

/* Dummy SQL function that uses the target APIs */
static void test_udf(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Get fuzzed data from the first argument if available */
    const uint8_t *data = NULL;
    size_t size = 0;
    
    if (argc > 0) {
        data = sqlite3_value_blob(argv[0]);
        size = sqlite3_value_bytes(argv[0]);
    }
    
    /* Test both auxdata and result functions */
    if (data && size > 0) {
        test_auxdata(ctx, data, size);
        test_result_functions(ctx, data, size);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static char *err_msg = NULL;
    
    /* Initialize database once */
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        /* Register our test function */
        sqlite3_create_function(db, "test_udf", 1, SQLITE_UTF8, NULL, 
                               test_udf, NULL, NULL);
        initialized = 1;
    }
    
    if (!db) {
        return 0;
    }
    
    /* Create a table and insert fuzzed data */
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS fuzz_test (data BLOB);";
    sqlite3_exec(db, create_table, NULL, NULL, &err_msg);
    
    /* Insert fuzzed data */
    sqlite3_stmt *stmt;
    const char *insert_sql = "INSERT INTO fuzz_test (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    /* Call our test function with the fuzzed data */
    const char *select_sql = "SELECT test_udf(data) FROM fuzz_test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    /* Also test with direct blob binding */
    const char *direct_sql = "SELECT test_udf(?);";
    if (sqlite3_prepare_v2(db, direct_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    /* Test with file if needed */
    if (Size > 0) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            /* Use the file in a query */
            const char *file_sql = "SELECT test_udf(readfile('./dummy_file'));";
            if (sqlite3_prepare_v2(db, file_sql, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            remove("./dummy_file");
        }
    }
    
    /* Clean up table for next iteration */
    sqlite3_exec(db, "DELETE FROM fuzz_test;", NULL, NULL, &err_msg);
    
    /* Free any error message */
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    return 0;
}