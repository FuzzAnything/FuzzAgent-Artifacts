// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* Include curl headers */
#include <curl/curl.h>
#include <curl/multi.h>

/* Disable type checking macros that cause compilation issues in fuzzing context */
#define CURL_DISABLE_TYPECHECK 1

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Prepare a safe URL string from input data */
    char url[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url, Data, url_len);
    url[url_len] = '\0';

    /* Configure easy handle with safe defaults */
    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_DNS_USE_GLOBAL_CACHE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);

    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    int ev_bitmask = 0;

    /* Extract socket file descriptor from input if available */
    if (Size >= 4) {
        sockfd = *(const int32_t *)Data;
    }

    /* Extract event bitmask from input if available, ensuring we have enough data */
    if (Size >= 8) {
        ev_bitmask = *(const int32_t *)(Data + 4);
    }

    /* Fuzz curl_multi_assign */
    void *sockp = (void *)(uintptr_t)(Size % 1000);
    (void)curl_multi_assign(multi_handle, sockfd, sockp);

    /* Fuzz curl_multi_perform */
    (void)curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket (deprecated) */
    (void)curl_multi_socket(multi_handle, sockfd, &running_handles);

    /* Fuzz curl_multi_socket_action */
    (void)curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);

    /* Fuzz curl_multi_socket_all (deprecated) */
    (void)curl_multi_socket_all(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}