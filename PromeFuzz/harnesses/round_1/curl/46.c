// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <unistd.h>

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        initialized = 1;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 0L);

    if (Size > 0) {
        CURLoption option = (CURLoption)(Data[0] % 300);
        long param = (long)(Size > 4 ? *(uint32_t*)(Data + 1) : Size);
        curl_easy_setopt(curl, option, param);
    }

    curl_easy_perform(curl);

    CURL *dup = curl_easy_duphandle(curl);
    if (dup) {
        curl_easy_setopt(dup, CURLOPT_URL, "file://./dummy_file");
        curl_easy_perform(dup);
        curl_easy_reset(dup);
        curl_easy_cleanup(dup);
    }

    if (Size > 8) {
        size_t sent = 0;
        const void *buffer = Data;
        size_t buflen = Size > 1024 ? 1024 : Size;
        curl_easy_send(curl, buffer, buflen, &sent);
    }

    curl_easy_reset(curl);
    curl_easy_cleanup(curl);

    unlink("./dummy_file");
    return 0;
}