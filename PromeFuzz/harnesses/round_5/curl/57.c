// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/curl.h"
#include "curl/multi.h"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        remove("./dummy_file");
        return 0;
    }

    // Initialize easy handle with dummy transfer
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    int running_handles = 0;
    int ret = 0;
    curl_socket_t dummy_socket = 0;
    void *dummy_sockp = NULL;

    // Use fuzzer data to determine which functions to call and with what parameters
    uint8_t selector = Data[0] % 6;
    int ev_bitmask = (Data[0] & 0x07); // Use lower 3 bits for event mask
    int timeout_ms = (Data[0] & 0x7F); // Use lower 7 bits for timeout

    // Prepare extra waitfd structure if needed
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = dummy_socket;
    extra_fds[0].events = ev_bitmask;
    extra_fds[0].revents = 0;

    switch (selector) {
        case 0:
            curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            break;
        case 1:
            curl_multi_assign(multi_handle, dummy_socket, dummy_sockp);
            break;
        case 2:
            curl_multi_perform(multi_handle, &running_handles);
            break;
        case 3:
            curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
            break;
        case 4:
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            break;
        case 5:
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
    }

    // Try calling all functions in different combinations to explore more states
    if (Size > 1) {
        int second_selector = Data[1] % 3;
        switch (second_selector) {
            case 0:
                curl_multi_perform(multi_handle, &running_handles);
                curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret);
                break;
            case 1:
                curl_multi_assign(multi_handle, dummy_socket, dummy_sockp);
                curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
                break;
            case 2:
                curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
                curl_multi_perform(multi_handle, &running_handles);
                break;
        }
    }

    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    remove("./dummy_file");

    return 0;
}