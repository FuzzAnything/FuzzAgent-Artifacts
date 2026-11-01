// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize a buffer for operations */
    size_t buffer_size = Size > 1024 ? 1024 : Size;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Set up a dummy URL for connection */
    (void)curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com");

    /* Set CONNECT_ONLY as required by the target functions */
    (void)curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Perform the connection attempt */
    (void)curl_easy_perform(curl);

    /* Fuzz curl_ws_recv */
    size_t recv_len = 0;
    struct curl_ws_frame *meta = NULL;
    (void)curl_ws_recv(curl, buffer, buffer_size, &recv_len, (const struct curl_ws_frame **)&meta);

    /* Fuzz curl_ws_send */
    size_t sent_len = 0;
    curl_off_t fragsize = (Size > 0) ? Data[0] : 0;
    unsigned int flags = (Size > 1) ? Data[1] : 0;
    (void)curl_ws_send(curl, buffer, buffer_size, &sent_len, fragsize, flags);

    /* Fuzz curl_ws_meta */
    (void)curl_ws_meta(curl);

    /* Fuzz curl_easy_recv */
    size_t n_recv = 0;
    (void)curl_easy_recv(curl, buffer, buffer_size, &n_recv);

    /* Fuzz curl_easy_send */
    size_t n_sent = 0;
    (void)curl_easy_send(curl, buffer, buffer_size, &n_sent);

    /* Cleanup */
    free(buffer);
    curl_easy_cleanup(curl);

    return 0;
}