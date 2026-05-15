// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_func_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

static sqlite3_value* create_protected_value(const uint8_t *data, size_t size, int value_type) {
    sqlite3 *db = NULL;
    sqlite3_open(":memory:", &db);
    if (!db) return NULL;
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    switch (value_type % 5) {
        case 0:
            sqlite3_bind_null(stmt, 1);
            break;
        case 1:
            if (size > 0) {
                sqlite3_bind_int(stmt, 1, (int)data[0]);
            } else {
                sqlite3_bind_int(stmt, 1, 0);
            }
            break;
        case 2:
            if (size > 0) {
                sqlite3_bind_double(stmt, 1, (double)data[0]);
            } else {
                sqlite3_bind_double(stmt, 1, 0.0);
            }
            break;
        case 3:
            if (size > 0) {
                char *text = (char*)malloc(size + 1);
                if (text) {
                    memcpy(text, data, size);
                    text[size] = '\0';
                    sqlite3_bind_text(stmt, 1, text, -1, free);
                } else {
                    sqlite3_bind_null(stmt, 1);
                }
            } else {
                sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
            }
            break;
        case 4:
            if (size > 0) {
                sqlite3_bind_blob(stmt, 1, data, size > 100 ? 100 : size, SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_blob(stmt, 1, NULL, 0, SQLITE_STATIC);
            }
            break;
    }
    
    sqlite3_step(stmt);
    sqlite3_value *value = sqlite3_column_value(stmt, 0);
    
    sqlite3_value *protected_value = NULL;
    if (value) {
        protected_value = sqlite3_value_dup(value);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected_value;
}

static void test_value_functions(sqlite3_value *value) {
    if (!value) return;
    
    const void *text16le = sqlite3_value_text16le(value);
    (void)text16le;
    
    const void *text16be = sqlite3_value_text16be(value);
    (void)text16be;
    
    const void *text16 = sqlite3_value_text16(value);
    (void)text16;
    
    const void *blob = sqlite3_value_blob(value);
    (void)blob;
    
    int int_val = sqlite3_value_int(value);
    (void)int_val;
}

static void test_result_value(void) {
    sqlite3 *db = NULL;
    sqlite3_open(":memory:", &db);
    if (!db) return;
    
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
        test_func_impl, NULL, NULL);
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, 42);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    for (int i = 0; i < 5; i++) {
        sqlite3_value *value = create_protected_value(data, size, i);
        if (value) {
            test_value_functions(value);
            sqlite3_value_free(value);
        }
    }
    
    test_result_value();
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
        
        sqlite3 *db = NULL;
        if (sqlite3_open("./dummy_file", &db) == SQLITE_OK) {
            sqlite3_stmt *stmt = NULL;
            const char *sql = "SELECT ?";
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                if (size > 0) {
                    sqlite3_bind_blob(stmt, 1, data, size > 100 ? 100 : size, SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, 1);
                }
                
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    sqlite3_value *col_value = sqlite3_column_value(stmt, 0);
                    if (col_value) {
                        sqlite3_value *protected_val = sqlite3_value_dup(col_value);
                        if (protected_val) {
                            test_value_functions(protected_val);
                            sqlite3_value_free(protected_val);
                        }
                    }
                }
                sqlite3_finalize(stmt);
            }
            sqlite3_close(db);
        }
        remove("./dummy_file");
    }
    
    return 0;
}