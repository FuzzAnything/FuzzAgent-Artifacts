// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
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
#include <fcntl.h>
#include <sys/select.h>

/* 
 * Helper to create a dummy easy handle with minimal setup.
 * Returns NULL on failure.
 */
static CURL *create_dummy_easy_handle(void) {
    CURL *handle = curl_easy_init();
    if (!handle) {
        return NULL;
    }
    
    /* Set a minimal URL to avoid some internal errors, though not strictly required for all ops */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100L);
    
    return handle;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Use input data to drive logic branches and randomization.
     * We use a simple modulo approach to select operations.
     */
    unsigned int op_selector = (Size > 0) ? Data[0] % 6 : 0;
    
    /* Initialize multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy_handle = create_dummy_easy_handle();
    int easy_added = 0;
    CURLMcode res;

    /* Add easy handle to multi handle if we have enough data */
    if (Size > 1 && easy_handle) {
        if (curl_multi_add_handle(multi, easy_handle) == CURLM_OK) {
            easy_added = 1;
        }
    }

    switch (op_selector) {
        case 0:
            /* Test curl_multi_waitfds */
            {
                unsigned int fd_count = 0;
                /* First call with size=0 to get count */
                res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
                
                if (res == CURLM_OK && fd_count > 0) {
                    /* Allocate buffer for fds */
                    struct curl_waitfd *fds = malloc(fd_count * sizeof(struct curl_waitfd));
                    if (fds) {
                        /* Call again to fill buffer */
                        res = curl_multi_waitfds(multi, fds, fd_count, &fd_count);
                        /* Optionally use fds for select/poll simulation if needed, but not strictly required for fuzzing */
                        free(fds);
                    }
                }
            }
            break;

        case 1:
            /* Test curl_multi_notify_disable */
            {
                unsigned int notification_flags = (Size > 1) ? Data[1] : 0;
                res = curl_multi_notify_disable(multi, notification_flags);
                (void)res; /* Ignore return for fuzzing stability */
            }
            break;

        case 2:
            /* Test curl_multi_notify_enable */
            {
                unsigned int notification_flags = (Size > 1) ? Data[1] : 0;
                res = curl_multi_notify_enable(multi, notification_flags);
                (void)res;
            }
            break;

        case 3:
            /* Test curl_multi_remove_handle */
            if (easy_added && easy_handle) {
                res = curl_multi_remove_handle(multi, easy_handle);
                easy_added = 0; /* Mark as removed */
            } else {
                /* Try to remove a handle that wasn't added or is NULL */
                res = curl_multi_remove_handle(multi, easy_handle);
            }
            (void)res;
            break;

        case 4:
            /* Test curl_multi_waitfds with invalid parameters (if Size allows) */
            {
                struct curl_waitfd dummy_fd = { -1, 0, 0 };
                unsigned int fd_count = 0;
                /* Pass a small size to potentially trigger edge cases */
                res = curl_multi_waitfds(multi, &dummy_fd, 1, &fd_count);
                (void)res;
            }
            break;

        case 5:
            /* Test curl_multi_remove_handle with invalid easy handle */
            {
                CURL *invalid_handle = NULL;
                res = curl_multi_remove_handle(multi, invalid_handle);
                (void)res;
            }
            break;
    }

    /* Cleanup */
    if (easy_added && easy_handle) {
        curl_multi_remove_handle(multi, easy_handle);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi);

    return 0;
}