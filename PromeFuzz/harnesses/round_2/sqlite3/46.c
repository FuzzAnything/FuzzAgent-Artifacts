// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:157041:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
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

static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;
static size_t g_fuzz_offset = 0;

static uint8_t read_byte(void) {
    if (g_fuzz_offset >= g_fuzz_size) {
        return 0;
    }
    return g_fuzz_data[g_fuzz_offset++];
}

static int read_int(void) {
    int value = 0;
    for (int i = 0; i < 4 && g_fuzz_offset < g_fuzz_size; i++) {
        value = (value << 8) | g_fuzz_data[g_fuzz_offset++];
    }
    return value;
}

static const char *read_string(void) {
    static char buf[256];
    size_t i = 0;
    while (i < sizeof(buf) - 1 && g_fuzz_offset < g_fuzz_size) {
        uint8_t c = g_fuzz_data[g_fuzz_offset++];
        if (c == 0) break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

static sqlite3 *create_test_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return db;
}

static void test_sqlite3_vtab_config(sqlite3 *db) {
    if (!db) return;
    
    int op = read_int() % 10;
    int arg = read_int();
    
    sqlite3_vtab_config(db, op, arg);
}

static void test_sqlite3_vtab_collation(void) {
    sqlite3_index_info idx_info = {0};
    int nConstraint = read_int() % 10;
    
    idx_info.nConstraint = nConstraint;
    
    for (int i = 0; i < nConstraint; i++) {
        const char *collation = sqlite3_vtab_collation(&idx_info, i);
        (void)collation;
    }
}

static void test_sqlite3_vtab_distinct(void) {
    sqlite3_index_info idx_info = {0};
    int nOrderBy = read_int() % 10;
    
    idx_info.nOrderBy = nOrderBy;
    
    int distinct = sqlite3_vtab_distinct(&idx_info);
    (void)distinct;
}

static void test_sqlite3_vtab_in(void) {
    sqlite3_index_info idx_info = {0};
    int nConstraint = read_int() % 10;
    
    idx_info.nConstraint = nConstraint;
    
    for (int i = 0; i < nConstraint; i++) {
        int bHandle = read_int() % 3 - 1;
        int result = sqlite3_vtab_in(&idx_info, i % nConstraint, bHandle);
        (void)result;
    }
}

static void test_sqlite3_vtab_rhs_value(void) {
    sqlite3_index_info idx_info = {0};
    int nConstraint = read_int() % 10;
    
    idx_info.nConstraint = nConstraint;
    
    for (int i = 0; i < nConstraint; i++) {
        sqlite3_value *val = NULL;
        int rc = sqlite3_vtab_rhs_value(&idx_info, i % nConstraint, &val);
        (void)rc;
        (void)val;
    }
}

static void test_sqlite3_overload_function(sqlite3 *db) {
    if (!db) return;
    
    const char *func_name = read_string();
    int nArg = read_int() % 10;
    
    sqlite3_overload_function(db, func_name, nArg);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    g_fuzz_offset = 0;
    
    sqlite3 *db = create_test_db();
    if (!db) {
        return 0;
    }
    
    test_sqlite3_vtab_config(db);
    
    g_fuzz_offset = 0;
    test_sqlite3_vtab_collation();
    
    g_fuzz_offset = 0;
    test_sqlite3_vtab_distinct();
    
    g_fuzz_offset = 0;
    test_sqlite3_vtab_in();
    
    g_fuzz_offset = 0;
    test_sqlite3_vtab_rhs_value();
    
    g_fuzz_offset = 0;
    test_sqlite3_overload_function(db);
    
    sqlite3_close(db);
    
    return 0;
}