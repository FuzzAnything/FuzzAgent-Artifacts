// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <curl/easy.h>

static void dummy_malloc(size_t size) {
    (void)size;
}

static void dummy_free(void *ptr) {
    (void)ptr;
}

static void *dummy_realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}

static char *dummy_strdup(const char *str) {
    (void)str;
    return NULL;
}

static void *dummy_calloc(size_t nmemb, size_t size) {
    (void)nmemb;
    (void)size;
    return NULL;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    size_t sent = 0;
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    curl_off_t fragsize = 0;

    if (Size > 0) {
        flags = Data[0] & 0xFF;
        if (Size > 1) {
            frame_len = (curl_off_t)(Data[1] & 0xFF);
        }
        if (Size > 2) {
            fragsize = (curl_off_t)(Data[2] & 0xFF);
        }
    }

    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    if (Size >= 4) {
        // Ensure the URL string is null-terminated to prevent heap-buffer-overflow in strlen
        char url_buf[256];
        size_t copy_len = (Size - 4) < sizeof(url_buf) - 1 ? (Size - 4) : sizeof(url_buf) - 1;
        memcpy(url_buf, &Data[3], copy_len);
        url_buf[copy_len] = '\0';
        res = curl_easy_setopt(curl, CURLOPT_URL, url_buf);
        (void)res;
    }

    if (Size >= 5) {
        res = curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, Data[4]);
        (void)res;
    }

    res = curl_easy_perform(curl);
    (void)res;

    res = curl_easy_upkeep(curl);
    (void)res;

    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    if (Size >= 6) {
        res = curl_ws_send(curl, (const void *)&Data[5], Size - 5, &sent, fragsize, flags);
        (void)res;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}