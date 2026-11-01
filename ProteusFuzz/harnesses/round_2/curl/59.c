// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize curl options to enable WebSocket and Connect Only mode
    curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);

    // Attempt to perform the connection (non-blocking or short timeout)
    // This is required for curl_ws_recv and curl_ws_send to be valid
    curl_easy_perform(curl);

    // Fuzz curl_ws_meta
    // Note: This returns NULL if not in a callback, but we call it to test internal state handling
    const struct curl_ws_frame *meta = curl_ws_meta(curl);
    (void)meta; // Suppress unused variable warning

    // Prepare buffer for recv/send
    size_t max_buf_size = Size > 65536 ? 65536 : Size;
    char *recv_buf = malloc(max_buf_size);
    char *send_buf = malloc(max_buf_size);
    
    if (!recv_buf || !send_buf) {
        curl_easy_cleanup(curl);
        return 0;
    }

    // Copy fuzz data to buffers
    memcpy(recv_buf, Data, max_buf_size);
    memcpy(send_buf, Data, max_buf_size);

    // Fuzz curl_ws_recv
    size_t recv_len = 0;
    const struct curl_ws_frame *recv_meta = NULL;
    curl_ws_recv(curl, recv_buf, max_buf_size, &recv_len, &recv_meta);

    // Fuzz curl_ws_send with various flags and fragment sizes derived from input
    size_t sent_len = 0;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;

    // Ensure we have enough data before accessing indices
    if (Size >= 3) {
        fragsize = (curl_off_t)(Data[0] * 256 + Data[1]);
        flags = (unsigned int)(Data[2] % 4); // Limit flags to reasonable range
    } else if (Size == 2) {
        fragsize = (curl_off_t)(Data[0] * 256 + Data[1]);
    } else if (Size == 1) {
        fragsize = (curl_off_t)Data[0];
    }

    curl_ws_send(curl, send_buf, max_buf_size, &sent_len, fragsize, flags);

    // Cleanup
    free(recv_buf);
    free(send_buf);
    curl_easy_cleanup(curl);

    return 0;
}