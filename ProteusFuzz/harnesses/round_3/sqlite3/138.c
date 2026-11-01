// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_* functions.
 *
 * Strategy:
 * 1. Create an in-memory database.
 * 2. Register a custom SQL function that receives sqlite3_value arguments.
 * 3. Inside the function, call the target APIs:
 *    - sqlite3_value_text16
 *    - sqlite3_value_bytes
 *    - sqlite3_value_text
 *    - sqlite3_value_text16le
 *    - sqlite3_value_subtype
 *    - sqlite3_value_blob
 * 4. Execute SQL statements that pass various data types (text, blob, int, null)
 *    to the custom function to trigger different code paths.
 * 5. Use the fuzz input to generate SQL statements and bind parameters.
 */

static void custom_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;

    if (argc < 1) return;

    sqlite3_value *val = argv[0];

    /* Call target APIs */
    const void *text16 = sqlite3_value_text16(val);
    (void)text16; /* Suppress unused warning */

    int bytes = sqlite3_value_bytes(val);
    (void)bytes;

    const unsigned char *text = sqlite3_value_text(val);
    (void)text;

    const void *text16le = sqlite3_value_text16le(val);
    (void)text16le;

    unsigned int subtype = sqlite3_value_subtype(val);
    (void)subtype;

    const void *blob = sqlite3_value_blob(val);
    (void)blob;
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Create in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom function with SQLITE_SUBTYPE flag to enable subtype testing */
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_SUBTYPE, NULL, custom_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a test table */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Generate SQL based on input data */
    /* We'll use the input to create dynamic SQL statements that call our function */
    /* with various argument types */
    
    char sql[1024];
    
    if (Size > 0) {
        /* Use first few bytes to determine operation type */
        uint8_t op_type = Data[0] % 4;
        
        switch (op_type) {
            case 0:
                /* Test with text */
                if (Size > 1) {
                    int text_len = Data[1] % 100;
                    if (text_len > (int)Size - 2) text_len = (int)Size - 2;
                    if (text_len > 0) {
                        /* Safe access with bounds checking */
                        if (Size >= 6) {
                            snprintf(sql, sizeof(sql), "SELECT fuzz_func(X'%02x%02x%02x%02x')", 
                                     Data[2] & 0xFF, Data[3] & 0xFF, Data[4] & 0xFF, Data[5] & 0xFF);
                        } else {
                            snprintf(sql, sizeof(sql), "SELECT fuzz_func('test_string')");
                        }
                    } else {
                        snprintf(sql, sizeof(sql), "SELECT fuzz_func('test_string')");
                    }
                } else {
                    snprintf(sql, sizeof(sql), "SELECT fuzz_func('default_text')");
                }
                break;
                
            case 1:
                /* Test with blob */
                if (Size > 2) {
                    int blob_len = Data[1] % 50;
                    if (blob_len > (int)Size - 2) blob_len = (int)Size - 2;
                    if (blob_len > 0) {
                        /* Safe access with bounds checking */
                        if (Size >= 5) {
                            snprintf(sql, sizeof(sql), "SELECT fuzz_func(X'%02x%02x%02x')", 
                                     Data[2] & 0xFF, Data[3] & 0xFF, Data[4] & 0xFF);
                        } else {
                            snprintf(sql, sizeof(sql), "SELECT fuzz_func(X'48454C4C4F')");
                        }
                    } else {
                        snprintf(sql, sizeof(sql), "SELECT fuzz_func(X'48454C4C4F')");
                    }
                } else {
                    snprintf(sql, sizeof(sql), "SELECT fuzz_func(X'48454C4C4F')");
                }
                break;
                
            case 2:
                /* Test with integer */
                if (Size > 2) {
                    int int_val = (Data[1] << 8) | (Data[2] & 0xFF);
                    snprintf(sql, sizeof(sql), "SELECT fuzz_func(%d)", int_val);
                } else {
                    snprintf(sql, sizeof(sql), "SELECT fuzz_func(42)");
                }
                break;
                
            case 3:
                /* Test with NULL */
                snprintf(sql, sizeof(sql), "SELECT fuzz_func(NULL)");
                break;
        }
    } else {
        snprintf(sql, sizeof(sql), "SELECT fuzz_func('default')");
    }

    /* Execute the SQL statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Process row if needed - for fuzzing, we just want to trigger the function */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Also test with bound parameters */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_func(?)", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Bind different types based on input */
        if (Size > 0) {
            uint8_t bind_type = Data[0] % 3;
            switch (bind_type) {
                case 0:
                    /* Bind as text */
                    if (Size > 10) {
                        char text_buf[11];
                        memcpy(text_buf, Data + 1, 10);
                        text_buf[10] = '\0';
                        sqlite3_bind_text(stmt, 1, text_buf, 10, SQLITE_STATIC);
                    } else {
                        sqlite3_bind_text(stmt, 1, "fuzz_text", -1, SQLITE_STATIC);
                    }
                    break;
                case 1:
                    /* Bind as blob */
                    if (Size > 5) {
                        sqlite3_bind_blob(stmt, 1, Data + 1, Size - 1, SQLITE_STATIC);
                    } else {
                        sqlite3_bind_blob(stmt, 1, "blob", 4, SQLITE_STATIC);
                    }
                    break;
                case 2:
                    /* Bind as integer */
                    if (Size > 2) {
                        int int_val = (Data[1] << 8) | (Data[2] & 0xFF);
                        sqlite3_bind_int(stmt, 1, int_val);
                    } else {
                        sqlite3_bind_int(stmt, 1, 12345);
                    }
                    break;
            }
        } else {
            sqlite3_bind_text(stmt, 1, "default", -1, SQLITE_STATIC);
        }
        
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Process row */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Test with multiple arguments */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_func(?), fuzz_func(?), fuzz_func(?)", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        if (Size > 3) {
            sqlite3_bind_text(stmt, 1, "arg1", 4, SQLITE_STATIC);
            sqlite3_bind_blob(stmt, 2, Data, Size, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, Data[Size-1]);
        } else {
            sqlite3_bind_text(stmt, 1, "a", 1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, "b", 1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, "c", 1, SQLITE_STATIC);
        }
        
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Process row */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}