// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* 
 * The provided "Target API Functions" (Wcurl_easy_setopt_err_*) are actually
 * compile-time warning macros defined in typecheck-gcc.h, not runtime functions.
 * They cannot be invoked directly in the fuzz driver.
 * 
 * The only actual runtime function listed is curl_multi_get_handles.
 * This driver focuses on fuzzing the state of the multi handle and invoking
 * curl_multi_get_handles to test memory management and handle retrieval logic.
 */

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Configure the easy handle with fuzzing data where applicable */
    /* Set a dummy URL to ensure the handle is valid */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    
    /* Use fuzzing data to set various options safely */
    /* CURLOPT_TIMEOUT_MS: use part of data as timeout */
    if (Size >= 4) {
        uint32_t timeout = *(const uint32_t*)(Data);
        curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, (long)timeout);
    }

    /* CURLOPT_MAX_RECV_SPEED_LARGE: limit speed based on input */
    if (Size >= 8) {
        curl_off_t speed = *(const curl_off_t*)(Data);
        /* Ensure speed is positive to avoid immediate errors */
        if (speed > 0) {
            curl_easy_setopt(easy, CURLOPT_MAX_RECV_SPEED_LARGE, speed);
        }
    }

    /* Add the easy handle to the multi handle */
    if (curl_multi_add_handle(multi, easy) == CURLM_OK) {
        /* 
         * Target API: curl_multi_get_handles
         * Retrieves all active easy handles from the multi handle.
         */
        CURL **handles = curl_multi_get_handles(multi);
        
        if (handles) {
            /* Iterate through the returned array */
            int count = 0;
            while (handles[count] != NULL) {
                count++;
                /* Safety check to prevent infinite loops */
                if (count > 10000) {
                    break;
                }
            }
            
            /* Free the allocated array as per documentation */
            curl_free(handles);
        }
    }

    /* Cleanup easy handle */
    curl_easy_cleanup(easy);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi);

    /* Cleanup global environment */
    curl_global_cleanup();

    return 0;
}