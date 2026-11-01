// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Ensure input has at least one byte for URL
    if (Size < 1) {
        curl_easy_cleanup(curl);
        return 0;
    }

    // Create a null-terminated string from input for URL
    char *url_buf = malloc(Size + 1);
    if (!url_buf) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memcpy(url_buf, Data, Size);
    url_buf[Size] = '\0';

    // Basic setup to trigger various code paths
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);

    // Fuzz various option types based on input data
    if (Size >= 2) {
        long long_val = (long)(Data[0] << 8 | Data[1]);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, long_val);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, long_val);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, long_val);
    }

    if (Size >= 4) {
        curl_off_t off_val = ((curl_off_t)Data[0] << 24) | ((curl_off_t)Data[1] << 16) |
                             ((curl_off_t)Data[2] << 8) | ((curl_off_t)Data[3]);
        curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void*)off_val);
    }

    // Test string options
    if (Size > 10) {
        char *str_buf = malloc(Size);
        if (str_buf) {
            memcpy(str_buf, Data, Size);
            str_buf[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
            curl_easy_setopt(curl, CURLOPT_REFERER, str_buf);
            curl_easy_setopt(curl, CURLOPT_COOKIE, str_buf);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_buf);
            free(str_buf);
        }
    }

    // Test slist options
    if (Size > 20) {
        struct curl_slist *list = NULL;
        char *header = malloc(Size + 1);
        if (header) {
            memcpy(header, Data, Size);
            header[Size] = '\0';
            list = curl_slist_append(list, header);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
            curl_slist_free_all(list);
            free(header);
        }
    }

    // Perform the request (will likely fail but exercises the code)
    curl_easy_perform(curl);

    // Test curl_easy_getinfo with various types
    long response_code;
    double total_time;
    curl_off_t download_size;
    curl_socket_t socket_fd;
    struct curl_tlssessioninfo *tls_info = NULL;
    struct curl_certinfo *cert_info = NULL;
    char *effective_url;
    struct curl_slist *cookies = NULL;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_size);
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_fd);
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

    // Cleanup
    if (tls_info) {
        // tls_info is managed by libcurl, do not free
    }
    if (cert_info) {
        // cert_info is managed by libcurl, do not free
    }
    if (cookies) {
        curl_slist_free_all(cookies);
    }

    curl_easy_cleanup(curl);
    free(url_buf);

    return 0;
}