// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:174663:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:174663:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
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

static int dummy_compare(void *pArg, int len1, const void *data1, int len2, const void *data2) {
    return 0;
}

static void dummy_destroy(void *pArg) {
    // Do nothing
}

static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    // Create a dummy collation when needed
    sqlite3_create_collation(db, zName, eTextRep, pArg, dummy_compare);
}

static void collation_needed_callback16(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    // Create a dummy collation when needed (UTF-16 version)
    sqlite3_create_collation16(db, zName, eTextRep, pArg, dummy_compare);
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Use the first byte to determine which functions to test
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test sqlite3_collation_needed
    if (selector & 0x01) {
        void *pArg = (void *)(Data + (offset % Size));
        sqlite3_collation_needed(db, pArg, collation_needed_callback);
        offset = (offset + 1) % Size;
    }
    
    // Test sqlite3_collation_needed16
    if (selector & 0x02) {
        void *pArg = (void *)(Data + (offset % Size));
        sqlite3_collation_needed16(db, pArg, collation_needed_callback16);
        offset = (offset + 1) % Size;
    }
    
    // Test sqlite3_create_collation
    if (selector & 0x04) {
        // Extract a collation name from the input
        size_t name_len = 0;
        if (Size > offset) {
            name_len = Size - offset;
            if (name_len > 100) name_len = 100;
        }
        
        if (name_len > 0) {
            char *collation_name = (char *)malloc(name_len + 1);
            if (collation_name) {
                memcpy(collation_name, Data + offset, name_len);
                collation_name[name_len] = '\0';
                
                // Use various encodings
                int encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16};
                int encoding_idx = 0;
                if (Size > offset) {
                    encoding_idx = (Data[offset] % 4);
                }
                
                sqlite3_create_collation(db, collation_name, encodings[encoding_idx], 
                                        (void *)(Data + (offset % Size)), dummy_compare);
                
                free(collation_name);
                offset = (offset + name_len) % Size;
            }
        }
    }
    
    // Test sqlite3_create_collation_v2
    if (selector & 0x08) {
        // Extract a collation name from the input
        size_t name_len = 0;
        if (Size > offset) {
            name_len = Size - offset;
            if (name_len > 100) name_len = 100;
        }
        
        if (name_len > 0) {
            char *collation_name = (char *)malloc(name_len + 1);
            if (collation_name) {
                memcpy(collation_name, Data + offset, name_len);
                collation_name[name_len] = '\0';
                
                // Use various encodings
                int encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16};
                int encoding_idx = 0;
                if (Size > offset) {
                    encoding_idx = (Data[offset] % 4);
                }
                
                sqlite3_create_collation_v2(db, collation_name, encodings[encoding_idx],
                                           (void *)(Data + (offset % Size)), dummy_compare, dummy_destroy);
                
                free(collation_name);
                offset = (offset + name_len) % Size;
            }
        }
    }
    
    // Test sqlite3_create_collation16
    if (selector & 0x10) {
        // Extract a collation name from the input
        size_t name_len = 0;
        if (Size > offset) {
            name_len = Size - offset;
            if (name_len > 100) name_len = 100;
        }
        
        if (name_len > 0) {
            // Create UTF-16 version of the name (simplified - just duplicate bytes)
            void *name16 = malloc(name_len * 2 + 2);
            if (name16) {
                // Simple conversion: each byte becomes a 16-bit char
                uint8_t *src = (uint8_t *)(Data + offset);
                uint16_t *dst = (uint16_t *)name16;
                for (size_t i = 0; i < name_len; i++) {
                    dst[i] = src[i];
                }
                dst[name_len] = 0; // Null terminator
                
                // Use various encodings
                int encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16};
                int encoding_idx = 0;
                if (Size > offset) {
                    encoding_idx = (Data[offset] % 4);
                }
                
                sqlite3_create_collation16(db, name16, encodings[encoding_idx],
                                          (void *)(Data + (offset % Size)), dummy_compare);
                
                free(name16);
                offset = (offset + name_len) % Size;
            }
        }
    }
    
    // Try to use a collation in a simple query
    if (selector & 0x20) {
        sqlite3_stmt *stmt;
        const char *sql = "SELECT 'test' COLLATE BINARY;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}