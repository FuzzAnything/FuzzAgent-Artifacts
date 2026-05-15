// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "curl/curl.h"
#include "curl/mprintf.h"

static void call_mvsnprintf(char *buffer, size_t maxlen, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsnprintf(buffer, maxlen, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare buffers and variables
    char buffer[1024];
    char buffer2[1024];
    size_t maxlen = Size % 1024 + 1; // Ensure non-zero maxlen
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Prepare strings for comparison functions
    const char *s1 = NULL;
    const char *s2 = NULL;
    size_t compare_len = 0;
    
    if (Size - data_offset > 2) {
        // Create two null-terminated strings from the data
        size_t s1_len = (Data[data_offset] % (Size - data_offset - 1)) + 1;
        s1 = (const char *)(Data + data_offset);
        data_offset += s1_len;
        
        if (Size - data_offset > 1) {
            size_t s2_len = (Data[data_offset] % (Size - data_offset - 1)) + 1;
            s2 = (const char *)(Data + data_offset);
            data_offset += s2_len;
            
            compare_len = (Data[data_offset] % 256) + 1;
        }
    }
    
    // Test curl_msprintf
    if (selector & 0x01) {
        int result = curl_msprintf(buffer, "Test %.*s %d", 
                                   (int)(Size % 100), 
                                   (const char *)Data, 
                                   (int)Size);
        (void)result; // Ignore return value
    }
    
    // Test curl_msnprintf
    if (selector & 0x02) {
        int result = curl_msnprintf(buffer, maxlen, "Format: %.*s", 
                                    (int)(Size % 100), 
                                    (const char *)Data);
        (void)result;
    }
    
    // Test curl_mvsnprintf using helper function
    if (selector & 0x04) {
        call_mvsnprintf(buffer, maxlen, "Test %d %.*s", 
                        (int)Size, 
                        (int)(Size % 100), 
                        (const char *)Data);
    }
    
    // Test curl_strnequal
    if (selector & 0x08 && s1 && s2) {
        int result = curl_strnequal(s1, s2, compare_len);
        (void)result;
    }
    
    // Test curl_strequal
    if (selector & 0x10 && s1 && s2) {
        int result = curl_strequal(s1, s2);
        (void)result;
    }
    
    // Test curl_mprintf
    if (selector & 0x20) {
        int result = curl_mprintf("Size: %zu, Data: %.*s\n", 
                                  Size, 
                                  (int)(Size % 100), 
                                  (const char *)Data);
        (void)result;
    }
    
    // Additional test with file operations if needed
    if (selector & 0x40) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size % 1024, fp);
            fclose(fp);
        }
    }
    
    return 0;
}