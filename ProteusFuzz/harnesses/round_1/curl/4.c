// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
#include <unistd.h>

/* Dummy callback to satisfy type checks if needed, though not directly used in target functions */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to associate with the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to make the handle valid for adding */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Parse input data to diversify function calls */
    uint32_t timeout_ms = 0;
    uint32_t extra_nfds = 0;
    uint32_t sockfd = 0;
    uint32_t error_code = 0;

    if (Size >= 4) {
        timeout_ms = *(const uint32_t *)(Data);
    }
    if (Size >= 8) {
        extra_nfds = *(const uint32_t *)(Data + 4);
    }
    if (Size >= 12) {
        sockfd = *(const uint32_t *)(Data + 8);
    }
    if (Size >= 16) {
        error_code = *(const uint32_t *)(Data + 12);
    }

    /* Limit extra_nfds to prevent allocation of huge arrays */
    if (extra_nfds > 100) {
        extra_nfds = 100;
    }

    /* Prepare extra_fds for curl_multi_wait */
    struct curl_waitfd *extra_fds = NULL;
    if (extra_nfds > 0) {
        extra_fds = (struct curl_waitfd *)malloc(extra_nfds * sizeof(struct curl_waitfd));
        if (!extra_fds) {
            extra_nfds = 0;
        } else {
            for (unsigned int i = 0; i < extra_nfds; i++) {
                /* Use dummy file descriptors; in a real scenario, these would be valid fds */
                extra_fds[i].fd = (curl_socket_t)(i + 1);
                extra_fds[i].events = 1; /* CURL_WAIT_POLLIN */
                extra_fds[i].revents = 0;
            }
        }
    }

    int ret = 0;
    CURLMcode mcode;

    /* 1. Test curl_multi_wait */
    mcode = curl_multi_wait(multi_handle, extra_fds, extra_nfds, (int)timeout_ms, &ret);
    (void)mcode; /* Suppress unused warning */

    /* 2. Test curl_multi_strerror with various codes */
    const char *err_str = curl_multi_strerror((CURLMcode)error_code);
    (void)err_str; /* Suppress unused warning */

    /* 3. Test curl_multi_assign */
    void *sockp = (void *)(uintptr_t)0x12345678; /* Dummy pointer */
    mcode = curl_multi_assign(multi_handle, (curl_socket_t)sockfd, sockp);
    (void)mcode;

    /* 4. Test curl_multi_remove_handle */
    mcode = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)mcode;

    /* Cleanup */
    if (extra_fds) {
        free(extra_fds);
    }

    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}