// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_easy_upkeep at easy.c:1320:10 in easy.h
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
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include curl headers in the correct order to ensure macros are defined */
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

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

static int dummy_conv_callback(char *ptr, int length, void *userdata) {
    (void)ptr;
    (void)length;
    (void)userdata;
    return 0;
}

static int dummy_interleave_callback(CURL *curl, int what, void *userdata) {
    (void)curl;
    (void)what;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize a dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Set up basic options to ensure a valid session state */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEFUNCTION, dummy_interleave_callback);
    curl_easy_setopt(curl, CURLOPT_INTERLEAVEDATA, (void*)Data);

    /* Perform the transfer to ensure getinfo works correctly */
    (void)curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo with various CURLINFO options */
    long long_val = 0;
    double double_val = 0.0;
    char *string_val = NULL;
    struct curl_slist *slist_val = NULL;
    struct curl_tlssessioninfo tls_session_info;
    struct curl_certinfo cert_info;
    curl_socket_t socket_val = 0;
    curl_off_t off_t_val = 0;

    /* Use fuzzing data to select CURLINFO options */
    /* Limit range to valid known values to avoid undefined behavior with invalid enums */
    CURLINFO info_type = (CURLINFO)(Data[0] % 100);

    switch (info_type) {
        case CURLINFO_RESPONSE_CODE:
        case CURLINFO_HTTP_CONNECTCODE:
        case CURLINFO_HTTPAUTH_AVAIL:
        case CURLINFO_PROXYAUTH_AVAIL:
        case CURLINFO_OS_ERRNO:
        case CURLINFO_NUM_CONNECTS:
        case CURLINFO_FTP_ENTRY_PATH:
        case CURLINFO_PRIVATE:
        case CURLINFO_EFFECTIVE_METHOD:
        case CURLINFO_EFFECTIVE_URL:
        case CURLINFO_FILETIME:
        case CURLINFO_SIZE_UPLOAD:
        case CURLINFO_SIZE_DOWNLOAD:
        case CURLINFO_SPEED_DOWNLOAD:
        case CURLINFO_SPEED_UPLOAD:
        case CURLINFO_HEADER_SIZE:
        case CURLINFO_REQUEST_SIZE:
        case CURLINFO_SSL_VERIFYRESULT:
        case CURLINFO_REDIRECT_COUNT:
            curl_easy_getinfo(curl, info_type, &long_val);
            break;
        case CURLINFO_TOTAL_TIME:
        case CURLINFO_NAMELOOKUP_TIME:
        case CURLINFO_CONNECT_TIME:
        case CURLINFO_PRETRANSFER_TIME:
        case CURLINFO_STARTTRANSFER_TIME:
        case CURLINFO_REDIRECT_TIME:
        case CURLINFO_SIZE_UPLOAD_T:
        case CURLINFO_SIZE_DOWNLOAD_T:
        case CURLINFO_SPEED_DOWNLOAD_T:
        case CURLINFO_SPEED_UPLOAD_T:
            curl_easy_getinfo(curl, info_type, &double_val);
            break;
        case CURLINFO_SSL_ENGINES:
        case CURLINFO_COOKIELIST:
            curl_easy_getinfo(curl, info_type, &slist_val);
            if (slist_val) {
                curl_slist_free_all(slist_val);
            }
            break;
        case CURLINFO_TLS_SSL_PTR:
        case CURLINFO_TLS_SESSION:
            curl_easy_getinfo(curl, info_type, &tls_session_info);
            break;
        case CURLINFO_CERTINFO:
            curl_easy_getinfo(curl, info_type, &cert_info);
            break;
        case CURLINFO_LASTSOCKET:
            curl_easy_getinfo(curl, info_type, &socket_val);
            break;
        default:
            /* Try to get info with invalid types to test error handling */
            curl_easy_getinfo(curl, info_type, &long_val);
            curl_easy_getinfo(curl, info_type, &double_val);
            curl_easy_getinfo(curl, info_type, &string_val);
            break;
    }

    /* Fuzz curl_easy_upkeep */
    (void)curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}