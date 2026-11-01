// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* 
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_initialize -> sqlite3_open -> sqlite3_malloc64 -> sqlite3_deserialize 
 * -> sqlite3_file_control -> sqlite3_exec
 */

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    void *allocated_buf = NULL;
    unsigned char *deserialize_buf = NULL;
    sqlite3_int64 buf_size = 0;
    int rc;

    /* Step 1: Initialize SQLite */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Step 2: Open a database connection (using in-memory to avoid file system issues) */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* Step 3: Allocate memory using sqlite3_malloc64 */
    if (Size > 0) {
        /* Use input size to determine allocation size, cap it to avoid huge allocations */
        sqlite3_uint64 alloc_size = (sqlite3_uint64)Size;
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
        allocated_buf = sqlite3_malloc64(alloc_size);
        if (allocated_buf != NULL) {
            /* Fill allocated memory with fuzz data to ensure it's used */
            memcpy(allocated_buf, Data, (alloc_size < Size) ? alloc_size : Size);
        }
    }

    /* Step 4: Prepare for sqlite3_deserialize */
    /* We need a valid database header. If input is too small, create a minimal valid header. */
    /* A minimal SQLite header is 100 bytes. */
    if (Size < 100) {
        /* Create a minimal valid SQLite header buffer */
        deserialize_buf = (unsigned char *)sqlite3_malloc64(100);
        if (deserialize_buf == NULL) {
            goto cleanup;
        }
        memset(deserialize_buf, 0, 100);
        /* Set magic string "SQLite format 3\000" */
        memcpy(deserialize_buf, "SQLite format 3\000", 16);
        /* Set page size (2 bytes at offset 16) to 4096 (0x1000) */
        deserialize_buf[16] = 0x10;
        deserialize_buf[17] = 0x00;
        /* Set file format write version (1) and read version (1) at offset 18, 19 */
        deserialize_buf[18] = 1;
        deserialize_buf[19] = 1;
        /* Set reserved space per page (0) at offset 20 */
        deserialize_buf[20] = 0;
        /* Set max embedded payload fraction (64) at offset 21 */
        deserialize_buf[21] = 64;
        /* Set min embedded payload fraction (32) at offset 22 */
        deserialize_buf[22] = 32;
        /* Set leaf payload fraction (32) at offset 23 */
        deserialize_buf[23] = 32;
        /* Set file change counter at offset 24 (4 bytes) */
        deserialize_buf[24] = 1;
        deserialize_buf[25] = 0;
        deserialize_buf[26] = 0;
        deserialize_buf[27] = 0;
        /* Set database size in pages at offset 28 (4 bytes) */
        deserialize_buf[28] = 1;
        deserialize_buf[29] = 0;
        deserialize_buf[30] = 0;
        deserialize_buf[31] = 0;
        /* Set first freelist trunk page (0) at offset 32 */
        deserialize_buf[32] = 0;
        deserialize_buf[33] = 0;
        deserialize_buf[34] = 0;
        deserialize_buf[35] = 0;
        /* Set total number of freelist pages (0) at offset 36 */
        deserialize_buf[36] = 0;
        deserialize_buf[37] = 0;
        deserialize_buf[38] = 0;
        deserialize_buf[39] = 0;
        /* Set schema cookie at offset 40 */
        deserialize_buf[40] = 1;
        deserialize_buf[41] = 0;
        deserialize_buf[42] = 0;
        deserialize_buf[43] = 0;
        /* Set schema format number (4) at offset 44 */
        deserialize_buf[44] = 4;
        deserialize_buf[45] = 0;
        deserialize_buf[46] = 0;
        deserialize_buf[47] = 0;
        /* Set default page cache size (0) at offset 48 */
        deserialize_buf[48] = 0;
        deserialize_buf[49] = 0;
        deserialize_buf[50] = 0;
        deserialize_buf[51] = 0;
        /* Set largest root b-tree page number (0) at offset 52 */
        deserialize_buf[52] = 0;
        deserialize_buf[53] = 0;
        deserialize_buf[54] = 0;
        deserialize_buf[55] = 0;
        /* Set text encoding (1 for UTF-8) at offset 56 */
        deserialize_buf[56] = 1;
        deserialize_buf[57] = 0;
        deserialize_buf[58] = 0;
        deserialize_buf[59] = 0;
        /* Set user version at offset 60 */
        deserialize_buf[60] = 0;
        deserialize_buf[61] = 0;
        deserialize_buf[62] = 0;
        deserialize_buf[63] = 0;
        /* Set incremental vacuum mode (0) at offset 64 */
        deserialize_buf[64] = 0;
        deserialize_buf[65] = 0;
        deserialize_buf[66] = 0;
        deserialize_buf[67] = 0;
        /* Set application ID (0) at offset 68 */
        deserialize_buf[68] = 0;
        deserialize_buf[69] = 0;
        deserialize_buf[70] = 0;
        deserialize_buf[71] = 0;
        /* Set reserved (0) at offset 72-91 */
        memset(&deserialize_buf[72], 0, 20);
        /* Set version-valid-for number (0) at offset 92 */
        deserialize_buf[92] = 0;
        deserialize_buf[93] = 0;
        deserialize_buf[94] = 0;
        deserialize_buf[95] = 0;
        /* Set SQLite version number (0) at offset 96 */
        deserialize_buf[96] = 0;
        deserialize_buf[97] = 0;
        deserialize_buf[98] = 0;
        deserialize_buf[99] = 0;
        buf_size = 100;
    } else {
        /* Use input data directly if it's large enough */
        deserialize_buf = (unsigned char *)Data;
        buf_size = Size;
    }

    /* Step 4: Deserialize database */
    /* Note: We pass NULL for zSchema to use "main". 
       We set flags to 0 (no FREEONCLOSE, no RESIZEABLE) to manage memory manually. */
    if (deserialize_buf != NULL) {
        rc = sqlite3_deserialize(db, NULL, deserialize_buf, buf_size, buf_size, 0);
        /* Ignore return code, we want to explore states even on error */
    }

    /* Step 5: File Control */
    /* Try different opcodes and db names */
    void *file_ptr = NULL;
    
    /* Try SQLITE_FCNTL_FILE_POINTER on "main" */
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_FILE_POINTER, &file_ptr);
    
    /* Try SQLITE_FCNTL_FILE_POINTER on NULL (main) */
    file_ptr = NULL;
    rc = sqlite3_file_control(db, NULL, SQLITE_FCNTL_FILE_POINTER, &file_ptr);

    /* Try SQLITE_FCNTL_VFS_POINTER on "main" */
    void *vfs_ptr = NULL;
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_VFS_POINTER, &vfs_ptr);

    /* Try an invalid db name to trigger SQLITE_ERROR */
    rc = sqlite3_file_control(db, "invalid_db_name", SQLITE_FCNTL_FILE_POINTER, &file_ptr);

    /* Step 6: Execute SQL */
    /* Construct SQL from input data, ensuring it's null-terminated and safe */
    char *sql = NULL;
    size_t sql_len = Size;
    
    /* Ensure we don't use more than available memory */
    if (sql_len > 0) {
        /* Find first null byte to truncate SQL if present */
        size_t null_pos = strnlen((const char *)Data, Size);
        if (null_pos < Size) {
            sql_len = null_pos;
        }
        
        /* Allocate space for SQL + null terminator */
        sql = (char *)sqlite3_malloc64(sql_len + 1);
        if (sql != NULL) {
            memcpy(sql, Data, sql_len);
            sql[sql_len] = '\0';
            
            /* Execute SQL */
            rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
            
            /* Free error message if set */
            if (errmsg != NULL) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            
            sqlite3_free(sql);
        }
    }

    /* Cleanup */
cleanup:
    if (allocated_buf != NULL) {
        sqlite3_free(allocated_buf);
    }
    /* Note: deserialize_buf points to either malloc'd memory or input Data. 
       If malloc'd, we must free it. If it's Data, we don't free it. */
    if (Size < 100 && deserialize_buf != NULL && deserialize_buf != (unsigned char *)Data) {
        sqlite3_free(deserialize_buf);
    }

    if (db != NULL) {
        sqlite3_close(db);
    }

    sqlite3_shutdown();
    return 0;
}