// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Prepare dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Set up basic options to allow perform to complete without network
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

    // Use fuzzing data to set various options
    if (Size > 0) {
        long opt_val = (long)(Data[0] & 0xFF);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, opt_val);
    }
    if (Size > 1) {
        long opt_val2 = (long)(Data[1] & 0xFF);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, opt_val2);
    }
    if (Size > 2) {
        long opt_val3 = (long)(Data[2] & 0xFF);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, opt_val3);
    }

    // Perform the transfer
    curl_easy_perform(curl);

    // Fuzz curl_easy_getinfo with various info types
    char *str_buf = NULL;
    long long_buf = 0;
    double double_buf = 0.0;
    struct curl_slist *slist_buf = NULL;
    struct curl_tlssessioninfo tls_buf = {0};
    struct curl_certinfo cert_buf = {0};
    curl_socket_t socket_buf = -1;
    curl_off_t off_buf = 0;

    // Iterate through possible CURLINFO types to explore states
    for (int i = 0; i < 100; i++) {
        CURLINFO info_type = (CURLINFO)i;
        CURLcode res;

        switch (info_type) {
            case CURLINFO_EFFECTIVE_URL:
            case CURLINFO_CONTENT_TYPE:
                res = curl_easy_getinfo(curl, info_type, &str_buf);
                break;
            case CURLINFO_SSL_ENGINES:
            case CURLINFO_COOKIELIST:
                res = curl_easy_getinfo(curl, info_type, &slist_buf);
                break;
            case CURLINFO_RESPONSE_CODE:
            case CURLINFO_HTTP_CONNECTCODE:
            case CURLINFO_NUM_CONNECTS:
            case CURLINFO_REDIRECT_COUNT:
            case CURLINFO_SIZE_DOWNLOAD:
            case CURLINFO_SIZE_UPLOAD:
            case CURLINFO_SPEED_DOWNLOAD:
            case CURLINFO_SPEED_UPLOAD:
            case CURLINFO_HEADER_SIZE:
            case CURLINFO_REQUEST_SIZE:
            case CURLINFO_SSL_VERIFYRESULT:
            case CURLINFO_HTTP_VERSION:
            case CURLINFO_PROXY_SSL_VERIFYRESULT:
            case CURLINFO_SCHEME:
            case CURLINFO_LOCAL_IP:
            case CURLINFO_LOCAL_PORT:
            case CURLINFO_PRIMARY_IP:
            case CURLINFO_PRIMARY_PORT:
            case CURLINFO_APPCONNECT_TIME:
            case CURLINFO_PRETRANSFER_TIME:
            case CURLINFO_STARTTRANSFER_TIME:
            case CURLINFO_TOTAL_TIME:
            case CURLINFO_NAMELOOKUP_TIME:
            case CURLINFO_CONNECT_TIME:
            case CURLINFO_REDIRECT_TIME:
                res = curl_easy_getinfo(curl, info_type, &long_buf);
                break;
            case CURLINFO_TLS_SSL_PTR:
            case CURLINFO_TLS_SESSION:
                res = curl_easy_getinfo(curl, info_type, &tls_buf);
                break;
            case CURLINFO_CERTINFO:
                res = curl_easy_getinfo(curl, info_type, &cert_buf);
                break;
            case CURLINFO_ACTIVESOCKET:
                res = curl_easy_getinfo(curl, info_type, &socket_buf);
                break;
            case CURLINFO_SIZE_DOWNLOAD_T:
            case CURLINFO_SIZE_UPLOAD_T:
            case CURLINFO_SPEED_DOWNLOAD_T:
            case CURLINFO_SPEED_UPLOAD_T:
            case CURLINFO_FILETIME_T:
            case CURLINFO_CONTENT_LENGTH_DOWNLOAD_T:
            case CURLINFO_CONTENT_LENGTH_UPLOAD_T:
                res = curl_easy_getinfo(curl, info_type, &off_buf);
                break;
            default:
                // Try with long as default for unknown types
                res = curl_easy_getinfo(curl, info_type, &long_buf);
                break;
        }

        // Cleanup slist if allocated
        if (slist_buf) {
            curl_slist_free_all(slist_buf);
            slist_buf = NULL;
        }
    }

    // Cleanup
    curl_easy_cleanup(curl);
    return 0;
}