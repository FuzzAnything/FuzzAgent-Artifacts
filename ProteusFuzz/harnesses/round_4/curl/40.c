// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl API functions */

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input buffer is null-terminated for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Test curl_getdate with various date string inputs */
    time_t result = curl_getdate(input_str, NULL);
    (void)result; /* Suppress unused variable warning */

    /* 2. Test curl_version_info with different CURLversion enums */
    curl_version_info_data *version_info = NULL;
    /* Iterate through valid CURLversion enum values up to CURLVERSION_LAST */
    for (int i = 0; i < CURLVERSION_LAST; i++) {
        CURLversion stamp = (CURLversion)i;
        version_info = curl_version_info(stamp);
        if (version_info) {
            /* Access fields to ensure they are valid */
            (void)version_info->version;
            (void)version_info->version_num;
            (void)version_info->host;
            (void)version_info->features;
            (void)version_info->ssl_version;
            (void)version_info->libz_version;
            (void)version_info->ares;
            (void)version_info->libidn;
            (void)version_info->libssh_version;
            (void)version_info->brotli_version;
            (void)version_info->nghttp2_version;
            (void)version_info->quic_version;
            (void)version_info->cainfo;
            (void)version_info->capath;
            (void)version_info->zstd_version;
            (void)version_info->hyper_version;
            (void)version_info->gsasl_version;
            (void)version_info->rtmp_version;
        }
    }

    /* 3. Test curl_version */
    char *version_str = curl_version();
    if (version_str) {
        (void)version_str; /* Suppress unused variable warning */
    }

    /* 4. Test curl_easy_init and curl_easy_getinfo with valid types */
    CURL *curl = curl_easy_init();
    if (curl) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        (void)response_code;

        double total_time = 0.0;
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        (void)total_time;

        curl_off_t size_download = 0;
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
        (void)size_download;

        curl_socket_t socket_fd = CURL_SOCKET_BAD;
        curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_fd);
        (void)socket_fd;

        char *effective_url = NULL;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        (void)effective_url;

        /* Cleanup curl handle */
        curl_easy_cleanup(curl);
    }

    /* 5. Test curl_easy_setopt with valid types to trigger type checks */
    CURL *curl2 = curl_easy_init();
    if (curl2) {
        /* Set a valid URL option */
        curl_easy_setopt(curl2, CURLOPT_URL, "http://example.com");

        /* Set a valid long option */
        long follow_location = 1L;
        curl_easy_setopt(curl2, CURLOPT_FOLLOWLOCATION, follow_location);

        /* Set a valid callback option (using NULL to avoid implementation details) */
        curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, NULL);

        /* Cleanup curl handle */
        curl_easy_cleanup(curl2);
    }

    /* Cleanup allocated memory */
    free(input_str);

    return 0;
}