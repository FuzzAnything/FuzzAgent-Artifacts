// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a minimal valid SQLite database header
 * to ensure sqlite3_deserialize has valid input data.
 */
static void create_minimal_db_header(unsigned char *buf, size_t size) {
    if (size < 100) return;

    /* SQLite file format header */
    const char *magic = "SQLite format 3\000";
    size_t magic_len = strlen(magic);

    if (size < magic_len) return;
    memcpy(buf, magic, magic_len);

    /* Page size (bytes 16-17): 4096 (0x1000) */
    buf[16] = 0x10;
    buf[17] = 0x00;

    /* File format write version (byte 18): 1 (rollback) */
    buf[18] = 0x01;
    /* File format read version (byte 19): 1 (rollback) */
    buf[19] = 0x01;

    /* Reserved space per page (byte 20): 0 */
    buf[20] = 0x00;

    /* Max embedded payload fraction (byte 21): 64 */
    buf[21] = 0x40;
    /* Min embedded payload fraction (byte 22): 32 */
    buf[22] = 0x20;
    /* Leaf payload fraction (byte 23): 32 */
    buf[23] = 0x20;

    /* File change counter (bytes 24-27): 1 */
    buf[24] = 0x00;
    buf[25] = 0x00;
    buf[26] = 0x00;
    buf[27] = 0x01;

    /* Database size in pages (bytes 28-31): 1 */
    buf[28] = 0x00;
    buf[29] = 0x00;
    buf[30] = 0x00;
    buf[31] = 0x01;

    /* First freelist trunk page (bytes 32-35): 0 */
    buf[32] = 0x00;
    buf[33] = 0x00;
    buf[34] = 0x00;
    buf[35] = 0x00;

    /* Total number of freelist pages (bytes 36-39): 0 */
    buf[36] = 0x00;
    buf[37] = 0x00;
    buf[38] = 0x00;
    buf[39] = 0x00;

    /* Schema cookie (bytes 40-43): 1 */
    buf[40] = 0x00;
    buf[41] = 0x00;
    buf[42] = 0x00;
    buf[43] = 0x01;

    /* Schema format number (bytes 44-47): 4 */
    buf[44] = 0x00;
    buf[45] = 0x00;
    buf[46] = 0x00;
    buf[47] = 0x04;

    /* Default page cache size (bytes 48-51): 0 */
    buf[48] = 0x00;
    buf[49] = 0x00;
    buf[50] = 0x00;
    buf[51] = 0x00;

    /* Largest root b-tree page (bytes 52-55): 0 */
    buf[52] = 0x00;
    buf[53] = 0x00;
    buf[54] = 0x00;
    buf[55] = 0x00;

    /* Text encoding (bytes 56-59): 1 (UTF-8) */
    buf[56] = 0x00;
    buf[57] = 0x00;
    buf[58] = 0x00;
    buf[59] = 0x01;

    /* User version (bytes 60-63): 0 */
    buf[60] = 0x00;
    buf[61] = 0x00;
    buf[62] = 0x00;
    buf[63] = 0x00;

    /* Incremental vacuum mode (bytes 64-67): 0 */
    buf[64] = 0x00;
    buf[65] = 0x00;
    buf[66] = 0x00;
    buf[67] = 0x00;

    /* Application ID (bytes 68-71): 0 */
    buf[68] = 0x00;
    buf[69] = 0x00;
    buf[70] = 0x00;
    buf[71] = 0x00;

    /* Reserved for expansion (bytes 72-91): 0 */
    memset(&buf[72], 0, 20);

    /* Version valid for number (bytes 92-95): 1 */
    buf[92] = 0x00;
    buf[93] = 0x00;
    buf[94] = 0x00;
    buf[95] = 0x01;

    /* SQLite version number (bytes 96-99): 3000000 */
    buf[96] = 0x2C;
    buf[97] = 0xB0;
    buf[98] = 0x00;
    buf[99] = 0x00;

    /* Fill rest with zeros to ensure valid page structure */
    if (size > 100) {
        memset(&buf[100], 0, size - 100);
    }
}

