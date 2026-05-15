// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_format_callback(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use a simple format string with some fuzzer-controlled data
    const char *format = "%.*s";
    sqlite3_str_appendf(str, format, (int)(size % 256), data);
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3 database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Test sqlite3_str_new with database
    sqlite3_str *str1 = sqlite3_str_new(db);
    if (str1 == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Test sqlite3_str_new without database (NULL)
    sqlite3_str *str2 = sqlite3_str_new(NULL);
    
    // Use fuzzer data to control operations
    if (Size > 0) {
        uint8_t operation = Data[0] % 6;
        size_t data_offset = 1;
        
        switch (operation) {
            case 0:
                // Test sqlite3_str_reset
                sqlite3_str_reset(str1);
                sqlite3_str_reset(str2);
                break;
                
            case 1:
                // Test sqlite3_str_appendf with str1
                if (Size > data_offset) {
                    size_t append_size = Size - data_offset;
                    if (append_size > 0) {
                        dummy_format_callback(str1, Data + data_offset, append_size);
                    }
                }
                break;
                
            case 2:
                // Test sqlite3_str_appendf with str2
                if (Size > data_offset) {
                    size_t append_size = Size - data_offset;
                    if (append_size > 0) {
                        dummy_format_callback(str2, Data + data_offset, append_size);
                    }
                }
                break;
                
            case 3:
                // Test sqlite3_str_value
                {
                    char *value1 = sqlite3_str_value(str1);
                    char *value2 = sqlite3_str_value(str2);
                    (void)value1; // Avoid unused variable warning
                    (void)value2;
                }
                break;
                
            case 4:
                // Test sqlite3_str_finish
                {
                    char *finished1 = sqlite3_str_finish(str1);
                    if (finished1) {
                        sqlite3_free(finished1);
                    }
                    str1 = sqlite3_str_new(db); // Create new one after finish
                    
                    char *finished2 = sqlite3_str_finish(str2);
                    if (finished2) {
                        sqlite3_free(finished2);
                    }
                    str2 = sqlite3_str_new(NULL);
                }
                break;
                
            case 5:
                // Test sqlite3_str_free
                sqlite3_str_free(str1);
                sqlite3_str_free(str2);
                str1 = sqlite3_str_new(db);
                str2 = sqlite3_str_new(NULL);
                break;
        }
    }
    
    // Additional test: append some data if we have it
    if (Size > 0 && str1) {
        // Use a different format to exercise more code paths
        const char *format2 = "Data size: %zu, First byte: 0x%02x";
        sqlite3_str_appendf(str1, format2, Size, Data[0]);
    }
    
    // Test sqlite3_str_value again after potential modifications
    if (str1) {
        char *final_value = sqlite3_str_value(str1);
        (void)final_value;
    }
    
    // Cleanup
    if (str1) {
        char *finished = sqlite3_str_finish(str1);
        if (finished) {
            sqlite3_free(finished);
        }
    }
    
    if (str2) {
        sqlite3_str_free(str2);
    }
    
    sqlite3_close(db);
    
    return 0;
}