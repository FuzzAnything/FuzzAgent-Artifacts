// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    CURLMcode res;

    // Initialize easy handle for multi operations
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set a dummy URL to make the handle valid
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        // Add to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    uint8_t action = Data[0];
    uint8_t sub_action = Data[1];

    switch (action) {
        case 0:
            // Test curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            (void)res;
            break;

        case 1:
            // Test curl_multi_socket_all (deprecated)
            res = curl_multi_socket_all(multi_handle, &running_handles);
            (void)res;
            break;

        case 2:
            // Test curl_multi_socket (deprecated)
            sockfd = (curl_socket_t)(sub_action * 100 - 50);
            res = curl_multi_socket(multi_handle, sockfd, &running_handles);
            (void)res;
            break;

        case 3:
            // Test curl_multi_assign
            sockfd = (curl_socket_t)(sub_action * 100 - 50);
            // Ensure we have enough data before accessing Data[2]
            if (Size > 2) {
                sockp = (void *)(size_t)(Data[2] % 256);
                res = curl_multi_assign(multi_handle, sockfd, sockp);
                (void)res;
            }
            break;

        case 4:
            // Test curl_multi_strerror with various error codes
            int error_code = 0;
            if (Size > 4) {
                error_code = Data[2] | (Data[3] << 8) | (Data[4] << 16);
            }
            const char *str = curl_multi_strerror((CURLMcode)error_code);
            (void)str;
            break;

        default:
            // Fallback: try a mix of operations
            res = curl_multi_perform(multi_handle, &running_handles);
            (void)res;
            sockfd = (curl_socket_t)(sub_action);
            res = curl_multi_socket(multi_handle, sockfd, &running_handles);
            (void)res;
            break;
    }

    // Cleanup
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}