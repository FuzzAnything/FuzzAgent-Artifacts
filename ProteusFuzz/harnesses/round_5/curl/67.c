// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
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
#include <curl/easy.h>
#include <curl/websockets.h>
#include <curl/multi.h>

/* Dummy callback functions for mime data matching curl_read_callback signature */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userp;
    return 0;
}

static int dummy_seek_callback(void *userp, curl_off_t offset, int origin) {
    (void)userp;
    (void)offset;
    (void)origin;
    return 0;
}

static void dummy_free_callback(void *userp) {
    (void)userp;
}

/* Helper to create a dummy file for testing */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    size_t sent = 0;
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    curl_off_t fragsize = 0;
    curl_off_t datasize = 0;

    /* Initialize global curl */
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create dummy file if needed */
    create_dummy_file();

    /* Parse input data to drive fuzzing logic */
    if (Size > 0) {
        flags = Data[0];
        if (Size > 1) {
            frame_len = (curl_off_t)Data[1];
        }
        if (Size > 2) {
            fragsize = (curl_off_t)Data[2];
        }
        if (Size > 3) {
            datasize = (curl_off_t)Data[3];
        }
    }

    /* Initialize curl handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Set a basic URL to avoid immediate failures */
    curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com/socket");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Try to perform connection */
    res = curl_easy_perform(curl);
    (void)res;

    /* Test curl_ws_start_frame */
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    /* Test curl_ws_send */
    const char *dummy_buffer = "fuzzing data";
    size_t dummy_buflen = strlen(dummy_buffer);
    res = curl_ws_send(curl, dummy_buffer, dummy_buflen, &sent, fragsize, flags);
    (void)res;

    /* Test curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res;

    /* Test MIME related functions */
    mime = curl_mime_init(curl);
    if (mime) {
        part = curl_mime_addpart(mime);
        if (part) {
            /* Test curl_mime_data_cb with dummy callbacks */
            res = curl_mime_data_cb(part, datasize, dummy_read_callback, 
                                    dummy_seek_callback, dummy_free_callback, 
                                    (void*)(size_t)flags);
            (void)res;
        }
        curl_mime_free(mime);
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}