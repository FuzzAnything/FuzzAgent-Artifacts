// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize basic options to avoid immediate failures */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    /* Create MIME structure for mime_data testing */
    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = NULL;
    if (mime) {
        part = curl_mime_addpart(mime);
    }

    /* Buffer for recv/send operations */
    char buffer[4096];
    size_t n = 0;
    size_t recv_len = 0;
    const struct curl_ws_frame *meta = NULL;

    /* Parse input data to drive fuzzing logic */
    size_t idx = 0;
    uint8_t op = (Size > 0) ? Data[idx++] : 0;

    switch (op % 7) {
        case 0: {
            /* Test curl_easy_upkeep */
            curl_easy_upkeep(curl);
            break;
        }
        case 1: {
            /* Test curl_ws_recv */
            size_t len = (Size > idx) ? (size_t)Data[idx++] : 0;
            if (len > sizeof(buffer)) len = sizeof(buffer);
            curl_ws_recv(curl, buffer, len, &recv_len, &meta);
            break;
        }
        case 2: {
            /* Test curl_easy_send */
            size_t len = (Size > idx) ? (size_t)Data[idx++] : 0;
            if (len > sizeof(buffer)) len = sizeof(buffer);
            if (idx + len > Size) len = Size - idx;
            curl_easy_send(curl, Data + idx, len, &n);
            break;
        }
        case 3: {
            /* Test curl_mime_data */
            if (part) {
                size_t len = (Size > idx) ? (size_t)Data[idx++] : 0;
                if (len > Size - idx) len = Size - idx;
                curl_mime_data(part, (const char *)(Data + idx), len);
            }
            break;
        }
        case 4: {
            /* Test curl_easy_recv */
            size_t len = (Size > idx) ? (size_t)Data[idx++] : 0;
            if (len > sizeof(buffer)) len = sizeof(buffer);
            curl_easy_recv(curl, buffer, len, &n);
            break;
        }
        case 5: {
            /* Test curl_easy_ssls_import */
            size_t key_len = (Size > idx) ? (size_t)Data[idx++] : 0;
            size_t hmac_len = (Size > idx) ? (size_t)Data[idx++] : 0;
            size_t data_len = (Size > idx) ? (size_t)Data[idx++] : 0;
            
            const char *key = (key_len > 0 && idx < Size) ? (const char *)(Data + idx) : "";
            idx += key_len;
            if (idx > Size) idx = Size;

            const unsigned char *hmac = (hmac_len > 0 && idx < Size) ? Data + idx : NULL;
            idx += hmac_len;
            if (idx > Size) idx = Size;

            const unsigned char *sdata = (data_len > 0 && idx < Size) ? Data + idx : NULL;
            
            curl_easy_ssls_import(curl, key, hmac, hmac_len, sdata, data_len);
            break;
        }
        case 6: {
            /* Combined operations for state exploration */
            curl_easy_upkeep(curl);
            if (part) {
                size_t len = (Size > idx) ? (size_t)Data[idx++] : 0;
                if (len > Size - idx) len = Size - idx;
                curl_mime_data(part, (const char *)(Data + idx), len);
            }
            size_t recv_len_local = 0;
            curl_ws_recv(curl, buffer, 100, &recv_len_local, &meta);
            curl_easy_recv(curl, buffer, 100, &n);
            break;
        }
    }

    /* Cleanup */
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);

    return 0;
}