// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_easy_duphandle at easy.c:952:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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

/* Dummy callbacks to satisfy type checking if needed, though not strictly invoked */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    (void)handle; (void)type; (void)data; (void)size; (void)userp;
    return 0;
}

static int dummy_trailer_cb(CURL *handle, struct curl_slist **list, void *userp) {
    (void)handle; (void)list; (void)userp;
    return 0;
}

static int dummy_chunk_end_cb(CURL *handle, void *userp) {
    (void)handle; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a base handle */
    CURL *base_handle = curl_easy_init();
    if (!base_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to configure the base handle diversely */
    /* Set URL if data is long enough */
    if (Size >= 10) {
        char url[256];
        size_t url_len = (Size < 255) ? Size : 255;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(base_handle, CURLOPT_URL, url);
    }

    /* Set various options using input data to explore states */
    if (Size > 0) {
        long opt_val = (long)Data[0];
        curl_easy_setopt(base_handle, CURLOPT_TIMEOUT, opt_val);
    }
    if (Size > 1) {
        long opt_val = (long)Data[1];
        curl_easy_setopt(base_handle, CURLOPT_CONNECTTIMEOUT, opt_val);
    }

    /* Set callbacks to test type checking macros indirectly via setopt */
    curl_easy_setopt(base_handle, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(base_handle, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(base_handle, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(base_handle, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
    curl_easy_setopt(base_handle, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);

    /* Test curl_easy_duphandle with the configured base handle */
    CURL *dup_handle = curl_easy_duphandle(base_handle);
    if (dup_handle) {
        /* Verify duplication worked by checking a simple option if possible, 
           or just ensure it doesn't crash when used */
        
        /* Try to perform a dummy operation or set more options on the duplicate */
        if (Size > 2) {
            long opt_val = (long)Data[2];
            curl_easy_setopt(dup_handle, CURLOPT_VERBOSE, opt_val);
        }

        /* Cleanup duplicate handle */
        curl_easy_cleanup(dup_handle);
    }

    /* Cleanup base handle */
    curl_easy_cleanup(base_handle);

    /* Cleanup global */
    curl_global_cleanup();

    return 0;
}