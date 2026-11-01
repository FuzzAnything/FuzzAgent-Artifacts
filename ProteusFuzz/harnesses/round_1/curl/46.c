// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_wait at multi.c:1574:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create a dummy easy handle to add to the multi handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to ensure the handle is somewhat configured
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 1L);

    CURLMcode mcode = curl_multi_add_handle(multi_handle, easy_handle);
    if (mcode != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Prepare extra_fds for curl_multi_wait and curl_multi_poll
    // We create a simple pipe to have a valid file descriptor
    int pipefd[2];
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    int ret = 0;
    int timeout_ms = 0;

    if (Size >= 4) {
        timeout_ms = (int)(Data[0] & 0xFF);
        if (timeout_ms > 1000) timeout_ms = 1000; // Cap timeout for fuzzing safety
    }

    if (Size >= 5) {
        int fd_count = (int)(Data[4] & 0x03); // 0, 1, or 2 extra fds
        if (fd_count > 0) {
            if (pipe(pipefd) == 0) {
                extra_fds[0].fd = pipefd[0];
                extra_fds[0].events = CURL_WAIT_POLLIN;
                extra_fds[0].revents = 0;
                extra_nfds = 1;

                if (fd_count > 1) {
                    extra_fds[1].fd = pipefd[1];
                    extra_fds[1].events = CURL_WAIT_POLLOUT;
                    extra_fds[1].revents = 0;
                    extra_nfds = 2;
                }
            }
        }
    }

    // Fuzz curl_multi_wait
    mcode = curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    
    // Fuzz curl_multi_strerror with various error codes
    // Use input data to generate random error codes
    // Note: Using only valid CURLMcode values defined in curl/multi.h
    CURLMcode test_errors[] = {
        CURLM_OK, 
        CURLM_CALL_MULTI_PERFORM, 
        CURLM_BAD_HANDLE,
        CURLM_BAD_EASY_HANDLE, 
        CURLM_OUT_OF_MEMORY, 
        CURLM_INTERNAL_ERROR,
        CURLM_ADDED_ALREADY,
        CURLM_RECURSIVE_API_CALL,
        CURLM_WAKEUP_FAILURE,
        CURLM_BAD_SOCKET,
        CURLM_LAST
    };
    
    for (int i = 0; i < sizeof(test_errors) / sizeof(test_errors[0]); i++) {
        const char *msg = curl_multi_strerror(test_errors[i]);
        (void)msg; // Suppress unused variable warning
    }

    // Use input data to fuzz error code index
    if (Size > 0) {
        int idx = Data[0] % (sizeof(test_errors) / sizeof(test_errors[0]));
        const char *fuzz_msg = curl_multi_strerror(test_errors[idx]);
        (void)fuzz_msg;
    }

    // Fuzz curl_multi_poll
    ret = 0;
    mcode = curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    // Cleanup extra file descriptors
    if (extra_nfds > 0 && pipefd[0] >= 0) {
        close(pipefd[0]);
    }
    if (extra_nfds > 1 && pipefd[1] >= 0) {
        close(pipefd[1]);
    }

    // Cleanup curl handles
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    // Cleanup global environment
    curl_global_cleanup();

    return 0;
}