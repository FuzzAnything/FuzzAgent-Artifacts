// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:78471:25 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:78603:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static sqlite3_value *create_protected_value(const uint8_t *data, size_t size) {
    // Create a protected sqlite3_value using sqlite3_value_dup
    // First create an unprotected value using sqlite3_bind_value approach
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *protected_val = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a simple statement that returns a value
    if (sqlite3_prepare_v2(db, "SELECT ?", -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind data based on input
    if (size == 0) {
        sqlite3_bind_null(stmt, 1);
    } else {
        // Use first byte to determine type
        uint8_t type_selector = data[0] % 5;
        
        switch (type_selector) {
            case 0: // NULL
                sqlite3_bind_null(stmt, 1);
                break;
            case 1: // INTEGER
                if (size >= 9) {
                    int64_t int_val = *(int64_t *)(data + 1);
                    sqlite3_bind_int64(stmt, 1, int_val);
                } else {
                    sqlite3_bind_int(stmt, 1, 42);
                }
                break;
            case 2: // REAL
                if (size >= 9) {
                    double real_val = *(double *)(data + 1);
                    sqlite3_bind_double(stmt, 1, real_val);
                } else {
                    sqlite3_bind_double(stmt, 1, 3.14159);
                }
                break;
            case 3: // TEXT
                if (size > 1) {
                    // Use data as text (excluding first byte)
                    char *text = (char *)malloc(size);
                    if (text) {
                        memcpy(text, data + 1, size - 1);
                        text[size - 1] = '\0';
                        sqlite3_bind_text(stmt, 1, text, -1, SQLITE_TRANSIENT);
                        free(text);
                    } else {
                        sqlite3_bind_text(stmt, 1, "test", -1, SQLITE_TRANSIENT);
                    }
                } else {
                    sqlite3_bind_text(stmt, 1, "test", -1, SQLITE_TRANSIENT);
                }
                break;
            case 4: // BLOB
                if (size > 1) {
                    sqlite3_bind_blob(stmt, 1, data + 1, (int)(size - 1), SQLITE_TRANSIENT);
                } else {
                    static const uint8_t dummy_blob[] = {0x01, 0x02, 0x03};
                    sqlite3_bind_blob(stmt, 1, dummy_blob, sizeof(dummy_blob), SQLITE_TRANSIENT);
                }
                break;
        }
    }
    
    // Execute the statement
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get unprotected value from column
        sqlite3_value *unprotected_val = sqlite3_column_value(stmt, 0);
        
        // Duplicate to get protected value
        protected_val = sqlite3_value_dup(unprotected_val);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected_val;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    // Create a protected sqlite3_value object using public API
    sqlite3_value *val = create_protected_value(Data, Size);
    if (!val) {
        return 0;
    }
    
    // Test all target API functions
    const void *blob_data = sqlite3_value_blob(val);
    int byte_count = sqlite3_value_bytes(val);
    int no_change = sqlite3_value_nochange(val);
    int byte16_count = sqlite3_value_bytes16(val);
    int int_val = sqlite3_value_int(val);
    int val_type = sqlite3_value_type(val);
    
    // Additional calls to explore more states
    // Call functions multiple times to test state changes
    const void *blob_data2 = sqlite3_value_blob(val);
    int byte_count2 = sqlite3_value_bytes(val);
    
    // Test text-related functions
    const unsigned char *text = sqlite3_value_text(val);
    const void *text16 = sqlite3_value_text16(val);
    const void *text16be = sqlite3_value_text16be(val);
    const void *text16le = sqlite3_value_text16le(val);
    
    // Test other value extraction functions
    int64_t int64_val = sqlite3_value_int64(val);
    double double_val = sqlite3_value_double(val);
    
    // Test numeric type
    int numeric_type = sqlite3_value_numeric_type(val);
    
    // Test frombind (should return 0 for our test values)
    int from_bind = sqlite3_value_frombind(val);
    
    // Clean up
    sqlite3_value_free(val);
    
    return 0;
}