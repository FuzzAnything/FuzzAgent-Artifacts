// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>
#include <curl/typecheck-gcc.h>

/* 
 * Fuzz driver for curl WebSocket and low-level socket functions.
 * Targets: curl_ws_recv, curl_easy_recv, curl_easy_send, curl_easy_reset.
 * Note: Wcurl_easy_setopt_err_* are compile-time macros, not runtime functions.
 */

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Create a dummy file if needed for options like CURLOPT_CAINFO */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Initialize buffer for receiving data */
    char recv_buffer[4096];
    size_t n_recv = 0;
    size_t n_sent = 0;
    struct curl_ws_frame *meta = NULL;

    /* Set a dummy URL to ensure curl has something to work with */
    curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080");

    /* Try to set CONNECT_ONLY to prepare for low-level socket operations */
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Use dummy file for various string options to explore different states */
    if (fp) {
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLKEY, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLCERT, "./dummy_file");
    }

    /* Attempt to perform the connection (will likely fail in fuzzing env, but exercises code) */
    curl_easy_perform(curl);

    /* 
     * Fuzz curl_ws_recv: 
     * Requires a valid WebSocket connection. In fuzzing, this will likely fail,
     * but it tests the error handling paths.
     */
    curl_ws_recv(curl, recv_buffer, sizeof(recv_buffer), &n_recv, &meta);

    /* 
     * Fuzz curl_easy_recv:
     * Reads raw data from the socket.
     */
    curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &n_recv);

    /* 
     * Fuzz curl_easy_send:
     * Sends raw data. We use the input data as the payload.
     */
    if (Size > 0) {
        curl_easy_send(curl, Data, Size, &n_sent);
    }

    /* 
     * Fuzz curl_easy_reset:
     * Resets the handle to initial state.
     */
    curl_easy_reset(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}