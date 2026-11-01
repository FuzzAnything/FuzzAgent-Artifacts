// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>

/* 
 * Fuzz driver for curl API functions.
 * Target functions:
 * 1. Wcurl_multi_setopt_err_charpp (Compile-time check, simulated via setopt calls)
 * 2. curl_easy_unescape
 * 3. curl_mprintf
 * 4. curl_multi_info_read
 * 5. curl_msprintf
 * 6. curl_strequal
 */

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Prepare handles */
    CURL *easy_handle = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();

    if (!easy_handle || !multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* 
     * 1. Simulate Wcurl_multi_setopt_err_charpp and other type checks.
     */
    
    /* Try setting a string option (char **) to trigger charpp check logic if applicable */
    if (Size > 0) {
        char *url_buf = malloc(Size + 1);
        if (url_buf) {
            memcpy(url_buf, Data, Size);
            url_buf[Size] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
            free(url_buf);
        }
    }

    /* Try setting a long option */
    long long_val = 0;
    if (Size >= sizeof(long)) {
        memcpy(&long_val, Data, sizeof(long));
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, long_val);
    }

    /* Try setting a callback option (void *) */
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);

    /* Try setting a multi option */
    if (Size >= sizeof(long)) {
        long mc_val = 0;
        memcpy(&mc_val, Data, sizeof(long));
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, mc_val);
    }

    /* 
     * 2. curl_easy_unescape
     */
    int outlen = 0;
    char *unescaped = curl_easy_unescape(easy_handle, (const char *)Data, (int)Size, &outlen);
    if (unescaped) {
        free(unescaped);
    }

    /* 
     * 3. curl_mprintf
     * FIX: Use %.*s format specifier to pass the exact size of the data.
     * This prevents strlen from reading past the buffer if Data is not null-terminated.
     */
    if (Size > 0) {
        curl_mprintf("%.*s", (int)Size, (const char *)Data);
    }

    /* 
     * 4. curl_multi_info_read
     */
    int msgs_left = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_left);
    (void)msg;

    /* 
     * 5. curl_msprintf
     * FIX: Use %.*s format specifier here as well.
     */
    char buffer[1024];
    if (Size > 0) {
        int written = curl_msprintf(buffer, "%.*s", (int)Size, (const char *)Data);
        (void)written;
    }

    /* 
     * 6. curl_strequal
     */
    if (Size > 0) {
        char s1[1024];
        size_t copy_len = (Size < 1023) ? Size : 1023;
        memcpy(s1, Data, copy_len);
        s1[copy_len] = '\0';
        
        int eq = curl_strequal(s1, s1);
        (void)eq;
        
        int eq2 = curl_strequal(s1, "test");
        (void)eq2;
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}