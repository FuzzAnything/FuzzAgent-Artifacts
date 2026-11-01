// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Target APIs:
 * - curl_multi_strerror
 * - curl_multi_wakeup
 * - curl_multi_assign
 * - curl_multi_remove_handle
 * - curl_multi_setopt
 */

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = NULL;
    CURL *easy = NULL;

    /* Initialize multi handle */
    multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    /* Initialize easy handle */
    easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    /*
     * 1. Test curl_multi_strerror
     * Use input data to generate various CURLMcode values.
     */
    if (Size >= 2) {
        CURLMcode code = (CURLMcode)((Data[0] << 8) | Data[1]);
        const char *msg = curl_multi_strerror(code);
        if (msg) {
            /* Consume the string to avoid unused warning */
            size_t len = strlen(msg);
            if (len > 0) {
                volatile char c = msg[0];
                (void)c;
            }
        }
    }

    /*
     * 2. Test curl_multi_setopt
     * Use input data to select an option and value.
     */
    if (Size >= 3) {
        CURLMoption option = (CURLMoption)Data[2];
        long val_long = (long)((Data[0] << 16) | (Data[1] << 8) | Data[2]);
        curl_multi_setopt(multi, option, val_long);
    }

    /*
     * 3. Test curl_multi_assign
     * Use input data as socket and pointer.
     */
    if (Size >= 4) {
        curl_socket_t sockfd = (curl_socket_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
        void *sockp = (void *)(uintptr_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
        curl_multi_assign(multi, sockfd, sockp);
    }

    /*
     * 4. Test curl_multi_remove_handle
     * Add easy handle to multi first, then remove.
     */
    curl_multi_add_handle(multi, easy);
    curl_multi_remove_handle(multi, easy);

    /*
     * 5. Test curl_multi_wakeup
     * This function is meant to wake up a sleeping poll; calling it
     * without a poll is safe but may return an error.
     */
    curl_multi_wakeup(multi);

    /* Cleanup */
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}