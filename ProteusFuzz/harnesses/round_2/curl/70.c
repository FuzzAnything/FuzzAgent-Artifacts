// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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

/* Dummy callback to satisfy type checking if needed */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Set a dummy URL to ensure the handle is valid for operations */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Extract data for fuzzing parameters */
    int running_handles = 0;
    curl_socket_t sockfd = (curl_socket_t)(Data[0] | (Data[1] << 8));
    int ev_bitmask = (int)(Data[2] | (Data[3] << 8));
    void *sockp = (void *)(uintptr_t)(Data[0] ^ Data[1] ^ Data[2] ^ Data[3]);

    /* Fuzz curl_multi_assign */
    (void)curl_multi_assign(multi_handle, sockfd, sockp);

    /* Fuzz curl_multi_socket (deprecated but tested) */
    (void)curl_multi_socket(multi_handle, sockfd, &running_handles);

    /* Fuzz curl_multi_socket_all (deprecated but tested) */
    (void)curl_multi_socket_all(multi_handle, &running_handles);

    /* Fuzz curl_multi_perform */
    (void)curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket_action */
    (void)curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}