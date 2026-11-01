// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Callback function for sqlite3_exec.
 * Returns 0 to continue execution.
 */
static int exec_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment: Initialize SQLite */
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    void *malloced_ptr = NULL;
    unsigned char *deserialize_buf = NULL;
    sqlite3_int64 db_size = 0;
    sqlite3_int64 buf_size = 0;

    /* 
     * Create a dummy file if needed, though we will primarily use in-memory 
     * or the specific dummy_file path as requested.
     */
    const char *filename = "./dummy_file";

    /* 2. Open database */
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }

    /* 3. Invoke sqlite3_malloc64 */
    /* Use input data to determine size, ensuring it's within reasonable bounds */
    if (Size >= 2) {
        sqlite3_uint64 req_size = ((sqlite3_uint64)Data[0] << 8) | Data[1];
        /* Cap size to prevent OOM during fuzzing, but allow large enough to test 64-bit logic */
        if (req_size > 1024 * 1024) {
            req_size = 1024 * 1024;
        }
        malloced_ptr = sqlite3_malloc64(req_size);
        if (malloced_ptr != NULL) {
            /* Write some data to the allocated buffer to ensure it's touched */
            memset(malloced_ptr, 0x42, req_size);
        }
    }

    /* 4. Invoke sqlite3_deserialize */
    /* 
     * We need a valid serialized database header. 
     * A minimal valid SQLite header is 100 bytes. 
     * If input is large enough, we try to use a portion as the buffer.
     * Otherwise, we allocate a minimal valid header.
     */
    if (Size >= 100) {
        deserialize_buf = (unsigned char *)Data;
        db_size = Size;
        buf_size = Size;
    } else {
        /* Allocate a minimal buffer with a valid-ish header for testing */
        deserialize_buf = (unsigned char *)sqlite3_malloc64(100);
        if (deserialize_buf) {
            /* Write a minimal valid SQLite header signature */
            memcpy(deserialize_buf, "SQLite format 3\000", 16);
            memset(deserialize_buf + 16, 0, 84);
            /* Set page size (bytes 16-17) to 4096 */
            deserialize_buf[16] = 0x10;
            deserialize_buf[17] = 0x00;
            /* Set file format write version (byte 18) to 1 */
            deserialize_buf[18] = 1;
            /* Set file format read version (byte 19) to 1 */
            deserialize_buf[19] = 1;
            db_size = 100;
            buf_size = 100;
        }
    }

    if (deserialize_buf != NULL) {
        /* 
         * Attempt to deserialize into the 'main' schema.
         * Note: This might fail if the buffer isn't a valid full DB, 
         * which is expected behavior for fuzzing.
         */
        rc = sqlite3_deserialize(db, "main", deserialize_buf, db_size, buf_size, 0);
        /* Ignore return code for fuzzing exploration, but ensure we don't leak if we allocated */
        if (Size < 100 && deserialize_buf != (unsigned char *)Data) {
            sqlite3_free(deserialize_buf);
        }
    }

    /* 5. Invoke sqlite3_file_control */
    /* 
     * Try a few opcodes. SQLITE_FCNTL_FILE_POINTER is safe to try.
     * We pass a dummy pointer to receive the result.
     */
    sqlite3_file *file_ptr = NULL;
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_FILE_POINTER, &file_ptr);
    (void)rc; /* Ignore result for fuzzing flow */

    /* 6. Invoke sqlite3_exec */
    /* 
     * Construct a simple SQL string from input or use a default.
     * We must ensure the SQL is valid UTF-8 and doesn't contain nulls in the middle 
     * unless we handle it carefully. For fuzzing, we'll create a safe SQL string.
     */
    const char *sql = "SELECT 1;";
    if (Size > 0) {
        /* Try to interpret input as SQL, but ensure it's null-terminated and safe */
        /* We limit the length to avoid huge allocations in sqlite3_exec */
        size_t sql_len = (Size < 1000) ? Size : 1000;
        char *safe_sql = (char *)sqlite3_malloc64(sql_len + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, sql_len);
            safe_sql[sql_len] = '\0';
            /* Remove any embedded nulls to make it a valid C string for exec */
            /* Actually, sqlite3_exec expects a null-terminated string. 
               If input has nulls, it truncates. This is fine for fuzzing. */
            rc = sqlite3_exec(db, safe_sql, exec_callback, NULL, &errmsg);
            if (errmsg != NULL) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            sqlite3_free(safe_sql);
        }
    } else {
        rc = sqlite3_exec(db, sql, exec_callback, NULL, &errmsg);
        if (errmsg != NULL) {
            sqlite3_free(errmsg);
        }
    }

    /* Cleanup */
    if (malloced_ptr != NULL) {
        sqlite3_free(malloced_ptr);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }
    sqlite3_shutdown();

    return 0;
}