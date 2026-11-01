// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
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

/* Callback for curl_easy_ssls_export matching the expected signature */
static CURLcode ssls_export_callback(void *userptr, void *session_handle,
                                     const char *protocol,
                                     const unsigned char *session_id,
                                     size_t session_id_len,
                                     const unsigned char *session,
                                     size_t session_len,
                                     long session_age,
                                     int is_resumable,
                                     const char *cipher_name,
                                     size_t cipher_name_len) {
    (void)userptr;
    (void)session_handle;
    (void)protocol;
    (void)session_id;
    (void)session_id_len;
    (void)session;
    (void)session_len;
    (void)session_age;
    (void)is_resumable;
    (void)cipher_name;
    (void)cipher_name_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    size_t sent = 0;
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    curl_off_t fragsize = 0;

    if (Size < 4) {
        return 0;
    }

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

    /* Extract parameters from fuzz input */
    flags = Data[0];
    frame_len = (curl_off_t)(Data[1] | (Data[2] << 8) | (Data[3] << 16));
    fragsize = (curl_off_t)(Data[3] | (Data[2] << 8) | (Data[1] << 16));

    /* Set some basic options to make the handle valid for testing */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Fuzz curl_easy_ssls_export */
    res = curl_easy_ssls_export(curl, ssls_export_callback, NULL);
    (void)res;

    /* Fuzz curl_ws_start_frame */
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    /* Fuzz curl_ws_send */
    if (Size > 4) {
        res = curl_ws_send(curl, (const void *)(Data + 4), Size - 4, &sent, fragsize, flags);
        (void)res;
    }

    /* Fuzz curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res;

    /* Fuzz curl_easy_perform */
    res = curl_easy_perform(curl);
    (void)res;

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}