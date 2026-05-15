// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "curl/curl.h"
#include "curl/mprintf.h"
#include "curl/multi.h"

static char *helper_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

static void dummy_push_callback(void) {
    // This is a dummy function to simulate push callback context
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // 1. curl_version - simplest, no setup needed
    const char *version_str = curl_version();
    if (version_str) {
        // Use the version string in some way to avoid dead code elimination
        volatile char dummy = version_str[0];
        (void)dummy;
    }

    // 2. curl_getenv - use fuzzer input as environment variable name
    if (Size > 0 && Size < 256) {
        char *env_name = (char *)malloc(Size + 1);
        if (env_name) {
            memcpy(env_name, Data, Size);
            env_name[Size] = '\0';
            char *env_value = curl_getenv(env_name);
            if (env_value) {
                curl_free(env_value);
            }
            free(env_name);
        }
    }

    // 3. curl_maprintf - format string with fuzzer data
    if (Size > 0) {
        // Use first byte as format specifier hint
        char format[32];
        switch (Data[0] % 4) {
            case 0: snprintf(format, sizeof(format), "Test: %%.%us", (unsigned)(Size % 100)); break;
            case 1: snprintf(format, sizeof(format), "Value: %%.%ds", (int)(Size % 100)); break;
            case 2: snprintf(format, sizeof(format), "%%s"); break;
            default: snprintf(format, sizeof(format), "%%x");
        }
        
        char *test_str = NULL;
        if (Size > 1) {
            // Use remaining data as string argument
            char *arg = (char *)malloc(Size);
            if (arg) {
                memcpy(arg, Data + 1, Size - 1);
                arg[Size - 1] = '\0';
                test_str = curl_maprintf(format, arg);
                free(arg);
            }
        } else {
            test_str = curl_maprintf("Empty input");
        }
        
        if (test_str) {
            curl_free(test_str);
        }
    }

    // 4. curl_mvaprintf - similar to curl_maprintf but with va_list
    if (Size > 0) {
        char *test_str = NULL;
        if (Size > 1) {
            char *arg = (char *)malloc(Size);
            if (arg) {
                memcpy(arg, Data + 1, Size - 1);
                arg[Size - 1] = '\0';
                test_str = helper_mvaprintf("Formatted: %s", arg);
                free(arg);
            }
        } else {
            test_str = helper_mvaprintf("Size: %zu", Size);
        }
        
        if (test_str) {
            curl_free(test_str);
        }
    }

    // 5. & 6. curl_pushheader_byname and curl_pushheader_bynum
    // These require valid pushheaders context which only exists in push callback
    // We'll simulate minimal context and expect NULL returns
    dummy_push_callback();
    
    // Create dummy pushheaders pointer (will be NULL in reality)
    struct curl_pushheaders *pushheaders = NULL;
    
    // Try curl_pushheader_byname with various names
    const char *header_names[] = {":path", ":method", ":authority", ":scheme", "custom"};
    for (size_t i = 0; i < sizeof(header_names)/sizeof(header_names[0]); i++) {
        char *header = curl_pushheader_byname(pushheaders, header_names[i]);
        // Should always be NULL outside callback context
        if (header) {
            // Should not happen, but handle just in case
            volatile char dummy = header[0];
            (void)dummy;
        }
    }
    
    // Try curl_pushheader_bynum with various indices
    for (size_t i = 0; i < 10; i++) {
        char *header = curl_pushheader_bynum(pushheaders, i);
        // Should always be NULL outside callback context
        if (header) {
            // Should not happen, but handle just in case
            volatile char dummy = header[0];
            (void)dummy;
        }
    }

    // Try using fuzzer data as header name
    if (Size > 0 && Size < 256) {
        char *header_name = (char *)malloc(Size + 1);
        if (header_name) {
            memcpy(header_name, Data, Size);
            header_name[Size] = '\0';
            char *header = curl_pushheader_byname(pushheaders, header_name);
            if (header) {
                volatile char dummy = header[0];
                (void)dummy;
            }
            free(header_name);
        }
    }

    return 0;
}