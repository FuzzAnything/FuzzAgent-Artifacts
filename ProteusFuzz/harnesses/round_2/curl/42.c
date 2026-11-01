// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_seek_callback(void *userdata, curl_off_t offset, int origin) {
    (void)userdata;
    (void)offset;
    (void)origin;
    return 0;
}

static void dummy_free_callback(void *userdata) {
    (void)userdata;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;

    // Initialize global curl
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    // Initialize easy handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Basic setup to prevent immediate failures
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);

    // Create a dummy file for potential file-based operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Fuzz curl_ws_start_frame
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    
    // Safe extraction of flags and frame_len with bounds checking
    if (Size >= sizeof(unsigned int)) {
        flags = *(const unsigned int *)(Data);
    }
    
    if (Size >= sizeof(curl_off_t)) {
        frame_len = *(const curl_off_t *)(Data);
    } else if (Size >= sizeof(unsigned int)) {
        // Fallback if curl_off_t is larger than available data but we have at least 4 bytes
        frame_len = *(const unsigned int *)(Data);
    }

    // Attempt to start a WebSocket frame
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    // Fuzz curl_ws_send
    size_t sent = 0;
    curl_off_t fragsize = 0;
    
    if (Size >= sizeof(curl_off_t)) {
        fragsize = *(const curl_off_t *)(Data);
    } else if (Size >= sizeof(unsigned int)) {
        fragsize = *(const unsigned int *)(Data);
    }

    // Use the input data as the buffer
    res = curl_ws_send(curl, Data, Size, &sent, fragsize, flags);
    (void)res;

    // Fuzz curl_easy_upkeep
    res = curl_easy_upkeep(curl);
    (void)res;

    // Fuzz curl_easy_perform
    res = curl_easy_perform(curl);
    (void)res;

    // Fuzz curl_mime_data_cb
    mime = curl_mime_init(curl);
    if (mime) {
        part = curl_mime_addpart(mime);
        if (part) {
            void *arg = NULL;
            if (Size > 0) {
                arg = malloc(1);
                if (arg) {
                    memset(arg, 0, 1);
                }
            }

            curl_off_t datasize = 0;
            if (Size >= sizeof(curl_off_t)) {
                datasize = *(const curl_off_t *)(Data);
            } else if (Size >= sizeof(unsigned int)) {
                datasize = *(const unsigned int *)(Data);
            }

            res = curl_mime_data_cb(part, datasize, dummy_read_callback, dummy_seek_callback, dummy_free_callback, arg);
            (void)res;

            if (arg) {
                free(arg);
            }
        }
        curl_mime_free(mime);
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}