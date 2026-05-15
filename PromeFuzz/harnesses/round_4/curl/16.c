// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Workaround for curl header compilation issues */
#ifndef CURL_DISABLE_TYPECHECK
#define CURL_DISABLE_TYPECHECK
#endif

#include "curl/multi.h"
#include "curl/easy.h"

static CURL *create_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    }
    return easy;
}

static void cleanup_easy_handle(CURL *easy) {
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    uint8_t operation = Data[0] % 7;
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;

    switch (operation) {
        case 0:
            multi_handle = curl_multi_init();
            if (multi_handle) {
                curl_multi_cleanup(multi_handle);
            }
            break;

        case 1:
            multi_handle = curl_multi_init();
            easy_handle = create_easy_handle();
            if (multi_handle && easy_handle) {
                curl_multi_add_handle(multi_handle, easy_handle);
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            if (easy_handle) {
                cleanup_easy_handle(easy_handle);
            }
            if (multi_handle) {
                curl_multi_cleanup(multi_handle);
            }
            break;

        case 2:
            multi_handle = curl_multi_init();
            if (multi_handle) {
                curl_multi_perform(multi_handle, &running_handles);
                curl_multi_cleanup(multi_handle);
            }
            break;

        case 3:
            easy_handle = create_easy_handle();
            if (easy_handle) {
                curl_easy_upkeep(easy_handle);
                cleanup_easy_handle(easy_handle);
            }
            break;

        case 4:
            multi_handle = curl_multi_init();
            easy_handle = create_easy_handle();
            if (multi_handle && easy_handle) {
                curl_multi_add_handle(multi_handle, easy_handle);
                curl_multi_perform(multi_handle, &running_handles);
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            if (easy_handle) {
                cleanup_easy_handle(easy_handle);
            }
            if (multi_handle) {
                curl_multi_cleanup(multi_handle);
            }
            break;

        case 5:
            curl_multi_cleanup(NULL);
            break;

        case 6:
            multi_handle = curl_multi_init();
            if (multi_handle) {
                for (int i = 0; i < 3 && Size > (size_t)i; ++i) {
                    easy_handle = create_easy_handle();
                    if (easy_handle) {
                        curl_multi_add_handle(multi_handle, easy_handle);
                        curl_multi_perform(multi_handle, &running_handles);
                        cleanup_easy_handle(easy_handle);
                    }
                }
                curl_multi_cleanup(multi_handle);
            }
            break;
    }

    return 0;
}