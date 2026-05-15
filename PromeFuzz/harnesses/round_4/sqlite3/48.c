// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:174663:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_compare(void *pArg, int n1, const void *s1, int n2, const void *s2) {
    return 0;
}

static void dummy_destroy(void *pArg) {
    // Do nothing
}

static void dummy_collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    // Do nothing
}

static void dummy_collation_needed16_callback(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Use the first byte to determine which functions to test
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Test sqlite3_collation_needed
    if (selector & 0x01) {
        sqlite3_collation_needed(db, NULL, dummy_collation_needed_callback);
    }
    
    // Test sqlite3_collation_needed16
    if (selector & 0x02) {
        sqlite3_collation_needed16(db, NULL, dummy_collation_needed16_callback);
    }
    
    // Test sqlite3_create_collation
    if (data_offset < Size) {
        size_t name_len = (Size - data_offset) / 2;
        if (name_len > 0) {
            char *collation_name = (char *)malloc(name_len + 1);
            if (collation_name) {
                size_t copy_len = name_len < (Size - data_offset) ? name_len : (Size - data_offset);
                memcpy(collation_name, Data + data_offset, copy_len);
                collation_name[copy_len] = '\0';
                
                sqlite3_create_collation(db, collation_name, SQLITE_UTF8, NULL, dummy_compare);
                free(collation_name);
            }
        }
    }
    
    // Test sqlite3_create_collation_v2
    if (data_offset < Size) {
        size_t name_len = (Size - data_offset) / 3;
        if (name_len > 0) {
            char *collation_name = (char *)malloc(name_len + 1);
            if (collation_name) {
                size_t copy_len = name_len < (Size - data_offset) ? name_len : (Size - data_offset);
                memcpy(collation_name, Data + data_offset, copy_len);
                collation_name[copy_len] = '\0';
                
                sqlite3_create_collation_v2(db, collation_name, SQLITE_UTF8, NULL, dummy_compare, dummy_destroy);
                free(collation_name);
            }
        }
    }
    
    // Test sqlite3_create_collation16
    if (data_offset < Size) {
        size_t name_len = (Size - data_offset) / 4;
        if (name_len > 0) {
            void *collation_name = malloc(name_len * 2 + 2);
            if (collation_name) {
                size_t copy_len = name_len * 2 < (Size - data_offset) ? name_len * 2 : (Size - data_offset);
                memcpy(collation_name, Data + data_offset, copy_len);
                // Null terminate UTF-16 string
                ((uint8_t *)collation_name)[copy_len] = 0;
                ((uint8_t *)collation_name)[copy_len + 1] = 0;
                
                sqlite3_create_collation16(db, collation_name, SQLITE_UTF16, NULL, dummy_compare);
                free(collation_name);
            }
        }
    }
    
    // Test sqlite3_vtab_collation - requires a dummy sqlite3_index_info structure
    if (selector & 0x04) {
        // Create a minimal sqlite3_index_info structure
        sqlite3_index_info idx_info = {0};
        const char *coll_name = sqlite3_vtab_collation(&idx_info, 0);
        (void)coll_name; // Suppress unused variable warning
    }
    
    // Create a dummy file and write data to it
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}