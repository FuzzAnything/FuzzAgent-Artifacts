// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Prepare a dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Parse input data to drive fuzzing logic */
    uint32_t seed = *(const uint32_t *)Data;
    int action = (seed % 100);

    /* Set some basic options on the easy handle to make it valid */
    if (Size > 10) {
        char url_buf[256];
        size_t url_len = (Size > 256) ? 256 : Size;
        memcpy(url_buf, Data + 4, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    }

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Fuzz curl_multi_setopt */
    if (action < 20) {
        /* Try setting various options */
        CURLMcode res;
        /* CURLMOPT_MAXCONNECTS */
        long max_conn = (long)(seed % 100);
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conn);
        (void)res;
        
        /* CURLMOPT_PIPELINING */
        long pipelining = (long)(seed % 3);
        res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
        (void)res;

        /* CURLMOPT_MAX_HOST_CONNECTIONS */
        long max_host = (long)(seed % 20);
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host);
        (void)res;
    }

    /* Fuzz curl_multi_remove_handle */
    if (action < 40) {
        CURLMcode res = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)res;
        /* Try removing again or with invalid handle if logic dictates */
        if (seed % 2 == 0) {
            res = curl_multi_remove_handle(multi_handle, easy_handle);
            (void)res;
        }
    }

    /* Fuzz curl_multi_wakeup */
    if (action < 60) {
        CURLMcode res = curl_multi_wakeup(multi_handle);
        (void)res;
    }

    /* Fuzz curl_multi_strerror with various error codes */
    if (action < 80) {
        CURLMcode error_code = (CURLMcode)(seed % 100);
        const char *str = curl_multi_strerror(error_code);
        if (str) {
            /* Use the string to avoid unused variable warning */
            size_t len = strlen(str);
            (void)len;
        }
    }

    /* Cleanup easy handle if it was added back or not removed */
    /* Note: We need to be careful not to double free or remove non-added handles */
    /* Since we removed it in the <40 branch, we might need to re-add if we want to test removal again,
       but for simplicity, we just clean up what we have. */
    /* If we removed it above, curl_easy_cleanup is safe. 
       If we didn't remove it, we should remove it before cleanup or just cleanup the multi handle which handles it.
       However, curl_multi_cleanup handles removing all handles. */

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup global environment */
    curl_global_cleanup();

    /* Remove dummy file */
    if (dummy_file) {
        /* dummy_file was already closed, just remove the file */
        remove("./dummy_file");
    }

    return 0;
}