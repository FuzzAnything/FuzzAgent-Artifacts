// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Helper to safely extract integers from fuzz input */
static int32_t get_int32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) return 0;
    int32_t val = 0;
    memcpy(&val, &data[*offset], 4);
    *offset += 4;
    return val;
}

static uint8_t get_uint8(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 > size) return 0;
    uint8_t val = data[*offset];
    *offset += 1;
    return val;
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    size_t offset = 0;
    
    /* Ensure we have enough data for basic operations */
    if (Size < 2) return 0;

    /* 
     * Step 1: Prepare environment
     * Initialize global config if possible (sqlite3_config)
     * Note: sqlite3_config must be called before sqlite3_initialize or after sqlite3_shutdown.
     * Since the fuzzer runs in a loop, we attempt it, but it may fail if initialized.
     */
    int32_t op = get_int32(Data, Size, &offset);
    int32_t arg1 = get_int32(Data, Size, &offset);
    
    /* Try global config with safe options or ignore if misuse */
    sqlite3_config(op, arg1);

    /* Reset auto extensions */
    sqlite3_reset_auto_extension();

    /* Open a database connection using a dummy file */
    const char *filename = "./dummy_file";
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        /* If we can't open, try in-memory as fallback */
        sqlite3_close(db);
        rc = sqlite3_open(":memory:", &db);
    }

    if (!db) {
        return 0;
    }

    /* 
     * Step 2: Fuzz target functions
     */

    /* 1. sqlite3_enable_load_extension */
    int onoff = get_uint8(Data, Size, &offset) & 1;
    sqlite3_enable_load_extension(db, onoff);

    /* 2. sqlite3_db_config */
    /* We need to handle SQLITE_DBCONFIG_LOOKASIDE carefully as per instructions */
    int db_op = get_int32(Data, Size, &offset);
    int db_arg1 = get_int32(Data, Size, &offset);
    int db_arg2 = get_int32(Data, Size, &offset);

    if (db_op == SQLITE_DBCONFIG_LOOKASIDE) {
        /* 
         * Fix: The third argument must be a valid pointer to a memory buffer.
         * Allocate a buffer for the lookaside pool.
         */
        void *lookaside_buf = NULL;
        int sz = db_arg1;
        int cnt = db_arg2;

        /* Ensure sz is valid: multiple of sizeof(void*) and > 0 */
        if (sz > 0 && (sz % sizeof(void*)) == 0) {
            /* Allocate a buffer large enough for cnt slots of size sz */
            if (cnt > 0) {
                size_t total_size = (size_t)sz * (size_t)cnt;
                if (total_size > 1024 * 1024) total_size = 1024 * 1024; /* Cap allocation */
                lookaside_buf = malloc(total_size);
            }
        }

        if (lookaside_buf) {
            sqlite3_db_config(db, db_op, lookaside_buf, sz, cnt);
            free(lookaside_buf);
        } else {
            /* If we couldn't allocate, try with NULL or skip to avoid crash */
            sqlite3_db_config(db, db_op, NULL, 0, 0);
        }
    } else {
        /* For other ops, pass arguments directly */
        sqlite3_db_config(db, db_op, db_arg1, db_arg2);
    }

    /* 3. sqlite3_total_changes */
    /* Ensure some state changes might have occurred, though not strictly necessary for fuzzing */
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;

    /* 
     * Step 3: Cleanup
     * Close the database connection safely
     */
    sqlite3_close_v2(db);

    /* Remove the dummy file if it exists */
    unlink(filename);

    return 0;
}