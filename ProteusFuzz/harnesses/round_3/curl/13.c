// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static void cleanup_result(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Split input into parts for different functions
    size_t part_len = Size / 3;
    const char *input1 = (const char *)(Data);
    const char *input2 = (const char *)(Data + part_len);
    const char *input3 = (const char *)(Data + 2 * part_len);
    size_t len1 = part_len;
    size_t len2 = part_len;
    size_t len3 = Size - 2 * part_len;

    // Ensure null termination for string functions where needed
    char *unescaped = NULL;
    char *escaped = NULL;
    char *escaped2 = NULL;

    // Test curl_easy_unescape
    // Only pass length if the data is valid or length is 0. 
    // The library may read past the provided length if it expects null termination 
    // but we provide a length. However, the API contract says "length" bytes.
    // The crash suggests the library is reading beyond the buffer. 
    // We must ensure the input buffer is null-terminated if the library relies on it,
    // or strictly rely on the length parameter.
    // To be safe against internal strlen calls if length is 0 or if the library
    // has a bug, we ensure the buffer is null-terminated at the specified length.
    // But we cannot modify the input buffer. 
    // Instead, we pass a length that is strictly within bounds.
    // The error shows strlen being called. This happens if length is 0 in curl_easy_unescape?
    // Looking at the stack trace: alloc = (length ? length : strlen(string));
    // If length is 0, it calls strlen. If the buffer is not null-terminated within the fuzz input,
    // strlen will read past the buffer.
    // Fix: Ensure we never pass length=0 unless the string is null-terminated.
    // Since we can't guarantee null termination in raw fuzz data, we avoid length=0 for unescape/escape
    // unless we verify null termination.
    
    int len1_int = (int)len1;
    int len2_int = (int)len2;
    int len3_int = (int)len3;

    // For curl_easy_unescape: if len1 is 0, we must ensure input1 is null-terminated.
    // Since we can't guarantee that, we skip the call if len1 is 0, or ensure we pass a valid length.
    // Actually, if len1 > 0, the library uses length. If len1 == 0, it uses strlen.
    // To prevent overflow, we must ensure that if we pass 0, the string is null-terminated.
    // We can't modify Data. So we simply avoid passing 0 length for these functions 
    // unless we check for null termination.
    
    // Safer approach: Only call if length > 0, or if length == 0 and we find a null terminator.
    // But checking for null terminator in raw data is expensive and complex.
    // Simpler: Just ensure we don't pass 0 length if the data might not be null-terminated.
    // However, the API allows 0 length to mean "null-terminated string".
    // The crash happens because the fuzzer provides raw bytes that don't end in \0 within the buffer.
    // So if we pass length=0, strlen runs off the end.
    // Solution: If len1 == 0, we skip the call or handle it carefully.
    // Actually, we can just ensure we pass a non-zero length if the data isn't null-terminated.
    // But we don't know if it's null-terminated.
    // Best fix: If len1 == 0, we check if Data[0] is '\0'. If not, we don't call with 0 length.
    // Or, we can just force length to be at least 1 if the first byte isn't null? No, that changes semantics.
    // The robust fix: If length is 0, we MUST ensure the string is null-terminated.
    // Since we can't ensure that, we simply skip the call if length is 0.
    
    if (len1 > 0) {
        int outlen = 0;
        unescaped = curl_easy_unescape(handle, input1, len1_int, &outlen);
        if (unescaped) {
            cleanup_result(unescaped);
        }
    } else if (len1 == 0 && input1[0] == '\0') {
        int outlen = 0;
        unescaped = curl_easy_unescape(handle, input1, 0, &outlen);
        if (unescaped) {
            cleanup_result(unescaped);
        }
    }

    // Test curl_easy_escape
    if (len2 > 0) {
        escaped = curl_easy_escape(handle, input2, len2_int);
        if (escaped) {
            cleanup_result(escaped);
        }
    } else if (len2 == 0 && input2[0] == '\0') {
        escaped = curl_easy_escape(handle, input2, 0);
        if (escaped) {
            cleanup_result(escaped);
        }
    }

    // Test curl_escape
    if (len3 > 0) {
        escaped2 = curl_escape(input3, len3_int);
        if (escaped2) {
            cleanup_result(escaped2);
        }
    } else if (len3 == 0 && input3[0] == '\0') {
        escaped2 = curl_escape(input3, 0);
        if (escaped2) {
            cleanup_result(escaped2);
        }
    }

    // Test curl_msprintf
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // Use a safe format string derived from input to avoid crashes
    // We construct a simple format string from the first few bytes of input1
    char format_buf[64];
    int fmt_len = len1 < 63 ? (int)len1 : 63;
    // Copy safely
    if (fmt_len > 0) {
        memcpy(format_buf, input1, fmt_len);
        format_buf[fmt_len] = '\0';
        // Ensure format string is safe by stripping dangerous chars
        for (int i = 0; i < fmt_len; i++) {
            if (format_buf[i] == '%') {
                format_buf[i] = ' ';
            }
        }
        int msprintf_ret = curl_msprintf(buffer, "%s", format_buf);
        (void)msprintf_ret;
    }

    // Test curl_mprintf
    // Only print if format string is safe
    if (fmt_len > 0) {
        int mprintf_ret = curl_mprintf("%s", format_buf);
        (void)mprintf_ret;
    }

    // Test curl_strequal
    // Ensure we don't pass NULL if we expect non-NULL, but curl_strequal handles NULLs.
    // However, if input1 or input2 are not null-terminated, strequal might read past buffer?
    // curl_strequal usually uses strcmp or similar, which requires null termination.
    // If the strings are not null-terminated, this could also overflow.
    // We should only call strequal if the strings are null-terminated or we limit the comparison?
    // curl_strequal signature: int curl_strequal(const char *s1, const char *s2);
    // It likely uses strcmp. If s1 or s2 are not null-terminated, it will read past the buffer.
    // We must ensure null termination for strequal as well.
    // Since we can't modify the buffer, we only call strequal if we know they are null-terminated.
    // Or we can create temporary null-terminated copies.
    
    // Let's create safe copies for strequal to avoid overflow.
    char safe_s1[1024];
    char safe_s2[1024];
    size_t copy_len1 = len1 < sizeof(safe_s1) - 1 ? len1 : sizeof(safe_s1) - 1;
    size_t copy_len2 = len2 < sizeof(safe_s2) - 1 ? len2 : sizeof(safe_s2) - 1;
    
    memcpy(safe_s1, input1, copy_len1);
    safe_s1[copy_len1] = '\0';
    memcpy(safe_s2, input2, copy_len2);
    safe_s2[copy_len2] = '\0';
    
    int strequal_ret = curl_strequal(safe_s1, safe_s2);
    (void)strequal_ret;

    // Cleanup handle
    curl_easy_cleanup(handle);

    return 0;
}