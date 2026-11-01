// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Set a dummy URL to allow initialization without actual network */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Initialize buffers for send/recv operations */
    size_t buffer_size = (Size > 1024) ? 1024 : Size;
    char *send_buffer = (char *)malloc(buffer_size);
    char *recv_buffer = (char *)malloc(buffer_size);
    if (!send_buffer || !recv_buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memcpy(send_buffer, Data, buffer_size);

    size_t n = 0;
    size_t sent = 0;
    size_t recv = 0;
    const struct curl_ws_frame *meta = NULL;
    const struct curl_ws_frame *metap = NULL;

    /* Test curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Test curl_easy_send */
    curl_easy_send(curl, send_buffer, buffer_size, &n);

    /* Test curl_easy_recv */
    curl_easy_recv(curl, recv_buffer, buffer_size, &n);

    /* Test curl_ws_meta */
    meta = curl_ws_meta(curl);
    (void)meta;

    /* Test curl_ws_send */
    curl_ws_send(curl, send_buffer, buffer_size, &sent, (curl_off_t)buffer_size, 0);

    /* Test curl_ws_recv */
    curl_ws_recv(curl, recv_buffer, buffer_size, &recv, &metap);

    /* Cleanup */
    free(send_buffer);
    free(recv_buffer);
    curl_easy_cleanup(curl);

    return 0;
}