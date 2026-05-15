// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
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
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Use first byte to determine fuzzing path
    uint8_t path = Data[0];
    size_t offset = 1;
    
    // Set various options based on fuzzer input
    if (Size > offset + 1) {
        // Set URL from input data
        size_t url_len = Data[offset] % (Size - offset);
        if (url_len > 0) {
            char *url = malloc(url_len + 1);
            if (url) {
                memcpy(url, Data + offset, url_len);
                url[url_len] = '\0';
                curl_easy_setopt(curl, CURLOPT_URL, url);
                free(url);
            }
            offset += url_len;
        }
    }
    
    // Set boolean options
    if (Size > offset) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[offset] & 1));
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, (long)((Data[offset] >> 1) & 1));
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, (long)((Data[offset] >> 2) & 1));
        offset++;
    }
    
    // Set connection-only mode for send testing
    if (Size > offset) {
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, (long)(Data[offset] & 1));
        offset++;
    }
    
    // Write dummy file for possible file upload
    if (Size > offset) {
        size_t file_size = (Size - offset) % 1024;
        write_dummy_file(Data + offset, file_size);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        FILE *file = fopen("./dummy_file", "rb");
        if (file) {
            curl_easy_setopt(curl, CURLOPT_READDATA, file);
        }
        offset += file_size;
    }
    
    // Perform the transfer
    if (path % 3 == 0) {
        curl_easy_perform(curl);
    }
    
    // Test send if in connect-only mode
    if (path % 5 == 0 && Size > offset) {
        size_t send_size = (Size - offset) % 256;
        size_t sent = 0;
        if (send_size > 0) {
            curl_easy_send(curl, Data + offset, send_size, &sent);
        }
        offset += send_size;
    }
    
    // Test duplicate handle
    if (path % 2 == 0) {
        CURL *dup = curl_easy_duphandle(curl);
        if (dup) {
            if (Size > offset) {
                curl_easy_setopt(dup, CURLOPT_VERBOSE, (long)(Data[offset] & 1));
                offset++;
            }
            curl_easy_perform(dup);
            curl_easy_cleanup(dup);
        }
    }
    
    // Test reset
    if (path % 7 == 0) {
        curl_easy_reset(curl);
        // Set a simple option after reset
        if (Size > offset) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[offset] & 1));
            offset++;
        }
        curl_easy_perform(curl);
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}