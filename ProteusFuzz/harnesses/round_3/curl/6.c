// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
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

    // Fuzz curl_multi_add_handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_assign with various socket values derived from input
    if (Size >= 4) {
        int sock_fd = *(const int *)(Data);
        void *custom_data = (void *)(uintptr_t)(Data[3] & 0xFF);
        curl_multi_assign(multi_handle, (curl_socket_t)sock_fd, custom_data);
    }

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_strerror with various error codes
    if (Size >= 2) {
        CURLMcode error_code = (CURLMcode)(Data[0] | (Data[1] << 8));
        const char *error_str = curl_multi_strerror(error_code);
        (void)error_str; // Use the result to avoid unused warning
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}