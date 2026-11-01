// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_header at headers.c:55:11 in header.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/header.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl header manipulation and utility functions.
 * Targets: curl_easy_header, curl_easy_nextheader, curl_mprintf, curl_strequal.
 * Note: Wcurl_* macros are compile-time checks and cannot be fuzzed at runtime.
 */

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global state */
    CURLcode init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (init_res != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * Prepare a minimal HTTP request to ensure headers might be available.
     * We use a dummy URL. In a real fuzzing scenario, this might fail to connect,
     * but curl_easy_header/nextheader should handle the error state gracefully.
     */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com/dummy");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L); /* HEAD request */
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 1L); /* Fail fast */
    curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT_MS, 1L);

    /* Perform the request to populate headers (or fail) */
    curl_easy_perform(easy);

    /*
     * Extract fuzzing parameters from input data
     */
    size_t idx = 0;
    size_t len = 0;
    int request_id = 0;
    unsigned int origin = 0;
    const char *name = NULL;
    const char *str1 = NULL;
    const char *str2 = NULL;
    const char *fmt = NULL;

    /* Parse integers safely */
    if (Size > 0) request_id = (int)Data[0];
    if (Size > 1) origin = (unsigned int)Data[1];
    if (Size > 2) len = (size_t)Data[2];
    if (Size > 3) idx = (size_t)Data[3];

    /*
     * Test 1: curl_easy_header
     * We try to retrieve a header by name and index.
     * We construct a name string from the input if possible.
     */
    struct curl_header *hout = NULL;
    
    /* Create a temporary buffer for the header name */
    char header_name_buf[256] = {0};
    if (Size > 4) {
        size_t name_len = (Size - 4 < 255) ? (Size - 4) : 255;
        memcpy(header_name_buf, Data + 4, name_len);
        name = header_name_buf;
    } else {
        name = "Content-Type"; /* Default fallback */
    }

    /* Invoke curl_easy_header with various combinations */
    /* Case A: Valid handle, valid name, index 0 */
    curl_easy_header(easy, name, 0, origin, request_id, &hout);
    
    /* Case B: Index from input */
    curl_easy_header(easy, name, idx, origin, request_id, &hout);
    
    /* Case C: NULL name (should trigger BAD_ARGUMENT) */
    curl_easy_header(easy, NULL, 0, origin, request_id, &hout);
    
    /* Case D: Invalid request ID (large value) */
    curl_easy_header(easy, name, 0, origin, -1, &hout);

    /*
     * Test 2: curl_easy_nextheader
     * Iterate through headers.
     */
    struct curl_header *prev = NULL;
    struct curl_header *next = NULL;

    /* Start iteration */
    next = curl_easy_nextheader(easy, origin, request_id, NULL);
    
    /* Continue iteration if we got a header */
    if (next) {
        prev = next;
        next = curl_easy_nextheader(easy, origin, request_id, prev);
    }

    /* Try with invalid origin */
    next = curl_easy_nextheader(easy, 99999, request_id, NULL);
    
    /* Try with invalid request ID */
    next = curl_easy_nextheader(easy, origin, -1, NULL);

    /*
     * Test 3: curl_mprintf
     * Format strings using input data.
     */
    char fmt_buf[64] = {0};
    if (Size > 8) {
        size_t fmt_len = (Size - 8 < 63) ? (Size - 8) : 63;
        memcpy(fmt_buf, Data + 8, fmt_len);
        fmt = fmt_buf;
    } else {
        fmt = "%s %d";
    }

    /* Ensure format string is safe-ish for fuzzing (avoid infinite loops if possible) */
    /* We just call it; if it crashes, the fuzzer catches it. */
    curl_mprintf(fmt, "fuzz", (int)idx);
    
    /* Test with NULL format (should handle gracefully or crash, fuzzer checks) */
    /* Note: curl_mprintf usually expects valid format, passing NULL is undefined but testable */
    /* We skip passing NULL to avoid immediate abort in some builds, but test edge cases */
    curl_mprintf("test", (const char*)NULL);

    /*
     * Test 4: curl_strequal
     * Compare strings from input.
     */
    char str1_buf[256] = {0};
    char str2_buf[256] = {0};

    if (Size > 10) {
        size_t s1_len = (Size - 10 < 255) ? (Size - 10) : 255;
        memcpy(str1_buf, Data + 10, s1_len);
        str1 = str1_buf;
    } else {
        str1 = "test";
    }

    if (Size > 260) {
        size_t s2_len = (Size - 260 < 255) ? (Size - 260) : 255;
        memcpy(str2_buf, Data + 260, s2_len);
        str2 = str2_buf;
    } else {
        str2 = "TEST";
    }

    int eq1 = curl_strequal(str1, str2);
    int eq2 = curl_strequal(NULL, NULL);
    int eq3 = curl_strequal(str1, NULL);
    int eq4 = curl_strequal(NULL, str2);

    /* Cleanup */
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    return 0;
}