// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Include curl headers which define CURL, CURLcode, etc. */
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

static void setup_dummy_file(void) {
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        const char *data = "dummy content for testing";
        write(fd, data, strlen(data));
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file();

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Prepare buffer for send/recv operations
    size_t max_buf_size = 1024;
    size_t actual_buf_size = (Size > max_buf_size) ? max_buf_size : Size;
    char *buffer = (char *)malloc(actual_buf_size);
    if (!buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memcpy(buffer, Data, actual_buf_size);

    // Initialize size trackers
    size_t n = 0;
    size_t sent = 0;
    size_t recv = 0;
    curl_off_t fragsize = (Size > 0) ? (curl_off_t)Data[0] : 0;
    unsigned int flags = (Size > 1) ? Data[1] : 0;

    // Configure curl for CONNECT_ONLY to enable low-level socket operations
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    // Attempt to establish connection (may fail, which is fine for fuzzing)
    (void)curl_easy_perform(curl);

    // 1. Fuzz curl_easy_upkeep
    (void)curl_easy_upkeep(curl);

    // 2. Fuzz curl_easy_send
    (void)curl_easy_send(curl, buffer, actual_buf_size, &n);

    // 3. Fuzz curl_easy_recv
    (void)curl_easy_recv(curl, buffer, actual_buf_size, &n);

    // 4. Fuzz curl_ws_meta (will likely return NULL outside callback, but safe to call)
    const struct curl_ws_frame *meta = curl_ws_meta(curl);
    (void)meta;

    // 5. Fuzz curl_ws_send
    (void)curl_ws_send(curl, buffer, actual_buf_size, &sent, fragsize, flags);

    // 6. Fuzz curl_ws_recv
    const struct curl_ws_frame *meta_recv = NULL;
    (void)curl_ws_recv(curl, buffer, actual_buf_size, &recv, &meta_recv);

    // Cleanup
    free(buffer);
    curl_easy_cleanup(curl);

    return 0;
}