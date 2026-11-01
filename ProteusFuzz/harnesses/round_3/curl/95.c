// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    setup_dummy_file();

    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_FILE, fopen("./dummy_file", "w"));
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = (void *)(uintptr_t)Data[0];

    // Fuzz curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Fuzz curl_multi_assign
    if (Size > 1) {
        sockfd = (curl_socket_t)Data[1];
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Fuzz curl_multi_strerror with various error codes
    for (int i = 0; i < Size && i < 100; i++) {
        CURLMcode error_code = (CURLMcode)Data[i];
        const char *str = curl_multi_strerror(error_code);
        (void)str; // Use the result to avoid unused variable warnings
    }

    // Fuzz curl_multi_socket (deprecated)
    if (Size > 2) {
        sockfd = (curl_socket_t)Data[2];
        curl_multi_socket(multi_handle, sockfd, &running_handles);
    }

    // Fuzz curl_multi_socket_action
    if (Size > 3) {
        sockfd = (curl_socket_t)Data[3];
        int ev_bitmask = Data[4] % 4; // Limit event bitmask to reasonable values
        curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
    }

    // Fuzz curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}