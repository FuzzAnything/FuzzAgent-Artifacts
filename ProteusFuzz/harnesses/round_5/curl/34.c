// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>

static uint32_t read_uint32(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + sizeof(uint32_t) > Size) {
        return 0;
    }
    const uint8_t *p = Data + offset;
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURLMcode res;
    int running_handles = 0;
    curl_socket_t sock = -1;
    void *sockp = NULL;
    int ev_bitmask = 0;

    if (Size < 4) {
        return 0;
    }

    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Extract parameters from fuzz input safely
    uint32_t param1 = read_uint32(Data, Size, 0);
    uint32_t param2 = read_uint32(Data, Size, 4);
    uint32_t param3 = read_uint32(Data, Size, 8);

    // Initialize socket with fuzzed value (clamped to avoid extreme values)
    sock = (curl_socket_t)(param1 % 1024);

    // Initialize event bitmask
    ev_bitmask = (int)(param2 % 16);

    // Initialize sockp pointer (using a dummy static buffer)
    static char dummy_buffer[256];
    if (param3 & 1) {
        sockp = dummy_buffer;
    }

    // Test curl_multi_assign
    res = curl_multi_assign(multi_handle, sock, sockp);
    if (res == CURLM_OK) {
        // Test curl_multi_socket_action
        res = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
        (void)res;

        // Test curl_multi_socket (deprecated)
        res = curl_multi_socket(multi_handle, sock, &running_handles);
        (void)res;

        // Test curl_multi_socket_all (deprecated)
        res = curl_multi_socket_all(multi_handle, &running_handles);
        (void)res;

        // Test curl_multi_perform
        res = curl_multi_perform(multi_handle, &running_handles);
        (void)res;
    }

    // Test curl_multi_strerror with various error codes
    const char *str = curl_multi_strerror(res);
    (void)str;

    // Test with CURLM_OK and other potential error codes
    str = curl_multi_strerror(CURLM_OK);
    (void)str;
    str = curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)str;
    str = curl_multi_strerror(CURLM_RECURSIVE_API_CALL);
    (void)str;

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}