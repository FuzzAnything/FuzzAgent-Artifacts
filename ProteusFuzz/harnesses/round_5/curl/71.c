// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for curl_easy_duphandle and related type-checking macros.
 * Since the Wcurl_* macros are compile-time checks, we focus on fuzzing
 * the runtime behavior of curl_easy_duphandle with various inputs.
 */

static void dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *easy_handle = NULL;
    CURL *dup_handle = NULL;
    CURLcode res;

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a base handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to configure the handle in diverse ways */
    if (Size > 0) {
        /* Set URL from input if it looks like a string */
        char url_buf[256];
        size_t url_len = (Size < 256) ? Size : 256;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';
        
        /* Try to set URL, but ensure it's a valid string for curl */
        if (url_len > 0 && url_buf[0] != '\0') {
            curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
        }

        /* Set various options based on input bytes to explore different states */
        if (Size > 1) {
            long long_val = (long)Data[0];
            curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, long_val);
            curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, (long)Data[1]);
        }

        if (Size > 2) {
            curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, (long)Data[2]);
        }

        /* Set callback functions to test type-checking macros indirectly */
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_callback);
        curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void *)Data);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void *)Data);

        /* Test CURLOPT_PREREQDATA and related callback options if available */
        if (Size > 3) {
            curl_easy_setopt(easy_handle, CURLOPT_PREREQDATA, (void *)&Data[3]);
        }
    }

    /* Test curl_easy_duphandle with the configured handle */
    dup_handle = curl_easy_duphandle(easy_handle);
    
    if (dup_handle) {
        /* Perform some operations on the duplicated handle to ensure it's valid */
        char error_buffer[CURL_ERROR_SIZE];
        curl_easy_setopt(dup_handle, CURLOPT_ERRORBUFFER, error_buffer);
        
        /* Try to get some info from the duplicated handle */
        long response_code;
        curl_easy_getinfo(dup_handle, CURLINFO_RESPONSE_CODE, &response_code);
        
        /* Cleanup duplicated handle */
        curl_easy_cleanup(dup_handle);
    }

    /* Cleanup original handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup libcurl */
    curl_global_cleanup();

    return 0;
}