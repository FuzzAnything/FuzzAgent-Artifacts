// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/* Include curl.h first to get CURL_EXTERN definition */
#include "curl/curl.h"
/* Then include the other headers */
#include "curl/easy.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    /* Test curl_easy_setopt with various options */
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    /* Test curl_easy_perform */
    CURLcode res = curl_easy_perform(curl);
    
    /* Test curl_easy_getinfo with various info types */
    if (res == CURLE_OK || res == CURLE_OPERATION_TIMEDOUT) {
        long response_code;
        double total_time;
        char *effective_url = NULL;
        curl_off_t upload_size;
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_size);
    }
    
    /* Test curl_easy_send and curl_easy_recv for CONNECT_ONLY connections */
    if (Size > 10) {
        size_t sent = 0;
        size_t received = 0;
        char buffer[256];
        
        curl_easy_send(curl, Data, Size > 256 ? 256 : Size, &sent);
        curl_easy_recv(curl, buffer, sizeof(buffer), &received);
    }
    
    /* Test curl_ws_recv if WebSocket support is available */
    if (Size > 20) {
        char ws_buffer[128];
        size_t ws_received = 0;
        const struct curl_ws_frame *meta = NULL;
        
        curl_ws_recv(curl, ws_buffer, sizeof(ws_buffer), &ws_received, &meta);
    }
    
    /* Clean up */
    curl_easy_cleanup(curl);
    remove_dummy_file();
    
    return 0;
}