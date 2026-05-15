// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_xProgress(void *pArg) {
    (void)pArg;
    return 0;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    // Initialize a dummy sqlite3 database connection
    static sqlite3 *db = NULL;
    static int initialized = 0;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        // Set some callbacks to explore more states
        sqlite3_progress_handler(db, 1000, dummy_xProgress, NULL);
        initialized = 1;
    }
    
    // Create dynamic string objects
    sqlite3_str *str1 = sqlite3_str_new(db);
    sqlite3_str *str2 = sqlite3_str_new(NULL);
    
    if (!str1 || !str2) {
        sqlite3_str_free(str1);
        sqlite3_str_free(str2);
        return 0;
    }
    
    // Write fuzzer data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Test sqlite3_str_appendf with various format strings
    if (Size > 0) {
        // Use first byte to determine format string
        uint8_t format_selector = Data[0];
        const char *format_string;
        
        switch (format_selector % 4) {
            case 0: format_string = "Test: %d"; break;
            case 1: format_string = "Value: %.*s"; break;
            case 2: format_string = "Hex: %x"; break;
            default: format_string = "Simple"; break;
        }
        
        // Append formatted text to both string objects
        if (Size > 1) {
            sqlite3_str_appendf(str1, format_string, (int)Data[1]);
            sqlite3_str_appendf(str2, format_string, (int)Data[1]);
        }
        
        // Append more data if available
        if (Size > 10) {
            sqlite3_str_appendf(str1, " More: %.*s", 
                                (int)(Size - 2 > 100 ? 100 : Size - 2), 
                                Data + 2);
        }
    }
    
    // Test sqlite3_str_value
    char *value1 = sqlite3_str_value(str1);
    char *value2 = sqlite3_str_value(str2);
    (void)value1; // Use variables to avoid unused warnings
    (void)value2;
    
    // Test sqlite3_str_reset
    sqlite3_str_reset(str1);
    
    // Append more data after reset
    if (Size > 5) {
        sqlite3_str_appendf(str1, "Reset: %.*s", 
                            (int)(Size > 50 ? 50 : Size), 
                            Data);
    }
    
    // Test sqlite3_str_finish
    char *finished1 = sqlite3_str_finish(str1);
    if (finished1) {
        sqlite3_free(finished1);
    }
    
    // Test sqlite3_str_free
    sqlite3_str_free(str2);
    
    // Test with NULL pointers (should be safe)
    sqlite3_str_free(NULL);
    char *null_value = sqlite3_str_value(NULL);
    (void)null_value;
    sqlite3_str_reset(NULL);
    
    // Create another string to test finish with empty string
    sqlite3_str *str3 = sqlite3_str_new(db);
    char *finished3 = sqlite3_str_finish(str3);
    if (finished3) {
        sqlite3_free(finished3);
    }
    
    // Test with maximum allocation scenarios
    sqlite3_str *str4 = sqlite3_str_new(db);
    if (str4) {
        // Try to append a large amount of data
        for (int i = 0; i < 10 && Size > i; i++) {
            sqlite3_str_appendf(str4, "Chunk %d: %c", i, Data[i % Size]);
        }
        sqlite3_str_free(str4);
    }
    
    return 0;
}