// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
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

static void write_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file();

    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Use input data to drive option settings
    long opt_val = (long)Data[0];
    if (Size > 1) {
        opt_val = (long)(Data[0] | (Data[1] << 8));
    }

    // Test curl_easy_setopt with various option types based on input
    // CURLOPT_TIMEOUT is a long option
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, opt_val);

    // Set a dummy URL if possible
    if (Size > 10) {
        char url[11];
        memcpy(url, Data + 10, 10);
        url[10] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    }

    // Set callbacks
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);

    // Set connect only for websocket functions
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    // Perform to establish connection (may fail, which is fine for fuzzing)
    curl_easy_perform(curl);

    // Fuzz WebSocket functions
    unsigned int flags = (unsigned int)Data[0];
    curl_off_t frame_len = (curl_off_t)(Size > 1 ? Data[1] : 0);

    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    size_t sent = 0;
    curl_off_t fragsize = (curl_off_t)(Size > 2 ? Data[2] : 0);
    unsigned int send_flags = (unsigned int)Data[0];
    res = curl_ws_send(curl, Data, Size, &sent, fragsize, send_flags);
    (void)res;

    char recv_buf[256];
    size_t recv = 0;
    const struct curl_ws_frame *meta = NULL;
    res = curl_ws_recv(curl, recv_buf, sizeof(recv_buf), &recv, &meta);
    (void)res;

    // Test curl_ws_meta
    const struct curl_ws_frame *current_meta = curl_ws_meta(curl);
    (void)current_meta;

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}