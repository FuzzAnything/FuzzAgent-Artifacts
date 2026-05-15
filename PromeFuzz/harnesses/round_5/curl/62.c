// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl.h"
#include "easy.h"
#include "websockets.h"

static void dummy_write(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void dummy_cleanup(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    // Write data to dummy file for potential URL usage
    dummy_write(Data, Size);
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // 1. curl_easy_setopt with various options
    if (offset < Size) {
        CURLoption opt = (CURLoption)(Data[offset] % 300); // Approximate range
        offset++;
        
        // Handle different parameter types based on option
        switch (opt) {
            case CURLOPT_URL:
                if (offset < Size) {
                    char url[] = "file://./dummy_file";
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                }
                break;
            case CURLOPT_CONNECT_ONLY:
                if (offset < Size) {
                    long connect_only = Data[offset] % 3;
                    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, connect_only);
                    offset++;
                }
                break;
            case CURLOPT_WRITEDATA:
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
                break;
            case CURLOPT_VERBOSE:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
                break;
            default:
                // Try with long parameter
                if (offset < Size) {
                    long param = (long)Data[offset];
                    curl_easy_setopt(curl, opt, param);
                    offset++;
                }
                break;
        }
    }
    
    // 2. curl_easy_perform (may fail, but that's OK for fuzzing)
    if (selector & 0x01) {
        curl_easy_perform(curl);
    }
    
    // 3. curl_easy_getinfo with various info types
    if (offset < Size && (selector & 0x02)) {
        CURLINFO info = (CURLINFO)(Data[offset] % 0x40000000); // Approximate range
        offset++;
        
        // Prepare variables for different return types
        double dval;
        long lval;
        char *cval = NULL;
        curl_off_t oval;
        struct curl_slist *slist = NULL;
        
        switch (info) {
            case CURLINFO_TOTAL_TIME:
                curl_easy_getinfo(curl, info, &dval);
                break;
            case CURLINFO_RESPONSE_CODE:
                curl_easy_getinfo(curl, info, &lval);
                break;
            case CURLINFO_EFFECTIVE_URL:
                curl_easy_getinfo(curl, info, &cval);
                break;
            case CURLINFO_CONTENT_LENGTH_DOWNLOAD_T:
                curl_easy_getinfo(curl, info, &oval);
                break;
            case CURLINFO_SSL_VERIFYRESULT:
                curl_easy_getinfo(curl, info, &lval);
                break;
            default:
                // Try with long parameter
                curl_easy_getinfo(curl, info, &lval);
                break;
        }
    }
    
    // 4. curl_easy_send (requires CONNECT_ONLY connection)
    if (offset < Size && (selector & 0x04)) {
        size_t sent = 0;
        size_t send_len = (Size - offset) > 1024 ? 1024 : (Size - offset);
        curl_easy_send(curl, Data + offset, send_len, &sent);
        offset += (send_len < (Size - offset)) ? send_len : 0;
    }
    
    // 5. curl_ws_send (requires WebSocket connection)
    if (offset < Size && (selector & 0x08)) {
        size_t sent = 0;
        size_t send_len = (Size - offset) > 1024 ? 1024 : (Size - offset);
        unsigned int flags = (offset < Size) ? Data[offset] : 0;
        offset++;
        curl_off_t fragsize = (offset < Size) ? Data[offset] : 0;
        offset++;
        curl_ws_send(curl, Data + offset, send_len, &sent, fragsize, flags);
        offset += (send_len < (Size - offset)) ? send_len : 0;
    }
    
    // 6. curl_ws_recv (requires WebSocket connection)
    if (offset < Size && (selector & 0x10)) {
        void *buffer = malloc(1024);
        size_t recv = 0;
        const struct curl_ws_frame *meta = NULL;
        if (buffer) {
            curl_ws_recv(curl, buffer, 1024, &recv, &meta);
            free(buffer);
        }
    }
    
    curl_easy_cleanup(curl);
    dummy_cleanup();
    
    return 0;
}