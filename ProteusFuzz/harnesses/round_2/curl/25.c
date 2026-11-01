// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/websockets.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static helper to initialize a dummy file if needed */
static void init_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

/* Static helper to safely clean up a CURL handle */
static void cleanup_handle(CURL *curl) {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize dummy file if needed */
    init_dummy_file();

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Basic setup to allow connection-only mode */
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L); /* Short timeout to avoid hanging */
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    /* Perform initial connection attempt */
    CURLcode res = curl_easy_perform(curl);
    /* Note: res might be CURLE_COULDNT_CONNECT or similar, which is expected in fuzzing */
    /* We proceed to test send/recv functions regardless, as they handle invalid states */

    /* Prepare buffers for send/recv operations */
    size_t max_buffer_size = 4096;
    size_t actual_size = (Size > max_buffer_size) ? max_buffer_size : Size;
    char *send_buffer = malloc(actual_size + 1);
    char *recv_buffer = malloc(actual_size + 1);
    size_t n_sent = 0;
    size_t n_recv = 0;
    size_t n_ws_sent = 0;
    size_t n_ws_recv = 0;
    const struct curl_ws_frame *ws_meta = NULL;

    if (!send_buffer || !recv_buffer) {
        free(send_buffer);
        free(recv_buffer);
        cleanup_handle(curl);
        return 0;
    }

    memcpy(send_buffer, Data, actual_size);
    send_buffer[actual_size] = '\0';

    /* Fuzz curl_easy_send */
    curl_easy_send(curl, send_buffer, actual_size, &n_sent);

    /* Fuzz curl_easy_recv */
    curl_easy_recv(curl, recv_buffer, actual_size, &n_recv);

    /* Fuzz curl_ws_meta */
    ws_meta = curl_ws_meta(curl);
    (void)ws_meta; /* Suppress unused variable warning */

    /* Fuzz curl_ws_send */
    curl_ws_send(curl, send_buffer, actual_size, &n_ws_sent, 1024, 0);

    /* Fuzz curl_ws_recv */
    curl_ws_recv(curl, recv_buffer, actual_size, &n_ws_recv, &ws_meta);

    /* Cleanup */
    free(send_buffer);
    free(recv_buffer);
    cleanup_handle(curl);

    return 0;
}