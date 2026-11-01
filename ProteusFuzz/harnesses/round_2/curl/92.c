// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_multi(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    // Extract data for various parameters with bounds checking
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    CURLMcode error_code = CURLM_OK;

    if (Size >= sizeof(CURLMcode)) {
        memcpy(&error_code, Data, sizeof(CURLMcode));
    }
    if (Size >= sizeof(CURLMcode) + sizeof(curl_socket_t)) {
        memcpy(&sockfd, Data + sizeof(CURLMcode), sizeof(curl_socket_t));
    }

    // 1. Test curl_multi_strerror with various error codes
    const char *str = curl_multi_strerror(error_code);
    (void)str; // Suppress unused variable warning

    // 2. Test curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi, &running_handles);

    // 3. Test curl_multi_perform
    curl_multi_perform(multi, &running_handles);

    // 4. Test curl_multi_socket (deprecated)
    curl_multi_socket(multi, sockfd, &running_handles);

    // 5. Test curl_multi_wakeup
    curl_multi_wakeup(multi);

    // 6. Test curl_multi_assign
    curl_multi_assign(multi, sockfd, sockp);

    // Cleanup
    cleanup_multi(multi);

    return 0;
}