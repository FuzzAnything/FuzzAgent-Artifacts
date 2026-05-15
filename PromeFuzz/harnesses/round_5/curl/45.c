// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/header.h"
#include "curl/websockets.h"
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    size_t data_index = 0;
    
    // Fuzz curl_easy_setopt with various options
    for (int i = 0; i < 10 && data_index < Size; i++) {
        CURLoption option = (CURLoption)(Data[data_index++] % 30000);
        
        switch (option) {
            case CURLOPT_URL: {
                const char *url = "file://./dummy_file";
                curl_easy_setopt(curl, CURLOPT_URL, url);
                break;
            }
            case CURLOPT_CONNECT_ONLY: {
                long connect_only = Data[data_index++] % 3;
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, connect_only);
                break;
            }
            case CURLOPT_HEADERDATA:
            case CURLOPT_WRITEDATA: {
                FILE *dummy = fopen("./dummy_file", "rb");
                if (dummy) {
                    curl_easy_setopt(curl, option, dummy);
                    fclose(dummy);
                }
                break;
            }
            case CURLOPT_POSTFIELDS: {
                if (Size - data_index > 0) {
                    size_t len = Data[data_index++] % (Size - data_index + 1);
                    if (len > 0) {
                        char *postdata = (char*)malloc(len);
                        if (postdata) {
                            memcpy(postdata, Data + data_index, len);
                            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
                            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)len);
                            free(postdata);
                        }
                        data_index += len;
                    }
                }
                break;
            }
            default: {
                // Try setting as long
                if (data_index < Size) {
                    long val = (long)Data[data_index++];
                    curl_easy_setopt(curl, option, val);
                }
                break;
            }
        }
    }
    
    // Fuzz curl_easy_perform
    if (data_index < Size && (Data[data_index++] % 2)) {
        curl_easy_perform(curl);
    }
    
    // Fuzz curl_easy_header and curl_easy_nextheader
    if (data_index < Size) {
        unsigned int origin = Data[data_index++] % 8;
        int request = Data[data_index++] % 3;
        
        struct curl_header *header = NULL;
        struct curl_header *prev = NULL;
        
        // Try to get a header by name
        if (data_index + 1 < Size) {
            size_t name_len = Data[data_index++] % 16;
            if (name_len > 0 && data_index + name_len <= Size) {
                char *name = (char*)malloc(name_len + 1);
                if (name) {
                    memcpy(name, Data + data_index, name_len);
                    name[name_len] = '\0';
                    data_index += name_len;
                    
                    size_t index = Data[data_index++] % 4;
                    struct curl_header *hout = NULL;
                    curl_easy_header(curl, name, index, origin, request, &hout);
                    free(name);
                }
            }
        }
        
        // Iterate through headers
        for (int i = 0; i < 5 && data_index < Size; i++) {
            header = curl_easy_nextheader(curl, origin, request, prev);
            if (!header) break;
            prev = header;
            
            if (Data[data_index++] % 3 == 0) {
                break;
            }
        }
    }
    
    // Fuzz curl_easy_recv
    if (data_index < Size && (Data[data_index++] % 2)) {
        char buffer[1024];
        size_t n = 0;
        size_t buflen = Data[data_index++] % sizeof(buffer);
        if (buflen > 0) {
            curl_easy_recv(curl, buffer, buflen, &n);
        }
    }
    
    // Fuzz curl_ws_send
    if (data_index < Size && (Data[data_index++] % 2)) {
        char buffer[1024];
        size_t sent = 0;
        size_t buflen = 0;
        
        if (data_index < Size) {
            buflen = Data[data_index++] % sizeof(buffer);
            if (buflen > 0 && data_index + buflen <= Size) {
                memcpy(buffer, Data + data_index, buflen);
                data_index += buflen;
                
                curl_off_t fragsize = 0;
                if (data_index + sizeof(curl_off_t) <= Size) {
                    memcpy(&fragsize, Data + data_index, sizeof(curl_off_t));
                    data_index += sizeof(curl_off_t);
                }
                
                unsigned int flags = 0;
                if (data_index < Size) {
                    flags = Data[data_index++] % 256;
                }
                
                curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
            }
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}