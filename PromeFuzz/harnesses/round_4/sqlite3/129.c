// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function is registered as a user-defined function
}

static void test_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    static int call_count = 0;
    call_count++;
    
    switch (call_count % 4) {
        case 0:
            sqlite3_result_int(ctx, 42);
            break;
        case 1:
            sqlite3_result_error_toobig(ctx);
            break;
        case 2:
            sqlite3_result_zeroblob(ctx, 100);
            break;
        case 3:
            sqlite3_result_error_nomem(ctx);
            break;
    }
}

static void error_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    const char *error_msg = (const char *)sqlite3_user_data(ctx);
    if (error_msg) {
        sqlite3_result_error(ctx, error_msg, -1);
    } else {
        sqlite3_result_error_nomem(ctx);
    }
}

static void db_handle_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3 *db_handle = sqlite3_context_db_handle(ctx);
    if (db_handle) {
        sqlite3_result_int(ctx, 1);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void int_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int value = 42;
    if (argc > 0) {
        value = sqlite3_value_int(argv[0]);
    }
    sqlite3_result_int(ctx, value);
}

static void zeroblob_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int n = 0;
    if (argc > 0) {
        n = sqlite3_value_int(argv[0]);
        if (n < 0) n = 0;
    }
    sqlite3_result_zeroblob(ctx, n);
}

static void random_int_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int value = 0;
    sqlite3_result_int(ctx, value);
}

static void nomem_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_error_nomem(ctx);
}

static void multi_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        int mode = sqlite3_value_int(argv[0]);
        switch (mode % 5) {
            case 0:
                sqlite3_result_int(ctx, 123);
                break;
            case 1:
                sqlite3_result_error_toobig(ctx);
                break;
            case 2:
                sqlite3_result_zeroblob(ctx, 50);
                break;
            case 3:
                sqlite3_result_error(ctx, "Test error", -1);
                break;
            case 4:
                sqlite3_result_error_nomem(ctx);
                break;
        }
    }
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    rc = sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, 
                                 dummy_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT dummy_func()", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    sqlite3_create_function(db, "db_handle_func", 0, SQLITE_UTF8, NULL,
                           db_handle_func, NULL, NULL);
    
    sqlite3_stmt *stmt2 = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT db_handle_func()", -1, &stmt2, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt2);
        sqlite3_finalize(stmt2);
    }
    
    if (Size > 0) {
        uint8_t selector = Data[0] % 6;
        size_t data_offset = 1;
        
        switch (selector) {
            case 0:
                sqlite3_create_function(db, "int_func", 1, SQLITE_UTF8, NULL,
                                       int_func, NULL, NULL);
                
                if (Size > data_offset) {
                    int int_value = (int)Data[data_offset];
                    char sql[100];
                    snprintf(sql, sizeof(sql), "SELECT int_func(%d)", int_value);
                    sqlite3_stmt *stmt3 = NULL;
                    rc = sqlite3_prepare_v2(db, sql, -1, &stmt3, NULL);
                    if (rc == SQLITE_OK) {
                        sqlite3_step(stmt3);
                        sqlite3_finalize(stmt3);
                    }
                }
                break;
                
            case 1:
                sqlite3_create_function(db, "toobig_func", 0, SQLITE_UTF8, NULL,
                                       test_func, NULL, NULL);
                
                sqlite3_stmt *stmt4 = NULL;
                rc = sqlite3_prepare_v2(db, "SELECT toobig_func()", -1, &stmt4, NULL);
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt4);
                    sqlite3_finalize(stmt4);
                }
                break;
                
            case 2:
                sqlite3_create_function(db, "zeroblob_func", 1, SQLITE_UTF8, NULL,
                                       zeroblob_func, NULL, NULL);
                
                if (Size > data_offset) {
                    int blob_size = (int)Data[data_offset];
                    char sql[100];
                    snprintf(sql, sizeof(sql), "SELECT zeroblob_func(%d)", blob_size);
                    sqlite3_stmt *stmt5 = NULL;
                    rc = sqlite3_prepare_v2(db, sql, -1, &stmt5, NULL);
                    if (rc == SQLITE_OK) {
                        sqlite3_step(stmt5);
                        sqlite3_finalize(stmt5);
                    }
                }
                break;
                
            case 3:
                if (Size > data_offset) {
                    size_t msg_len = Size - data_offset;
                    if (msg_len > 100) msg_len = 100;
                    
                    char *error_msg = (char *)malloc(msg_len + 1);
                    if (error_msg) {
                        memcpy(error_msg, Data + data_offset, msg_len);
                        error_msg[msg_len] = '\0';
                        
                        sqlite3_create_function(db, "error_func", 0, SQLITE_UTF8, 
                                               (void *)error_msg, error_func, NULL, NULL);
                        
                        sqlite3_stmt *stmt6 = NULL;
                        rc = sqlite3_prepare_v2(db, "SELECT error_func()", -1, &stmt6, NULL);
                        if (rc == SQLITE_OK) {
                            sqlite3_step(stmt6);
                            sqlite3_finalize(stmt6);
                        }
                        
                        free(error_msg);
                    }
                }
                break;
                
            case 4:
                sqlite3_create_function(db, "random_int_func", 0, SQLITE_UTF8, NULL,
                                       random_int_func, NULL, NULL);
                
                sqlite3_stmt *stmt7 = NULL;
                rc = sqlite3_prepare_v2(db, "SELECT random_int_func()", -1, &stmt7, NULL);
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt7);
                    sqlite3_finalize(stmt7);
                }
                break;
                
            case 5:
                sqlite3_create_function(db, "nomem_func", 0, SQLITE_UTF8, NULL,
                                       nomem_func, NULL, NULL);
                
                sqlite3_stmt *stmt8 = NULL;
                rc = sqlite3_prepare_v2(db, "SELECT nomem_func()", -1, &stmt8, NULL);
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt8);
                    sqlite3_finalize(stmt8);
                }
                break;
        }
    }
    
    sqlite3_create_function(db, "multi_func", 1, SQLITE_UTF8, NULL,
                           multi_func, NULL, NULL);
    
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        char sql[100];
        snprintf(sql, sizeof(sql), "SELECT multi_func(%d)", i);
        sqlite3_stmt *stmt9 = NULL;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt9, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt9);
            sqlite3_finalize(stmt9);
        }
    }
    
    sqlite3_close(db);
    
    return 0;
}