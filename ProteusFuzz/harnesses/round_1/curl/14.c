// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <curl/multi.h>

static int create_dummy_socket(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    return sock;
}

static void close_socket(int sock) {
    if (sock >= 0) {
        close(sock);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int sock = create_dummy_socket();
    CURL *easy_handle = curl_easy_init();
    long timeout_ms = 0;
    curl_off_t offt_val = 0;
    void *custom_data = (void *)(size_t)Data[0];

    // Fuzz curl_multi_assign
    if (sock >= 0) {
        curl_multi_assign(multi_handle, sock, custom_data);
    }

    // Fuzz curl_multi_strerror with various error codes
    for (size_t i = 0; i < 10 && i < Size; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *str = curl_multi_strerror(code);
        if (str) {
            (void)strlen(str);
        }
    }

    // Fuzz curl_multi_get_offt
    for (size_t i = 0; i < 5 && i < Size; i++) {
        CURLMinfo_offt info = (CURLMinfo_offt)Data[i];
        curl_multi_get_offt(multi_handle, info, &offt_val);
    }

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_timeout
    curl_multi_timeout(multi_handle, &timeout_ms);

    // Fuzz curl_multi_remove_handle
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    // Cleanup
    if (sock >= 0) {
        close_socket(sock);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}