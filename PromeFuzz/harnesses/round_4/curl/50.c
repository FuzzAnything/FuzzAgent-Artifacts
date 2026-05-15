// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "curl/curl.h"
#include "curl/header.h"
#include "curl/websockets.h"
#include "curl/easy.h"

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

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    unsigned int origin = Data[0] & 3;
    int request = Data[0] & 1;
    size_t index = Data[0] % 8;
    
    char *name_buf = malloc(16);
    if (name_buf) {
        snprintf(name_buf, 16, "Header%d", Data[0] % 10);
    }
    
    struct curl_header *hout = NULL;
    CURLHcode hcode = curl_easy_header(curl, name_buf, index, origin, request, &hout);
    
    if (hcode == CURLHE_OK && hout) {
        struct curl_header *next = NULL;
        do {
            next = curl_easy_nextheader(curl, origin, request, next ? next : hout);
        } while (next);
    }
    
    free(name_buf);
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    
    if (Size > 10) {
        long opt = (Data[1] % 50) + 1;
        long val = Data[2];
        curl_easy_setopt(curl, (CURLoption)opt, val);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK || res == CURLE_UNSUPPORTED_PROTOCOL) {
        char recv_buf[256];
        size_t n = 0;
        curl_easy_recv(curl, recv_buf, sizeof(recv_buf), &n);
        
        if (Size > 20) {
            size_t sent = 0;
            curl_off_t fragsize = (Data[3] << 8) | Data[4];
            unsigned int flags = Data[5] & 0xFF;
            curl_ws_send(curl, Data + 6, split > 6 ? split - 6 : 1, &sent, fragsize, flags);
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}