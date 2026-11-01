// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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

/* 
 * The provided target functions (Wcurl_easy_setopt_err_*, Wcurl_easy_getinfo_err_*)
 * are compiler warning macros defined in typecheck-gcc.h. They are not runtime functions.
 * To fuzz the underlying logic they guard (curl_easy_setopt and curl_easy_getinfo),
 * we will invoke these actual API functions with diverse inputs derived from the fuzzing data.
 * The macros will trigger compiler warnings if types are mismatched, but the runtime
 * behavior is driven by curl_easy_setopt/getinfo.
 */

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata) {
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

    /* Initialize error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));
    
    /* Set error buffer option */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* Use fuzzing data to construct various options */
    
    /* 1. Fuzz URL (String option) */
    if (Size > 0) {
        /* Ensure null termination for string options */
        char *url = (char *)malloc(Size + 1);
        if (url) {
            memcpy(url, Data, Size);
            url[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, url);
            free(url);
        }
    }

    /* 2. Fuzz Long options (e.g., CURLOPT_VERBOSE, CURLOPT_TIMEOUT) */
    if (Size > 0) {
        long val_long = (long)(Data[0]);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, val_long);
        
        if (Size > 1) {
            long timeout = (long)(Data[1]);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        }
    }

    /* 3. Fuzz slist options (e.g., CURLOPT_HTTPHEADER) */
    if (Size > 10) {
        struct curl_slist *headers = NULL;
        /* Create a simple header list from data */
        char header_buf[256];
        size_t header_len = (Size < 255) ? Size : 255;
        memcpy(header_buf, Data, header_len);
        header_buf[header_len] = '\0';
        
        headers = curl_slist_append(headers, header_buf);
        if (headers) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_slist_free_all(headers);
        }
    }

    /* 4. Fuzz Callback options */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
    
    /* 5. Fuzz POST data */
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size);
    }

    /* 6. Fuzz curl_easy_getinfo with various types */
    char *str_info = NULL;
    long long_info = 0;
    double double_info = 0.0;
    curl_socket_t socket_info = CURL_SOCKET_BAD;
    curl_off_t off_t_info = 0;
    struct curl_slist *slist_info = NULL;

    /* String info */
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_info);
    
    /* Long info */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
    
    /* Double info */
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
    
    /* Socket info */
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
    
    /* Off_t info */
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_t_info);
    
    /* Slist info (SSL engines or cookielist) */
    /* Note: CURLINFO_SSL_ENGINES is deprecated in some versions, but we try it */
    curl_easy_getinfo(curl, CURLINFO_SSL_ENGINES, &slist_info);
    if (slist_info) {
        curl_slist_free_all(slist_info);
    }

    /* Attempt a perform to exercise more state, though it may fail without network */
    /* We ignore the return value as we are fuzzing the setup logic primarily */
    (void)curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}