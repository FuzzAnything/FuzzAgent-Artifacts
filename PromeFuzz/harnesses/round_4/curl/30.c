// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
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
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        remove_dummy_file();
        return 0;
    }
    
    char url[256];
    snprintf(url, sizeof(url), "file://./dummy_file");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (Size & 1) ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, (Size & 2) ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, (Size & 4) ? 1L : 0L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK && (Size & 1)) {
        char buffer[1024];
        size_t n = 0;
        curl_easy_recv(curl, buffer, sizeof(buffer), &n);
    }
    
    CURL *dup = curl_easy_duphandle(curl);
    if (dup) {
        curl_easy_reset(dup);
        curl_easy_cleanup(dup);
    }
    
    curl_easy_reset(curl);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);
    remove_dummy_file();
    
    return 0;
}