// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>

// Define the callback type matching the expected signature for curl_ssls_export_cb
// Based on the error message, the expected signature is:
// CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, 
//              const unsigned char *, unsigned long, long, int, const char *, unsigned long)
// Parameters: (userptr, ssl_ctx, session_id, session_id_ptr, session_id_len, 
//              session, session_len, age, version, protocol, protocol_len)
static CURLcode dummy_ssl_export_cb(void *userptr, void *ssl_ctx, 
                                    const char *session_id, const unsigned char *session_id_ptr, 
                                    unsigned long session_id_len, 
                                    const unsigned char *session, unsigned long session_len, 
                                    long age, int version, const char *protocol, unsigned long protocol_len) {
    (void)userptr;
    (void)ssl_ctx;
    (void)session_id;
    (void)session_id_ptr;
    (void)session_id_len;
    (void)session;
    (void)session_len;
    (void)age;
    (void)version;
    (void)protocol;
    (void)protocol_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize libcurl
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    // Sanitize input for curl_global_trace
    char *trace_config = (char *)malloc(Size + 1);
    if (!trace_config) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(trace_config, Data, Size);
    trace_config[Size] = '\0';

    // Call curl_global_trace
    curl_global_trace(trace_config);
    free(trace_config);

    // Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Allocate error buffer dynamically
    char *error_buffer = (char *)malloc(CURL_ERROR_SIZE);
    if (!error_buffer) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Set error buffer
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    // Prepare a sanitized URL from input
    char *url_buffer = (char *)malloc(Size + 1);
    if (!url_buffer) {
        free(error_buffer);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_buffer, Data, Size);
    url_buffer[Size] = '\0';

    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url_buffer);

    // Set other options to explore more states
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "fuzz-agent");

    // Perform the request
    curl_easy_perform(curl);

    // Call curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Get some info
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    // Try to export SSL sessions (if supported)
    curl_easy_ssls_export(curl, dummy_ssl_export_cb, NULL);

    // Cleanup
    free(url_buffer);
    free(error_buffer);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}