// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
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

    // Fuzz curl_multi_setopt with various options if data is sufficient
    if (Size >= 2) {
        CURLMoption option = (CURLMoption)Data[0];
        int val = (int)Data[1];
        
        // Try setting options that accept integers
        if (option == CURLMOPT_MAXCONNECTS) {
            curl_multi_setopt(multi_handle, option, val);
        } else if (option == CURLMOPT_PIPELINING) {
            curl_multi_setopt(multi_handle, option, val);
        } else if (option == CURLMOPT_MAX_HOST_CONNECTIONS) {
            curl_multi_setopt(multi_handle, option, val);
        } else if (option == CURLMOPT_MAX_PIPELINE_LENGTH) {
            curl_multi_setopt(multi_handle, option, val);
        } else if (option == CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE) {
            curl_multi_setopt(multi_handle, option, val);
        } else if (option == CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE) {
            curl_multi_setopt(multi_handle, option, val);
        } else {
            // Generic attempt for other options, might fail but safe for fuzzing
            // We pass an integer value for most options to avoid macro argument errors
            curl_multi_setopt(multi_handle, option, val);
        }
    }

    // Fuzz curl_multi_add_handle (implicit requirement for multi operations)
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    // Try removing again to test idempotency or error handling
    curl_multi_remove_handle(multi_handle, easy_handle);
    // Try removing with NULL handle
    curl_multi_remove_handle(multi_handle, NULL);

    // Fuzz curl_multi_assign
    curl_socket_t sockfd = (curl_socket_t)(Size > 2 ? Data[2] : 0);
    void *sockp = (void *)(uintptr_t)(Size > 3 ? Data[3] : 0);
    curl_multi_assign(multi_handle, sockfd, sockp);
    // Try with invalid socket or handle
    curl_multi_assign(multi_handle, -1, sockp);
    curl_multi_assign(NULL, sockfd, sockp);

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);
    curl_multi_wakeup(NULL);

    // Fuzz curl_multi_strerror with various error codes
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *str = curl_multi_strerror(code);
        (void)str; // Use the string to avoid unused variable warning
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}