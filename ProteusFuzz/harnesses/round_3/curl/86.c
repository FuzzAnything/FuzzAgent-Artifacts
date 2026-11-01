// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

#define DUMMY_FILE "./dummy_file"

static void init_dummy_file(void) {
    FILE *f = fopen(DUMMY_FILE, "w");
    if (f) {
        const char *content = "Dummy content for fuzzing curl file operations.";
        fwrite(content, 1, strlen(content), f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILE);
}

static CURLcode dummy_export_cb(void *userptr, void *ssl, 
                                const char *protocol,
                                const unsigned char *cipher,
                                unsigned long cipher_len,
                                const unsigned char *peer_cert,
                                unsigned long peer_cert_len,
                                long peer_cert_depth,
                                int peer_cert_verified,
                                const char *peer_cert_issuer,
                                unsigned long peer_cert_issuer_len) {
    (void)userptr; (void)ssl; (void)protocol; (void)cipher;
    (void)cipher_len; (void)peer_cert; (void)peer_cert_len;
    (void)peer_cert_depth; (void)peer_cert_verified;
    (void)peer_cert_issuer; (void)peer_cert_issuer_len;
    return CURLE_OK;
}

static CURLcode safe_perform(CURL *handle) {
    if (!handle) return CURLE_BAD_FUNCTION_ARGUMENT;
    curl_easy_setopt(handle, CURLOPT_URL, "http://127.0.0.1:1/");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    return curl_easy_perform(handle);
}

static CURLcode safe_ssl_export(CURL *handle) {
    if (!handle) return CURLE_BAD_FUNCTION_ARGUMENT;
    return curl_easy_ssls_export(handle, dummy_export_cb, NULL);
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    init_dummy_file();

    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        cleanup_dummy_file();
        return 0;
    }

    curl_easy_setopt(handle, CURLOPT_URL, "http://127.0.0.1:1/");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);

    if (Size > 0) {
        if (Data[0] % 2 == 0) {
            curl_easy_setopt(handle, CURLOPT_CAINFO, DUMMY_FILE);
        } else {
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, DUMMY_FILE);
        }
    }

    safe_perform(handle);

    long long_val = 0;
    double double_val = 0.0;
    curl_off_t off_val = 0;
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    struct curl_slist *slist_val = NULL;
    struct curl_certinfo *certinfo_val = NULL;
    struct curl_tlssessioninfo *tlssession_val = NULL;

    size_t i = 0;
    while (i < Size) {
        CURLINFO info_type = (CURLINFO)(Data[i] % 100);
        CURLcode res = CURLE_OK;

        res = curl_easy_getinfo(handle, info_type, &long_val);
        (void)res;

        res = curl_easy_getinfo(handle, info_type, &double_val);
        (void)res;

        res = curl_easy_getinfo(handle, info_type, &off_val);
        (void)res;

        res = curl_easy_getinfo(handle, info_type, &socket_val);
        (void)res;

        res = curl_easy_getinfo(handle, info_type, &slist_val);
        if (slist_val) {
            curl_slist_free_all(slist_val);
            slist_val = NULL;
        }

        res = curl_easy_getinfo(handle, info_type, &certinfo_val);

        res = curl_easy_getinfo(handle, info_type, &tlssession_val);

        i++;
    }

    curl_easy_upkeep(handle);
    safe_ssl_export(handle);

    curl_easy_cleanup(handle);
    curl_global_cleanup();
    cleanup_dummy_file();

    return 0;
}