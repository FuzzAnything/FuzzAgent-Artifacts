// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_easy_perform at easy.c:817:10 in easy.h
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

/* Fuzz driver for curl type-checking macros and associated API usage.
 * Since the target "functions" are actually compile-time macros,
 * this driver exercises the underlying curl_easy_setopt and curl_easy_getinfo
 * calls with various valid and invalid configurations to ensure robustness.
 */

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

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize variables for getinfo calls */
    double d_val = 0.0;
    long l_val = 0;
    curl_off_t o_val = 0;
    curl_socket_t s_val = CURL_SOCKET_BAD;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlssession = NULL;
    struct curl_slist *slist = NULL;
    char *str_val = NULL;
    char error_buf[CURL_ERROR_SIZE];

    /* Use input data to drive option selection and values */
    uint8_t option_type = Data[0] % 10;
    uint8_t getinfo_type = (Data[0] + 1) % 10;

    /* Set a basic URL to make the handle somewhat valid */
    if (Size > 10) {
        char url[64];
        size_t url_len = (Size < 64) ? Size : 64;
        memcpy(url, Data + 1, url_len);
        url[url_len] = '\0';
        /* Ensure it looks like a URL for safety, though curl will handle invalid ones */
        if (url_len > 4 && strncmp(url, "http", 4) != 0) {
            url[0] = 'h'; url[1] = 't'; url[2] = 't'; url[3] = 'p';
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/fuzz");
    }

    /* Exercise CURLOPT_POSTFIELDS (Wcurl_easy_setopt_err_postfields) */
    if (option_type == 0 && Size > 1) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (void*)(Data + 1));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size - 1);
    }

    /* Exercise string options */
    if (option_type == 1 && Size > 1) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, (char*)(Data + 1));
    }

    /* Exercise callback options (Wcurl_easy_setopt_err_ssl_ctx_cb, etc.) */
    if (option_type == 2) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    }

    /* Exercise long options */
    if (option_type == 3) {
        long timeout = (long)(Data[0] * 10);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }

    /* Perform a dummy perform to trigger state changes for getinfo */
    /* We use CURLOPT_NOBODY to avoid actual network traffic */
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);
    curl_easy_perform(curl);

    /* Exercise curl_easy_getinfo with various types */
    switch (getinfo_type) {
        case 0: /* Double (Wcurl_easy_getinfo_err_double) */
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &d_val);
            curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &d_val);
            break;
        case 1: /* Long */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &l_val);
            curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &l_val);
            break;
        case 2: /* Off_t */
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &o_val);
            curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &o_val);
            break;
        case 3: /* Socket (Wcurl_easy_getinfo_err_curl_socket) */
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &s_val);
            break;
        case 4: /* Certinfo (Wcurl_easy_getinfo_err_curl_certinfo) */
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
            if (certinfo) {
                curl_slist_free_all(certinfo->certinfo);
                free(certinfo);
            }
            break;
        case 5: /* TLSSessionInfo (Wcurl_easy_getinfo_err_curl_tlssessioninfo) */
            curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssession);
            /* Note: curl_tlssessioninfo cleanup is complex, usually handled by curl internally or specific callbacks */
            break;
        case 6: /* Slist (Wcurl_easy_getinfo_err_curl_slist) */
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist);
            if (slist) {
                curl_slist_free_all(slist);
            }
            break;
        case 7: /* String */
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_val);
            /* str_val is internal to curl, do not free */
            break;
        case 8: /* Invalid/Edge case: NULL pointer (should be handled gracefully by curl) */
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, NULL);
            break;
        case 9: /* Invalid option index */
            curl_easy_getinfo(curl, (CURLINFO)(-1), &d_val);
            break;
    }

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}