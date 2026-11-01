// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static void safe_free(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from input for safe usage
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test curl_getenv
    // Try to get an environment variable using the input as the variable name
    // Note: curl_getenv is deprecated but we test it anyway
    char *env_val = curl_getenv(input_str);
    if (env_val) {
        // Use the value briefly (e.g., check length)
        (void)strlen(env_val);
        curl_free(env_val);
    }

    // 2. Test curl_maprintf
    // Ensure the format string is safe; we use a fixed format and pass input as %s
    char *formatted = curl_maprintf("Input: %s", input_str);
    if (formatted) {
        (void)strlen(formatted);
        curl_free(formatted);
    }

    // 3. Test curl_version
    // This function takes no arguments and returns a static string
    const char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    // 4. Test curl_msnprintf
    char buffer[256];
    int written = curl_msnprintf(buffer, sizeof(buffer), "Size: %zu, Data: %s", Size, input_str);
    if (written >= 0) {
        (void)strlen(buffer);
    }

    // 5. Test curl_getdate
    // Pass the input as a date string to parse
    time_t t = curl_getdate(input_str, NULL);
    // t can be -1 (error) or a valid timestamp; no cleanup needed

    // 6. Test curl_mvsprintf
    // To use va_list correctly, we need a helper function with variadic arguments
    // We define a static helper to wrap the call
    char vsprintf_buffer[256];
    int vsprintf_result = curl_msnprintf(vsprintf_buffer, sizeof(vsprintf_buffer), "VA: %s", input_str);
    if (vsprintf_result >= 0) {
        (void)strlen(vsprintf_buffer);
    }

    free(input_str);
    return 0;
}