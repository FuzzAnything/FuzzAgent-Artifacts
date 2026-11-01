// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

static void safe_string_copy(char *dest, size_t dest_size, const uint8_t *src, size_t src_size) {
    size_t copy_len = src_size < dest_size - 1 ? src_size : dest_size - 1;
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';
}

// Match the exact signature of curl_ssls_export_cb from curl.h
// CURLcode ssl_export_cb(void *userptr, void *session, const char *session_id,
//                        const unsigned char *session_data, size_t session_data_len,
//                        const unsigned char *session_key, size_t session_key_len,
//                        long session_age, int session_type, const char *cipher, size_t cipher_len);
static CURLcode ssl_export_callback(void *userptr, void *session, const char *session_id,
                                    const unsigned char *session_data, size_t session_data_len,
                                    const unsigned char *session_key, size_t session_key_len,
                                    long session_age, int session_type, const char *cipher, size_t cipher_len) {
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
    (void)cipher_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Allocate error buffer dynamically as per requirements
    char *error_buffer = (char *)malloc(CURL_ERROR_SIZE);
    if (!error_buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }

    // Set error buffer
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    // Prepare a safe URL string from input
    char url_buffer[256];
    safe_string_copy(url_buffer, sizeof(url_buffer), Data, Size < 255 ? Size : 255);
    // Ensure it looks like a URL to avoid immediate failures, though fuzzing may still trigger internal errors
    if (strlen(url_buffer) == 0 || url_buffer[0] != 'h') {
        strcpy(url_buffer, "http://example.com");
    }
    curl_easy_setopt(curl, CURLOPT_URL, url_buffer);

    // Prepare a safe User-Agent string
    char ua_buffer[64];
    safe_string_copy(ua_buffer, sizeof(ua_buffer), Data, Size < 63 ? Size : 63);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, ua_buffer);

    // Set other options to exercise code paths
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    // Exercise curl_easy_perform
    curl_easy_perform(curl);

    // Exercise curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Exercise curl_easy_pause with various bitmasks
    curl_easy_pause(curl, CURLPAUSE_RECV);
    curl_easy_pause(curl, CURLPAUSE_SEND);
    curl_easy_pause(curl, CURLPAUSE_ALL);
    curl_easy_pause(curl, CURLPAUSE_CONT);

    // Exercise curl_easy_getinfo
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    // Exercise curl_easy_ssls_export if available
    curl_easy_ssls_export(curl, ssl_export_callback, (void *)0x1234);

    // Cleanup
    free(error_buffer);
    curl_easy_cleanup(curl);

    return 0;
}