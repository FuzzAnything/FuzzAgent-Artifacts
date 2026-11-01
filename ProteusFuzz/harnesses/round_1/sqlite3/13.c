// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
}

static int setup_db(sqlite3 **db_out) {
    int rc;
    rc = sqlite3_open(":memory:", db_out);
    if (rc != SQLITE_OK) {
        return -1;
    }
    const char *create_sql = "CREATE TABLE test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(*db_out, create_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }
    const char *insert_sql = "INSERT INTO test_blob (id, data) VALUES (1, X'DEADBEEFCAFE0000');";
    rc = sqlite3_prepare_v2(*db_out, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *err_msg;
    int rc;

    if (Size < 4) {
        return 0;
    }

    if (setup_db(&db) != 0) {
        return 0;
    }

    int nArg = (int)Data[0] % 5 - 2; 
    int eTextRep = (Data[1] % 3) * 100; 
    
    char safe_func_name[256] = {0};
    if (Size > 10) {
        size_t copy_len = (Size - 2) < 255 ? (Size - 2) : 255;
        memcpy(safe_func_name, &Data[2], copy_len);
    } else {
        strcpy(safe_func_name, "fuzz_func");
    }
    const char *func_name = safe_func_name;

    // 1. sqlite3_blob_write
    sqlite3_stmt *prep_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test_blob WHERE id = 1", -1, &prep_stmt, NULL);
    if (rc == SQLITE_OK && sqlite3_step(prep_stmt) == SQLITE_ROW) {
        rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, SQLITE_OPEN_READWRITE, &blob);
    }
    if (prep_stmt) sqlite3_finalize(prep_stmt);

    if (blob) {
        int n = (int)Data[Size - 2] % 16;
        int offset = (int)Data[Size - 1] % 16;
        int blob_size = sqlite3_blob_bytes(blob);
        
        // Ensure we do not write beyond the BLOB size
        if (blob_size > 0 && n > 0 && offset >= 0) {
            if (offset + n > blob_size) {
                n = blob_size - offset;
                if (n <= 0) n = 0;
            }
            
            // Ensure write_buf is valid and points to readable memory
            // If Size is small, use a static buffer. If large, use input data.
            const void *write_buf;
            if (Size > 16) {
                write_buf = &Data[10];
            } else {
                static char static_buf[16] = "AAAAAAAAAAAAAAAA";
                write_buf = static_buf;
            }
            
            rc = sqlite3_blob_write(blob, write_buf, n, offset);
        }
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    // 2. sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    // Re-open blob for read test
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test_blob WHERE id = 1", -1, &prep_stmt, NULL);
    if (rc == SQLITE_OK && sqlite3_step(prep_stmt) == SQLITE_ROW) {
        rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, SQLITE_OPEN_READONLY, &blob);
    }
    if (prep_stmt) sqlite3_finalize(prep_stmt);

    // 3. sqlite3_blob_read
    if (blob) {
        char read_buf[16] = {0};
        int n_read = (int)Data[Size - 2] % 16;
        int offset_read = (int)Data[Size - 1] % 16;
        int blob_size = sqlite3_blob_bytes(blob);

        if (blob_size > 0 && n_read > 0 && offset_read >= 0) {
            if (offset_read + n_read > blob_size) {
                n_read = blob_size - offset_read;
                if (n_read <= 0) n_read = 0;
            }
            rc = sqlite3_blob_read(blob, read_buf, n_read, offset_read);
        }
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    // 4. sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    // 5. sqlite3_create_function
    rc = sqlite3_create_function(db, func_name, nArg, eTextRep, NULL, dummy_func, NULL, NULL);
    (void)rc;

    // 6. sqlite3_prepare_v2
    char safe_sql[1001] = {0};
    if (Size > 20) {
        int sql_len = (int)(Size - 20);
        if (sql_len > 1000) sql_len = 1000;
        memcpy(safe_sql, &Data[20], sql_len);
    } else {
        strcpy(safe_sql, "SELECT 1");
    }
    const char *sql = safe_sql;
    int sql_len = (int)strlen(sql);

    rc = sqlite3_prepare_v2(db, sql, sql_len, &stmt, NULL);
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return 0;
}