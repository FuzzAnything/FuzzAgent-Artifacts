// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static CURL *create_dummy_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    curl_global_init(CURL_GLOBAL_ALL);
    
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    CURL *easy = create_dummy_easy_handle();
    if (easy) {
        curl_multi_add_handle(multi, easy);
    }

    unsigned int seed = 0;
    if (Size >= sizeof(seed)) {
        memcpy(&seed, Data, sizeof(seed));
    }

    size_t offset = 0;

    /* curl_multi_setopt with various options */
    if (offset + 2 <= Size) {
        CURLMoption opt = (CURLMoption)(Data[offset] % 10);
        offset++;
        long param = (long)Data[offset];
        offset++;
        curl_multi_setopt(multi, opt, param);
    }

    /* curl_multi_remove_handle and re-add */
    if (easy) {
        curl_multi_remove_handle(multi, easy);
        if (Size > 0 && (Data[0] & 0x01)) {
            curl_multi_add_handle(multi, easy);
        }
    }

    /* curl_multi_get_offt */
    if (offset + 1 <= Size) {
        CURLMinfo_offt info = (CURLMinfo_offt)(Data[offset] % 4);
        offset++;
        curl_off_t value;
        curl_multi_get_offt(multi, info, &value);
    }

    /* curl_multi_waitfds */
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi, NULL, 0, &fd_count);
    if (fd_count > 0) {
        struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }

    /* curl_multi_perform */
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);

    /* curl_multi_timeout */
    long timeout = 0;
    curl_multi_timeout(multi, &timeout);

    /* Cleanup */
    if (easy) {
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    return 0;
}