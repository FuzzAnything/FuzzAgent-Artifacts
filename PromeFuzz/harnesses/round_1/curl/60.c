// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    char url[256];
    snprintf(url, sizeof(url), "file://./dummy_file");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        char buffer[1024];
        size_t n = 0;
        const struct curl_ws_frame *meta = NULL;
        
        curl_ws_recv(curl, buffer, sizeof(buffer), &n, &meta);
        curl_easy_recv(curl, buffer, sizeof(buffer), &n);
        
        size_t sent = 0;
        curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &sent);
    }
    
    long http_code = 0;
    double total_time = 0.0;
    char *effective_url = NULL;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}