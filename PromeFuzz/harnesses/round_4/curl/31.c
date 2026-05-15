// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
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
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    write_dummy_file(Data, Size);

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    CURLcode res;
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    
    size_t opt_idx = Data[0] % 6;
    switch (opt_idx) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[0] & 1));
            break;
        case 1:
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, (long)(Data[0] & 1));
            break;
        case 2:
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (long)(Data[0] & 1));
            break;
        case 3:
            curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, (long)(Data[0] & 1));
            break;
        case 4:
            if (Size > 1) {
                char *user_agent = (char *)malloc(Size);
                if (user_agent) {
                    memcpy(user_agent, Data, Size);
                    user_agent[Size-1] = '\0';
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
                    free(user_agent);
                }
            }
            break;
        case 5:
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(Data[0]));
            break;
    }

    res = curl_easy_perform(curl);
    (void)res;

    if (opt_idx == 2 && (Data[0] & 1)) {
        char buffer[1024];
        size_t n = 0;
        curl_easy_recv(curl, buffer, sizeof(buffer), &n);
    }

    CURL *dup_curl = curl_easy_duphandle(curl);
    if (dup_curl) {
        curl_easy_reset(dup_curl);
        curl_easy_setopt(dup_curl, CURLOPT_URL, "file://./dummy_file");
        curl_easy_perform(dup_curl);
        curl_easy_cleanup(dup_curl);
    }

    curl_easy_reset(curl);
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    unlink("./dummy_file");

    return 0;
}