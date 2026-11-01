// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>

static CURLcode ssls_export_callback(
    void *userptr,
    void *session,
    const char *session_id,
    const unsigned char *session_data,
    unsigned long session_data_len,
    const unsigned char *session_key,
    unsigned long session_key_len,
    long session_version,
    int session_type,
    const char *session_protocol,
    unsigned long session_protocol_len) {
    (void)userptr;
    (void)session;
    (void)session_id;
    (void)session_data;
    (void)session_data_len;
    (void)session_key;
    (void)session_key_len;
    (void)session_version;
    (void)session_type;
    (void)session_protocol;
    (void)session_protocol_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Ensure string options are null-terminated
    char *url_buf = NULL;
    char *post_buf = NULL;

    if (Size > 0) {
        url_buf = malloc(Size + 1);
        if (url_buf) {
            memcpy(url_buf, Data, Size);
            url_buf[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url_buf);
        }
    }

    if (Size > 0) {
        post_buf = malloc(Size + 1);
        if (post_buf) {
            memcpy(post_buf, Data, Size);
            post_buf[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf);
        }
    }

    // Fuzz curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Fuzz curl_easy_pause with various bitmasks
    if (Size > 0) {
        int bitmask = Data[0] & 0x7; // Limit to valid bits
        curl_easy_pause(curl, bitmask);
    }

    // Fuzz curl_easy_perform
    curl_easy_perform(curl);

    // Fuzz curl_easy_getinfo with various info types
    long http_code = 0;
    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    // Fuzz curl_easy_ssls_export
    curl_easy_ssls_export(curl, ssls_export_callback, (void *)Data);

    // Cleanup
    if (url_buf) free(url_buf);
    if (post_buf) free(post_buf);
    curl_easy_cleanup(curl);

    return 0;
}