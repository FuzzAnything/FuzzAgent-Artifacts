// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:90957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:90950:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    sqlite3_blob* blob = NULL;
    const char* errmsg = NULL;
    char* sql = NULL;
    int rc;
    size_t sql_len;
    int blob_size;
    char buffer[1024];
    size_t offset, write_size, read_size;

    if (Size < 4) return 0;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;

    rc = sqlite3_exec(db, "CREATE TABLE t1 (id INTEGER PRIMARY KEY, data BLOB)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_exec(db, "INSERT INTO t1 (data) VALUES (zeroblob(1024))", NULL, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_blob_open(db, "main", "t1", "data", 1, 0, &blob);
    if (rc != SQLITE_OK) goto cleanup;

    blob_size = sqlite3_blob_bytes(blob);
    if (blob_size <= 0) goto cleanup;

    offset = Data[0] % blob_size;
    write_size = Data[1] % sizeof(buffer);
    if (write_size > Size - 2) write_size = Size - 2;
    if (write_size > blob_size - offset) write_size = blob_size - offset;

    rc = sqlite3_blob_write(blob, Data + 2, write_size, offset);
    errmsg = sqlite3_errmsg(db);

    offset = Data[2] % blob_size;
    read_size = Data[3] % sizeof(buffer);
    if (read_size > blob_size - offset) read_size = blob_size - offset;

    rc = sqlite3_blob_read(blob, buffer, read_size, offset);
    errmsg = sqlite3_errmsg(db);

    sql_len = Size > 100 ? 100 : Size;
    sql = (char*)malloc(sql_len + 1);
    if (!sql) goto cleanup;
    memcpy(sql, Data, sql_len);
    sql[sql_len] = 0;

    rc = sqlite3_create_function(db, sql, -1, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);

    const char* tail = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT * FROM t1", -1, &stmt, &tail);
    if (stmt) sqlite3_finalize(stmt);

cleanup:
    if (sql) free(sql);
    if (blob) sqlite3_blob_close(blob);
    if (db) sqlite3_close(db);
    return 0;
}