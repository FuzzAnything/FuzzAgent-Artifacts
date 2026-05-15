// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int dummy_running_handles = 0;

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Extract some values from fuzzer input
    int dummy_int = 0;
    curl_socket_t dummy_socket = 0;
    void *dummy_ptr = NULL;
    
    if (Size >= sizeof(int)) {
        memcpy(&dummy_int, Data, sizeof(int));
    }
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&dummy_socket, Data, sizeof(curl_socket_t));
    }
    if (Size >= sizeof(void*)) {
        size_t offset = Size >= 8 ? 8 : Size;
        dummy_ptr = (void*)(Data + (offset % Size));
    }

    // Test curl_multi_setopt with various options
    uint8_t opt_selector = Data[0] % 8;
    CURLMcode result;
    
    switch (opt_selector) {
        case 0:
            result = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, dummy_int);
            break;
        case 1:
            result = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, dummy_int);
            break;
        case 2:
            result = curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, dummy_int);
            break;
        case 3:
            result = curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, dummy_int);
            break;
        case 4:
            result = curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, dummy_int);
            break;
        case 5:
            result = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, dummy_ptr);
            break;
        case 6:
            result = curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, dummy_ptr);
            break;
        case 7:
            result = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, dummy_int);
            break;
    }

    // Test deprecated curl_multi_socket
    if (Size >= sizeof(int) + sizeof(curl_socket_t)) {
        result = curl_multi_socket(multi_handle, dummy_socket, &dummy_running_handles);
    }

    // Test curl_multi_assign
    if (Size >= sizeof(curl_socket_t) + sizeof(void*)) {
        result = curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);
    }

    // Test deprecated curl_multi_socket_all
    if (Size >= sizeof(int)) {
        result = curl_multi_socket_all(multi_handle, &dummy_running_handles);
    }

    // Test curl_multi_perform
    if (Size >= sizeof(int)) {
        result = curl_multi_perform(multi_handle, &dummy_running_handles);
    }

    // Test curl_multi_socket_action with various event bitmasks
    if (Size >= sizeof(curl_socket_t) + 2*sizeof(int)) {
        int ev_bitmask = dummy_int & 0x7;
        result = curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &dummy_running_handles);
    }

    curl_multi_cleanup(multi_handle);
    
    return 0;
}