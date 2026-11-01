// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_libversion_number at sqlite3.c:185744:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int read_int(const uint8_t **data, size_t *size, size_t *idx) {
    if (*idx + sizeof(int) > *size) return 0;
    int val;
    memcpy(&val, &(*data)[*idx], sizeof(int));
    *idx += sizeof(int);
    return val;
}

static sqlite3_int64 read_int64(const uint8_t **data, size_t *size, size_t *idx) {
    if (*idx + sizeof(sqlite3_int64) > *size) return 0;
    sqlite3_int64 val;
    memcpy(&val, &(*data)[*idx], sizeof(sqlite3_int64));
    *idx += sizeof(sqlite3_int64);
    return val;
}

static const char* read_text(const uint8_t **data, size_t *size, size_t *idx, int *len_out) {
    if (*idx + 1 > *size) {
        *len_out = 0;
        return "";
    }
    int len = (*data)[*idx];
    (*idx)++;
    if (len == 0 || *idx + len > *size) {
        *len_out = 0;
        return "";
    }
    *len_out = len;
    return (const char*)&(*data)[*idx];
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO test_table VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    if (Size < 10) {
        return 0;
    }

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    const char *create_table_sql = "CREATE TABLE test_table (c1 INTEGER, c2 INTEGER, c3 TEXT, c4 INTEGER, c5 INTEGER, c6 TEXT, c7 INTEGER, c8 INTEGER, c9 TEXT, c10 INTEGER, c11 INTEGER, c12 TEXT, c13 TEXT, c14 TEXT, c15 INTEGER, c16 INTEGER, c17 INTEGER, c18 INTEGER, c19 TEXT, c20 INTEGER, c21 INTEGER, c22 INTEGER, c23 INTEGER, c24 INTEGER, c25 INTEGER, c26 INTEGER, c27 INTEGER, c28 INTEGER, c29 INTEGER, c30 TEXT, c31 INTEGER);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    size_t idx = 0;
    int param_idx = 1;
    int text_len;

    // Bind sequence as specified
    sqlite3_bind_int64(stmt, param_idx++, read_int64(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int64(stmt, param_idx++, read_int64(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int64(stmt, param_idx++, read_int64(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    
    // Call sqlite3_libversion_number
    sqlite3_libversion_number();
    
    sqlite3_bind_int(stmt, param_idx++, read_int(&Data, &Size, &idx));
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, param_idx++, read_text(&Data, &Size, &idx, &text_len), text_len, SQLITE_TRANSIENT);

    // Execute the statement
    sqlite3_step(stmt);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}