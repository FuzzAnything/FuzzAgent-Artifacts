// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Custom destructor for testing xDel parameter */
static void custom_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/* Helper to get a safe index from fuzz data */
static int get_index(size_t *offset, size_t size, int max, const uint8_t *Data) {
    if (*offset >= size) return 1;
    int idx = Data[*offset] % max + 1;
    if (idx > max) idx = max;
    if (idx < 1) idx = 1;
    (*offset)++;
    return idx;
}

/* Helper to get a safe length */
static int get_length(size_t *offset, size_t size, int max_len, const uint8_t *Data) {
    if (*offset + 2 > size) return 0;
    int len = (Data[*offset] << 8) | Data[*offset + 1];
    (*offset) += 2;
    if (len > max_len) len = max_len;
    if (len < 0) len = 0;
    return len;
}

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int param_count;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to bind data to */
    const char *sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, col_text TEXT, col_blob BLOB, col_ptr BLOB);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an INSERT statement with placeholders */
    const char *insert_sql = "INSERT INTO test_table (id, col_text, col_blob, col_ptr) VALUES (?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count == 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    size_t offset = 0;
    int max_len = 1024 * 1024; /* 1MB limit for fuzzing safety */

    /* Fuzz sqlite3_bind_text64 */
    if (offset < Size) {
        int idx = get_index(&offset, Size, param_count, Data);
        int len = get_length(&offset, Size, max_len, Data);
        int encoding = (offset < Size) ? Data[offset] % 4 : 0;
        if (offset < Size) offset++;
        
        /* Map encoding constants */
        int enc_val = SQLITE_UTF8;
        if (encoding == 1) enc_val = SQLITE_UTF16;
        else if (encoding == 2) enc_val = SQLITE_UTF16BE;
        else if (encoding == 3) enc_val = SQLITE_UTF16LE;

        const char *text_ptr = NULL;
        if (len > 0 && offset + len <= Size) {
            text_ptr = (const char*)&Data[offset];
            offset += len;
        }

        int del_mode = (offset < Size) ? Data[offset] : 0;
        if (offset < Size) offset++;
        
        void (*del_func)(void*) = NULL;
        if (del_mode == 0) del_func = SQLITE_STATIC;
        else if (del_mode == 1) del_func = SQLITE_TRANSIENT;
        else if (del_mode == 2) {
            /* Allocate memory to test custom destructor */
            char *buf = NULL;
            if (len > 0) buf = malloc(len + 1);
            if (buf) {
                if (text_ptr) {
                    memcpy(buf, text_ptr, len);
                    buf[len] = 0;
                }
                text_ptr = buf;
                del_func = custom_destructor;
            } else {
                del_func = SQLITE_STATIC;
            }
        } else {
            del_func = SQLITE_TRANSIENT;
        }

        rc = sqlite3_bind_text64(stmt, idx, text_ptr, (sqlite3_uint64)len, del_func, enc_val);
        (void)rc;
    }

    /* Fuzz sqlite3_bind_blob */
    if (offset < Size) {
        int idx = get_index(&offset, Size, param_count, Data);
        int len = get_length(&offset, Size, max_len, Data);
        
        const void *blob_ptr = NULL;
        if (len > 0 && offset + len <= Size) {
            blob_ptr = (const void*)&Data[offset];
            offset += len;
        }

        int del_mode = (offset < Size) ? Data[offset] : 0;
        if (offset < Size) offset++;
        
        void (*del_func)(void*) = NULL;
        if (del_mode == 0) del_func = SQLITE_STATIC;
        else if (del_mode == 1) del_func = SQLITE_TRANSIENT;
        else if (del_mode == 2) {
            void *buf = NULL;
            if (len > 0) buf = malloc(len);
            if (buf) {
                if (blob_ptr) {
                    memcpy(buf, blob_ptr, len);
                }
                blob_ptr = buf;
                del_func = custom_destructor;
            } else {
                del_func = SQLITE_STATIC;
            }
        } else {
            del_func = SQLITE_TRANSIENT;
        }

        rc = sqlite3_bind_blob(stmt, idx, blob_ptr, len, del_func);
        (void)rc;
    }

    /* Fuzz sqlite3_bind_pointer */
    if (offset < Size) {
        int idx = get_index(&offset, Size, param_count, Data);
        /* Create a dummy pointer */
        void *ptr = (void*)&Data[offset];
        if (offset < Size) offset++;
        
        const char *type_name = "test_type";
        int del_mode = (offset < Size) ? Data[offset] : 0;
        if (offset < Size) offset++;
        
        void (*del_func)(void*) = NULL;
        if (del_mode == 0) del_func = NULL;
        else if (del_mode == 1) del_func = custom_destructor;
        else del_func = NULL;

        rc = sqlite3_bind_pointer(stmt, idx, ptr, type_name, del_func);
        (void)rc;
    }

    /* Fuzz sqlite3_bind_value */
    /* We can't easily construct an arbitrary sqlite3_value, so we test with NULL */
    if (offset < Size) {
        int idx = get_index(&offset, Size, param_count, Data);
        rc = sqlite3_bind_value(stmt, idx, NULL);
        (void)rc;
    }

    /* Fuzz sqlite3_bind_text16 */
    if (offset < Size) {
        int idx = get_index(&offset, Size, param_count, Data);
        int len = get_length(&offset, Size, max_len, Data);
        /* Ensure even length for UTF-16 */
        if (len % 2 != 0) len--;
        
        const void *text16_ptr = NULL;
        if (len > 0 && offset + len <= Size) {
            text16_ptr = (const void*)&Data[offset];
            offset += len;
        }

        int del_mode = (offset < Size) ? Data[offset] : 0;
        if (offset < Size) offset++;
        
        void (*del_func)(void*) = NULL;
        if (del_mode == 0) del_func = SQLITE_STATIC;
        else if (del_mode == 1) del_func = SQLITE_TRANSIENT;
        else if (del_mode == 2) {
            void *buf = NULL;
            if (len > 0) buf = malloc(len);
            if (buf) {
                if (text16_ptr) {
                    memcpy(buf, text16_ptr, len);
                }
                text16_ptr = buf;
                del_func = custom_destructor;
            } else {
                del_func = SQLITE_STATIC;
            }
        } else {
            del_func = SQLITE_TRANSIENT;
        }

        rc = sqlite3_bind_text16(stmt, idx, text16_ptr, len, del_func);
        (void)rc;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    (void)rc;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}