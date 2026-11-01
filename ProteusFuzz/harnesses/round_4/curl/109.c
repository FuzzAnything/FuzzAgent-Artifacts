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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include curl headers. 
   Note: curl/easy.h depends on curl/curl.h for type definitions.
   We include curl.h first to ensure CURL, CURLcode, etc. are defined.
*/
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl_easy_getinfo, curl_easy_perform, and curl_easy_upkeep */

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Create a dummy URL from input data to avoid network dependency issues */
    /* We use a fixed local URL to ensure curl_easy_perform completes quickly */
    const char *url = "http://127.0.0.1:1"; /* Likely to fail fast or timeout, which is fine for fuzzing */
    
    /* Set basic options */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L); /* Short timeout */
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    /* Use input data to diversify options if possible, but keep it safe */
    if (Size > 0) {
        /* Use first byte to select a random option to set, if safe */
        uint8_t option_selector = Data[0];
        
        /* Set a dummy string option if selector suggests it */
        if (option_selector < 10) {
            char dummy_str[10];
            size_t len = Size < 9 ? Size : 9;
            memcpy(dummy_str, Data, len);
            dummy_str[len] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, dummy_str);
        }
    }

    /* Perform the transfer */
    CURLcode perform_res = curl_easy_perform(curl);
    
    /* Call curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Fuzz curl_easy_getinfo with various CURLINFO options */
    /* We iterate through a range of possible CURLINFO values */
    for (int i = 0; i < 50; i++) {
        CURLINFO info_type = (CURLINFO)i;
        void *info_ptr = NULL;
        char *str_ptr = NULL;
        long long_ptr = 0;
        double double_ptr = 0.0;
        curl_off_t off_t_ptr = 0;
        curl_socket_t socket_ptr = 0;
        struct curl_slist *slist_ptr = NULL;
        struct curl_tlssessioninfo *tls_ptr = NULL;
        struct curl_certinfo *cert_ptr = NULL;

        /* Select pointer based on info type range to avoid type mismatch */
        if (info_type >= CURLINFO_STRING && info_type < CURLINFO_LONG) {
            /* String info */
            str_ptr = (char *)malloc(256);
            if (str_ptr) {
                memset(str_ptr, 0, 256);
                curl_easy_getinfo(curl, info_type, &str_ptr);
                free(str_ptr);
            }
        } else if (info_type >= CURLINFO_LONG && info_type < CURLINFO_DOUBLE) {
            /* Long info */
            curl_easy_getinfo(curl, info_type, &long_ptr);
        } else if (info_type >= CURLINFO_DOUBLE && info_type < CURLINFO_SLIST) {
            /* Double info */
            curl_easy_getinfo(curl, info_type, &double_ptr);
        } else if (info_type >= CURLINFO_SLIST && info_type < CURLINFO_OFF_T) {
            /* SLIST info */
            curl_easy_getinfo(curl, info_type, &slist_ptr);
            if (slist_ptr) {
                curl_slist_free_all(slist_ptr);
            }
        } else if (info_type >= CURLINFO_OFF_T) {
            /* Off_t info */
            curl_easy_getinfo(curl, info_type, &off_t_ptr);
        } else if (info_type == CURLINFO_TLS_SSL_PTR || info_type == CURLINFO_TLS_SESSION) {
            /* TLS session info */
            struct curl_tlssessioninfo tls_info;
            memset(&tls_info, 0, sizeof(tls_info));
            curl_easy_getinfo(curl, info_type, &tls_info);
        } else if (info_type == CURLINFO_CERTINFO) {
            /* Cert info */
            struct curl_certinfo cert_info;
            memset(&cert_info, 0, sizeof(cert_info));
            curl_easy_getinfo(curl, info_type, &cert_info);
        } else if (info_type == CURLINFO_PRIVATE) {
            /* Private info (string) */
            str_ptr = (char *)malloc(256);
            if (str_ptr) {
                memset(str_ptr, 0, 256);
                curl_easy_getinfo(curl, info_type, &str_ptr);
                free(str_ptr);
            }
        } else {
            /* Default to long for unknown or edge cases to avoid crashes */
            curl_easy_getinfo(curl, info_type, &long_ptr);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}