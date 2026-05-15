// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include "curl/curl.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare dummy file for curl_mfprintf
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (!dummy_file) {
        return 0;
    }

    // Split input into parts for different functions
    size_t part_size = Size / 8;
    if (part_size == 0) {
        part_size = 1;
    }

    // Test curl_strnequal
    {
        const char *s1 = (const char *)Data;
        const char *s2 = (const char *)Data + (Size > part_size ? part_size : 0);
        size_t n = part_size;
        
        // Test with non-NULL pointers
        curl_strnequal(s1, s2, n);
        
        // Test with NULL cases
        curl_strnequal(NULL, NULL, n);
        curl_strnequal(s1, NULL, n);
        curl_strnequal(NULL, s2, n);
    }

    // Test curl_strequal
    {
        // Create null-terminated strings for comparison
        char *str1 = malloc(part_size + 1);
        char *str2 = malloc(part_size + 1);
        
        if (str1 && str2) {
            size_t copy_size1 = part_size < Size ? part_size : Size;
            memcpy(str1, Data, copy_size1);
            str1[copy_size1] = '\0';
            
            size_t copy_size2 = (2 * part_size) < Size ? part_size : (Size > part_size ? Size - part_size : 0);
            memcpy(str2, Data + part_size, copy_size2);
            str2[copy_size2] = '\0';
            
            curl_strequal(str1, str2);
            curl_strequal(NULL, NULL);
            curl_strequal(str1, NULL);
            curl_strequal(NULL, str2);
        }
        
        free(str1);
        free(str2);
    }

    // Test curl_mfprintf
    {
        // Create a format string from the data
        char *format = malloc(part_size + 1);
        if (format) {
            size_t copy_size = part_size < Size ? part_size : Size;
            memcpy(format, Data, copy_size);
            format[copy_size] = '\0';
            
            // Use different types of arguments
            int int_val = (int)Data[0];
            const char *str_val = (const char *)Data + (Size > part_size ? part_size : 0);
            
            curl_mfprintf(dummy_file, format, int_val);
            curl_mfprintf(dummy_file, format, str_val);
            curl_mfprintf(dummy_file, "%s", format);
            
            free(format);
        }
    }

    // Test curl_msnprintf
    {
        char buffer[256];
        char *format = malloc(part_size + 1);
        
        if (format) {
            size_t copy_size = part_size < Size ? part_size : Size;
            memcpy(format, Data, copy_size);
            format[copy_size] = '\0';
            
            int int_val = (int)Data[0];
            const char *str_val = (const char *)Data + (Size > part_size ? part_size : 0);
            
            curl_msnprintf(buffer, sizeof(buffer), format, int_val);
            curl_msnprintf(buffer, sizeof(buffer), format, str_val);
            curl_msnprintf(buffer, sizeof(buffer), "%s", format);
            
            free(format);
        }
    }

    // Test curl_mprintf
    {
        char *format = malloc(part_size + 1);
        if (format) {
            size_t copy_size = part_size < Size ? part_size : Size;
            memcpy(format, Data, copy_size);
            format[copy_size] = '\0';
            
            int int_val = (int)Data[0];
            const char *str_val = (const char *)Data + (Size > part_size ? part_size : 0);
            
            curl_mprintf(format, int_val);
            curl_mprintf(format, str_val);
            curl_mprintf("%s", format);
            
            free(format);
        }
    }

    // Test curl_msprintf
    {
        char buffer[256];
        char *format = malloc(part_size + 1);
        
        if (format) {
            size_t copy_size = part_size < Size ? part_size : Size;
            memcpy(format, Data, copy_size);
            format[copy_size] = '\0';
            
            int int_val = (int)Data[0];
            const char *str_val = (const char *)Data + (Size > part_size ? part_size : 0);
            
            curl_msprintf(buffer, format, int_val);
            curl_msprintf(buffer, format, str_val);
            curl_msprintf(buffer, "%s", format);
            
            free(format);
        }
    }

    // Cleanup
    fclose(dummy_file);
    
    return 0;
}