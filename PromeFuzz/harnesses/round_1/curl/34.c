// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_pause at easy.c:1136:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void init_curl_handle(CURL **curl) {
    *curl = curl_easy_init();
    if (*curl) {
        curl_easy_setopt(*curl, CURLOPT_URL, "ws://example.com/dummy");
        curl_easy_setopt(*curl, CURLOPT_CONNECT_ONLY, 2L);
        curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(*curl, CURLOPT_TIMEOUT_MS, 100L);
    }
}

static void cleanup_curl_handle(CURL *curl) {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl_initialized = 1;
    }

    if (Size < 1) return 0;

    CURL *curl = NULL;
    init_curl_handle(&curl);
    if (!curl) return 0;

    unsigned char selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    if (fuzz_size > 4096) fuzz_size = 4096;

    switch (selector) {
        case 0: {
            const struct curl_ws_frame *meta = curl_ws_meta(curl);
            (void)meta;
            break;
        }
        case 1: {
            void *buffer = malloc(fuzz_size);
            if (buffer && fuzz_size > 0) {
                memcpy(buffer, fuzz_data, fuzz_size);
            }
            size_t recv = 0;
            const struct curl_ws_frame *meta = NULL;
            CURLcode res = curl_ws_recv(curl, buffer ? buffer : (void *)fuzz_data,
                                        buffer ? fuzz_size : 0, &recv, &meta);
            (void)res;
            free(buffer);
            break;
        }
        case 2: {
            int bitmask = 0;
            if (fuzz_size > 0) {
                bitmask = fuzz_data[0] & (CURLPAUSE_RECV | CURLPAUSE_SEND);
            }
            CURLcode res = curl_easy_pause(curl, bitmask);
            (void)res;
            break;
        }
        case 3: {
            void *buffer = malloc(fuzz_size);
            if (buffer && fuzz_size > 0) {
                memcpy(buffer, fuzz_data, fuzz_size);
            }
            size_t sent = 0;
            curl_off_t fragsize = 0;
            unsigned int flags = 0;
            if (fuzz_size > sizeof(curl_off_t)) {
                memcpy(&fragsize, fuzz_data, sizeof(curl_off_t));
            }
            if (fuzz_size > sizeof(curl_off_t) + 1) {
                flags = fuzz_data[sizeof(curl_off_t)] & 0xFF;
            }
            CURLcode res = curl_ws_send(curl, buffer ? buffer : (void *)fuzz_data,
                                        buffer ? fuzz_size : 0, &sent, fragsize, flags);
            (void)res;
            free(buffer);
            break;
        }
        case 4: {
            void *buffer = malloc(fuzz_size);
            if (buffer && fuzz_size > 0) {
                memcpy(buffer, fuzz_data, fuzz_size);
            }
            size_t n = 0;
            CURLcode res = curl_easy_recv(curl, buffer ? buffer : (void *)fuzz_data,
                                          buffer ? fuzz_size : 0, &n);
            (void)res;
            free(buffer);
            break;
        }
        case 5: {
            void *buffer = malloc(fuzz_size);
            if (buffer && fuzz_size > 0) {
                memcpy(buffer, fuzz_data, fuzz_size);
            }
            size_t n = 0;
            CURLcode res = curl_easy_send(curl, buffer ? buffer : (void *)fuzz_data,
                                          buffer ? fuzz_size : 0, &n);
            (void)res;
            free(buffer);
            break;
        }
    }

    if (write_dummy_file(Data, Size) == 0) {
        FILE *file = fopen("./dummy_file", "rb");
        if (file) {
            fclose(file);
        }
    }

    cleanup_curl_handle(curl);
    return 0;
}