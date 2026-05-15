// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

static void fuzz_operations(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine operation mix
    uint8_t op_selector = data[0];
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size - 1;
    
    // Append formatted text using some of the data
    if (remaining_size > 0) {
        // Create a null-terminated string from part of the input
        size_t str_len = remaining_size > 100 ? 100 : remaining_size;
        char *temp_str = (char*)malloc(str_len + 1);
        if (temp_str) {
            memcpy(temp_str, remaining_data, str_len);
            temp_str[str_len] = '\0';
            
            // Use different format strings based on op_selector
            if ((op_selector & 0x01) && str_len > 0) {
                sqlite3_str_appendf(str, "Text: %s", temp_str);
            }
            if ((op_selector & 0x02) && str_len > 4) {
                int num = *(int*)remaining_data;
                sqlite3_str_appendf(str, "Num: %d", num);
            }
            if ((op_selector & 0x04)) {
                sqlite3_str_appendf(str, "Fixed");
            }
            
            free(temp_str);
        }
    }
    
    // Sometimes reset the string
    if (op_selector & 0x08) {
        sqlite3_str_reset(str);
        
        // Append something after reset
        if (remaining_size > 0) {
            size_t append_len = remaining_size > 50 ? 50 : remaining_size;
            char append_buf[51];
            memcpy(append_buf, remaining_data, append_len);
            append_buf[append_len] = '\0';
            sqlite3_str_appendf(str, "Reset: %s", append_buf);
        }
    }
    
    // Get current value (might be NULL)
    char *current = sqlite3_str_value(str);
    (void)current; // Use value to avoid compiler warning
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    // Create a dynamic string object
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) {
        return 0; // Out of memory
    }
    
    // Perform various operations on the string
    fuzz_operations(str, Data, Size);
    
    // Test different completion paths
    if (Size > 0 && (Data[0] & 0x10)) {
        // Finish and free the string
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
        // str is now invalid, don't use it
        return 0;
    } else if (Size > 0 && (Data[0] & 0x20)) {
        // Get value then free
        char *value = sqlite3_str_value(str);
        (void)value;
        sqlite3_str_free(str);
        return 0;
    } else {
        // Normal path: finish then free
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
    }
    
    // Create another string for more testing
    if (Size > 1) {
        sqlite3_str *str2 = sqlite3_str_new(NULL);
        if (str2) {
            // Use different part of input
            size_t offset = Size > 10 ? 10 : 1;
            fuzz_operations(str2, Data + offset, Size - offset);
            
            // Always clean up
            sqlite3_str_free(str2);
        }
    }
    
    return 0;
}