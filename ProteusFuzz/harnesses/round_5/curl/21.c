// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <curl/typecheck-gcc.h>

static void setup_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    if (size == 0) return;
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Setup dummy file if needed
    setup_dummy_file("./dummy_file", Data, Size);

    // Initialize buffers and variables
    char recv_buf[4096];
    char send_buf[4096];
    size_t recv_n = 0;
    size_t send_n = 0;
    struct curl_ws_frame *meta = NULL;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;

    // Parse input to determine fuzzing behavior
    int mode = Data[0] % 10;

    // Configure curl handle
    curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_WHATEVER);

    // Perform connection (may fail, but we proceed to test APIs)
    CURLcode res = curl_easy_perform(curl);

    // Fuzz curl_ws_recv
    if (mode == 0) {
        res = curl_ws_recv(curl, recv_buf, sizeof(recv_buf), &recv_n, (const struct curl_ws_frame **)&meta);
        (void)res;
    }

    // Fuzz curl_ws_send
    if (mode == 1) {
        size_t send_len = Size > sizeof(send_buf) ? sizeof(send_buf) : Size;
        if (send_len > 0) {
            memcpy(send_buf, Data, send_len);
        }
        // Fix: Ensure we have at least 2 bytes before accessing Data[1]
        if (Size >= 2) {
            fragsize = (curl_off_t)(Data[1] * 100);
        } else {
            fragsize = 0;
        }
        // Fix: Ensure we have at least 3 bytes before accessing Data[2]
        if (Size >= 3) {
            flags = Data[2] % 256;
        } else {
            flags = 0;
        }
        res = curl_ws_send(curl, send_buf, send_len, &send_n, fragsize, flags);
        (void)res;
    }

    // Fuzz curl_ws_meta
    if (mode == 2) {
        const struct curl_ws_frame *frame = curl_ws_meta(curl);
        (void)frame;
    }

    // Fuzz curl_easy_recv
    if (mode == 3) {
        res = curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &recv_n);
        (void)res;
    }

    // Fuzz curl_easy_send
    if (mode == 4) {
        size_t send_len = Size > sizeof(send_buf) ? sizeof(send_buf) : Size;
        if (send_len > 0) {
            memcpy(send_buf, Data, send_len);
        }
        res = curl_easy_send(curl, send_buf, send_len, &send_n);
        (void)res;
    }

    // Fuzz various setopt calls with type checking macros
    if (mode == 5) {
        char *str_arg = (char *)Data;
        long long_arg = 0;
        void *cb_data = (void *)Data;

        // String option
        curl_easy_setopt(curl, CURLOPT_URL, str_arg);
        
        // Long option: Ensure safe extraction
        if (Size >= 2) {
            long_arg = (long)(Data[0] | (Data[1] << 8));
        } else {
            long_arg = 0;
        }
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, long_arg);
        
        // Callback data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, cb_data);
    }

    // Fuzz with invalid or edge-case inputs
    if (mode == 6) {
        res = curl_ws_recv(curl, NULL, 0, &recv_n, (const struct curl_ws_frame **)&meta);
        (void)res;
    }

    if (mode == 7) {
        res = curl_ws_send(curl, NULL, 0, &send_n, 0, 0);
        (void)res;
    }

    if (mode == 8) {
        res = curl_easy_recv(curl, NULL, 0, &recv_n);
        (void)res;
    }

    if (mode == 9) {
        res = curl_easy_send(curl, NULL, 0, &send_n);
        (void)res;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    return 0;
}