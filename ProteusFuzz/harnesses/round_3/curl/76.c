// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
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

    // Use input data to drive fuzzing logic
    // Ensure we have enough data before accessing indices
    int action = Data[0] % 10;
    int socket_fd = (int)(Data[1] % 1024);
    int error_code = (int)(Data[2] % 100);
    int option_value = (int)(Data[3] % 100);

    // Fuzz curl_multi_setopt
    if (action == 1) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)option_value);
    }

    // Fuzz curl_multi_assign
    if (action == 2) {
        void *custom_ptr = (void *)(uintptr_t)(Data[3] % 1000);
        curl_multi_assign(multi_handle, (curl_socket_t)socket_fd, custom_ptr);
    }

    // Fuzz curl_multi_remove_handle
    if (action == 3) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // Fuzz curl_multi_wakeup
    if (action == 4) {
        curl_multi_wakeup(multi_handle);
    }

    // Fuzz curl_multi_strerror with various error codes
    if (action == 5) {
        const char *str = curl_multi_strerror((CURLMcode)error_code);
        (void)str; // Suppress unused variable warning
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}