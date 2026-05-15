// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test curl_easy_init
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Test curl_easy_setopt with various options
    // Use fuzzer data to determine which options to set
    uint8_t opt_selector = Data[0] % 8;
    
    switch (opt_selector) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/test");
            break;
        case 1:
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            break;
        case 2:
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            break;
        case 3:
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            break;
        case 4:
            curl_easy_setopt(curl, CURLOPT_READDATA, fopen("./dummy_file", "rb"));
            break;
        case 5:
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
            break;
        case 6:
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            break;
        case 7:
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            break;
    }
    
    // Test curl_easy_perform
    CURLcode res = curl_easy_perform(curl);
    
    // Test curl_easy_recv if CONNECT_ONLY was set
    if (opt_selector == 3 && res == CURLE_OK) {
        char buffer[1024];
        size_t n;
        curl_easy_recv(curl, buffer, sizeof(buffer), &n);
    }
    
    // Test curl_easy_duphandle
    CURL *dup_curl = curl_easy_duphandle(curl);
    if (dup_curl) {
        curl_easy_setopt(dup_curl, CURLOPT_URL, "http://localhost/duplicate");
        curl_easy_perform(dup_curl);
        curl_easy_cleanup(dup_curl);
    }
    
    // Test curl_easy_reset
    curl_easy_reset(curl);
    
    // Use the reset handle again
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/reset");
    curl_easy_perform(curl);
    
    // Cleanup
    curl_easy_cleanup(curl);
    
    return 0;
}