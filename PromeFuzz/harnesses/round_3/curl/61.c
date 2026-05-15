// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        } else {
            const char dummy[] = "dummy data";
            fwrite(dummy, 1, sizeof(dummy) - 1, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Write fuzzer input to dummy file for potential file-based operations */
    write_dummy_file(Data, Size);

    /* Test curl_easy_setopt with various options */
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    
    if (Size > 0) {
        /* Use first byte to determine some options */
        uint8_t opt_selector = Data[0];
        
        if (opt_selector & 0x01) {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        }
        if (opt_selector & 0x02) {
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        }
        if (opt_selector & 0x04) {
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
        }
    }

    /* Test curl_easy_perform */
    CURLcode res = curl_easy_perform(curl);

    /* Test curl_easy_strerror on both success and failure */
    const char *error_str = curl_easy_strerror(res);
    (void)error_str; /* Avoid unused variable warning */

    /* Test curl_easy_getinfo with various info types */
    if (res == CURLE_OK || res == CURLE_PARTIAL_FILE) {
        long http_code = 0;
        double total_time = 0.0;
        curl_off_t upload_size = 0;
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_size);
    }

    /* Test curl_easy_recv if CONNECT_ONLY was set */
    if (Size > 0 && (Data[0] & 0x01)) {
        char recv_buffer[1024];
        size_t n = 0;
        CURLcode recv_res = curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &n);
        
        /* Test curl_easy_strerror on recv result */
        if (recv_res != CURLE_OK) {
            const char *recv_error = curl_easy_strerror(recv_res);
            (void)recv_error;
        }
    }

    /* Test curl_ws_recv if CONNECT_ONLY was set and we have WebSocket support */
    if (Size > 0 && (Data[0] & 0x01)) {
        char ws_buffer[1024];
        size_t ws_recv = 0;
        const struct curl_ws_frame *meta = NULL;
        CURLcode ws_res = curl_ws_recv(curl, ws_buffer, sizeof(ws_buffer), &ws_recv, &meta);
        
        /* Test curl_easy_strerror on WebSocket result */
        if (ws_res != CURLE_OK && ws_res != CURLE_AGAIN) {
            const char *ws_error = curl_easy_strerror(ws_res);
            (void)ws_error;
        }
    }

    /* Clean up */
    curl_easy_cleanup(curl);
    
    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}