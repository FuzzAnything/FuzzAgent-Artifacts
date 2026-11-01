// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Initialize basic options to ensure a valid handle state for testing
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:0");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0L);

    // Attempt to perform a connection (likely to fail due to localhost:0, but handle is valid)
    (void)curl_easy_perform(curl);

    // Test curl_easy_upkeep
    (void)curl_easy_upkeep(curl);

    // Test curl_easy_recv
    char recv_buf[256];
    size_t recv_n = 0;
    (void)curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &recv_n);

    // Test curl_easy_send
    size_t send_n = 0;
    // Use input data as send buffer if size is reasonable, otherwise use static data
    const char *send_data = (Size > 0 && Size < 1024) ? (const char *)Data : "test";
    size_t send_len = (Size > 0 && Size < 1024) ? Size : 4;
    (void)curl_easy_send(curl, send_data, send_len, &send_n);

    // Test curl_ws_recv
    char ws_buf[256];
    size_t ws_recv = 0;
    struct curl_ws_frame *ws_meta = NULL;
    (void)curl_ws_recv(curl, ws_buf, sizeof(ws_buf), &ws_recv, &ws_meta);

    // Test curl_mime_data
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input data for mime content
            if (Size > 0) {
                (void)curl_mime_data(part, (const char *)Data, Size);
            } else {
                (void)curl_mime_data(part, "empty", 5);
            }
        }
        curl_mime_free(mime);
    }

    // Test curl_easy_ssls_import
    // We use dummy data for session key, hmac, and sdata
    const char *session_key = (Size > 10) ? (const char *)Data : "dummy_key";
    const unsigned char *shmac = (Size > 20) ? Data + 10 : (const unsigned char *)"dummy_hmac";
    size_t shmac_len = (Size > 20) ? 10 : 10;
    const unsigned char *sdata = (Size > 30) ? Data + 20 : (const unsigned char *)"dummy_sdata";
    size_t sdata_len = (Size > 30) ? (Size - 30) : 10;
    (void)curl_easy_ssls_import(curl, session_key, shmac, shmac_len, sdata, sdata_len);

    // Cleanup
    curl_easy_cleanup(curl);

    return 0;
}