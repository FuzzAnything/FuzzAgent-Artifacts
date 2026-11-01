// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/* 
 * The curl_ssls_export_cb signature is complex and varies by curl version.
 * Based on the error message, the expected signature is:
 * CURLcode (*)(void *userptr, void *session, const char *data1, const unsigned char *data2, 
 *              unsigned long len1, const unsigned char *data3, unsigned long len2, 
 *              long time, int flags, const char *data4, unsigned long len3)
 * 
 * We define a compatible callback that ignores all arguments to avoid compilation errors.
 */
static CURLcode export_callback(void *userptr, void *session, 
                                const char *data1, const unsigned char *data2, 
                                unsigned long len1, const unsigned char *data3, 
                                unsigned long len2, long time, int flags, 
                                const char *data4, unsigned long len3) {
    (void)userptr;
    (void)session;
    (void)data1;
    (void)data2;
    (void)len1;
    (void)data3;
    (void)len2;
    (void)time;
    (void)flags;
    (void)data4;
    (void)len3;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Initialize global trace with a safe default or derived from input
    // Ensure null termination if we construct a string
    char trace_config[256];
    size_t trace_len = (Size < 255) ? Size : 255;
    memcpy(trace_config, Data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);

    // Prepare URL from input, ensuring null termination
    char url_buf[1024];
    size_t url_len = (Size < 1023) ? Size : 1023;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    // Prepare POST fields from input, ensuring null termination and lifetime management
    // We allocate a separate buffer to ensure it stays valid during perform
    char *post_data = NULL;
    if (Size > 0) {
        post_data = malloc(Size + 1);
        if (post_data) {
            memcpy(post_data, Data, Size);
            post_data[Size] = '\0';
        }
    }

    // Set options
    curl_easy_setopt(handle, CURLOPT_URL, url_buf);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "FuzzingAgent/1.0");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    
    if (post_data) {
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data);
    }

    // Perform the transfer
    curl_easy_perform(handle);

    // Get info after perform
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    struct curl_certinfo certinfo;
    memset(&certinfo, 0, sizeof(certinfo));
    curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo);

    // Call upkeep
    curl_easy_upkeep(handle);

    // Attempt SSL session export
    curl_easy_ssls_export(handle, export_callback, NULL);

    // Cleanup
    if (post_data) {
        free(post_data);
    }
    curl_easy_cleanup(handle);

    return 0;
}