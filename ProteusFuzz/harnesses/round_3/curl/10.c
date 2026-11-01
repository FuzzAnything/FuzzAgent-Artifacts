// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Fuzz driver for curl multi-handle functions.
 * Target functions:
 * - curl_multi_assign
 * - curl_multi_strerror
 * - curl_multi_remove_handle
 * - curl_multi_get_offt
 * - curl_multi_wakeup
 * - curl_multi_timeout
 */

static void init_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    init_dummy_file();

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    /* Add easy handle to multi handle to enable some operations */
    curl_multi_add_handle(multi, easy);

    /* Use input data to drive fuzzing logic with bounds checking */
    uint8_t op = Data[0] % 10;
    curl_socket_t sockfd = 0;
    curl_off_t offt_val = 0;
    long timeout_ms = 0;
    CURLMcode ret;

    if (Size >= 2) {
        sockfd = (curl_socket_t)(Data[0] + Data[1]);
    }

    switch (op) {
        case 0:
            /* curl_multi_assign */
            {
                void *custom_ptr = NULL;
                if (Size >= 3) {
                    custom_ptr = (void *)(intptr_t)(Data[2] + (Size >= 4 ? Data[3] : 0));
                }
                ret = curl_multi_assign(multi, sockfd, custom_ptr);
                (void)ret;
            }
            break;

        case 1:
            /* curl_multi_strerror */
            {
                CURLMcode err_code = (CURLMcode)(Size >= 3 ? Data[2] % 30 : 0);
                const char *str = curl_multi_strerror(err_code);
                if (str) {
                    /* Consume string to avoid unused warning */
                    (void)str[0];
                }
            }
            break;

        case 2:
            /* curl_multi_remove_handle */
            ret = curl_multi_remove_handle(multi, easy);
            (void)ret;
            break;

        case 3:
            /* curl_multi_get_offt - try different info types */
            {
                CURLMinfo_offt info_type = (CURLMinfo_offt)(Size >= 3 ? Data[2] % 10 : 0);
                ret = curl_multi_get_offt(multi, info_type, &offt_val);
                (void)ret;
                (void)offt_val;
            }
            break;

        case 4:
            /* curl_multi_wakeup */
            ret = curl_multi_wakeup(multi);
            (void)ret;
            break;

        case 5:
            /* curl_multi_timeout */
            ret = curl_multi_timeout(multi, &timeout_ms);
            (void)ret;
            (void)timeout_ms;
            break;

        case 6:
            /* Mix: assign then wakeup */
            {
                void *custom_ptr = NULL;
                if (Size >= 3) {
                    custom_ptr = (void *)(intptr_t)(Data[2] + (Size >= 4 ? Data[3] : 0));
                }
                ret = curl_multi_assign(multi, sockfd, custom_ptr);
                (void)ret;
                ret = curl_multi_wakeup(multi);
                (void)ret;
            }
            break;

        case 7:
            /* Mix: timeout then remove handle */
            ret = curl_multi_timeout(multi, &timeout_ms);
            (void)ret;
            ret = curl_multi_remove_handle(multi, easy);
            (void)ret;
            break;

        case 8:
            /* Mix: get_offt then strerror */
            {
                CURLMinfo_offt info_type = (CURLMinfo_offt)(Size >= 3 ? Data[2] % 10 : 0);
                ret = curl_multi_get_offt(multi, info_type, &offt_val);
                (void)ret;
                CURLMcode err_code = (CURLMcode)(Size >= 4 ? Data[3] % 30 : 0);
                const char *str = curl_multi_strerror(err_code);
                if (str) {
                    (void)str[0];
                }
            }
            break;

        case 9:
            /* Stress: multiple operations */
            {
                void *custom_ptr = NULL;
                if (Size >= 3) {
                    custom_ptr = (void *)(intptr_t)(Data[2] + (Size >= 4 ? Data[3] : 0));
                }
                ret = curl_multi_assign(multi, sockfd, custom_ptr);
                (void)ret;
                ret = curl_multi_wakeup(multi);
                (void)ret;
                ret = curl_multi_timeout(multi, &timeout_ms);
                (void)ret;
                {
                    CURLMinfo_offt info_type = (CURLMinfo_offt)(Size >= 5 ? Data[4] % 10 : 0);
                    ret = curl_multi_get_offt(multi, info_type, &offt_val);
                    (void)ret;
                }
                {
                    CURLMcode err_code = (CURLMcode)(Size >= 6 ? Data[5] % 30 : 0);
                    const char *str = curl_multi_strerror(err_code);
                    if (str) {
                        (void)str[0];
                    }
                }
            }
            break;

        default:
            break;
    }

    /* Cleanup */
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}