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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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

/* Dummy write callback to satisfy CURLOPT_WRITEFUNCTION */
static size_t dummy_write_callback(void *ptr, size_t size, size_t nmemb, void *user_data) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)user_data;
    return size * nmemb;
}

/* Dummy read callback to satisfy CURLOPT_READFUNCTION */
static size_t dummy_read_callback(void *ptr, size_t size, size_t nmemb, void *user_data) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)user_data;
    return 0;
}

/* Dummy debug callback */
static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *user_data) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)user_data;
    return 0;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize variables for getinfo calls */
    double d_val;
    long l_val;
    curl_off_t off_val;
    curl_socket_t sock_val;
    struct curl_slist *slist_val = NULL;
    struct curl_certinfo *certinfo_val = NULL;
    struct curl_tlssessioninfo *tlsinfo_val = NULL;
    char *str_val = NULL;

    /* Use input data to drive option selection and values */
    size_t idx = 0;
    unsigned char option_type = Data[idx] % 10;
    idx++;

    /* Set a dummy URL to ensure curl is configured minimally */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);

    /* Fuzz various option types based on input */
    if (idx < Size) {
        long long_opt = (long)Data[idx];
        idx++;
        if (option_type == 1) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, long_opt);
        } else if (option_type == 2) {
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_opt);
        } else if (option_type == 3) {
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, long_opt);
        }
    }

    /* Fuzz string options */
    if (idx < Size) {
        size_t str_len = (Size - idx) > 100 ? 100 : (Size - idx);
        char *str_buf = malloc(str_len + 1);
        if (str_buf) {
            memcpy(str_buf, Data + idx, str_len);
            str_buf[str_len] = '\0';
            idx += str_len;

            if (option_type == 4) {
                curl_easy_setopt(curl, CURLOPT_USERAGENT, str_buf);
            } else if (option_type == 5) {
                curl_easy_setopt(curl, CURLOPT_REFERER, str_buf);
            } else if (option_type == 6) {
                curl_easy_setopt(curl, CURLOPT_COOKIE, str_buf);
            }
            free(str_buf);
        }
    }

    /* Fuzz slist options */
    if (idx < Size) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Host: example.com");
        if (headers) {
            if (option_type == 7) {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            }
            curl_slist_free_all(headers);
        }
    }

    /* Perform a dummy perform to transition state */
    curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo with various types */
    /* Wcurl_easy_getinfo_err_double */
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &d_val);
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME, &d_val);

    /* Wcurl_easy_getinfo_err_long */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &l_val);
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &l_val);
    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &l_val);
    curl_easy_getinfo(curl, CURLINFO_SSL_VERIFYRESULT, &l_val);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_METHOD, &str_val); /* String but safe */
    curl_easy_getinfo(curl, CURLINFO_PRIVATE, &str_val);

    /* Wcurl_easy_getinfo_err_curl_off_t */
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_val);
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &off_val);
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &off_val);
    curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &off_val);
    curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &off_val);

    /* Wcurl_easy_getinfo_err_curl_socket */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_val);

    /* Wcurl_easy_getinfo_err_curl_slist */
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_val);
    if (slist_val) {
        curl_slist_free_all(slist_val);
    }

    /* Wcurl_easy_getinfo_err_curl_certinfo */
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo_val);
    if (certinfo_val) {
        /* curl_certinfo cleanup is complex, usually handled by curl, 
           but for fuzzing we just note it was retrieved */
    }

    /* Wcurl_easy_getinfo_err_curl_tlssessioninfo */
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlsinfo_val);
    curl_easy_getinfo(curl, CURLINFO_TLS_SSL_PTR, &tlsinfo_val);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}