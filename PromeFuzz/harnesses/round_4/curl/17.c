// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void dummy_socket_callback(CURL *easy, curl_socket_t s, int action, void *userp, void *socketp) {}
static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) { return 0; }

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create a dummy easy handle and add to multi
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set a dummy URL
        curl_easy_setopt(easy, CURLOPT_URL, "http://localhost");
        curl_multi_add_handle(multi_handle, easy);
    }

    // Use fuzz data to determine parameters
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    int ev_bitmask = 0;
    void *sockp = NULL;
    
    // Extract parameters from fuzz data if available
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&sockfd, Data, sizeof(curl_socket_t));
    }
    if (Size >= sizeof(curl_socket_t) + sizeof(int)) {
        memcpy(&ev_bitmask, Data + sizeof(curl_socket_t), sizeof(int));
    }
    if (Size >= sizeof(curl_socket_t) + 2*sizeof(int)) {
        sockp = (void *)(Data + sizeof(curl_socket_t) + sizeof(int));
    }

    // Test curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Test curl_multi_assign
    curl_multi_assign(multi_handle, sockfd, sockp);

    // Test curl_multi_socket (deprecated)
    curl_multi_socket(multi_handle, sockfd, &running_handles);

    // Test curl_multi_socket_action
    curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);

    // Test curl_multi_setopt with various options
    // Set socket callback
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
    // Set timer callback
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
    // Set max connections
    long maxconns = 10;
    if (Size >= sizeof(long)) {
        memcpy(&maxconns, Data, sizeof(long));
    }
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, maxconns);
    // Set pipeline
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 0L);
    // Set max total connections
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 100L);

    // Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Cleanup
    if (easy) {
        curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}