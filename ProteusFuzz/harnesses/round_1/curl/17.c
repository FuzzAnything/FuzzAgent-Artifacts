// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
#define CURL_STATICLIB
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

static size_t dummy_read(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static int dummy_seek(void *userp, curl_off_t offset, int origin) {
    (void)userp; (void)offset; (void)origin;
    return CURL_SEEKFUNC_OK;
}

static void dummy_free(void *userp) {
    (void)userp;
    // Do nothing
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    size_t sent = 0;
    
    if (Size == 0) {
        return 0;
    }

    // Initialize libcurl globally
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    // Create a CURL handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a dummy URL to avoid immediate errors in some functions
    res = curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Create a MIME structure and part for testing curl_mime_data_cb
    mime = curl_mime_init(curl);
    if (mime) {
        part = curl_mime_addpart(mime);
        if (part) {
            // Fuzz curl_mime_data_cb with varying parameters
            curl_off_t datasize = (Size > 0) ? (curl_off_t)Data[0] * 100 : 0;
            res = curl_mime_data_cb(part, datasize, dummy_read, dummy_seek, dummy_free, (void *)Data);
            (void)res; // Ignore result for fuzzing

            curl_mime_free(mime);
        }
    }

    // Test curl_ws_start_frame
    unsigned int flags = (Size > 1) ? Data[1] : 0;
    curl_off_t frame_len = (Size > 2) ? (curl_off_t)Data[2] * 1000 : 0;
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res; // Ignore result for fuzzing

    // Test curl_ws_send
    size_t buflen = (Size > 3) ? Data[3] : 0;
    size_t fragsize = (Size > 4) ? Data[4] : 0;
    unsigned int send_flags = (Size > 5) ? Data[5] : 0;
    res = curl_ws_send(curl, Data, buflen, &sent, fragsize, send_flags);
    (void)res; // Ignore result for fuzzing

    // Test curl_easy_perform with a dummy POST field
    if (Size > 6) {
        char *post_data = malloc(Size + 1);
        if (post_data) {
            memcpy(post_data, Data, Size);
            post_data[Size] = '\0'; // Null-terminate
            res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
            if (res == CURLE_OK) {
                res = curl_easy_perform(curl);
                (void)res; // Ignore result for fuzzing
            }
            free(post_data);
        }
    }

    // Test curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res; // Ignore result for fuzzing

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}