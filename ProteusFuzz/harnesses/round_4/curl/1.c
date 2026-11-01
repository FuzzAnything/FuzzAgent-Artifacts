// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Set a dummy URL to ensure the handle is valid
    curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080");
    // Set CONNECT_ONLY as required by the target functions
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    // Prepare buffers for send/recv operations
    size_t max_buf_size = 4096;
    size_t actual_size = Size;
    if (actual_size > max_buf_size) {
        actual_size = max_buf_size;
    }
    
    char *send_buf = malloc(actual_size);
    char *recv_buf = malloc(max_buf_size);
    
    if (!send_buf || !recv_buf) {
        if (send_buf) free(send_buf);
        if (recv_buf) free(recv_buf);
        curl_easy_cleanup(curl);
        return 0;
    }

    if (actual_size > 0) {
        memcpy(send_buf, Data, actual_size);
    }

    size_t sent = 0;
    size_t recv = 0;
    size_t n = 0;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;
    const struct curl_ws_frame *meta = NULL;

    // Safely derive fuzz parameters from input data
    if (Size > 0) {
        fragsize = (curl_off_t)Data[0] * 100; 
    }
    if (Size > 1) {
        flags = (unsigned int)Data[1];
    }

    // Fuzz curl_easy_perform first to establish state (or attempt to)
    curl_easy_perform(curl);

    // Fuzz curl_ws_send
    if (actual_size > 0) {
        curl_ws_send(curl, send_buf, actual_size, &sent, fragsize, flags);
    }

    // Fuzz curl_easy_send
    if (actual_size > 0) {
        curl_easy_send(curl, send_buf, actual_size, &n);
    }

    // Fuzz curl_easy_recv
    curl_easy_recv(curl, recv_buf, max_buf_size, &n);

    // Fuzz curl_ws_recv
    curl_ws_recv(curl, recv_buf, max_buf_size, &recv, &meta);

    // Fuzz curl_ws_meta
    meta = curl_ws_meta(curl);

    // Cleanup
    free(send_buf);
    free(recv_buf);
    curl_easy_cleanup(curl);

    return 0;
}