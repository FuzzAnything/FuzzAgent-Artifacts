// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void setup_fuzzed_string(const uint8_t *data, size_t size, char **out_str, size_t *out_size) {
    if (!data || size == 0) {
        *out_str = NULL;
        *out_size = 0;
        return;
    }

    // Ensure null-termination by allocating size + 1
    char *str = (char *)malloc(size + 1);
    if (!str) {
        *out_str = NULL;
        *out_size = 0;
        return;
    }

    memcpy(str, data, size);
    str[size] = '\0';
    *out_str = str;
    *out_size = size;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        return 0;
    }

    // Prepare sanitized string for curl_global_trace
    char *trace_config = NULL;
    size_t trace_size = 0;
    setup_fuzzed_string(Data, Size, &trace_config, &trace_size);

    // Call curl_global_trace if we have a valid string
    if (trace_config) {
        curl_global_trace(trace_config);
        free(trace_config);
        trace_config = NULL;
    }

    // Create a shared handle for curl_share_setopt
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // Try to set share options
    // We can try sharing DNS, cookies, etc.
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);

    // Create an easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Associate share with easy handle
    curl_easy_setopt(curl, CURLOPT_SHARE, share);

    // Prepare error buffer (must be at least CURL_ERROR_SIZE)
    char *error_buffer = (char *)malloc(CURL_ERROR_SIZE);
    if (error_buffer) {
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
    }

    // Prepare URL from fuzz input (sanitized)
    char *url_str = NULL;
    size_t url_size = 0;
    setup_fuzzed_string(Data, Size, &url_str, &url_size);

    if (url_str) {
        curl_easy_setopt(curl, CURLOPT_URL, url_str);
    } else {
        // Fallback URL if input is empty or invalid
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    // Set other options to explore more states
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // Perform the transfer
    curl_easy_perform(curl);

    // Get info after perform
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    // Perform upkeep
    curl_easy_upkeep(curl);

    // Cleanup
    if (url_str) {
        free(url_str);
    }
    if (error_buffer) {
        free(error_buffer);
    }

    curl_easy_cleanup(curl);
    curl_share_cleanup(share);
    curl_global_cleanup();

    return 0;
}