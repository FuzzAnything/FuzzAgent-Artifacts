// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_multi(CURLM *multi_handle) {
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURLMcode res;
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;

    // Fuzz curl_multi_strerror with various error codes
    if (Size >= sizeof(CURLMcode)) {
        CURLMcode error_code = *(const CURLMcode *)Data;
        const char *str = curl_multi_strerror(error_code);
        (void)str; // Suppress unused warning
    }

    // Fuzz curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_socket_all (deprecated)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_socket (deprecated)
    if (Size >= sizeof(curl_socket_t)) {
        sockfd = *(const curl_socket_t *)Data;
        res = curl_multi_socket(multi_handle, sockfd, &running_handles);
        (void)res;
    }

    // Fuzz curl_multi_wakeup
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    // Fuzz curl_multi_assign
    // We need enough data for sockfd (curl_socket_t) and a pointer (void*)
    // Note: In real fuzzing, the pointer value from data might be invalid, 
    // but the API just stores it. We ensure we don't read out of bounds.
    if (Size >= sizeof(curl_socket_t) + sizeof(void *)) {
        sockfd = *(const curl_socket_t *)Data;
        // We cast the next bytes to a pointer. 
        // This pointer is not dereferenced by curl_multi_assign, just stored.
        sockp = (void *)(uintptr_t)(*(const uint64_t *)(Data + sizeof(curl_socket_t)));
        res = curl_multi_assign(multi_handle, sockfd, sockp);
        (void)res;
    }

    cleanup_multi(multi_handle);
    return 0;
}