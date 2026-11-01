// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
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
    int ev_bitmask = 0;

    // Use data from input to influence fuzzing behavior
    uint8_t op = Data[0] % 10;
    uint8_t socket_val = Data[1] % 256;
    uint8_t event_val = Data[2] % 256;
    uint8_t error_code = Data[3] % 256;

    // Create a dummy file if needed
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Initialize a dummy CURL handle to add to multi_handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_FILE, NULL);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, NULL);
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    // Fuzz different operations based on input
    switch (op) {
        case 0:
            // curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            (void)res;
            break;

        case 1:
            // curl_multi_assign
            sockfd = (curl_socket_t)socket_val;
            sockp = (void *)(size_t)socket_val;
            res = curl_multi_assign(multi_handle, sockfd, sockp);
            (void)res;
            break;

        case 2:
            // curl_multi_strerror
            res = (CURLMcode)error_code;
            const char *str = curl_multi_strerror(res);
            (void)str;
            break;

        case 3:
            // curl_multi_socket (deprecated)
            sockfd = (curl_socket_t)socket_val;
            res = curl_multi_socket(multi_handle, sockfd, &running_handles);
            (void)res;
            break;

        case 4:
            // curl_multi_socket_action
            sockfd = (curl_socket_t)socket_val;
            ev_bitmask = event_val;
            res = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
            (void)res;
            break;

        case 5:
            // curl_multi_socket_all (deprecated)
            res = curl_multi_socket_all(multi_handle, &running_handles);
            (void)res;
            break;

        case 6:
            // Multiple calls to curl_multi_perform
            for (int i = 0; i < 5; i++) {
                res = curl_multi_perform(multi_handle, &running_handles);
                (void)res;
            }
            break;

        case 7:
            // Multiple calls to curl_multi_socket_action with varying event masks
            for (int i = 0; i < 5; i++) {
                sockfd = (curl_socket_t)(socket_val + i);
                ev_bitmask = (event_val + i) % 10;
                res = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
                (void)res;
            }
            break;

        case 8:
            // Test curl_multi_assign with invalid socket
            sockfd = -1;
            res = curl_multi_assign(multi_handle, sockfd, sockp);
            (void)res;
            break;

        case 9:
            // Test curl_multi_strerror with various error codes
            for (int i = 0; i < 10; i++) {
                res = (CURLMcode)(error_code + i);
                const char *str = curl_multi_strerror(res);
                (void)str;
            }
            break;
    }

    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    curl_multi_cleanup(multi_handle);

    return 0;
}