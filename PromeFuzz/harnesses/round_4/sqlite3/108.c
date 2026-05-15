// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 4) return 0;
    
    // Create sqlite3_str object using the proper API
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str == NULL) return 0;
    
    // Use input data to determine which operations to perform
    // Use first byte to determine operation mix
    uint8_t operation_mix = Data[0];
    
    // Use second byte for character parameter
    char char_param = (char)Data[1];
    
    // Use third and fourth bytes for integer parameters
    int int_param1 = (int)(Data[2]);
    int int_param2 = (int)(Data[3]);
    
    // Operation 1: Test sqlite3_str_appendchar
    if ((operation_mix & 0x01) != 0) {
        // Use int_param1 for N, char_param for C
        int N = int_param1 % 256;  // Limit to reasonable size
        if (N < 0) N = -N;  // Ensure non-negative
        
        sqlite3_str_appendchar(str, N, char_param);
        
        // Check error state after operation
        int err = sqlite3_str_errcode(str);
        if (err != 0) {
            // If we get an error, free and recreate the string to continue testing
            char *result = sqlite3_str_finish(str);
            if (result) sqlite3_free(result);
            str = sqlite3_str_new(NULL);
            if (str == NULL) return 0;
        }
    }
    
    // Operation 2: Test sqlite3_str_truncate
    if ((operation_mix & 0x02) != 0) {
        // Use int_param2 for N
        int N = int_param2 % 1024;  // Limit to reasonable size
        if (N < 0) N = -N;  // Ensure non-negative
        
        sqlite3_str_truncate(str, N);
        
        // Get current length after truncation
        int len = sqlite3_str_length(str);
        if (len < 0) len = 0;
    }
    
    // Operation 3: Test sqlite3_str_append with remaining input data
    if ((operation_mix & 0x04) != 0 && Size > 4) {
        // Use remaining data as the string to append
        size_t data_left = Size - 4;
        size_t append_size = data_left;
        
        // Limit append size to prevent excessive memory usage
        if (append_size > 4096) append_size = 4096;
        
        if (append_size > 0) {
            const char *append_data = (const char *)(Data + 4);
            
            // Ensure the data is null-terminated for safety
            char *safe_data = (char *)malloc(append_size + 1);
            if (safe_data != NULL) {
                memcpy(safe_data, append_data, append_size);
                safe_data[append_size] = '\0';
                
                sqlite3_str_append(str, safe_data, (int)append_size);
                
                free(safe_data);
            }
        }
    }
    
    // Operation 4: Test sqlite3_str_length
    if ((operation_mix & 0x08) != 0) {
        int length = sqlite3_str_length(str);
        // Length should be non-negative
        if (length < 0) length = 0;
    }
    
    // Operation 5: Test sqlite3_str_errcode
    if ((operation_mix & 0x10) != 0) {
        int err = sqlite3_str_errcode(str);
        // Error code should be one of SQLITE_OK, SQLITE_NOMEM, or SQLITE_TOOBIG
        // We don't need to validate, just call the function
        (void)err;
    }
    
    // Operation 6: Test sqlite3_str_value
    if ((operation_mix & 0x20) != 0) {
        char *value = sqlite3_str_value(str);
        // Value may be NULL if empty or invalid
        if (value != NULL) {
            // Access some bytes to ensure memory is valid
            // But don't write beyond allocated bounds
            int len = sqlite3_str_length(str);
            if (len > 0) {
                char first_char = value[0];
                (void)first_char;
            }
        }
    }
    
    // Additional test: Combine operations in different ways
    // Use different parts of input data for different operations
    
    // Test with negative N values (should be handled safely)
    if ((operation_mix & 0x40) != 0) {
        sqlite3_str_appendchar(str, -1, 'x');
        sqlite3_str_truncate(str, -1);
    }
    
    // Test with large N values
    if ((operation_mix & 0x80) != 0) {
        sqlite3_str_appendchar(str, 1000, 'y');
        sqlite3_str_truncate(str, 5000);
    }
    
    // Clean up using the proper API
    char *result = sqlite3_str_finish(str);
    if (result) sqlite3_free(result);
    
    return 0;
}