static int dummy_callback(void *data, int argc, char **argv, char **colv) {
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    unsigned char *serialized_buf = NULL;
    sqlite3_int64 db_size = 0;
    sqlite3_int64 buf_size = 0;
    unsigned int flags = 0;

    /* Step 1: Initialize SQLite */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Step 2: Open a database connection */
    /* Use ":memory:" to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* Step 3: Allocate memory using sqlite3_malloc64 */
    /* Use input data to determine allocation size, with bounds checking */
    if (Size > 0) {
        sqlite3_uint64 alloc_size = (sqlite3_uint64)Size;
        /* Cap allocation to prevent excessive memory usage */
        if (alloc_size > 1024 * 1024) { /* 1MB limit */
            alloc_size = 1024 * 1024;
        }
        if (alloc_size > 0) {
            serialized_buf = (unsigned char *)sqlite3_malloc64(alloc_size);
            if (serialized_buf) {
                /* Copy input data into buffer */
                size_t copy_len = (Size < alloc_size) ? Size : alloc_size;
                memcpy(serialized_buf, Data, copy_len);
                
                /* Ensure buffer is large enough for a minimal header */
                if (copy_len < 100) {
                    /* Only memset the remaining part up to 100 bytes */
                    size_t remaining = 100 - copy_len;
                    if (remaining < alloc_size - copy_len) {
                        memset(serialized_buf + copy_len, 0, remaining);
                    } else if (alloc_size > copy_len) {
                         memset(serialized_buf + copy_len, 0, alloc_size - copy_len);
                    }
                } else if (alloc_size > copy_len) {
                    /* If input was large enough, just zero out the rest of the buffer */
                    memset(serialized_buf + copy_len, 0, alloc_size - copy_len);
                }

                create_minimal_db_header(serialized_buf, (size_t)alloc_size);
                db_size = copy_len;
                buf_size = alloc_size;
            }
        }
    }

    /* Step 4: Deserialize the database */
    /* Only attempt if we have a valid buffer */
    if (serialized_buf && db_size > 0) {
        /* Determine flags based on input data to explore different states */
        if (Size > 0 && (Data[0] & 1)) {
            flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
        }
        if (Size > 1 && (Data[1] & 1)) {
            flags |= SQLITE_DESERIALIZE_RESIZEABLE;
        }
        
        rc = sqlite3_deserialize(db, "main", serialized_buf, db_size, buf_size, flags);
        /* Ignore return code for fuzzing, but ensure we don't use invalid buffer */
        if (rc != SQLITE_OK && (flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
            /* Buffer was freed by sqlite3_deserialize on error */
            serialized_buf = NULL;
        }
    }

    /* Step 5: File control */
    /* Use NULL for main database, SQLITE_FCNTL_FILE_POINTER as opcode */
    sqlite3_file *file_ptr = NULL;
    rc = sqlite3_file_control(db, NULL, SQLITE_FCNTL_FILE_POINTER, &file_ptr);
    /* Ignore return code */

    /* Step 6: Execute SQL */
    /* Use input data as SQL if it looks like text, otherwise use safe default */
    const char *sql = "SELECT 1;";
    if (Size > 0 && Size < 1024) {
        /* Check if data is printable ASCII */
        int is_printable = 1;
        for (size_t i = 0; i < Size; i++) {
            if (Data[i] < 32 || Data[i] > 126) {
                is_printable = 0;
                break;
            }
        }
        if (is_printable) {
            /* Create a safe SQL string from input */
            char *safe_sql = (char *)sqlite3_malloc64(Size + 2);
            if (safe_sql) {
                memcpy(safe_sql, Data, Size);
                safe_sql[Size] = '\0';
                /* Ensure it's a valid statement by appending a semicolon if needed */
                if (Size > 0 && safe_sql[Size-1] != ';') {
                    safe_sql[Size] = ';';
                    safe_sql[Size+1] = '\0';
                }
                rc = sqlite3_exec(db, safe_sql, dummy_callback, NULL, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
                sqlite3_free(safe_sql);
            }
        } else {
            /* Use default SQL for non-printable data */
            rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }
    } else {
        rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }

    /* Cleanup */
    sqlite3_close(db);
    sqlite3_shutdown();

    return 0;
}