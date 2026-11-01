// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return 0;
}

static int dummy_ssl_ctx_cb(CURL *curl, void *ssl_ctx, void *userptr) {
    (void)curl;
    (void)ssl_ctx;
    (void)userptr;
    return 0;
}

/* Correct signature for curl_ssls_export_cb based on curl.h */
static CURLcode dummy_ssls_export_cb(
    void *userptr,
    void *ssl_session,
    const char *protocol,
    const unsigned char *session_id,
    unsigned long session_id_len,
    const unsigned char *session,
    unsigned long session_len,
    long age,
    int age_type,
    const char *cipher,
    unsigned long cipher_len) {
    (void)userptr;
    (void)ssl_session;
    (void)protocol;
    (void)session_id;
    (void)session_id_len;
    (void)session;
    (void)session_len;
    (void)age;
    (void)age_type;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

static int dummy_conv_callback(char *in, size_t in_size, char *out, size_t out_size, size_t *out_bytes) {
    (void)in;
    (void)in_size;
    (void)out;
    (void)out_size;
    if (out_bytes) *out_bytes = 0;
    return 0;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *handle = curl_easy_init();
    if (!handle) return 0;

    /* Initialize a dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Set some basic options to ensure valid state for perform */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(handle, CURLOPT_SSL_CTX_FUNCTION, dummy_ssl_ctx_cb);
    curl_easy_setopt(handle, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(handle, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(handle, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);

    /* Fuzz curl_easy_pause with various bitmasks derived from input */
    int pause_mask = (Size > 0) ? (Data[0] & 0x07) : 0;
    curl_easy_pause(handle, pause_mask);

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(handle);

    /* Fuzz curl_easy_perform */
    curl_easy_perform(handle);

    /* Fuzz curl_easy_getinfo with various info types */
    long response_code = 0;
    double total_time = 0.0;
    char *url = NULL;
    curl_off_t size_download = 0;

    if (Size > 1) {
        int info_type = Data[1] % 100; /* Limit to a reasonable range */
        curl_easy_getinfo(handle, (CURLINFO)info_type, &response_code);
        curl_easy_getinfo(handle, (CURLINFO)info_type, &total_time);
        curl_easy_getinfo(handle, (CURLINFO)info_type, &url);
        curl_easy_getinfo(handle, (CURLINFO)info_type, &size_download);
    }

    /* Fuzz curl_easy_ssls_export with the corrected callback */
    curl_easy_ssls_export(handle, dummy_ssls_export_cb, (void *)Data);

    /* Cleanup */
    curl_easy_cleanup(handle);

    return 0;
}