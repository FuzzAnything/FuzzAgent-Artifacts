// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * sqlite3_initialize -> sqlite3_open -> sqlite3_malloc64 -> sqlite3_deserialize
 * -> sqlite3_file_control -> sqlite3_exec
 */

/* Simple callback for sqlite3_exec to consume results */
static int exec_callback(void *data, int argc, char **argv, char **colnames) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)colnames;
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    void *malloc_ptr = NULL;
    unsigned char *deserialize_buf = NULL;
    char *errmsg = NULL;
    int rc;

    /* 1. Prepare environment: Initialize SQLite */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* 2. Open database connection */
    /* Use a dummy file as per instructions if needed, but :memory: is safer for fuzzing */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* 3. Allocate memory using sqlite3_malloc64 */
    /* Use fuzz input to determine size, with bounds to prevent crashes */
    sqlite3_uint64 alloc_size = 0;
    if (Size >= sizeof(sqlite3_uint64)) {
        memcpy(&alloc_size, Data, sizeof(sqlite3_uint64));
    } else if (Size > 0) {
        alloc_size = Data[0];
    }
    
    /* Cap allocation size to avoid OOM or excessive resource usage */
    if (alloc_size > 1024 * 1024) { /* 1MB limit */
        alloc_size = 1024 * 1024;
    }

    malloc_ptr = sqlite3_malloc64(alloc_size);
    if (malloc_ptr != NULL) {
        /* Initialize memory to avoid uninitialized memory usage */
        memset(malloc_ptr, 0, alloc_size);
    }

    /* 4. Deserialize database */
    /* Prepare a buffer for deserialization */
    size_t buf_size = 0;
    if (Size > sizeof(sqlite3_uint64)) {
        buf_size = Size - sizeof(sqlite3_uint64);
    }
    
    /* Cap buffer size for safety */
    if (buf_size > 1024 * 1024) {
        buf_size = 1024 * 1024;
    }

    if (buf_size > 0) {
        deserialize_buf = (unsigned char *)sqlite3_malloc64(buf_size);
        if (deserialize_buf != NULL) {
            /* Copy fuzz data into buffer, ensuring it looks somewhat like a DB header if possible */
            /* For fuzzing, we just copy raw data. sqlite3_deserialize will handle invalid data */
            memcpy(deserialize_buf, Data + sizeof(sqlite3_uint64), buf_size);
            
            /* Try to deserialize into 'main' schema */
            /* Flags: SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE */
            rc = sqlite3_deserialize(db, "main", deserialize_buf, (sqlite3_int64)buf_size, (sqlite3_int64)buf_size, 
                                     SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE);
            /* If deserialize fails, it might free the buffer if FREEONCLOSE is set and connection closes later,
               but we handle cleanup manually if needed. */
        }
    }

    /* 5. File Control */
    /* Try a few different opcodes or just a safe one like SQLITE_FCNTL_FILE_POINTER */
    void *file_ptr = NULL;
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_FILE_POINTER, &file_ptr);
    (void)rc; /* Ignore result for fuzzing, just exercising the path */

    /* 6. Execute SQL */
    /* Construct a SQL string from remaining fuzz data or use a simple query */
    const char *sql = "SELECT 1;";
    size_t sql_len = 0;
    
    if (Size > sizeof(sqlite3_uint64) + buf_size) {
        /* Use remaining data as SQL, ensuring null termination */
        size_t remaining = Size - sizeof(sqlite3_uint64) - buf_size;
        if (remaining > 0 && remaining < 1024) {
            char *sql_buf = (char *)malloc(remaining + 1);
            if (sql_buf) {
                memcpy(sql_buf, Data + sizeof(sqlite3_uint64) + buf_size, remaining);
                sql_buf[remaining] = '\0';
                sql = sql_buf;
                sql_len = remaining;
                /* Execute */
                rc = sqlite3_exec(db, sql, exec_callback, NULL, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
                free(sql_buf);
            }
        }
    }
    
    /* If we didn't use custom SQL, run a simple one to ensure the path is hit */
    if (sql_len == 0) {
        rc = sqlite3_exec(db, "SELECT 1;", exec_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }

    /* 7. Cleanup */
    /* Free malloc_ptr if it was allocated */
    if (malloc_ptr != NULL) {
        sqlite3_free(malloc_ptr);
    }
    
    /* Note: deserialize_buf is freed by sqlite3_deserialize if FREEONCLOSE was set and db closes,
       but if it failed before setting the flag effectively or if we want to be safe: */
    if (deserialize_buf != NULL && (rc != SQLITE_OK || !(SQLITE_DESERIALIZE_FREEONCLOSE & 0))) { 
        /* Heuristic: if deserialize failed or we aren't sure, free it manually if it wasn't taken over */
        /* However, the API says if it fails and FREEONCLOSE is set, it frees it. 
           If it succeeds, it takes ownership. 
           If it fails and FREEONCLOSE is NOT set, we own it.
           Since we passed FREEONCLOSE, if it failed, it should have freed it. 
           If it succeeded, db owns it.
           So we generally don't free it here unless we are sure we own it.
           To be safe against double free, we rely on the API contract.
           But if the deserialize call didn't happen (buf_size == 0), we don't have a buffer.
        */
        /* If we allocated it but didn't pass it to deserialize (e.g. buf_size check failed), free it. */
        /* In our logic, if buf_size > 0, we allocated. If we called deserialize, we passed FREEONCLOSE.
           If deserialize returns error, it frees. If success, db frees on close.
           So we do NOT free here.
        */
    }

    /* Close database connection */
    if (db != NULL) {
        sqlite3_close(db);
    }

    /* Shutdown SQLite */
    sqlite3_shutdown();

    return 0;
}