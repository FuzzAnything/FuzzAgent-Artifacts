// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int create_dummy_file(void) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    const char *data = "dummy data for fuzzing";
    if (write(fd, data, strlen(data)) < 0) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Extract parameters from fuzz input
    CURLMcode error_code = (CURLMcode)((int32_t)Data[0]);
    curl_socket_t sockfd = (curl_socket_t)((int32_t)Data[1]);
    void *sockp = (void *)(uintptr_t)((uint32_t)Data[2]);
    int running_handles = 0;

    // Test 1: curl_multi_strerror
    const char *str = curl_multi_strerror(error_code);
    if (str) {
        // Ensure the string is null-terminated and non-empty for safety
        if (str[0] == '\0') {
            // Empty string is valid but we can just continue
        }
    }

    // Test 2: curl_multi_wakeup
    // This may fail if the handle is not in a polling state, which is expected
    curl_multi_wakeup(multi_handle);

    // Test 3: curl_multi_assign
    curl_multi_assign(multi_handle, sockfd, sockp);

    // Test 4: curl_multi_socket (Deprecated)
    curl_multi_socket(multi_handle, sockfd, &running_handles);

    // Test 5: curl_multi_socket_all (Deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Test 6: curl_multi_remove_handle
    // We need a valid CURL handle to remove. Create one for testing.
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Add the easy handle to the multi handle first
        curl_multi_add_handle(multi_handle, easy_handle);
        // Now try to remove it
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    // Cleanup multi handle
    curl_multi_cleanup(multi_handle);

    // Cleanup global environment
    curl_global_cleanup();

    return 0;
}