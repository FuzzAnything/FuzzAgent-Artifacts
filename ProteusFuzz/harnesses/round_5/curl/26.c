// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>
#include <time.h>

static void safe_free(void *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

// Helper function to allow va_start usage for curl_mvsprintf
// The last parameter must be a named argument for va_start to work correctly.
// We pass a dummy argument to satisfy the variadic requirement.
static void test_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = curl_mvsprintf(buffer, format, args);
    va_end(args);
    (void)len;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null-termination for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test curl_getdate
    // Parses RFC 2822 date strings.
    time_t timestamp = curl_getdate(input_str, NULL);
    (void)timestamp; // Result is ignored, we care about stability

    // 2. Test curl_escape
    // URL-encodes a string.
    char *escaped = curl_escape(input_str, Size);
    if (escaped) {
        // Optional: verify length or content, but primarily checking for crashes
        size_t escaped_len = strlen(escaped);
        (void)escaped_len;
        safe_free(escaped);
    }

    // 3. Test curl_unescape
    // Decodes URL-encoded strings.
    char *unescaped = curl_unescape(input_str, Size);
    if (unescaped) {
        safe_free(unescaped);
    }

    // 4. Test curl_maprintf
    // Formats string into dynamically allocated buffer.
    // We use the input data as the format string, which is risky but good for fuzzing.
    // We need to handle potential crashes if format is invalid.
    char *mapped = curl_maprintf("%s", input_str);
    if (mapped) {
        safe_free(mapped);
    }

    // 5. Test curl_msnprintf
    // Safe formatting into a fixed buffer.
    char buffer[1024];
    int written = curl_msnprintf(buffer, sizeof(buffer), "%s", input_str);
    (void)written;

    // 6. Test curl_mvsprintf
    // Formats using va_list.
    // We call a helper function to satisfy the va_start requirement.
    test_mvsprintf(buffer, "%s", input_str);

    // Cleanup
    free(input_str);

    return 0;
}