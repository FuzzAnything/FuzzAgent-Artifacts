// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Fuzz driver for curl API functions */

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize curl handle with basic options */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

    /* Prepare buffers and variables */
    char recv_buf[256];
    char send_buf[256];
    size_t n_recv = 0;
    size_t n_send = 0;
    struct curl_ws_frame *ws_meta = NULL;
    curl_mimepart *part = NULL;
    curl_mime *mime = NULL;

    /* Attempt to initialize MIME structures if data is sufficient */
    if (Size > 10) {
        mime = curl_mime_init(curl);
        if (mime) {
            part = curl_mime_addpart(mime);
            if (part) {
                /* Fuzz curl_mime_data */
                size_t datasize = (Size > 100) ? 100 : Size - 10;
                CURLcode mime_res = curl_mime_data(part, (const char *)(Data + 10), datasize);
                (void)mime_res;
            }
        }
    }

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Fuzz curl_easy_recv */
    curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n_recv);

    /* Fuzz curl_easy_send */
    size_t send_len = (Size > 256) ? 256 : Size;
    memcpy(send_buf, Data, send_len);
    curl_easy_send(curl, send_buf, send_len, &n_send);

    /* Fuzz curl_ws_recv */
    curl_ws_recv(curl, recv_buf, sizeof(recv_buf), &n_recv, &ws_meta);

    /* Fuzz curl_easy_ssls_import if data is sufficient */
    if (Size > 20) {
        const char *session_key = (const char *)(Data);
        const unsigned char *shmac = Data + 10;
        size_t shmac_len = (Size > 15) ? 5 : Size - 10;
        const unsigned char *sdata = Data + 15;
        size_t sdata_len = (Size > 20) ? Size - 15 : 0;

        curl_easy_ssls_import(curl, session_key, shmac, shmac_len, sdata, sdata_len);
    }

    /* Cleanup */
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);

    return 0;
}