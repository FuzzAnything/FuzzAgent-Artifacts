// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static CURLcode ssls_export_callback(void *handle, void *userptr,
                                     const char *id, const unsigned char *data, size_t data_len,
                                     const unsigned char *key, size_t key_len,
                                     long key_type, int key_bits,
                                     const char *cipher, unsigned long cipher_len) {
    (void)handle;
    (void)userptr;
    (void)id;
    (void)data;
    (void)data_len;
    (void)key;
    (void)key_len;
    (void)key_type;
    (void)key_bits;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;
    char *config_str = NULL;
    char *url_str = NULL;

    // Initialize curl globally
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a dummy file for potential file-based options
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Initialize config string from input (ensure null termination)
    config_str = malloc(Size + 1);
    if (!config_str) return 0;
    memcpy(config_str, Data, Size);
    config_str[Size] = '\0';

    // Initialize URL string from input (ensure null termination)
    url_str = malloc(Size + 1);
    if (!url_str) {
        free(config_str);
        return 0;
    }
    memcpy(url_str, Data, Size);
    url_str[Size] = '\0';

    curl = curl_easy_init();
    if (!curl) {
        free(config_str);
        free(url_str);
        return 0;
    }

    // Test curl_global_trace
    res = curl_global_trace(config_str);
    (void)res;

    // Configure the handle with various options to explore states
    curl_easy_setopt(curl, CURLOPT_URL, url_str);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, malloc(CURL_ERROR_SIZE));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Perform the transfer to ensure state is ready for getinfo
    res = curl_easy_perform(curl);
    (void)res;

    // Test curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res;

    // Test curl_easy_getinfo with various types
    long response_code = 0;
    double total_time = 0.0;
    curl_off_t download_size = 0;
    char *content_type = NULL;
    struct curl_slist *cookies = NULL;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlsinfo = NULL;
    curl_socket_t sock = CURL_SOCKET_BAD;

    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &download_size);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res == CURLE_OK && cookies) {
        curl_slist_free_all(cookies);
    }
    res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlsinfo);
    (void)res;
    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock);
    (void)res;

    // Test curl_easy_ssls_export
    res = curl_easy_ssls_export(curl, ssls_export_callback, NULL);
    (void)res;

    // Cleanup
    if (content_type) {
        // content_type is owned by curl, do not free
    }
    curl_easy_cleanup(curl);
    free(config_str);
    free(url_str);
    curl_global_cleanup();

    return 0;
}