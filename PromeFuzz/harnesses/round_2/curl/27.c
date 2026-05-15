// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "curl/curl.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }

    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    size_t remaining = Size > 1 ? Size - 1 : 0;

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    if (remaining > 0 && (selector == 0 || selector == 1 || selector == 3 || selector == 5)) {
        curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        
        curl_easy_perform(curl);
    }

    size_t buffer_size = remaining > 1024 ? 1024 : remaining;
    void *buffer = NULL;
    if (buffer_size > 0 && data_offset < Size) {
        buffer = malloc(buffer_size);
        if (buffer) {
            size_t copy_size = Size - data_offset > buffer_size ? buffer_size : Size - data_offset;
            memcpy(buffer, Data + data_offset, copy_size);
        }
    }

    size_t sent_received = 0;
    CURLcode res = CURLE_OK;

    switch (selector) {
        case 0:
            if (buffer) {
                res = curl_easy_send(curl, buffer, buffer_size, &sent_received);
            }
            break;
            
        case 1:
            if (buffer) {
                const struct curl_ws_frame *meta = NULL;
                res = curl_ws_recv(curl, buffer, buffer_size, &sent_received, &meta);
            }
            break;
            
        case 2:
            if (remaining >= 2) {
                uint8_t opt_selector = Data[data_offset] % 10;
                switch (opt_selector) {
                    case 0:
                        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                        break;
                    case 1:
                        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
                        break;
                    case 2:
                        curl_easy_setopt(curl, CURLOPT_USERAGENT, "fuzzer/1.0");
                        break;
                    case 3:
                        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                        break;
                    case 4:
                        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                        break;
                    case 5:
                        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
                        break;
                    case 6:
                        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3L);
                        break;
                    case 7:
                        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
                        break;
                    case 8:
                        curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
                        break;
                    case 9:
                        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
                        break;
                }
            }
            break;
            
        case 3:
            if (buffer) {
                res = curl_easy_recv(curl, buffer, buffer_size, &sent_received);
            }
            break;
            
        case 4:
            {
                const struct curl_ws_frame *meta = curl_ws_meta(curl);
                (void)meta;
            }
            break;
            
        case 5:
            if (buffer) {
                unsigned int flags = 0;
                if (remaining >= 2) {
                    flags = Data[data_offset] & 0x0F;
                }
                res = curl_ws_send(curl, buffer, buffer_size, &sent_received, 0, flags);
            }
            break;
    }

    if (buffer) {
        free(buffer);
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}