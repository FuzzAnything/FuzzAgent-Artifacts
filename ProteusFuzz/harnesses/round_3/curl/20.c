// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Prepare a dummy URL to ensure connection attempt */
    const char *url = "http://example.com";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);

    /* Perform the connection */
    (void)curl_easy_perform(curl);

    /* Buffers for send/recv operations */
    size_t max_buffer_size = 1024;
    if (Size > max_buffer_size) {
        Size = max_buffer_size;
    }
    char *send_buffer = malloc(Size);
    char *recv_buffer = malloc(max_buffer_size);
    if (!send_buffer || !recv_buffer) {
        curl_easy_cleanup(curl);
        free(send_buffer);
        free(recv_buffer);
        return 0;
    }
    memcpy(send_buffer, Data, Size);

    size_t n_sent = 0;
    size_t n_recv = 0;
    size_t recv_len = 0;
    const struct curl_ws_frame *meta = NULL;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;

    /* Fuzz curl_easy_send */
    (void)curl_easy_send(curl, send_buffer, Size, &n_sent);

    /* Fuzz curl_easy_recv */
    (void)curl_easy_recv(curl, recv_buffer, max_buffer_size, &n_recv);

    /* Fuzz curl_ws_send */
    /* Use a subset of input for fragsize and flags if available */
    if (Size >= 4) {
        fragsize = (curl_off_t)(Data[0] | (Data[1] << 8));
        flags = (unsigned int)(Data[2] | (Data[3] << 8));
    }
    (void)curl_ws_send(curl, send_buffer, Size, &n_sent, fragsize, flags);

    /* Fuzz curl_ws_recv */
    (void)curl_ws_recv(curl, recv_buffer, max_buffer_size, &recv_len, &meta);

    /* Fuzz curl_ws_meta */
    (void)curl_ws_meta(curl);

    /* Cleanup */
    free(send_buffer);
    free(recv_buffer);
    curl_easy_cleanup(curl);

    return 0;
}