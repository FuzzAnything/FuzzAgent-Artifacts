// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Entry point: LLVMFuzzerTestOneInput
 */

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode res;
    const char *str;
    int idx = 0;
    int socket_fd;
    void *sockp;
    CURLMoption option;
    curl_push_callback pushfn;

    if (Size < 4) {
        return 0;
    }

    /* Initialize multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Initialize easy handle for removal tests */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* 1. Fuzz curl_multi_strerror with various error codes */
    if (Size > 0) {
        CURLMcode error_code = (CURLMcode)Data[idx++];
        str = curl_multi_strerror(error_code);
        (void)str; /* Use the result to avoid unused warnings */
    }

    /* 2. Fuzz curl_multi_setopt */
    if (Size > 1) {
        option = (CURLMoption)Data[idx++];
        /* We only safely fuzz known options that don't require complex callbacks */
        /* For unknown options, curl_multi_setopt may return an error, which is fine for fuzzing */
        if (option == CURLMOPT_PUSHFUNCTION) {
            pushfn = NULL; /* Set to NULL to deny pushes */
            res = curl_multi_setopt(multi_handle, option, pushfn);
            (void)res;
        } else {
            /* For other options, we pass a dummy value or NULL depending on type */
            /* This is a simplified approach; real fuzzing might need more careful handling */
            res = curl_multi_setopt(multi_handle, option, NULL);
            (void)res;
        }
    }

    /* 3. Fuzz curl_multi_wakeup */
    if (Size > 2) {
        res = curl_multi_wakeup(multi_handle);
        (void)res;
    }

    /* 4. Fuzz curl_multi_assign */
    if (Size > 3) {
        socket_fd = (int)Data[idx++];
        sockp = (void *)(intptr_t)Data[idx]; /* Cast byte to pointer for fuzzing */
        res = curl_multi_assign(multi_handle, socket_fd, sockp);
        (void)res;
    }

    /* 5. Fuzz curl_multi_remove_handle */
    if (Size > 4) {
        res = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)res;
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}