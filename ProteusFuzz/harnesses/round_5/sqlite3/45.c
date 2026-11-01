// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Static helper to safely close a database connection if it is valid.
 */
static void safe_close(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
    }
}

/*
 * Static helper to interpret fuzz data as a signed integer.
 */
static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        return 0;
    }
    int val = 0;
    memcpy(&val, data + *offset, 4);
    *offset += 4;
    return val;
}

/*
 * Static helper to interpret fuzz data as a string (null-terminated or length-limited).
 */
static char *get_string_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    // Find null terminator or end of buffer
    size_t len = 0;
    while (*offset + len < size && data[*offset + len] != '\0') {
        len++;
    }
    
    if (len == 0) {
        (*offset)++; // Skip the null byte if present
        return NULL;
    }
    
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len + 1; // Skip string and null terminator
    
    return str;
}

/*
 * Static callback for sqlite3_collation_needed.
 * This is a dummy implementation to satisfy the API requirement.
 */
static void collation_callback(void *pClient, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pClient;
    (void)db;
    (void)eTextRep;
    (void)zName;
    /* Intentionally empty. In a real scenario, one might register a collation here. */
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;

    /* 
     * Step 1: Prepare environment.
     * Open a database connection. We use ":memory:" to avoid file system dependencies,
     * but we can also fuzz the filename if data allows.
     */
    const char *filename = ":memory:";
    
    // If the data starts with a valid string, use it as filename, otherwise default to :memory:
    // We try to parse a filename from the beginning of the buffer if it looks like a string
    if (Size > 1 && Data[0] != 0) {
        // Try to extract a filename string
        size_t fn_len = 0;
        while (fn_len < Size && Data[fn_len] != 0) fn_len++;
        if (fn_len > 0 && fn_len < 256) {
            char *fn_buf = malloc(fn_len + 1);
            if (fn_buf) {
                memcpy(fn_buf, Data, fn_len);
                fn_buf[fn_len] = 0;
                filename = fn_buf;
                offset = fn_len + 1; // Advance offset past filename
            }
        }
    }

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        // Even on error, sqlite3_open might return a handle, but if db is NULL, we stop.
        // If rc != SQLITE_OK but db is not NULL, we can still test other APIs on the invalid connection.
        if (db == NULL) {
            if (filename != ":memory:") free((void*)filename);
            return 0;
        }
    }

    /* 
     * Step 2 & 3: Explore program states by invoking target functions diversely.
     * We use the fuzz data to drive the arguments.
     */

    /* 1. sqlite3_limit */
    // We try multiple limit IDs and values
    if (offset < Size) {
        int limit_id = (int)Data[offset];
        offset++;
        if (offset < Size) {
            int new_val = get_int_from_data(Data, Size, &offset);
            sqlite3_limit(db, limit_id, new_val);
        }
        // Query the limit (newVal = -1)
        sqlite3_limit(db, limit_id, -1);
    }

    /* 2. sqlite3_db_release_memory */
    // This function takes only the db handle.
    sqlite3_db_release_memory(db);

    /* 4. sqlite3_db_config */
    // Operation codes and arguments
    if (offset < Size) {
        int op = (int)Data[offset];
        offset++;
        // sqlite3_db_config takes variable arguments. We try common ops.
        // SQLITE_DBCONFIG_LOOKASIDE (1) requires 3 args: void*, int, int
        // SQLITE_DBCONFIG_ENABLE_FKEY (2) requires 1 arg: int
        // SQLITE_DBCONFIG_ENABLE_TRIGGER (3) requires 1 arg: int
        // SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER (4) requires 1 arg: int
        // SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION (5) requires 1 arg: int
        
        if (op == 1 && offset + 4 <= Size) {
            // Lookaside: void *pArg, int nSize, int n
            int nSize = get_int_from_data(Data, Size, &offset);
            int n = get_int_from_data(Data, Size, &offset);
            sqlite3_db_config(db, op, (void*)0x1234, nSize, n);
        } else if (op >= 2 && op <= 5) {
            int enable = (offset < Size) ? (int)Data[offset] : 0;
            if (offset < Size) offset++;
            sqlite3_db_config(db, op, enable);
        } else {
            // Try with a generic integer argument for unknown ops (might fail safely)
            int arg = (offset < Size) ? (int)Data[offset] : 0;
            if (offset < Size) offset++;
            sqlite3_db_config(db, op, arg);
        }
    }

    /* 5. sqlite3_db_status */
    // Options: SQLITE_DBSTATUS_LOOKASIDE_USED, SQLITE_DBSTATUS_CACHE_USED, etc.
    if (offset < Size) {
        int op = (int)Data[offset];
        offset++;
        int cur = 0, hiwtr = 0;
        int reset = (offset < Size) ? (int)Data[offset] : 0;
        if (offset < Size) offset++;
        
        sqlite3_db_status(db, op, &cur, &hiwtr, reset);
        
        // Try resetting high-water mark
        sqlite3_db_status(db, op, &cur, &hiwtr, 1);
    }

    /* 6. sqlite3_collation_needed */
    // Register a callback. We use a static callback defined above.
    // We can pass arbitrary user data.
    if (offset < Size) {
        void *pClient = (void *)((uintptr_t)Data[offset]);
        offset++;
        sqlite3_collation_needed(db, pClient, collation_callback);
    }

    /* 
     * Step 4: Cleanup.
     * Always close the database connection.
     */
    safe_close(db);
    
    if (filename != ":memory:" && filename != NULL) {
        free((void*)filename);
    }

    return 0;
}