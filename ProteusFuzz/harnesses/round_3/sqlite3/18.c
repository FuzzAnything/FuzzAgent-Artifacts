// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to free error messages returned by sqlite3_exec
 */
static void free_error_msg(char **errmsg) {
    if (errmsg && *errmsg) {
        sqlite3_free(*errmsg);
        *errmsg = NULL;
    }
}

/*
 * Fuzz driver for sqlite3 API functions
 * Order: sqlite3_initialize -> sqlite3_open -> sqlite3_malloc64 -> sqlite3_deserialize -> sqlite3_file_control -> sqlite3_exec
 */
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    void *malloced_ptr = NULL;
    unsigned char *deserialize_buf = NULL;
    int rc;

    /* 1. Initialize SQLite library */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* 2. Open database connection */
    /* Use in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* 3. Allocate memory using sqlite3_malloc64 */
    /* Use input data to determine size, with bounds checking */
    sqlite3_uint64 alloc_size = 0;
    if (Size >= sizeof(sqlite3_uint64)) {
        memcpy(&alloc_size, Data, sizeof(sqlite3_uint64));
        /* Limit allocation size to prevent OOM or excessive memory usage */
        if (alloc_size > 1024 * 1024) { /* 1MB limit */
            alloc_size = 1024 * 1024;
        }
    } else if (Size > 0) {
        alloc_size = (sqlite3_uint64)Size;
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    }

    malloced_ptr = sqlite3_malloc64(alloc_size);
    if (malloced_ptr != NULL) {
        /* Fill with input data for potential use in deserialize */
        size_t copy_size = (Size > alloc_size) ? alloc_size : Size;
        memcpy(malloced_ptr, Data, copy_size);
        
        /* Prepare for deserialize */
        deserialize_buf = (unsigned char *)malloced_ptr;
        
        /* 4. Deserialize database */
        /* Use the allocated buffer as serialized database content */
        /* Flags: 0 (no FREEONCLOSE, no RESIZEABLE) to manage memory manually */
        rc = sqlite3_deserialize(db, "main", deserialize_buf, (sqlite3_int64)alloc_size, (sqlite3_int64)alloc_size, 0);
        /* Ignore return code for fuzzing, just proceed to next step */
    }

    /* 5. File control */
    /* Try different opcodes and database names */
    int opcodes[] = {SQLITE_FCNTL_FILE_POINTER, SQLITE_FCNTL_JOURNAL_POINTER, SQLITE_FCNTL_VFS_POINTER, SQLITE_FCNTL_DATA_VERSION};
    void *file_ptr = NULL;
    for (int i = 0; i < 4; i++) {
        rc = sqlite3_file_control(db, NULL, opcodes[i], &file_ptr);
        /* Ignore return code */
    }
    /* Try with "main" explicitly */
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_FILE_POINTER, &file_ptr);

    /* 6. Execute SQL */
    /* Use input data as SQL string, ensuring it's null-terminated */
    char *sql = NULL;
    if (Size > 0) {
        sql = (char *)sqlite3_malloc64(Size + 1);
        if (sql != NULL) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            /* Execute SQL */
            rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
            free_error_msg(&errmsg);
            sqlite3_free(sql);
        }
    }

    /* Cleanup */
    if (db != NULL) {
        sqlite3_close(db);
    }
    
    /* Free the malloced_ptr if it wasn't freed by sqlite3_deserialize (which it won't be with flag 0) */
    if (malloced_ptr != NULL) {
        sqlite3_free(malloced_ptr);
    }

    /* Shutdown SQLite library */
    sqlite3_shutdown();

    return 0;
}