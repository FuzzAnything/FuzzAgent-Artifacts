// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Callback for curl_easy_ssls_export matching the expected signature */
static CURLcode ssl_export_callback(void *userptr, void *session,
                                    const char *session_id,
                                    const unsigned char *session_data,
                                    size_t session_len,
                                    const unsigned char *session_data2,
                                    size_t session_len2,
                                    long session_time,
                                    int session_version,
                                    const char *session_cipher,
                                    size_t session_cipher_len) {
    (void)userptr;
    (void)session;
    (void)session_id;
    (void)session_data;
    (void)session_len;
    (void)session_data2;
    (void)session_len2;
    (void)session_time;
    (void)session_version;
    (void)session_cipher;
    (void)session_cipher_len;
    return CURLE_OK;
}

/* Write callback for CURLOPT_WRITEFUNCTION */
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    (void)contents;
    (void)userp;
    return size * nmemb;
}

/* Read callback for CURLOPT_READFUNCTION */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

/* Helper to create a null-terminated string from fuzz input */
static char *create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

/* Helper to create a list of strings for slist options */
static struct curl_slist *create_slist_from_input(const uint8_t *data, size_t size) {
    struct curl_slist *list = NULL;
    size_t offset = 0;
    
    while (offset < size) {
        size_t chunk_size = 0;
        /* Find next null terminator or take remaining bytes */
        while (offset + chunk_size < size && data[offset + chunk_size] != '\0') {
            chunk_size++;
        }
        
        if (chunk_size > 0) {
            char *str = create_null_terminated_string(&data[offset], chunk_size);
            if (str) {
                list = curl_slist_append(list, str);
                free(str);
            }
        }
        
        offset += chunk_size + 1; /* Skip the null terminator if present */
        if (offset > size) break;
    }
    
    return list;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *handle = curl_easy_init();
    if (!handle) return 0;

    /* Prepare buffers for options that require valid memory */
    char *url_str = create_null_terminated_string(Data, Size);
    char *postfields_str = create_null_terminated_string(Data, Size);
    char *useragent_str = create_null_terminated_string(Data, Size);
    char *proxy_str = create_null_terminated_string(Data, Size);
    struct curl_slist *headers = create_slist_from_input(Data, Size);
    struct curl_slist *resolve = create_slist_from_input(Data, Size);

    /* Set various options to explore different code paths */
    if (url_str) {
        curl_easy_setopt(handle, CURLOPT_URL, url_str);
    }
    
    if (postfields_str) {
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postfields_str);
        curl_easy_setopt(handle, CURLOPT_POST, 1L);
    }
    
    if (useragent_str) {
        curl_easy_setopt(handle, CURLOPT_USERAGENT, useragent_str);
    }
    
    if (proxy_str) {
        curl_easy_setopt(handle, CURLOPT_PROXY, proxy_str);
    }
    
    if (headers) {
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    }
    
    if (resolve) {
        curl_easy_setopt(handle, CURLOPT_RESOLVE, resolve);
    }

    /* Set callbacks */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(handle, CURLOPT_READDATA, (void *)Data);

    /* Set some numeric options */
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);

    /* Execute the transfer */
    curl_easy_perform(handle);

    /* Get info after perform */
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
    
    long total_time = 0;
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);
    
    double speed_download = 0.0;
    curl_easy_getinfo(handle, CURLINFO_SPEED_DOWNLOAD, &speed_download);
    
    char *effective_url = NULL;
    curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &effective_url);
    if (effective_url) {
        /* Do nothing, just retrieve */
    }

    /* Try to export SSL sessions */
    curl_easy_ssls_export(handle, ssl_export_callback, (void *)Data);

    /* Perform upkeep */
    curl_easy_upkeep(handle);

    /* Cleanup */
    if (url_str) free(url_str);
    if (postfields_str) free(postfields_str);
    if (useragent_str) free(useragent_str);
    if (proxy_str) free(proxy_str);
    
    if (headers) curl_slist_free_all(headers);
    if (resolve) curl_slist_free_all(resolve);
    
    curl_easy_cleanup(handle);

    return 0;
}