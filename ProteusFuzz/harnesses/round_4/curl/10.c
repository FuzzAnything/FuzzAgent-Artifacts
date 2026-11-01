// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_multi(CURLM *multi, CURL *easy) {
    if (multi && easy) {
        curl_multi_remove_handle(multi, easy);
    }
    if (easy) {
        curl_easy_cleanup(easy);
    }
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Basic setup to ensure the handle is valid for multi operations
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");

    CURLMcode res;
    res = curl_multi_add_handle(multi, easy);
    if (res != CURLM_OK) {
        cleanup_multi(multi, easy);
        return 0;
    }

    // Fuzzing logic based on input data
    unsigned int op = Data[0] % 6;

    if (op == 0) {
        // Test curl_multi_waitfds
        struct curl_waitfd *ufds = NULL;
        unsigned int fd_count = 0;
        
        // First call to get count
        res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
        
        if (res == CURLM_OK && fd_count > 0) {
            // Allocate buffer based on input size to vary allocation
            size_t alloc_size = (Size > 1000) ? 1000 : Size;
            ufds = (struct curl_waitfd *)malloc(alloc_size * sizeof(struct curl_waitfd));
            if (ufds) {
                unsigned int actual_count = 0;
                res = curl_multi_waitfds(multi, ufds, (unsigned int)alloc_size, &actual_count);
                (void)res; // Use result
                free(ufds);
            }
        }
    } else if (op == 1) {
        // Test curl_multi_assign
        // Ensure Size >= 3 before accessing Data[1] and Data[2]
        if (Size >= 3) {
            curl_socket_t sockfd = (curl_socket_t)(Data[1] % 1024);
            void *sockp = (void *)(uintptr_t)(Data[2] % 1000);
            res = curl_multi_assign(multi, sockfd, sockp);
            (void)res;
        }
    } else if (op == 2) {
        // Test curl_multi_remove_handle
        res = curl_multi_remove_handle(multi, easy);
        (void)res;
        // Re-add for subsequent ops if needed, but for fuzzing we might leave it removed
        if (res == CURLM_OK) {
            res = curl_multi_add_handle(multi, easy);
        }
    } else if (op == 3) {
        // Test curl_multi_strerror
        // Ensure Size >= 2 before accessing Data[1]
        if (Size >= 2) {
            CURLMcode err_code = (CURLMcode)(Data[1] % 256);
            const char *str = curl_multi_strerror(err_code);
            (void)str;
        }
    } else if (op == 4) {
        // Test curl_multi_wait
        // Ensure we have enough data before accessing Data[1], Data[2], and Data[3]
        if (Size >= 4) {
            struct curl_waitfd extra_fds[1];
            int ret = 0;
            int timeout_ms = (int)(Data[1] % 1000);
            
            extra_fds[0].fd = (curl_socket_t)(Data[2] % 1024);
            extra_fds[0].events = (short)(Data[3] % 4);
            extra_fds[0].revents = 0;
            
            res = curl_multi_wait(multi, extra_fds, 1, timeout_ms, &ret);
            (void)res;
            (void)ret;
        }
    } else {
        // op == 5: Mixed operations or stress test
        // Try to remove and re-add multiple times
        int iterations = (Size > 10) ? 10 : (int)Size;
        for (int i = 0; i < iterations; i++) {
            res = curl_multi_remove_handle(multi, easy);
            if (res == CURLM_OK) {
                res = curl_multi_add_handle(multi, easy);
            }
        }
    }

    cleanup_multi(multi, easy);
    return 0;
}