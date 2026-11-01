// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Static helper to safely close a database connection.
 */
static void safe_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Static helper to extract a 32-bit integer from the fuzz input.
 * Returns 0 if the size is insufficient.
 */
static int get_int(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) return 0;
    int val = (int)data[*offset] | ((int)data[*offset + 1] << 8) |
              ((int)data[*offset + 2] << 16) | ((int)data[*offset + 3] << 24);
    *offset += 4;
    return val;
}

/*
 * Static helper to extract a 64-bit integer from the fuzz input.
 */
static sqlite3_int64 get_int64(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 8 > size) return 0;
    sqlite3_int64 val = (sqlite3_int64)data[*offset] |
                        ((sqlite3_int64)data[*offset + 1] << 8) |
                        ((sqlite3_int64)data[*offset + 2] << 16) |
                        ((sqlite3_int64)data[*offset + 3] << 24) |
                        ((sqlite3_int64)data[*offset + 4] << 32) |
                        ((sqlite3_int64)data[*offset + 5] << 40) |
                        ((sqlite3_int64)data[*offset + 6] << 48) |
                        ((sqlite3_int64)data[*offset + 7] << 56);
    *offset += 8;
    return val;
}

/*
 * Static trace callback for sqlite3_trace_v2.
 */
static int trace_callback(unsigned uEvent, void *pArg, void *p1, void *p2) {
    (void)uEvent;
    (void)pArg;
    (void)p1;
    (void)p2;
    return 0;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;

    /* 
     * 1. Prepare the environment: Open a database.
     * We use ":memory:" to avoid file system dependencies, but we can
     * also try to use the fuzz data as a filename if it looks like a string.
     */
    if (Size > 0) {
        /* Try to interpret the beginning of the buffer as a filename, 
         * but ensure it's null-terminated or safe. 
         * For simplicity in fuzzing, we often just use ":memory:" or a dummy file.
         * Let's try to use the data as a filename if it's short and printable,
         * otherwise default to ":memory:".
         */
        const char *filename = ":memory:";
        
        /* Check if data looks like a valid filename (no null bytes in first 100 chars) */
        int valid_name = 1;
        int name_len = 0;
        if (Size > 100) name_len = 100;
        else name_len = (int)Size;
        
        for (int i = 0; i < name_len; i++) {
            if (Data[i] == '\0') {
                valid_name = 0;
                break;
            }
        }
        
        if (valid_name && name_len > 0) {
            /* Create a temporary buffer for the filename */
            char *tmp_name = malloc(name_len + 1);
            if (tmp_name) {
                memcpy(tmp_name, Data, name_len);
                tmp_name[name_len] = '\0';
                rc = sqlite3_open(tmp_name, &db);
                free(tmp_name);
            }
        } else {
            rc = sqlite3_open(":memory:", &db);
        }
    } else {
        rc = sqlite3_open(":memory:", &db);
    }

    if (rc != SQLITE_OK && db != NULL) {
        /* If open failed but we got a handle, we can still test error functions */
        safe_close(db);
        db = NULL;
    }

    /* 
     * 2. Invoke target functions with diverse states.
     */
    
    /* --- sqlite3_errcode --- */
    if (db) {
        int err = sqlite3_errcode(db);
        (void)err; /* Suppress unused variable warning */
    }

    /* --- sqlite3_error_offset --- */
    if (db) {
        int off = sqlite3_error_offset(db);
        (void)off;
    }

    /* --- sqlite3_db_config --- */
    if (db && Size >= 12) {
        int op = get_int(Data, &offset, Size);
        int arg1 = get_int(Data, &offset, Size);
        int arg2 = get_int(Data, &offset, Size);
        
        /* Handle SQLITE_DBCONFIG_LOOKASIDE specifically as per instructions */
        if (op == SQLITE_DBCONFIG_LOOKASIDE) {
            /* arg1 is size, arg2 is count */
            int sz = arg1;
            int cnt = arg2;
            
            /* Ensure sz is multiple of sizeof(void*) and > 0 */
            if (sz <= 0) sz = sizeof(void*);
            sz = (sz / sizeof(void*)) * sizeof(void*);
            if (sz == 0) sz = sizeof(void*);
            
            /* Ensure cnt is non-negative */
            if (cnt < 0) cnt = 0;
            
            /* Allocate a valid buffer for the lookaside */
            void *buf = malloc(sz * (cnt + 1));
            if (buf) {
                rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, buf, sz, cnt);
                free(buf);
            } else {
                /* If malloc fails, we can't test this specific config safely */
                (void)rc;
            }
        } else {
            /* For other operations, pass args directly */
            rc = sqlite3_db_config(db, op, arg1, arg2);
        }
    }

    /* --- sqlite3_db_status --- */
    if (db && Size >= 4) {
        int op = get_int(Data, &offset, Size);
        int cur = 0;
        int hiwtr = 0;
        int reset = (Size > 4) ? (Data[offset % 2] ? 1 : 0) : 0;
        
        rc = sqlite3_db_status(db, op, &cur, &hiwtr, reset);
        (void)cur;
        (void)hiwtr;
    }

    /* --- sqlite3_trace_v2 --- */
    if (db && Size >= 4) {
        unsigned mask = (unsigned)get_int(Data, &offset, Size);
        /* We use a static callback defined above */
        rc = sqlite3_trace_v2(db, mask, trace_callback, (void*)(size_t)mask);
    }

    /* 
     * Try to trigger an error to test error reporting functions more thoroughly.
     * We prepare a statement with potentially malformed SQL from the input.
     */
    if (db && Size > 0) {
        sqlite3_stmt *stmt = NULL;
        /* Ensure null termination for SQL string */
        char *sql = malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            
            if (rc != SQLITE_OK) {
                /* Error occurred, test error functions */
                int err_code = sqlite3_errcode(db);
                int err_off = sqlite3_error_offset(db);
                const char *err_msg = sqlite3_errmsg(db);
                (void)err_code;
                (void)err_off;
                (void)err_msg;
            }
            
            if (stmt) {
                sqlite3_finalize(stmt);
            }
            free(sql);
        }
    }

    /* 
     * 3. Cleanup.
     */
    safe_close(db);

    return 0;
}