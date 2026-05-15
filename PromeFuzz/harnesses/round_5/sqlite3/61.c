// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:174663:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy comparison function for collations */
static int dummy_compare(void *pArg, int len1, const void *data1, int len2, const void *data2) {
    /* Simple comparison: compare lengths first, then first bytes */
    if (len1 != len2) return len1 - len2;
    if (len1 > 0 && len2 > 0) {
        const unsigned char *c1 = (const unsigned char *)data1;
        const unsigned char *c2 = (const unsigned char *)data2;
        return *c1 - *c2;
    }
    return 0;
}

/* Dummy destroy function for collation */
static void dummy_destroy(void *pArg) {
    /* Nothing to free - just a placeholder */
}

/* Callback for collation needed */
static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    /* When a collation is needed, create one with dummy functions */
    sqlite3_create_collation_v2(db, zName, eTextRep, pArg, dummy_compare, dummy_destroy);
}

/* Callback for UTF-16 collation needed */
static void collation_needed16_callback(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    /* Convert UTF-16 name to UTF-8 for simplicity, then create collation */
    char name_buf[256];
    int i;
    const unsigned short *name16 = (const unsigned short *)zName;
    
    /* Simple conversion: just take low bytes (not proper UTF-16 to UTF-8) */
    for (i = 0; i < 255 && name16[i] != 0; i++) {
        name_buf[i] = (char)(name16[i] & 0xFF);
    }
    name_buf[i] = '\0';
    
    sqlite3_create_collation(db, name_buf, eTextRep, pArg, dummy_compare);
}

/* Helper to extract string from fuzz data */
static const char* get_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    const char *str = (const char *)(data + *offset);
    size_t len = 0;
    
    /* Find null terminator or end of data */
    while (*offset + len < size && str[len] != '\0') {
        len++;
    }
    
    if (*offset + len >= size) {
        return NULL; /* No null terminator found */
    }
    
    *offset += len + 1; /* Skip string and null terminator */
    return str;
}

/* Helper to extract UTF-16 string from fuzz data */
static const void* get_utf16_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 >= size) return NULL;
    
    const void *str = (const void *)(data + *offset);
    size_t len = 0;
    
    /* Find null terminator (two null bytes for UTF-16) or end of data */
    while (*offset + len + 1 < size) {
        if (data[*offset + len] == 0 && data[*offset + len + 1] == 0) {
            len += 2; /* Include null terminator */
            *offset += len;
            return str;
        }
        len++;
    }
    
    return NULL; /* No proper UTF-16 null terminator found */
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Extract parameters from fuzz data */
    const char *collation_name = get_string(Data, Size, &offset);
    const void *collation_name16 = get_utf16_string(Data, Size, &offset);
    
    if (collation_name == NULL) {
        collation_name = "test_collation";
    }
    
    /* Create dummy argument */
    int dummy_arg = 42;
    
    /* Test sqlite3_create_collation */
    if (collation_name) {
        sqlite3_create_collation(db, collation_name, SQLITE_UTF8, &dummy_arg, dummy_compare);
    }
    
    /* Test sqlite3_create_collation_v2 */
    if (collation_name) {
        sqlite3_create_collation_v2(db, collation_name, SQLITE_UTF16, &dummy_arg, 
                                   dummy_compare, dummy_destroy);
    }
    
    /* Test sqlite3_create_collation16 */
    if (collation_name16) {
        sqlite3_create_collation16(db, collation_name16, SQLITE_UTF16LE, 
                                  &dummy_arg, dummy_compare);
    }
    
    /* Test sqlite3_collation_needed */
    sqlite3_collation_needed(db, &dummy_arg, collation_needed_callback);
    
    /* Test sqlite3_collation_needed16 */
    sqlite3_collation_needed16(db, &dummy_arg, collation_needed16_callback);
    
    /* Test sqlite3_vtab_collation with a dummy index_info structure */
    if (Size >= sizeof(sqlite3_index_info)) {
        sqlite3_index_info *idx_info = (sqlite3_index_info *)Data;
        
        /* Only call if we have reasonable constraints */
        if (idx_info->nConstraint > 0 && idx_info->aConstraint != NULL) {
            for (int i = 0; i < idx_info->nConstraint && i < 10; i++) {
                const char *coll_name = sqlite3_vtab_collation(idx_info, i);
                (void)coll_name; /* Ignore result */
            }
        }
    }
    
    /* Try to use the collation in a simple query to trigger callbacks */
    if (collation_name) {
        char query[512];
        snprintf(query, sizeof(query), 
                "CREATE TABLE test(id INTEGER, name TEXT COLLATE %s)", 
                collation_name);
        sqlite3_exec(db, query, NULL, NULL, NULL);
        
        /* Try another query that might need collation */
        sqlite3_exec(db, "SELECT name FROM test ORDER BY name", NULL, NULL, NULL);
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}