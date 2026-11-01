// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize a dummy URL from input if possible, otherwise use a safe default
    char url[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url, Data, url_len);
    url[url_len] = '\0';

    // Ensure the URL is valid-ish for fuzzing purposes (avoid null bytes in middle)
    // We just set it to a dummy value if it looks too corrupted to avoid early exit
    if (url_len == 0 || url[0] == '\0') {
        strcpy(url, "http://localhost/");
    }

    // Set basic options to ensure perform doesn't fail immediately due to missing URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);

    // Use input data to set various options to explore different states
    // We interpret the input data to set different options
    
    // Set CURLOPT_POSTFIELDS if data looks like it could be POST data
    if (Size > 10) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size);
    }

    // Set CURLOPT_USERAGENT
    if (Size > 5) {
        char agent[64];
        size_t agent_len = (Size < 63) ? Size : 63;
        memcpy(agent, Data, agent_len);
        agent[agent_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);
    }

    // Set CURLOPT_REFERER
    if (Size > 8) {
        char referer[64];
        size_t referer_len = (Size < 63) ? Size : 63;
        memcpy(referer, Data + 8, referer_len);
        referer[referer_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_REFERER, referer);
    }

    // Set CURLOPT_COOKIE
    if (Size > 12) {
        char cookie[64];
        size_t cookie_len = (Size < 63) ? Size : 63;
        memcpy(cookie, Data + 12, cookie_len);
        cookie[cookie_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);
    }

    // Set CURLOPT_CUSTOMREQUEST
    if (Size > 16) {
        char method[32];
        size_t method_len = (Size < 31) ? Size : 31;
        memcpy(method, Data + 16, method_len);
        method[method_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    }

    // Set CURLOPT_HTTPHEADER using a list
    if (Size > 20) {
        struct curl_slist *headers = NULL;
        char header_line[128];
        size_t header_len = (Size < 127) ? Size : 127;
        memcpy(header_line, Data + 20, header_len);
        header_line[header_len] = '\0';
        headers = curl_slist_append(headers, header_line);
        if (headers) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
    }

    // Perform the transfer
    curl_easy_perform(curl);

    // Call curl_easy_upkeep to test connection pool maintenance
    curl_easy_upkeep(curl);

    // Retrieve some info after the transfer
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    char *content_type = NULL;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    // content_type is managed by curl, do not free

    // Cleanup
    curl_easy_cleanup(curl);

    return 0;
}