// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLcode res;
    CURL *curl = NULL;

    /* Initialize libcurl globally */
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a dummy file if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Prepare a buffer for POST data to ensure null-termination */
    char *post_data = NULL;
    size_t post_len = 0;
    
    if (Size > 0) {
        post_data = (char *)malloc(Size + 1);
        if (post_data) {
            memcpy(post_data, Data, Size);
            post_data[Size] = '\0'; 
            post_len = Size;
        }
    }

    /* Set up the curl handle */
    res = curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080");
    res = curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    
    if (post_data) {
        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_len);
    }

    /* 1. Test curl_easy_perform */
    res = curl_easy_perform(curl);

    /* 2. Test curl_easy_upkeep */
    res = curl_easy_upkeep(curl);

    /* 3. Test curl_ws_start_frame */
    unsigned int flags = (Size > 0) ? (unsigned int)Data[0] : 0;
    curl_off_t frame_len = 0;
    if (Size > 2) {
        frame_len = (curl_off_t)(Data[1] | ((curl_off_t)Data[2] << 8));
    }
    
    res = curl_ws_start_frame(curl, flags, frame_len);

    /* 4. Test curl_ws_send */
    size_t sent = 0;
    curl_off_t fragsize = 0;
    if (Size > 4) {
        fragsize = (curl_off_t)(Data[3] | ((curl_off_t)Data[4] << 8));
    }
    unsigned int send_flags = (Size > 5) ? (unsigned int)Data[5] : 0;
    
    const void *send_buf = Data;
    size_t send_buflen = Size;
    
    res = curl_ws_send(curl, send_buf, send_buflen, &sent, fragsize, send_flags);

    /* Cleanup */
    if (post_data) {
        free(post_data);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}