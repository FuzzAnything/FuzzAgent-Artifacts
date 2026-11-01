// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_mime_init at mime.c:1180:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

/* Dummy callback functions defined at file scope to avoid definition inside another function */
static size_t dummy_read(void *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userp;
    return 0;
}

static int dummy_seek(void *stream, curl_off_t offset, int origin) {
    (void)stream;
    (void)offset;
    (void)origin;
    return 0;
}

static void dummy_free(void *ptr) {
    (void)ptr;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    size_t sent;
    curl_off_t fragsize;
    unsigned int flags;

    if (Size < 1) {
        return 0;
    }

    /* Initialize libcurl globally */
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Parse input data to diversify function calls */
    unsigned char action = Data[0] % 6;

    if (Size > 1) {
        fragsize = (curl_off_t)(Data[1] * 100 + 1);
    } else {
        fragsize = 100;
    }

    if (Size > 2) {
        flags = Data[2] % 4;
    } else {
        flags = 0;
    }

    /* Set up a dummy URL to avoid immediate errors in some functions */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    switch (action) {
        case 0:
            /* Test curl_easy_upkeep */
            res = curl_easy_upkeep(curl);
            break;

        case 1:
            /* Test curl_easy_perform */
            res = curl_easy_perform(curl);
            break;

        case 2:
            /* Test curl_ws_send */
            sent = 0;
            res = curl_ws_send(curl, Data, Size, &sent, fragsize, flags);
            break;

        case 3:
            /* Test curl_ws_start_frame */
            res = curl_ws_start_frame(curl, flags, (curl_off_t)Size);
            break;

        case 4:
            /* Test curl_mime_data_cb */
            mime = curl_mime_init(curl);
            if (mime) {
                part = curl_mime_addpart(mime);
                if (part) {
                    /* Cast dummy_read to curl_read_callback to fix pointer type mismatch */
                    res = curl_mime_data_cb(part, (curl_off_t)Size, 
                                           (curl_read_callback)dummy_read, 
                                           dummy_seek, dummy_free, (void *)Data);
                }
                curl_mime_free(mime);
            }
            break;

        case 5:
            /* Combine multiple calls */
            res = curl_easy_upkeep(curl);
            if (res == CURLE_OK) {
                res = curl_easy_perform(curl);
            }
            if (res == CURLE_OK) {
                sent = 0;
                res = curl_ws_send(curl, Data, Size, &sent, fragsize, flags);
            }
            break;
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}