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
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
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

/*
 * Fuzz driver for curl library focusing on type-checking macros.
 * Since the target "functions" (Wcurl_*) are compile-time macros,
 * this driver fuzzes the underlying curl_easy_setopt and curl_easy_getinfo
 * functions with diverse inputs to trigger internal logic and potential
 * type-checking violations or runtime errors.
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

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize a dummy file if needed for file-based options */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Parse input to determine test scenario */
    int scenario = Data[0] % 10;
    long long_val = 0;
    double double_val = 0.0;
    char *str_val = NULL;
    struct curl_slist *slist = NULL;
    long option_idx = 0;
    long info_idx = 0;

    /* Extract values from input data */
    if (Size > 4) {
        long_val = (long)((Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4]);
    }
    if (Size > 8) {
        /* Simple heuristic to create a double from bytes */
        double_val = (double)long_val / 100.0;
    }

    /* Create a string from input if size allows */
    if (Size > 10) {
        str_val = (char *)malloc(Size - 10);
        if (str_val) {
            memcpy(str_val, &Data[10], Size - 10);
            str_val[Size - 10] = '\0';
            /* Ensure null termination and valid length */
            if (Size - 10 > 0) str_val[Size - 11] = '\0';
        }
    }

    /* Create a dummy slist */
    if (Size > 12) {
        slist = curl_slist_append(slist, "Header: Value");
        if (Size > 14) {
            slist = curl_slist_append(slist, "Header2: Value2");
        }
    }

    /* Select an option to set based on scenario */
    switch (scenario) {
        case 0:
            /* Long option */
            option_idx = (long)(Data[0] % 100);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, option_idx);
            break;
        case 1:
            /* String option */
            if (str_val) {
                curl_easy_setopt(curl, CURLOPT_URL, str_val);
            } else {
                curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
            }
            break;
        case 2:
            /* slist option */
            if (slist) {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
            }
            break;
        case 3:
            /* Callback option */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            break;
        case 4:
            /* File option */
            if (dummy_file) {
                /* Re-open for reading if needed, or use path */
                curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
            }
            break;
        case 5:
            /* Post fields */
            if (str_val && Size > 10) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_val);
            }
            break;
        case 6:
            /* Long option with random value */
            curl_easy_setopt(curl, CURLOPT_VERBOSE, long_val);
            break;
        case 7:
            /* Double option (if supported, e.g., CURLOPT_DNS_CACHE_TIMEOUT is long, but we test logic) */
            /* Note: curl_easy_setopt doesn't take double directly, but we test the flow */
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long)double_val);
            break;
        case 8:
            /* Invalid option type mixing (to test error handling) */
            /* Passing a string where a long is expected might trigger warnings or errors */
            if (str_val) {
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(size_t)str_val);
            }
            break;
        case 9:
            /* slist option */
            if (slist) {
                curl_easy_setopt(curl, CURLOPT_QUOTE, slist);
            }
            break;
    }

    /* Perform a dummy perform to trigger state changes if URL is set */
    /* We don't actually connect, but this exercises the internal state machine */
    if (str_val && strlen(str_val) > 0) {
        curl_easy_perform(curl);
    }

    /* Get info based on scenario */
    long info_long;
    double info_double;
    char *info_string;
    struct curl_slist *info_slist;
    struct curl_certinfo *certinfo;
    struct curl_tlssessioninfo *tlssession;
    curl_socket_t socket_val;
    curl_off_t off_val;

    int info_scenario = Data[0] % 10;

    switch (info_scenario) {
        case 0:
            /* Long info */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info_long);
            break;
        case 1:
            /* Double info */
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &info_double);
            break;
        case 2:
            /* String info */
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &info_string);
            break;
        case 3:
            /* slist info */
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &info_slist);
            if (info_slist) {
                curl_slist_free_all(info_slist);
            }
            break;
        case 4:
            /* Certinfo */
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
            break;
        case 5:
            /* TLS Session */
            curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssession);
            break;
        case 6:
            /* Socket */
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_val);
            break;
        case 7:
            /* Off_t */
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_val);
            break;
        case 8:
            /* Invalid info type mixing */
            /* Passing wrong pointer type */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info_double);
            break;
        case 9:
            /* Another slist */
            curl_easy_getinfo(curl, CURLINFO_SSL_ENGINES, &info_slist);
            if (info_slist) {
                curl_slist_free_all(info_slist);
            }
            break;
    }

    /* Cleanup */
    if (str_val) {
        free(str_val);
    }
    if (slist) {
        curl_slist_free_all(slist);
    }
    curl_easy_cleanup(curl);

    return 0;
}