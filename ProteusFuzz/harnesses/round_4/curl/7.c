// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strequal at strequal.c:76:5 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/header.h>
#include <curl/mprintf.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    CURL *easy = NULL;
    struct curl_header *hout = NULL;
    struct curl_header *prev = NULL;
    char *unescaped = NULL;
    int outlength = 0;
    char buffer[1024];
    int ret;
    int i;

    if (Size == 0) {
        return 0;
    }

    // Initialize curl easy handle
    easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    // Set a simple URL to ensure the handle is valid for some operations
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L); // HEAD request to avoid large body
    curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(easy, CURLOPT_MAXREDIRS, 0L);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 100L);

    // Perform the request to populate headers
    (void)curl_easy_perform(easy);

    // Fuzz curl_easy_header
    // Try different indices and names from the input data
    for (i = 0; i < Size && i < 100; i++) {
        size_t index = (Data[i] % 10) + 1;
        const char *name = "Content-Type";
        char name_buf[64];
        size_t name_len = 0;
        
        if (i < Size - 1) {
            // Ensure we don't read past the buffer end
            size_t remaining = Size - (i + 1);
            if (remaining > 0) {
                name_len = strnlen((const char *)(Data + i + 1), remaining);
                if (name_len == 0) name_len = 1; // Ensure non-empty if possible, but safe
                if (name_len > 63) name_len = 63;
                memcpy(name_buf, Data + i + 1, name_len);
                name_buf[name_len] = '\0';
                name = name_buf;
            } else {
                // Not enough data for a name, use default
                name = "Content-Type";
            }
        }

        struct curl_header *h = NULL;
        ret = curl_easy_header(easy, name, index, CURLH_HEADER, 0, &h);
        (void)ret;
        (void)h;
    }

    // Fuzz curl_easy_nextheader
    // Iterate through headers
    prev = NULL;
    for (i = 0; i < 20; i++) {
        prev = curl_easy_nextheader(easy, CURLH_HEADER, 0, prev);
        if (!prev) break;
    }

    // Fuzz curl_easy_unescape
    // Use the input data as the string to unescape
    if (Size > 0) {
        unescaped = curl_easy_unescape(easy, (const char *)Data, (int)Size, &outlength);
        if (unescaped) {
            free(unescaped);
            unescaped = NULL;
        }
    }

    // Fuzz curl_mprintf
    // Use a safe format string derived from input or a constant
    ret = curl_mprintf("Test: %s\n", (Size > 10) ? "safe" : "safe");
    (void)ret;

    // Fuzz curl_msprintf
    // Use a safe format string
    ret = curl_msprintf(buffer, "Buffer: %s", (Size > 10) ? "safe" : "safe");
    (void)ret;

    // Fuzz curl_strequal
    // Compare slices of input data
    if (Size >= 2) {
        char s1[64], s2[64];
        size_t len1 = (Data[0] % 64) + 1;
        size_t len2 = (Data[1] % 64) + 1;
        
        // Ensure we don't read past the buffer
        size_t available = Size - 2;
        if (len1 > available) len1 = available;
        if (len2 > available - len1) len2 = available - len1;
        
        if (len1 > 63) len1 = 63;
        if (len2 > 63) len2 = 63;

        if (len1 > 0) {
            memcpy(s1, Data + 2, len1);
            s1[len1] = '\0';
        } else {
            s1[0] = '\0';
        }

        if (len2 > 0) {
            memcpy(s2, Data + 2 + len1, len2);
            s2[len2] = '\0';
        } else {
            s2[0] = '\0';
        }

        ret = curl_strequal(s1, s2);
        (void)ret;
    }

    // Cleanup
    curl_easy_cleanup(easy);

    return 0;
}