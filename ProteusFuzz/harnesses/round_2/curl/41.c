// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

/* Callback for curl_easy_ssls_export matching the expected signature */
static CURLcode ssls_export_callback(
    void *userptr,
    void *session,
    const char *session_id,
    const unsigned char *session_data,
    unsigned long session_len,
    const unsigned char *cipher_data,
    unsigned long cipher_len,
    long cipher_id,
    int cipher_type,
    const char *cipher_name,
    unsigned long cipher_name_len) {
    (void)userptr;
    (void)session;
    (void)session_id;
    (void)session_data;
    (void)session_len;
    (void)cipher_data;
    (void)cipher_len;
    (void)cipher_id;
    (void)cipher_type;
    (void)cipher_name;
    (void)cipher_name_len;
    return CURLE_OK;
}

/* Callback for data writing */
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* Callback for data reading */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

/* Callback for header writing */
static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Create a safe null-terminated URL from input */
    char url_buf[256];
    size_t url_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    /* Create a safe null-terminated POST data from input */
    char post_buf[256];
    size_t post_len = (Size < sizeof(post_buf) - 1) ? Size : sizeof(post_buf) - 1;
    memcpy(post_buf, Data, post_len);
    post_buf[post_len] = '\0';

    /* Set basic options to ensure a valid state for perform */
    curl_easy_setopt(handle, CURLOPT_URL, url_buf);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

    /* Fuzz curl_easy_setopt with various options using input data */
    if (Size > 0) {
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_buf);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (long)post_len);
    }

    /* Fuzz curl_easy_perform */
    curl_easy_perform(handle);

    /* Fuzz curl_easy_getinfo */
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    double total_time = 0.0;
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);

    /* Fuzz curl_easy_pause */
    int pause_mask = (Size > 0) ? (Data[0] % 8) : 0;
    curl_easy_pause(handle, pause_mask);

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(handle);

    /* Fuzz curl_easy_ssls_export */
    curl_easy_ssls_export(handle, ssls_export_callback, NULL);

    /* Cleanup */
    curl_easy_cleanup(handle);

    return 0;
}