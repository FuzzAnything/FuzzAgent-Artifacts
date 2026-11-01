// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

/* 
 * The callback signature must match curl_ssls_export_cb exactly.
 * Based on the error message, the expected signature is:
 * CURLcode (*)(void *, void *, const char *, const unsigned char *, 
 *              unsigned long, const unsigned char *, unsigned long, 
 *              long, int, const char *, unsigned long)
 * Note: The userptr is NOT the last argument in the callback signature.
 */
static CURLcode ssls_export_callback(
    void *easy_handle,
    void *session_handle,
    const char *session_id,
    const unsigned char *session_id_len,
    unsigned long session_id_len_len,
    const unsigned char *session_data,
    unsigned long session_len,
    long session_type,
    int session_protocol,
    const char *session_protocol_str,
    unsigned long session_protocol_len) {
    (void)easy_handle;
    (void)session_handle;
    (void)session_id;
    (void)session_id_len;
    (void)session_id_len_len;
    (void)session_data;
    (void)session_len;
    (void)session_type;
    (void)session_protocol;
    (void)session_protocol_str;
    (void)session_protocol_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    char *url_buf = NULL;
    char *post_buf = NULL;
    long http_code = 0;
    double speed_download = 0.0;
    double speed_upload = 0.0;
    long size_download = 0;
    long size_upload = 0;
    long response_code = 0;

    if (Size == 0) {
        return 0;
    }

    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Initialize the handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Prepare URL buffer (null-terminated)
    url_buf = (char *)malloc(Size + 1);
    if (!url_buf) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_buf, Data, Size);
    url_buf[Size] = '\0';

    // Prepare POST data buffer (null-terminated)
    post_buf = (char *)malloc(Size + 1);
    if (!post_buf) {
        free(url_buf);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    memcpy(post_buf, Data, Size);
    post_buf[Size] = '\0';

    // Configure tracing if possible
    curl_global_trace("all");

    // Set options
    res = curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    if (res != CURLE_OK) {
        goto cleanup;
    }

    // Set user agent
    res = curl_easy_setopt(curl, CURLOPT_USERAGENT, "Fuzzer/1.0");
    if (res != CURLE_OK) {
        goto cleanup;
    }

    // Set POST fields
    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf);
    if (res != CURLE_OK) {
        goto cleanup;
    }

    // Set POST size explicitly
    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size);
    if (res != CURLE_OK) {
        goto cleanup;
    }

    // Set timeout
    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    if (res != CURLE_OK) {
        goto cleanup;
    }

    // Disable SSL verification for fuzzing (optional, but common in fuzzing)
    res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    if (res != CURLE_OK) {
        goto cleanup;
    }
    res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    if (res != CURLE_OK) {
        goto cleanup;
    }

    // Perform the request
    res = curl_easy_perform(curl);

    // Get info after perform
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &size_upload);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    // Try SSL session export
    curl_easy_ssls_export(curl, ssls_export_callback, NULL);

    // Try upkeep
    curl_easy_upkeep(curl);

cleanup:
    // Cleanup
    if (url_buf) free(url_buf);
    if (post_buf) free(post_buf);
    if (curl) curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}