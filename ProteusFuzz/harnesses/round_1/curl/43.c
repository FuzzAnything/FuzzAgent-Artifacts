// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Helper function to use va_list correctly for curl_mvsprintf
// Must be varargs to use va_start
static int static_helper_mvsprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[256];
    int result = curl_mvsprintf(buffer, format, args);
    
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Test curl_version
    const char *ver = curl_version();
    if (ver) {
        (void)strlen(ver);
    }

    // Test curl_getenv
    char *env_val = curl_getenv("PATH");
    if (env_val) {
        (void)strlen(env_val);
        curl_free(env_val);
    }

    // Test curl_url_strerror with various codes
    CURLUcode safe_codes[] = {CURLUE_OK, CURLUE_BAD_HANDLE, CURLUE_BAD_PARTPOINTER,
                              CURLUE_MALFORMED_INPUT, CURLUE_OUT_OF_MEMORY,
                              CURLUE_UNSUPPORTED_SCHEME, CURLUE_URLDECODE};
    size_t num_safe_codes = sizeof(safe_codes) / sizeof(safe_codes[0]);
    
    for (size_t i = 0; i < num_safe_codes; i++) {
        const char *err_str = curl_url_strerror(safe_codes[i]);
        if (err_str) {
            (void)strlen(err_str);
        }
    }

    // Test curl_msnprintf
    char buffer[256];
    if (Size >= 4) {
        // Ensure the input is null-terminated before passing to %s
        // We create a temporary buffer with a guaranteed null terminator
        char safe_input[256];
        size_t copy_len = (Size < sizeof(safe_input) - 1) ? Size : sizeof(safe_input) - 1;
        memcpy(safe_input, Data, copy_len);
        safe_input[copy_len] = '\0';

        int len = curl_msnprintf(buffer, sizeof(buffer), "%s", safe_input);
        (void)len;
    }

    // Test curl_maprintf
    char *mapped_str = NULL;
    if (Size >= 4) {
        char safe_input[256];
        size_t copy_len = (Size < sizeof(safe_input) - 1) ? Size : sizeof(safe_input) - 1;
        memcpy(safe_input, Data, copy_len);
        safe_input[copy_len] = '\0';

        mapped_str = curl_maprintf("%s", safe_input);
        if (mapped_str) {
            (void)strlen(mapped_str);
            curl_free(mapped_str);
            mapped_str = NULL;
        }
    }

    // Test curl_mvsprintf via helper
    if (Size >= 4) {
        char safe_input[256];
        size_t copy_len = (Size < sizeof(safe_input) - 1) ? Size : sizeof(safe_input) - 1;
        memcpy(safe_input, Data, copy_len);
        safe_input[copy_len] = '\0';

        int vlen = static_helper_mvsprintf("%s", safe_input);
        (void)vlen;
    }

    // Additional diverse tests with format variations
    if (Size >= 2) {
        int len2 = curl_msnprintf(buffer, sizeof(buffer), "%d", (int)Data[0]);
        (void)len2;

        char *mapped_str2 = curl_maprintf("%d", (int)Data[0]);
        if (mapped_str2) {
            curl_free(mapped_str2);
        }
    }

    if (Size >= 1) {
        char *mapped_str3 = curl_maprintf("%c", (char)Data[0]);
        if (mapped_str3) {
            curl_free(mapped_str3);
        }
    }

    return 0;
}