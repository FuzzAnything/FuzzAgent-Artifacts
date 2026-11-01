// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void custom_destructor(void *p) {
    (void)p;
    /* Custom destructor logic, intentionally empty for fuzzing */
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to allow binding and stepping */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB, text_col TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an INSERT statement with parameters */
    sql = "INSERT INTO test_table (id, data, text_col) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Fuzzing logic: bind various types of data based on input */
    int param_idx = 1;
    size_t offset = 0;
    
    /* Bind integer for id (parameter 1) */
    if (Size > 0) {
        int id_val = Data[offset++];
        sqlite3_bind_int(stmt, param_idx++, id_val);
    }

    /* Bind blob for data (parameter 2) using sqlite3_bind_blob */
    if (Size > offset) {
        int blob_len = (Size - offset) > 1024 ? 1024 : (int)(Size - offset);
        if (blob_len < 0) blob_len = 0;
        
        /* Try different destructor options */
        void (*destructor)(void*) = NULL;
        /* Check bounds before accessing Data[offset + blob_len] */
        if (Size > offset + blob_len) {
            if (Data[offset + blob_len] % 3 == 0) destructor = SQLITE_TRANSIENT;
            else if (Data[offset + blob_len] % 3 == 1) destructor = SQLITE_STATIC;
            else destructor = custom_destructor;
        } else {
            destructor = SQLITE_TRANSIENT;
        }
        
        rc = sqlite3_bind_blob(stmt, param_idx++, (const void*)(Data + offset), blob_len, destructor);
        if (rc != SQLITE_OK) {
            sqlite3_reset(stmt);
        }
        offset += blob_len;
    }

    /* Bind text for text_col (parameter 3) using sqlite3_bind_text64 */
    if (Size > offset) {
        int text_len = (Size - offset) > 1024 ? 1024 : (int)(Size - offset);
        if (text_len < 0) text_len = 0;
        
        unsigned char encoding = SQLITE_UTF8;
        /* Check bounds before accessing Data[offset + text_len] */
        if (Size > offset + text_len) {
            switch (Data[offset + text_len] % 4) {
                case 0: encoding = SQLITE_UTF8; break;
                case 1: encoding = SQLITE_UTF16; break;
                case 2: encoding = SQLITE_UTF16BE; break;
                case 3: encoding = SQLITE_UTF16LE; break;
            }
        }
        
        void (*text_destructor)(void*) = SQLITE_TRANSIENT;
        rc = sqlite3_bind_text64(stmt, param_idx++, (const char*)(Data + offset), text_len, text_destructor, encoding);
        if (rc != SQLITE_OK) {
            sqlite3_reset(stmt);
        }
        offset += text_len;
    }

    /* Additional fuzzing: try sqlite3_bind_text16 with native encoding */
    if (Size > offset) {
        int utf16_len = (Size - offset) > 512 ? 512 : (int)(Size - offset);
        if (utf16_len % 2 != 0) utf16_len--; /* Ensure even length for UTF-16 */
        if (utf16_len < 0) utf16_len = 0;
        
        rc = sqlite3_bind_text16(stmt, param_idx, (const void*)(Data + offset), utf16_len, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            sqlite3_reset(stmt);
        }
    }

    /* Additional fuzzing: try sqlite3_bind_pointer */
    if (Size > offset) {
        void *ptr = (void*)(uintptr_t)(Data[offset] & 0xFF);
        rc = sqlite3_bind_pointer(stmt, param_idx, ptr, "FUZZ_TYPE", custom_destructor);
        if (rc != SQLITE_OK) {
            sqlite3_reset(stmt);
        }
    }

    /* Additional fuzzing: try sqlite3_bind_value with a constructed value */
    if (Size > offset) {
        sqlite3_value *val = NULL;
        /* Create a dummy value by binding first then extracting (simplified) */
        /* Note: Direct construction of sqlite3_value is not possible externally, 
         * so we simulate by using a previous bind if possible, or skip.
         * For fuzzing, we can try to bind a NULL or use a value from column if we had a SELECT.
         * Here we skip direct sqlite3_value construction as it requires internal API.
         */
        (void)val; /* Suppress unused warning */
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    
    /* Handle execution results */
    if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
        /* Success, reset for potential reuse if we were looping */
        sqlite3_reset(stmt);
    } else if (rc != SQLITE_OK) {
        /* Error occurred, reset statement */
        sqlite3_reset(stmt);
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}