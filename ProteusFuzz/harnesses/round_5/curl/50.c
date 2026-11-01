// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <curl/easy.h>

/* Static helper functions to avoid name conflicts and allow definition outside main logic */
/* curl_read_callback expects char* as first argument, not void* */
static size_t dummy_read(char *ptr, size_t size, size_t nitems, void *userp) {
    (void)ptr; (void)size; (void)nitems; (void)userp;
    return 0;
}

static int dummy_seek(void *stream, curl_off_t offset, int origin) {
    (void)stream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_OK;
}

static void dummy_free(void *ptr) {
    (void)ptr;
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    size_t sent = 0;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;
    unsigned int frame_flags = 0;
    curl_off_t frame_len = 0;
    long global_flags = 0;

    if (Size < 4) {
        return 0;
    }

    /* Parse input for diversification */
    global_flags = (long)Data[0];
    fragsize = (curl_off_t)(Data[1] * 1000 + Data[2]);
    frame_len = (curl_off_t)(Data[3] * 10000 + Data[2]);
    flags = Data[0] & 0xFF;
    frame_flags = Data[1] & 0xFF;

    /* Initialize global curl */
    res = curl_global_init(global_flags);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Initialize easy handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Set some basic options to make the handle valid for some operations */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Create MIME structure for testing curl_mime_data_cb */
    mime = curl_mime_init(curl);
    if (mime) {
        part = curl_mime_addpart(mime);
    }

    /* Fuzz curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res;

    /* Fuzz curl_ws_send */
    /* Use input data as buffer if available */
    const void *buffer = Data;
    size_t buflen = Size;
    res = curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
    (void)res;

    /* Fuzz curl_mime_data_cb */
    if (part) {
        curl_off_t datasize = (Size > 0) ? (curl_off_t)Size : 0;
        res = curl_mime_data_cb(part, datasize, dummy_read, dummy_seek, dummy_free, (void*)Data);
        (void)res;
    }

    /* Fuzz curl_easy_perform */
    res = curl_easy_perform(curl);
    (void)res;

    /* Fuzz curl_ws_start_frame */
    res = curl_ws_start_frame(curl, frame_flags, frame_len);
    (void)res;

    /* Cleanup */
    if (mime) {
        curl_mime_free(mime);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}