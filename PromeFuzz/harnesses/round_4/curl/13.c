// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static int dummy_running_handles = 0;
static void *dummy_sockp = NULL;

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create dummy file if needed
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }

    // Use fuzzer data to derive parameters
    int running_handles = 0;
    curl_socket_t sockfd = 0;
    int ev_bitmask = 0;
    CURLMoption option = 0;
    long param = 0;

    // Extract values from fuzzer input with bounds checking
    if (Size >= sizeof(int)) {
        memcpy(&running_handles, Data, sizeof(int));
    }
    if (Size >= 2 * sizeof(int)) {
        memcpy(&sockfd, Data + sizeof(int), sizeof(curl_socket_t));
    }
    if (Size >= 3 * sizeof(int)) {
        memcpy(&ev_bitmask, Data + 2 * sizeof(int), sizeof(int));
    }
    if (Size >= 4 * sizeof(int)) {
        memcpy(&option, Data + 3 * sizeof(int), sizeof(CURLMoption));
    }
    if (Size >= 4 * sizeof(int) + sizeof(long)) {
        memcpy(&param, Data + 4 * sizeof(int), sizeof(long));
    }

    // Call target functions in different combinations
    // 1. curl_multi_socket_all (deprecated but still testable)
    curl_multi_socket_all(multi_handle, &dummy_running_handles);

    // 2. curl_multi_assign
    curl_multi_assign(multi_handle, sockfd, dummy_sockp);

    // 3. curl_multi_socket (deprecated but still testable)
    curl_multi_socket(multi_handle, sockfd, &dummy_running_handles);

    // 4. curl_multi_socket_action
    curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &dummy_running_handles);

    // 5. curl_multi_setopt with various options
    // Test with different option types
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, param);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, param);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, param);
    curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, param);
    curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, param);
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING_SITE_BL, param);
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING_SERVER_BL, param);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, param);
    
    // Also test with the option derived from fuzzer input
    curl_multi_setopt(multi_handle, option, param);

    // 6. curl_multi_perform
    curl_multi_perform(multi_handle, &dummy_running_handles);

    // Additional socket action calls with different event masks
    curl_multi_socket_action(multi_handle, sockfd, CURL_POLL_IN, &dummy_running_handles);
    curl_multi_socket_action(multi_handle, sockfd, CURL_POLL_OUT, &dummy_running_handles);
    curl_multi_socket_action(multi_handle, sockfd, CURL_POLL_INOUT, &dummy_running_handles);
    curl_multi_socket_action(multi_handle, sockfd, CURL_POLL_REMOVE, &dummy_running_handles);

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}