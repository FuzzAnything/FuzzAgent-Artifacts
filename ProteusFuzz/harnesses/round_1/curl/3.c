// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_version at version.c:145:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void safe_string_from_fuzz(const uint8_t *Data, size_t Size, char **out_str, size_t *out_len) {
    if (Size == 0) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }

    // Allocate buffer with extra space for null terminator
    size_t alloc_size = Size + 1;
    char *buf = (char *)malloc(alloc_size);
    if (!buf) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }

    memcpy(buf, Data, Size);
    buf[Size] = '\0'; // Ensure null termination
    *out_str = buf;
    *out_len = Size;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment (good practice, though not strictly required for these specific functions)
    // curl_global_init(CURL_GLOBAL_ALL);

    char *str1 = NULL;
    size_t len1 = 0;
    safe_string_from_fuzz(Data, Size, &str1, &len1);

    char *str2 = NULL;
    size_t len2 = 0;
    safe_string_from_fuzz(Data, Size, &str2, &len2);

    // 1. Fuzz curl_msnprintf
    // We use a fixed buffer size to test boundary conditions
    char buffer[256];
    int ret_msnprintf = 0;

    // Test with a safe format string that uses the input data as a string
    // We use %.*s to safely handle the raw data length without relying on internal nulls if we were passing raw pointers,
    // but here str1 is already null-terminated.
    if (str1) {
        ret_msnprintf = curl_msnprintf(buffer, sizeof(buffer), "Input: %s", str1);
        (void)ret_msnprintf; // Suppress unused warning
    }

    // Test with length specifier to ensure no overflow if str1 was not null-terminated (it is, but good practice)
    if (str1 && len1 > 0) {
        ret_msnprintf = curl_msnprintf(buffer, sizeof(buffer), "Len: %.*s", (int)len1, str1);
        (void)ret_msnprintf;
    }

    // 2. Fuzz curl_maprintf
    char *result_maprintf = NULL;
    if (str1) {
        // Construct a format string carefully. We assume str1 might contain format specifiers.
        // To prevent undefined behavior if str1 contains %s without args, we use a fixed format.
        result_maprintf = curl_maprintf("Fuzzed: %s", str1);
        if (result_maprintf) {
            curl_free(result_maprintf);
            result_maprintf = NULL;
        }
    }

    // 3. Fuzz curl_share_strerror
    // We use the first byte of input as a CURLSHcode (cast to int)
    int error_code = (int)(Data[0] & 0xFF);
    // Also try negative values and larger values by combining bytes
    if (Size > 1) {
        error_code = (int)((Data[0] << 8) | Data[1]);
    }
    const char *str_err = curl_share_strerror((CURLSHcode)error_code);
    if (str_err) {
        // Just consume the string to avoid unused warnings
        (void)str_err;
    }

    // 4. Fuzz curl_version
    char *version = curl_version();
    if (version) {
        // Consume version string
        (void)version;
    }

    // 5. Fuzz curl_getenv
    // Use str1 as the environment variable name
    char *env_val = NULL;
    if (str1) {
        env_val = curl_getenv(str1);
        if (env_val) {
            curl_free(env_val);
        }
    }

    // Cleanup allocated strings from safe_string_from_fuzz
    if (str1) {
        free(str1);
    }
    if (str2) {
        free(str2);
    }

    // Cleanup curl global environment
    // curl_global_cleanup();

    return 0;
}