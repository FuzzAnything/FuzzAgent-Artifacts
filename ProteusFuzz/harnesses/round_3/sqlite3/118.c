// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3_value_* functions.
 * This driver attempts to exercise various code paths by:
 * 1. Creating a database connection and preparing statements.
 * 2. Binding various types of data.
 * 3. Stepping through statements to get sqlite3_value objects.
 * 4. Calling the target API functions on these values.
 * 5. Properly cleaning up resources.
 */

static void cleanup_resources(sqlite3 **db, sqlite3_stmt **stmt) {
    if (stmt && *stmt) {
        sqlite3_finalize(*stmt);
        *stmt = NULL;
    }
    if (db && *db) {
        sqlite3_close(*db);
        *db = NULL;
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10;";
    
    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Prepare statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cleanup_resources(&db, &stmt);
        return 0;
    }

    /* Use input data to bind values */
    size_t idx = 0;
    int param_count = 10;
    
    for (int i = 1; i <= param_count; i++) {
        if (idx >= Size) break;
        
        /* Determine type based on first byte */
        uint8_t type_byte = Data[idx];
        idx++;
        
        if (idx >= Size) break;
        
        /* Use second byte to determine length (capped to avoid huge allocations) */
        int len = Data[idx] % 100 + 1;
        idx++;
        
        if (idx + len > Size) {
            len = Size - idx;
        }
        
        if (len <= 0) continue;
        
        switch (type_byte % 7) {
            case 0: /* TEXT */
                sqlite3_bind_text(stmt, i, (const char*)&Data[idx], len, SQLITE_TRANSIENT);
                break;
            case 1: /* BLOB */
                sqlite3_bind_blob(stmt, i, &Data[idx], len, SQLITE_TRANSIENT);
                break;
            case 2: /* INTEGER */
                if (len >= sizeof(int64_t)) {
                    int64_t val = 0;
                    memcpy(&val, &Data[idx], sizeof(int64_t));
                    sqlite3_bind_int64(stmt, i, val);
                } else {
                    sqlite3_bind_int(stmt, i, Data[idx]);
                }
                break;
            case 3: /* REAL */
                if (len >= sizeof(double)) {
                    double val = 0.0;
                    memcpy(&val, &Data[idx], sizeof(double));
                    sqlite3_bind_double(stmt, i, val);
                } else {
                    sqlite3_bind_double(stmt, i, (double)Data[idx]);
                }
                break;
            case 4: /* NULL */
                sqlite3_bind_null(stmt, i);
                break;
            case 5: /* TEXT16BE */
                /* Create UTF-16BE string */
                if (len % 2 != 0) len--;
                if (len > 0) {
                    sqlite3_bind_text16(stmt, i, (const unsigned char*)&Data[idx], len, SQLITE_TRANSIENT);
                }
                break;
            case 6: /* TEXT16LE */
                /* Create UTF-16LE string */
                if (len % 2 != 0) len--;
                if (len > 0) {
                    sqlite3_bind_text16(stmt, i, (const unsigned char*)&Data[idx], len, SQLITE_TRANSIENT);
                }
                break;
        }
        
        idx += len;
    }

    /* Execute statement */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        cleanup_resources(&db, &stmt);
        return 0;
    }

    /* Get values and test API functions */
    for (int i = 0; i < param_count; i++) {
        const sqlite3_value *val = sqlite3_column_value(stmt, i);
        if (!val) continue;

        /* 
         * sqlite3_column_value returns an UNPROTECTED value.
         * Accessors like sqlite3_value_text16be require a PROTECTED value (mutex held).
         * We must duplicate the value to get a protected copy before calling these APIs.
         */
        sqlite3_value *dup_val = sqlite3_value_dup(val);
        if (dup_val) {
            /* Test text retrieval functions on duplicated (protected) value */
            (void)sqlite3_value_text16be(dup_val);
            (void)sqlite3_value_text16(dup_val);
            (void)sqlite3_value_text16le(dup_val);
            (void)sqlite3_value_pointer(dup_val, "test_subtype");
            
            /* Free the duplicated value */
            sqlite3_value_free(dup_val);
        }

        /* 
         * Do NOT call text retrieval functions on the original 'val' here, 
         * as it is unprotected and will trigger an assertion failure in debug builds.
         * The original 'val' can only be used with sqlite3_result_value, 
         * sqlite3_bind_value, or sqlite3_value_dup.
         */
    }

    cleanup_resources(&db, &stmt);
    return 0;
}