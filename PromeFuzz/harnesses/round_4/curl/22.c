// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    size_t total_size = size * nitems;
    size_t *offset = (size_t *)userdata;
    size_t available = 1024 - *offset;
    size_t to_copy = total_size < available ? total_size : available;
    
    if (to_copy > 0) {
        memset(buffer, 'A' + (*offset % 26), to_copy);
        *offset += to_copy;
    }
    return to_copy;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    char url[256];
    snprintf(url, sizeof(url), "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    unsigned char test_byte = Data[0];
    size_t data_offset = 1;
    size_t remaining = Size > 1 ? Size - 1 : 0;
    
    switch (test_byte % 6) {
        case 0: {
            if (remaining > 0) {
                char *buffer = malloc(remaining);
                if (buffer) {
                    memcpy(buffer, Data + data_offset, remaining);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, remaining);
                }
            }
            curl_easy_perform(curl);
            break;
        }
        case 1: {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && remaining > 0) {
                char *send_buffer = malloc(remaining);
                size_t sent = 0;
                if (send_buffer) {
                    memcpy(send_buffer, Data + data_offset, remaining);
                    curl_easy_send(curl, send_buffer, remaining, &sent);
                    free(send_buffer);
                }
            }
            break;
        }
        case 2: {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && remaining > 0) {
                char *recv_buffer = malloc(remaining);
                size_t received = 0;
                if (recv_buffer) {
                    curl_easy_recv(curl, recv_buffer, remaining, &received);
                    free(recv_buffer);
                }
            }
            break;
        }
        case 3: {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && remaining > 0) {
                char *ws_buffer = malloc(remaining);
                size_t sent = 0;
                if (ws_buffer) {
                    memcpy(ws_buffer, Data + data_offset, remaining);
                    curl_ws_send(curl, ws_buffer, remaining, &sent, 0, CURLWS_BINARY);
                    free(ws_buffer);
                }
            }
            break;
        }
        case 4: {
            size_t read_offset = 0;
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            curl_easy_setopt(curl, CURLOPT_READDATA, &read_offset);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            
            if (remaining > 0) {
                curl_off_t frame_len = (curl_off_t)(Data[data_offset] % 256);
                unsigned int flags = CURLWS_BINARY;
                if (remaining > 1) {
                    flags = Data[data_offset + 1] % 256;
                }
                curl_ws_start_frame(curl, flags, frame_len);
            }
            break;
        }
        case 5: {
            int bitmask = 0;
            if (remaining > 0) {
                bitmask = Data[data_offset] % 4;
                if (bitmask == 0) bitmask = CURLPAUSE_RECV;
                else if (bitmask == 1) bitmask = CURLPAUSE_SEND;
                else if (bitmask == 2) bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND;
                else bitmask = CURLPAUSE_CONT;
            }
            curl_easy_pause(curl, bitmask);
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    unlink("./dummy_file");
    
    return 0;
}