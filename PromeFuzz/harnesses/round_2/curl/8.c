// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "curl/curl.h"
#include "curl/mprintf.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Initialize CURL handle for functions that require it
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Use first byte to decide which functions to call
    uint8_t selector = Size > 0 ? Data[0] : 0;
    
    // Prepare input string from fuzzer data
    const char *input_string = (const char *)Data;
    int input_length = Size > 0 ? (int)(Size < INT_MAX ? Size : INT_MAX) : 0;
    
    // Test curl_easy_escape
    if (selector & 0x01) {
        char *escaped = curl_easy_escape(curl, input_string, input_length);
        if (escaped) {
            curl_free(escaped);
        }
    }
    
    // Test curl_easy_unescape
    if (selector & 0x02) {
        int outlength = 0;
        char *unescaped = curl_easy_unescape(curl, input_string, input_length, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
    }
    
    // Test curl_unescape (deprecated)
    if (selector & 0x04) {
        char *unescaped_deprecated = curl_unescape(input_string, input_length);
        if (unescaped_deprecated) {
            curl_free(unescaped_deprecated);
        }
    }
    
    // Test curl_maprintf with various format strings
    if (selector & 0x08) {
        // Use different format strings based on input
        if (Size > 1) {
            char *formatted = NULL;
            uint8_t format_selector = Data[1] % 4;
            
            switch (format_selector) {
                case 0:
                    formatted = curl_maprintf("String: %.*s", input_length, input_string);
                    break;
                case 1:
                    formatted = curl_maprintf("Length: %d", input_length);
                    break;
                case 2:
                    formatted = curl_maprintf("Hex: %02x%02x", 
                        Size > 2 ? Data[2] : 0, 
                        Size > 3 ? Data[3] : 0);
                    break;
                case 3:
                    formatted = curl_maprintf("Test");
                    break;
            }
            
            if (formatted) {
                curl_free(formatted);
            }
        }
    }
    
    // Test curl_escape (obsolete)
    if (selector & 0x10) {
        char *escaped_obsolete = curl_escape(input_string, input_length);
        if (escaped_obsolete) {
            curl_free(escaped_obsolete);
        }
    }
    
    // Test curl_free with NULL pointer (should be safe)
    if (selector & 0x20) {
        curl_free(NULL);
    }
    
    // Chain operations: escape then unescape
    if (selector & 0x40 && Size > 0) {
        char *escaped = curl_easy_escape(curl, input_string, input_length);
        if (escaped) {
            int outlen = 0;
            char *unescaped = curl_easy_unescape(curl, escaped, 0, &outlen);
            curl_free(escaped);
            if (unescaped) {
                curl_free(unescaped);
            }
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    
    return 0;
}