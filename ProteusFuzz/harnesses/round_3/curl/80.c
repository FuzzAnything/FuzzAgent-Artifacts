// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

// Correct signature for curl_ssls_export_cb based on the error message
// CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
static CURLcode dummy_ssls_export_cb(void *userptr, void *handle, const char *session_id,
                                     const unsigned char *session_data, unsigned long session_data_len,
                                     const unsigned char *session_key, unsigned long session_key_len,
                                     long session_age, int session_protocol,
                                     const char *session_cipher, unsigned long session_cipher_len) {
    (void)userptr;
    (void)handle;
    (void)session_id;
    (void)session_data;
    (void)session_data_len;
    (void)session_key;
    (void)session_key_len;
    (void)session_age;
    (void)session_protocol;
    (void)session_cipher;
    (void)session_cipher_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Prepare a sanitized null-terminated string for curl_global_trace
    char *config_str = (char *)malloc(Size + 1);
    if (!config_str) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(config_str, Data, Size);
    config_str[Size] = '\0';

    // Call curl_global_trace with sanitized input
    curl_global_trace(config_str);

    free(config_str);

    // Create a curl handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to ensure the handle is in a valid state for operations
    // Using a dummy file or invalid URL to avoid network dependency in fuzzing
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 0L);

    // Fuzz curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Fuzz curl_easy_pause with various bitmasks
    int pause_masks[] = {
        CURLPAUSE_RECV,
        CURLPAUSE_SEND,
        CURLPAUSE_RECV | CURLPAUSE_SEND,
        CURLPAUSE_CONT,
        0
    };
    for (int i = 0; i < 5; i++) {
        curl_easy_pause(curl, pause_masks[i]);
    }

    // Fuzz curl_easy_perform
    // Note: This may fail due to invalid file or network issues, which is expected in fuzzing
    curl_easy_perform(curl);

    // Fuzz curl_easy_getinfo with various CURLINFO options
    long response_code = 0;
    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    // Fuzz curl_easy_ssls_export if available
    curl_easy_ssls_export(curl, dummy_ssls_export_cb, NULL);

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}