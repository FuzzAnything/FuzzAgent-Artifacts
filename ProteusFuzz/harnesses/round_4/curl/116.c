// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize variables for getinfo
    char *string_info = NULL;
    long long_info = 0;
    double double_info = 0.0;
    struct curl_slist *slist_info = NULL;
    struct curl_certinfo *certinfo = NULL;
    struct curl_tlssessioninfo *tlssessioninfo = NULL;
    curl_socket_t socket_info = CURL_SOCKET_BAD;
    curl_off_t off_t_info = 0;

    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Set up basic options using fuzz data
    if (Size > 0) {
        // Use data as URL (ensure null termination)
        char *url = malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
            free(url);
        }
    }

    // Set write and read callbacks
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);

    // Set various options based on fuzz data to explore states
    if (Size > 1) {
        long long_val = (long)(Data[0] << 8 | Data[1]);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, long_val);
    }

    // Set string options
    if (Size > 2) {
        char *str_val = malloc(Size - 1);
        if (str_val) {
            memcpy(str_val, Data + 2, Size - 2);
            str_val[Size - 2] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, str_val);
            curl_easy_setopt(curl, CURLOPT_REFERER, str_val);
            free(str_val);
        }
    }

    // Set callback data options
    void *cb_data = (void *)(uintptr_t)(Data[0]);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, cb_data);
    curl_easy_setopt(curl, CURLOPT_READDATA, cb_data);

    // Perform the request (will likely fail due to invalid URL, but exercises code paths)
    curl_easy_perform(curl);

    // Test curl_easy_getinfo with various types
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_info);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlssessioninfo);
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_info);

    // Cleanup
    if (slist_info) {
        curl_slist_free_all(slist_info);
    }
    if (certinfo) {
        // curl_certinfo is managed by curl, no direct free needed usually, 
        // but we ensure we don't leak if it was allocated
        // Note: In real usage, curl_easy_cleanup handles this, but for fuzzing we just ignore
    }
    if (tlssessioninfo) {
        // Similar to certinfo
    }

    curl_easy_cleanup(curl);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}