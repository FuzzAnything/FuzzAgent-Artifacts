// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/* Define CURL_EXTERN if not already defined by curl/curl.h to prevent compilation errors */
#ifndef CURL_EXTERN
#define CURL_EXTERN
#endif

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

/* Correct signature for curl_ssls_export_cb based on curl.h */
static CURLcode dummy_ssls_export_cb(void *userptr,
                                     void *protocol,
                                     const char *protocol_str,
                                     const unsigned char *session_id,
                                     unsigned long session_id_len,
                                     const unsigned char *session,
                                     unsigned long session_len,
                                     long age,
                                     int is_encrypted,
                                     const char *cipher_name,
                                     unsigned long cipher_len) {
    (void)userptr;
    (void)protocol;
    (void)protocol_str;
    (void)session_id;
    (void)session_id_len;
    (void)session;
    (void)session_len;
    (void)age;
    (void)is_encrypted;
    (void)cipher_name;
    (void)cipher_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;

    /* 1. Prepare environment */
    /* Try to initialize global trace with fuzzed data as config string */
    char *config_str = (char *)malloc(Size + 1);
    if (!config_str) return 0;
    memcpy(config_str, Data, Size);
    config_str[Size] = '\0';

    res = curl_global_trace(config_str);
    (void)res;
    free(config_str);

    /* Initialize easy handle */
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Set some basic options to make the handle valid for other functions */
    res = curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1");
    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L);
    res = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 1L);
    res = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);

    /* 2. Fuzz curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res;

    /* 3. Fuzz curl_easy_ssls_export */
    res = curl_easy_ssls_export(curl, dummy_ssls_export_cb, (void *)Data);
    (void)res;

    /* 4. Fuzz curl_easy_perform */
    res = curl_easy_perform(curl);
    (void)res;

    /* 5. Fuzz curl_easy_getinfo */
    char *str_ptr = NULL;
    long long_ptr = 0;
    double double_ptr = 0.0;
    struct curl_slist *slist_ptr = NULL;
    struct curl_tlssessioninfo tls_info = {0};
    struct curl_certinfo cert_info = {0};
    curl_socket_t socket_ptr = CURL_SOCKET_BAD;
    curl_off_t off_t_ptr = 0;

    res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_ptr);
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_ptr);
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &double_ptr);
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_ptr);
    if (slist_ptr) {
        curl_slist_free_all(slist_ptr);
    }
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
    /* Correctly handle cert_info.certinfo which is an array of curl_slist pointers */
    if (cert_info.certinfo) {
        for (int i = 0; i < cert_info.num_of_certs; i++) {
            if (cert_info.certinfo[i]) {
                curl_slist_free_all(cert_info.certinfo[i]);
            }
        }
        free(cert_info.certinfo);
    }
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_ptr);
    (void)res;

    res = curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_t_ptr);
    (void)res;

    /* 6. Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}