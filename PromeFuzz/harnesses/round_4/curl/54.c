// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    curl_easy_perform(curl);

    size_t buffer_size = Size > 1024 ? 1024 : Size;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }

    size_t bytes_processed = 0;
    const struct curl_ws_frame *meta = NULL;

    uint8_t selector = Data[0] % 6;
    switch (selector) {
        case 0:
            curl_ws_recv(curl, buffer, buffer_size, &bytes_processed, &meta);
            break;
        case 1:
            curl_easy_recv(curl, buffer, buffer_size, &bytes_processed);
            break;
        case 2:
            curl_easy_pause(curl, Data[0] & 3);
            break;
        case 3:
            curl_ws_send(curl, buffer, buffer_size, &bytes_processed,
                        (curl_off_t)buffer_size, Data[0] & 0xF);
            break;
        case 4:
            curl_ws_meta(curl);
            break;
        case 5:
            curl_easy_send(curl, buffer, buffer_size, &bytes_processed);
            break;
    }

    free(buffer);
    curl_easy_cleanup(curl);
    unlink("./dummy_file");
    return 0;
}