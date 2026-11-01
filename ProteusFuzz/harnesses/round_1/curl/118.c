// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
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

/* Dummy callback to satisfy type requirements if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize variables for getinfo */
    double d_val = 0.0;
    long l_val = 0;
    curl_off_t off_val = 0;
    curl_socket_t sock_val = CURL_SOCKET_BAD;
    char *str_val = NULL;
    struct curl_slist *slist_val = NULL;
    struct curl_certinfo *certinfo_val = NULL;
    struct curl_tlssessioninfo *tlsinfo_val = NULL;

    /* Parse input to determine which API paths to exercise */
    /* We use the first byte to select the target info type */
    uint8_t selector = Data[0] % 10;

    /* Set a dummy URL to ensure curl is in a valid state for getinfo */
    if (Size > 1) {
        char url_buf[256];
        size_t url_len = (Size > 255) ? 255 : Size - 1;
        memcpy(url_buf, Data + 1, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    /* Perform a dummy perform to populate some info (optional but good for state) */
    /* We don't actually need a network connection for getinfo to work on most fields,
       but performing ensures the handle is active. */
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L);
    (void)curl_easy_perform(curl);

    /* Execute getinfo based on selector to exercise type checking macros */
    CURLcode res;
    switch (selector) {
        case 0:
            /* Test double type (CURLINFO_DOUBLE) */
            /* The macro Wcurl_easy_getinfo_err_double will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &d_val);
            break;
        case 1:
            /* Test long type (CURLINFO_LONG) */
            /* The macro Wcurl_easy_getinfo_err_long will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &l_val);
            break;
        case 2:
            /* Test string type (CURLINFO_STRING) */
            /* The macro Wcurl_easy_getinfo_err_string will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_val);
            break;
        case 3:
            /* Test slist type (CURLINFO_SLIST) */
            /* The macro Wcurl_easy_getinfo_err_curl_slist will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_val);
            if (slist_val) {
                curl_slist_free_all(slist_val);
                slist_val = NULL;
            }
            break;
        case 4:
            /* Test socket type (CURLINFO_SOCKET) */
            /* The macro Wcurl_easy_getinfo_err_curl_socket will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sock_val);
            break;
        case 5:
            /* Test off_t type (CURLINFO_OFF_T) */
            /* The macro Wcurl_easy_getinfo_err_curl_off_t will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
            break;
        case 6:
            /* Test certinfo type (CURLINFO_CERTINFO) */
            /* The macro Wcurl_easy_getinfo_err_curl_certinfo will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo_val);
            /* Note: certinfo_val is not freed here as curl_easy_getinfo docs say 
               it returns a pointer to internal data or allocated list depending on version.
               Usually for CERTINFO it's a struct that needs no free, but if it's a list,
               we should be careful. Standard practice: don't free if it's internal. */
            break;
        case 7:
            /* Test tlssessioninfo type (CURLINFO_TLS_SESSION) */
            /* The macro Wcurl_easy_getinfo_err_curl_tlssessioninfo will warn if type is wrong */
            res = curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlsinfo_val);
            break;
        case 8:
            /* Test invalid info type to trigger error handling */
            res = curl_easy_getinfo(curl, 99999, &d_val);
            break;
        case 9:
            /* Test with NULL pointer (might be valid for some, invalid for others) */
            res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, NULL);
            break;
        default:
            break;
    }

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}