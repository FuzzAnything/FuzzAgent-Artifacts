// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Prepare a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Set a basic URL to allow curl_easy_perform to succeed in a non-CONNECT_ONLY mode
    // We will also try CONNECT_ONLY mode for recv/send/ws functions
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // Test curl_easy_perform
    curl_easy_perform(curl);

    // Test curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Test curl_easy_ssls_import with various inputs from fuzz data
    if (Size >= 16) {
        const char *session_key = (const char *)(Data);
        const unsigned char *shmac = Data + 8;
        size_t shmac_len = Size > 16 ? Size - 16 : 0;
        const unsigned char *sdata = Data + 8 + shmac_len;
        size_t sdata_len = Size > 8 + shmac_len ? Size - (8 + shmac_len) : 0;
        curl_easy_ssls_import(curl, session_key, shmac, shmac_len, sdata, sdata_len);
    }

    // Prepare for CONNECT_ONLY mode tests
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_perform(curl);

    // Test curl_easy_send
    if (Size > 0) {
        size_t n_sent = 0;
        curl_easy_send(curl, Data, Size, &n_sent);
    }

    // Test curl_easy_recv
    if (Size > 0) {
        char recv_buf[256];
        size_t n_recv = 0;
        curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n_recv);
    }

    // Test curl_ws_recv
    if (Size > 0) {
        char ws_buf[256];
        size_t n_ws = 0;
        struct curl_ws_frame *meta = NULL;
        curl_ws_recv(curl, ws_buf, sizeof(ws_buf), &n_ws, &meta);
        if (meta) {
            // meta is managed by curl, do not free
        }
    }

    // Cleanup
    curl_easy_cleanup(curl);
    return 0;
}