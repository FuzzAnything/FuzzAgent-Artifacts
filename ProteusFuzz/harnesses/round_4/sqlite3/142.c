// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a safe filename string from fuzz input.
 * Ensures the string is null-terminated and within reasonable bounds.
 */
static char *create_safe_filename(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return strdup(":memory:");
    }
    /* Limit filename size to prevent excessive memory usage */
    size_t max_len = 256;
    if (Size > max_len) Size = max_len;
    
    char *filename = malloc(Size + 1);
    if (!filename) return NULL;
    
    memcpy(filename, Data, Size);
    filename[Size] = '\0';
    
    return filename;
}

/*
 * Static trace callback for sqlite3_trace_v2
 */
static int trace_callback(unsigned mask, void *pCtx, void *p1, void *p2) {
    (void)mask;
    (void)pCtx;
    (void)p1;
    (void)p2;
    return 0;
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int dummy_int = 0;
    int prev_state = 0;
    
    /* 1. Prepare the environment */
    /* Create a safe filename from input or use a dummy file */
    char *filename = create_safe_filename(Data, Size);
    if (!filename) {
        filename = strdup(":memory:");
        if (!filename) return 0;
    }

    /* 2. Invoke target functions */
    
    /* Fuzz sqlite3_open */
    rc = sqlite3_open(filename, &db);
    
    if (db) {
        /* Fuzz sqlite3_db_config */
        /* Try various operations with fuzzed values */
        int op = (Size > 0) ? (Data[0] % 10) : 0;
        int val = (Size > 1) ? (Data[1] % 100) : 0;
        
        if (op == 0) {
            /* SQLITE_DBCONFIG_LOOKASIDE: db, op, sz, n, p */
            rc = sqlite3_db_config(db, op, 128, 1, NULL);
        } else if (op == 2) {
            /* SQLITE_DBCONFIG_TRIGGER_EQP: db, op, val, &prev */
            rc = sqlite3_db_config(db, op, val, &prev_state);
        } else if (op == 3) {
            /* SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE: db, op, val, &prev */
            rc = sqlite3_db_config(db, op, val, &prev_state);
        } else {
            /* Generic call, might fail but safe */
            rc = sqlite3_db_config(db, op, val, NULL);
        }

        /* Fuzz sqlite3_error_offset */
        int offset = sqlite3_error_offset(db);
        (void)offset;

        /* Fuzz sqlite3_trace_v2 */
        /* Use fuzzed mask and callback, ensuring bounds check for Size >= 4 */
        unsigned mask = 0;
        if (Size >= 4) {
            mask = (Data[2] | (Data[3] << 8));
        } else if (Size == 3) {
            mask = Data[2];
        }
        
        rc = sqlite3_trace_v2(db, mask, trace_callback, (void *)(size_t)0x1234);
        
        /* Also try disabling trace */
        rc = sqlite3_trace_v2(db, 0, NULL, NULL);

        /* Fuzz sqlite3_limit */
        int limit_id = (Size > 4) ? (Data[4] % 10) : 0;
        int limit_val = (Size > 5) ? (Data[5] % 1000) : 100;
        
        /* Query current limit (val = -1) */
        int old_limit = sqlite3_limit(db, limit_id, -1);
        (void)old_limit;
        
        /* Set limit */
        rc = sqlite3_limit(db, limit_id, limit_val);

        /* Fuzz sqlite3_db_status */
        int status_op = (Size > 6) ? (Data[6] % 5) : 0;
        int cur = 0, hiwtr = 0;
        int reset = (Size > 7) ? (Data[7] % 2) : 0;
        
        rc = sqlite3_db_status(db, status_op, &cur, &hiwtr, reset);

        /* Cleanup: Close the database */
        sqlite3_close(db);
        db = NULL;
    }
    
    /* Cleanup filename */
    free(filename);
    
    return 0;
}