// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:83994:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:84168:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:84168:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:83994:28 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to create a minimal valid SQLite database header in memory.
 */
static int create_dummy_db_header(unsigned char *buf, size_t size) {
    if (size < 100) return 0;

    /* SQLite magic string */
    const char *magic = "SQLite format 3\000";
    memcpy(buf, magic, 16);

    /* Page size (2 bytes, big endian) - set to 4096 */
    buf[16] = 0x10;
    buf[17] = 0x00;

    /* File format write version (1 byte) */
    buf[18] = 1;
    /* File format read version (1 byte) */
    buf[19] = 1;

    /* Reserved space at end of each page (1 byte) */
    buf[20] = 0;

    /* Maximum embedded payload fraction (1 byte) */
    buf[21] = 64;
    /* Minimum embedded payload fraction (1 byte) */
    buf[22] = 32;
    /* Leaf payload fraction (1 byte) */
    buf[23] = 32;

    /* File change counter (4 bytes) */
    buf[24] = 0; buf[25] = 0; buf[26] = 0; buf[27] = 1;

    /* Size of database file in pages (4 bytes) - set to 1 */
    buf[28] = 0; buf[29] = 0; buf[30] = 0; buf[31] = 1;

    /* First freelist trunk page (4 bytes) */
    buf[32] = 0; buf[33] = 0; buf[34] = 0; buf[35] = 0;

    /* Total number of freelist pages (4 bytes) */
    buf[36] = 0; buf[37] = 0; buf[38] = 0; buf[39] = 0;

    /* Schema cookie (4 bytes) */
    buf[40] = 0; buf[41] = 0; buf[42] = 0; buf[43] = 1;

    /* Schema format number (4 bytes) - version 4 */
    buf[44] = 0; buf[45] = 0; buf[46] = 0; buf[47] = 4;

    /* Default page cache size (4 bytes) */
    buf[48] = 0; buf[49] = 0; buf[50] = 0; buf[51] = 0;

    /* Largest root b-tree page number for auto-vacuum (4 bytes) */
    buf[52] = 0; buf[53] = 0; buf[54] = 0; buf[55] = 0;

    /* Text encoding (4 bytes) - 1 for UTF-8 */
    buf[56] = 0; buf[57] = 0; buf[58] = 0; buf[59] = 1;

    /* User version (4 bytes) */
    buf[60] = 0; buf[61] = 0; buf[62] = 0; buf[63] = 0;

    /* Incremental vacuum mode (4 bytes) */
    buf[64] = 0; buf[65] = 0; buf[66] = 0; buf[67] = 0;

    /* Application ID (4 bytes) */
    buf[68] = 0; buf[69] = 0; buf[70] = 0; buf[71] = 0;

    /* Reserved (20 bytes) */
    memset(buf + 72, 0, 20);

    /* Version-valid-for (4 bytes) */
    buf[92] = 0; buf[93] = 0; buf[94] = 0; buf[95] = 1;

    /* SQLite version number (4 bytes) */
    buf[96] = 0; buf[97] = 0; buf[98] = 0; buf[99] = 0;

    return 1;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    sqlite3_backup *backup = NULL;
    int rc;
    unsigned char *db_buffer = NULL;
    size_t db_size = 0;
    size_t buf_capacity = 0;
    int flags = 0;
    int page_count = 0;
    int remaining = 0;

    /* Guard against empty input */
    if (Size < 1) return 0;

    /* Use first byte to decide which API to fuzz */
    int mode = Data[0] % 3; 

    /* Initialize database connections */
    rc = sqlite3_open(":memory:", &db1);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_open(":memory:", &db2);
    if (rc != SQLITE_OK) goto cleanup;

    /* 
     * Mode 0: Fuzz sqlite3_deserialize
     */
    if (mode == 0) {
        /* Prepare a buffer for deserialization */
        if (Size < 100) {
            db_buffer = (unsigned char *)sqlite3_malloc(100);
            if (!db_buffer) goto cleanup;
            db_size = 100;
            buf_capacity = 100;
            create_dummy_db_header(db_buffer, db_size);
        } else {
            /* Use input data, but ensure header is valid */
            db_size = Size;
            buf_capacity = Size;
            db_buffer = (unsigned char *)sqlite3_malloc(db_size);
            if (!db_buffer) goto cleanup;
            memcpy(db_buffer, Data, db_size);
            
            /* Ensure the magic header is correct */
            const char *magic = "SQLite format 3\000";
            memcpy(db_buffer, magic, 16);
            /* Set page size to 4096 */
            db_buffer[16] = 0x10; db_buffer[17] = 0x00;
            /* Set page count to 1 */
            db_buffer[28] = 0; db_buffer[29] = 0; db_buffer[30] = 0; db_buffer[31] = 1;
        }

        /* Vary flags based on input */
        flags = 0;
        if (Size > 1) {
            int f = Data[1];
            if (f & 1) flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
            if (f & 2) flags |= SQLITE_DESERIALIZE_RESIZEABLE;
            if (f & 4) flags |= SQLITE_DESERIALIZE_READONLY;
        }

        /* Try deserializing into main, temp (should fail), or attached db */
        const char *schema = NULL;
        if (Size > 2) {
            int s = Data[2] % 3;
            if (s == 1) schema = "temp"; /* Should return SQLITE_ERROR */
            else if (s == 2) schema = "main";
            else schema = NULL; /* Default to main */
        }

        rc = sqlite3_deserialize(db1, schema, db_buffer, db_size, buf_capacity, flags);
        
        /* 
         * CRITICAL FIX:
         * If SQLITE_DESERIALIZE_FREEONCLOSE is set, SQLite takes ownership of the buffer
         * and will free it when the connection is closed. We must NOT free it here.
         * If the flag is NOT set, we are responsible for freeing it.
         * However, if we free it here and then try to use the DB (e.g., SELECT 1),
         * we get a use-after-free because SQLite still holds a pointer to the freed memory.
         * 
         * Strategy:
         * 1. If FREEONCLOSE is set: Do NOT free db_buffer. Let SQLite handle it on close.
         * 2. If FREEONCLOSE is NOT set: We must free it, BUT we must NOT use the DB afterwards
         *    because the DB might still be referencing the buffer internally if the operation
         *    succeeded and the buffer was used to initialize the in-memory DB.
         *    Actually, if FREEONCLOSE is NOT set, SQLite copies the data or manages it differently?
         *    According to docs: "If the SQLITE_DESERIALIZE_FREEONCLOSE bit is set... SQLite will invoke sqlite3_free()... when the database connection closes."
         *    If NOT set, the application must not modify the buffer until the connection closes.
         *    This implies SQLite keeps a pointer to the buffer even if FREEONCLOSE is not set.
         *    Therefore, we should NEVER free the buffer while the connection is open if we passed it to deserialize.
         *    The only safe way to free it is after sqlite3_close(db1).
         *    But if FREEONCLOSE is set, we CANNOT free it after close either (double free).
         *    
         *    Correct logic:
         *    - If FREEONCLOSE is set: Don't free. Let close() handle it.
         *    - If FREEONCLOSE is NOT set: We must free it, but only AFTER the connection is closed.
         *      However, we can't easily track "free after close" in this simple flow without a flag.
         *      So, we will set a flag to indicate if we need to free it later.
         */
        
        int need_to_free_buffer = 0;
        if (!(flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
            need_to_free_buffer = 1;
        }

        /* Try to execute a simple query to ensure the DB is usable if deserialize succeeded */
        if (rc == SQLITE_OK) {
            char *err_msg = NULL;
            rc = sqlite3_exec(db1, "SELECT 1;", NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
        }

        /* Close db1 first to release the buffer if FREEONCLOSE was set */
        if (db1) {
            sqlite3_close(db1);
            db1 = NULL;
        }

        /* Now free the buffer if we were responsible for it */
        if (need_to_free_buffer && db_buffer) {
            sqlite3_free(db_buffer);
            db_buffer = NULL;
        }
    }
    /* 
     * Mode 1: Fuzz sqlite3_backup_* APIs
     */
    else if (mode == 1) {
        /* Create a simple table and insert data in source (db1) */
        const char *sql_create = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT);";
        const char *sql_insert = "INSERT INTO test VALUES (1, 'hello');";
        
        char *err_msg = NULL;
        sqlite3_exec(db1, sql_create, NULL, NULL, &err_msg);
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_exec(db1, sql_insert, NULL, NULL, &err_msg);
        if (err_msg) sqlite3_free(err_msg);

        /* Initialize backup: db1 (source) -> db2 (dest) */
        backup = sqlite3_backup_init(db2, "main", db1, "main");

        if (backup) {
            /* Determine number of pages to copy based on input */
            int nPage = 1;
            if (Size > 3) {
                nPage = Data[3];
                /* Ensure nPage is not too large to avoid immediate OOM, but allow negative for "all" */
                if (nPage > 1000) nPage = 1000; 
            }

            /* Step the backup */
            rc = sqlite3_backup_step(backup, nPage);

            /* Query remaining and pagecount */
            remaining = sqlite3_backup_remaining(backup);
            page_count = sqlite3_backup_pagecount(backup);

            /* Perform a few more steps if not done */
            if (rc == SQLITE_OK && remaining > 0) {
                rc = sqlite3_backup_step(backup, 1);
                remaining = sqlite3_backup_remaining(backup);
                page_count = sqlite3_backup_pagecount(backup);
            }

            /* Finish backup */
            rc = sqlite3_backup_finish(backup);
            backup = NULL;
        }
    }
    /* 
     * Mode 2: Edge cases and error handling
     */
    else {
        /* Test backup with same source/dest (should fail) */
        backup = sqlite3_backup_init(db1, "main", db1, "main");
        if (backup) {
            sqlite3_backup_finish(backup);
            backup = NULL;
        }

        /* Test deserialize with NULL buffer (should fail) */
        rc = sqlite3_deserialize(db1, "main", NULL, 0, 0, 0);
        
        /* Test deserialize with "temp" schema (should fail) */
        rc = sqlite3_deserialize(db1, "temp", NULL, 0, 0, 0);
    }

cleanup:
    /* Close connections. 
       Note: If db1 was used in mode 0 with FREEONCLOSE, the buffer is freed here.
       If db1 was used in mode 0 without FREEONCLOSE, we freed the buffer before closing db1.
    */
    if (db1) sqlite3_close(db1);
    if (db2) sqlite3_close(db2);
    if (backup) sqlite3_backup_finish(backup);
    
    return 0;
}