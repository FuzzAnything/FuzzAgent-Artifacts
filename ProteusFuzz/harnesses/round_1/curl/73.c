// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
 * Correct signature for curl_ssls_export_cb based on curl.h:
 * CURLcode callback(void *userptr, void *session, const char *session_id,
 *                   const unsigned char *session_data, size_t session_data_len,
 *                   const unsigned char *session_key, size_t session_key_len,
 *                   long session_age, int session_type, const char *cipher,
 *                   unsigned long session_version);
 */
static CURLcode export_callback(void *userptr, void *session, const char *session_id,
                                const unsigned char *session_data, size_t session_data_len,
                                const unsigned char *session_key, size_t session_key_len,
                                long session_age, int session_type, const char *cipher,
                                unsigned long session_version) {
    (void)userptr;
    (void)session;
    (void)session_id;
    (void)session_data;
    (void)session_data_len;
    (void)session_key;
    (void)session_key_len;
    (void)session_age;
    (void)session_type;
    (void)cipher;
    (void)session_version;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *handle = NULL;

    // Initialize global curl
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    // Optional: Configure global tracing if data is sufficient
    if (Size > 10) {
        char *trace_config = malloc(Size + 1);
        if (trace_config) {
            memcpy(trace_config, Data, Size);
            trace_config[Size] = '\0';
            curl_global_trace(trace_config);
            free(trace_config);
        }
    }

    // Create a handle
    handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    // Prepare a null-terminated URL from fuzzing data
    char *url_buf = malloc(Size + 1);
    if (!url_buf) {
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_buf, Data, Size);
    url_buf[Size] = '\0';

    // Set URL
    res = curl_easy_setopt(handle, CURLOPT_URL, url_buf);
    if (res != CURLE_OK) {
        free(url_buf);
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        return 0;
    }

    // Set a user agent to avoid some server-side issues
    const char *user_agent = "fuzz-test-agent/1.0";
    curl_easy_setopt(handle, CURLOPT_USERAGENT, user_agent);

    // Set a timeout to prevent hanging
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100L);

    // Prepare POST data if needed (null-terminated)
    char *post_data = NULL;
    if (Size > 0) {
        post_data = malloc(Size + 1);
        if (post_data) {
            memcpy(post_data, Data, Size);
            post_data[Size] = '\0';
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data);
            // Keep post_data alive until cleanup
        }
    }

    // Perform the request
    res = curl_easy_perform(handle);

    // Try to get info after perform
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    double total_time = 0.0;
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);

    // Try SSL export if supported
    curl_easy_ssls_export(handle, export_callback, NULL);

    // Try upkeep
    curl_easy_upkeep(handle);

    // Cleanup
    curl_easy_cleanup(handle);
    free(url_buf);
    free(post_data);
    curl_global_cleanup();

    return 0;
}