// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include curl headers in the correct order to ensure type definitions are available */
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Use a dummy URL for connection setup. */
    const char *url = "http://example.com/dummy";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    /* Perform the connection attempt */
    CURLcode res = curl_easy_perform(curl);
    (void)res;

    /* Prepare buffers for send/recv operations */
    size_t max_buf = 1024;
    char *send_buf = malloc(max_buf);
    char *recv_buf = malloc(max_buf);
    if (!send_buf || !recv_buf) {
        curl_easy_cleanup(curl);
        free(send_buf);
        free(recv_buf);
        return 0;
    }

    /* Use input data to fill send buffer */
    size_t send_len = (Size < max_buf) ? Size : max_buf;
    memcpy(send_buf, Data, send_len);

    size_t n_recv = 0;
    size_t n_sent = 0;
    size_t recv_len = 0;
    size_t sent_len = 0;
    struct curl_ws_frame *meta = NULL;

    /* Test curl_easy_send */
    res = curl_easy_send(curl, send_buf, send_len, &n_sent);
    (void)res;

    /* Test curl_easy_recv */
    res = curl_easy_recv(curl, recv_buf, max_buf, &n_recv);
    (void)res;

    /* Test curl_ws_send with various flags and fragment sizes */
    curl_off_t fragsize = (Size > 0) ? (curl_off_t)Data[0] : 0;
    unsigned int flags = (Size > 1) ? Data[1] : 0;
    res = curl_ws_send(curl, send_buf, send_len, &sent_len, fragsize, flags);
    (void)res;

    /* Test curl_ws_recv */
    res = curl_ws_recv(curl, recv_buf, max_buf, &recv_len, &meta);
    (void)res;

    /* Cleanup */
    free(send_buf);
    free(recv_buf);
    curl_easy_cleanup(curl);

    return 0;
}