// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Write fuzz data to a dummy file for potential file-based operations
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }

    // Test curl_easy_setopt with various options
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)Size);
    }

    // Test curl_easy_perform (will fail without actual connection)
    CURLcode perform_result = curl_easy_perform(curl);

    // Test curl_easy_strerror with the result
    const char *error_str = curl_easy_strerror(perform_result);

    // Test curl_easy_getinfo with various info types
    long response_code;
    double total_time;
    char *effective_url = NULL;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);

    // Test curl_easy_send if we have data
    if (Size > 0) {
        size_t bytes_sent = 0;
        curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &bytes_sent);
    }

    // Test curl_ws_recv if we have data
    if (Size > 0) {
        char buffer[1024];
        size_t recv_len = 0;
        const struct curl_ws_frame *meta = NULL;
        curl_ws_recv(curl, buffer, sizeof(buffer), &recv_len, &meta);
    }

    // Clean up
    curl_easy_cleanup(curl);
    
    return 0;
}