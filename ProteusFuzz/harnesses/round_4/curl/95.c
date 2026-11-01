// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
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

/* Fuzz driver for curl multi-handle functions */

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
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

    /* Create an easy handle to attach to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to ensure the easy handle is valid for some operations */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);

    /* Attach easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        /* If add fails, we can still test remove with invalid handle or wakeup */
    }

    /* Use input data to drive fuzzing logic */
    uint8_t op = Data[0] % 10;

    switch (op) {
        case 0:
        case 1:
            /* Test curl_multi_wakeup */
            curl_multi_wakeup(multi_handle);
            break;

        case 2:
        case 3:
            /* Test curl_multi_remove_handle */
            curl_multi_remove_handle(multi_handle, easy_handle);
            break;

        case 4:
        case 5:
            /* Test curl_multi_setopt with valid options */
            if (Size > 1) {
                int option_val = Data[1];
                /* Try setting a few safe options */
                if (option_val == 0) {
                    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10);
                } else if (option_val == 1) {
                    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 1);
                } else if (option_val == 2) {
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 5);
                }
            }
            break;

        case 6:
        case 7:
            /* Test curl_multi_strerror with various error codes */
            if (Size > 1) {
                CURLMcode code = (CURLMcode)(Data[1] % 20);
                const char *str = curl_multi_strerror(code);
                (void)str; /* Use the result to avoid unused variable warning */
            }
            break;

        case 8:
        case 9:
            /* Test with invalid handles or edge cases */
            if (Size > 1) {
                if (Data[1] % 2 == 0) {
                    /* Try wakeup on a potentially invalid state */
                    curl_multi_wakeup(multi_handle);
                } else {
                    /* Try remove with NULL easy handle */
                    curl_multi_remove_handle(multi_handle, NULL);
                }
            }
            break;
    }

    /* Cleanup easy handle if still attached */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global environment */
    curl_global_cleanup();

    return 0;
}