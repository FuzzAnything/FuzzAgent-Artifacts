// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/* 
 * Callback for curl_easy_ssls_export.
 * Matches the signature expected by recent curl versions.
 */
static CURLcode ssl_export_callback(void *handle, void *userptr,
                                    const char *session_id,
                                    const unsigned char *session_data,
                                    size_t session_data_len,
                                    const unsigned char *session_data2,
                                    size_t session_data2_len,
                                    long timestamp,
                                    int type,
                                    const char *protocol,
                                    size_t protocol_len) {
    (void)handle;
    (void)userptr;
    (void)session_id;
    (void)session_data;
    (void)session_data_len;
    (void)session_data2;
    (void)session_data2_len;
    (void)timestamp;
    (void)type;
    (void)protocol;
    (void)protocol_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Initialize curl global options if needed
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        curl_easy_cleanup(handle);
        return 0;
    }

    // Try to set global trace with fuzzed input (null-terminated)
    char *trace_config = malloc(Size + 1);
    if (trace_config) {
        memcpy(trace_config, Data, Size);
        trace_config[Size] = '\0';
        curl_global_trace(trace_config);
        free(trace_config);
    }

    // Set various options using fuzzed data
    // Ensure null-termination for string options
    if (Size > 0) {
        char *url = malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(handle, CURLOPT_URL, url);
            free(url);
        }

        char *useragent = malloc(Size + 1);
        if (useragent) {
            memcpy(useragent, Data, Size);
            useragent[Size] = '\0';
            curl_easy_setopt(handle, CURLOPT_USERAGENT, useragent);
            free(useragent);
        }

        char *proxy = malloc(Size + 1);
        if (proxy) {
            memcpy(proxy, Data, Size);
            proxy[Size] = '\0';
            curl_easy_setopt(handle, CURLOPT_PROXY, proxy);
            free(proxy);
        }

        // Keep postfields alive until after curl_easy_perform
        char *postfields = malloc(Size + 1);
        if (postfields) {
            memcpy(postfields, Data, Size);
            postfields[Size] = '\0';
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postfields);
            curl_easy_setopt(handle, CURLOPT_POST, 1L);
        }

        // Perform the transfer while postfields is still valid
        res = curl_easy_perform(handle);
        (void)res;

        // Retrieve info after perform
        long http_code = 0;
        res = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
        (void)res;

        struct curl_certinfo certinfo;
        memset(&certinfo, 0, sizeof(certinfo));
        res = curl_easy_getinfo(handle, CURLINFO_CERTINFO, &certinfo);
        (void)res;

        // Free postfields after perform
        free(postfields);
    } else {
        // Perform even if no options set (with default URL or error)
        res = curl_easy_perform(handle);
        (void)res;
    }

    // Try SSL export if supported
    res = curl_easy_ssls_export(handle, ssl_export_callback, NULL);
    (void)res;

    // Perform upkeep
    res = curl_easy_upkeep(handle);
    (void)res;

    // Cleanup
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return 0;
}