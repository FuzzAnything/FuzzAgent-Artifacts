// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static CURL *create_curl_handle(const uint8_t *data, size_t size) {
    CURL *curl = curl_easy_init();
    if (!curl) return NULL;
    
    write_dummy_file(data, size);
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    return curl;
}

static void test_curl_easy_send(CURL *curl, const uint8_t *data, size_t size) {
    size_t sent = 0;
    if (size > 0) {
        curl_easy_send(curl, data, size, &sent);
    }
}

static void test_curl_easy_upkeep(CURL *curl) {
    curl_easy_upkeep(curl);
}

static void test_curl_ws_recv(CURL *curl, const uint8_t *data, size_t size) {
    char buffer[1024];
    size_t recv = 0;
    const struct curl_ws_frame *meta = NULL;
    
    if (size > 0 && size < sizeof(buffer)) {
        memcpy(buffer, data, size);
        curl_ws_recv(curl, buffer, sizeof(buffer), &recv, &meta);
    }
}

static void test_curl_easy_perform(CURL *curl) {
    curl_easy_perform(curl);
}

static void test_curl_easy_recv(CURL *curl, const uint8_t *data, size_t size) {
    char buffer[1024];
    size_t recv = 0;
    
    if (size > 0 && size < sizeof(buffer)) {
        memcpy(buffer, data, size);
        curl_easy_recv(curl, buffer, sizeof(buffer), &recv);
    }
}

static void test_curl_easy_pause(CURL *curl, const uint8_t *data, size_t size) {
    int bitmask = 0;
    if (size > 0) {
        bitmask = data[0] % 4;
        if (bitmask == 0) bitmask = CURLPAUSE_RECV;
        else if (bitmask == 1) bitmask = CURLPAUSE_SEND;
        else if (bitmask == 2) bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND;
        else bitmask = CURLPAUSE_CONT;
    }
    curl_easy_pause(curl, bitmask);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    if (size == 0) return 0;
    
    CURL *curl = create_curl_handle(data, size);
    if (!curl) return 0;
    
    uint8_t selector = data[0] % 6;
    const uint8_t *func_data = data + 1;
    size_t func_size = size > 1 ? size - 1 : 0;
    
    switch (selector) {
        case 0:
            test_curl_easy_send(curl, func_data, func_size);
            break;
        case 1:
            test_curl_easy_upkeep(curl);
            break;
        case 2:
            test_curl_ws_recv(curl, func_data, func_size);
            break;
        case 3:
            test_curl_easy_perform(curl);
            break;
        case 4:
            test_curl_easy_recv(curl, func_data, func_size);
            break;
        case 5:
            test_curl_easy_pause(curl, func_data, func_size);
            break;
    }
    
    curl_easy_cleanup(curl);
    
    return 0;
}