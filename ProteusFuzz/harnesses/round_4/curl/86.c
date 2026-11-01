// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <curl/typecheck-gcc.h>

/* Dummy callback for CURLOPT_READFUNCTION */
static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userptr) {
    (void)buffer;
    (void)size;
    (void)nitems;
    (void)userptr;
    return 0;
}

/* Dummy callback for CURLOPT_WRITEFUNCTION */
static size_t dummy_write_callback(char *buffer, size_t size, size_t nitems, void *userptr) {
    (void)buffer;
    (void)userptr;
    return size * nitems;
}

/* 
 * Dummy callback for curl_ssls_export_cb.
 * Based on curl.h definition:
 * typedef CURLcode (*curl_ssls_export_cb)(void *userptr, void *ssl,
 *     const char *id, const unsigned char *id_data, size_t id_len,
 *     const unsigned char *data, size_t data_len, long age, int is_server,
 *     const char *cipher, unsigned long cipher_id);
 */
static CURLcode dummy_ssls_export_cb(void *userptr, void *ssl,
    const char *id, const unsigned char *id_data, size_t id_len,
    const unsigned char *data, size_t data_len, long age, int is_server,
    const char *cipher, unsigned long cipher_id) {
    (void)userptr;
    (void)ssl;
    (void)id;
    (void)id_data;
    (void)id_len;
    (void)data;
    (void)data_len;
    (void)age;
    (void)is_server;
    (void)cipher;
    (void)cipher_id;
    return CURLE_OK;
}

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    long long_val = 0;
    double double_val = 0.0;
    char *string_val = NULL;
    curl_off_t off_val = 0;
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    struct curl_slist *slist = NULL;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;

    /* Initialize curl global */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create easy handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Ensure dummy file exists */
    setup_dummy_file();

    /* Set basic options to enable transfer simulation */
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);

    /* Fuzz option setting based on input data if possible */
    if (Size > 0) {
        long opt_long = (long)Data[0];
        if (opt_long > 0 && opt_long < CURLOPTTYPE_OBJECTPOINT) {
            /* Safe to set some long options */
            curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_long % 2);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, opt_long % 2);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, opt_long % 3);
        }

        if (Size > 1) {
            /* Try to set a URL from input if it looks safe (basic check) */
            char url[256];
            size_t url_len = (Size < 255) ? Size : 255;
            memcpy(url, Data, url_len);
            url[url_len] = '\0';
            /* Only set if it starts with http to avoid weird protocol issues */
            if (strncmp(url, "http", 4) == 0) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
            }
        }
    }

    /* Perform the transfer */
    res = curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo with various info types */
    /* String info */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_val);
    /* Long info */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &long_val);
    /* Double info */
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_val);
    /* Off_t info */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &off_val);
    /* Socket info */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_val);
    /* Slist info */
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist);
    if (slist) curl_slist_free_all(slist);
    /* Cert info */
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
    /* TLS session info */
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);

    /* Fuzz curl_easy_upkeep */
    res = curl_easy_upkeep(curl);

    /* Fuzz curl_easy_ssls_export */
    /* This might fail if SSL is not built in or no sessions, which is fine for fuzzing */
    res = curl_easy_ssls_export(curl, dummy_ssls_export_cb, (void *)Data);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}