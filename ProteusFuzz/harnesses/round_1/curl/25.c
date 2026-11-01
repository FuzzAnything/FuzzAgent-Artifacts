// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

static void cleanup_mime(curl_mime *mime) {
    if (mime) {
        curl_mime_free(mime);
    }
}

static void cleanup_handle(CURL *handle) {
    if (handle) {
        curl_easy_cleanup(handle);
    }
}

static size_t safe_read_size_t(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + sizeof(size_t)) {
        return 0;
    }
    size_t val;
    memcpy(&val, Data + offset, sizeof(size_t));
    return val;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        cleanup_handle(curl);
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        cleanup_mime(mime);
        cleanup_handle(curl);
        return 0;
    }

    // Fuzz curl_mime_data
    // Ensure we have enough data for size_t + some content
    if (Size >= sizeof(size_t) + 1) {
        size_t datasize = safe_read_size_t(Data, Size, 0);
        // Limit size to prevent allocation failures or excessive memory usage
        if (datasize > Size - sizeof(size_t)) {
            datasize = Size - sizeof(size_t);
        }
        if (datasize > 1024 * 1024) {
            datasize = 1024 * 1024;
        }
        const char *data_ptr = (const char *)(Data + sizeof(size_t));
        curl_mime_data(part, data_ptr, datasize);
    }

    // Fuzz curl_easy_ssls_import
    // We need session_key, shmac, shmac_len, sdata, sdata_len
    if (Size >= 4) {
        // Use a simple key length based on available data
        size_t key_len = 10;
        if (Size < key_len + 4) key_len = Size - 4;
        if (key_len > 0) {
            char *key_buf = malloc(key_len + 1);
            if (key_buf) {
                memcpy(key_buf, Data, key_len);
                key_buf[key_len] = '\0';
                
                size_t offset = key_len;
                if (Size > offset + 4) {
                    size_t shmac_len = safe_read_size_t(Data, Size, offset);
                    if (shmac_len > Size - offset - 4) shmac_len = Size - offset - 4;
                    if (shmac_len > 16) shmac_len = 16;
                    
                    const unsigned char *shmac = Data + offset + 4;
                    size_t sdata_offset = offset + 4 + shmac_len;
                    
                    size_t sdata_len = 4;
                    if (Size > sdata_offset + 4) {
                        sdata_len = safe_read_size_t(Data, Size, sdata_offset);
                    }
                    if (sdata_len > Size - sdata_offset - 4) sdata_len = Size - sdata_offset - 4;
                    if (sdata_len > 1024) sdata_len = 1024;
                    
                    const unsigned char *sdata = Data + sdata_offset + 4;
                    
                    curl_easy_ssls_import(curl, key_buf, shmac, shmac_len, sdata, sdata_len);
                }
                free(key_buf);
            }
        }
    }

    // Fuzz curl_easy_upkeep
    curl_easy_upkeep(curl);

    // Prepare for curl_easy_recv, curl_easy_send, curl_ws_recv
    size_t n = 0;
    char recv_buf[256];
    char send_buf[256];
    size_t recv_n = 0;
    size_t send_n = 0;
    struct curl_ws_frame *meta = NULL;

    // Fuzz curl_easy_recv
    if (Size >= sizeof(size_t)) {
        size_t buflen = safe_read_size_t(Data, Size, 0);
        if (buflen > 256) buflen = 256;
        if (buflen > 0) {
            curl_easy_recv(curl, recv_buf, buflen, &n);
        }
    }

    // Fuzz curl_easy_send
    if (Size >= sizeof(size_t)) {
        size_t buflen = safe_read_size_t(Data, Size, 0);
        if (buflen > 256) buflen = 256;
        if (buflen > 0) {
            curl_easy_send(curl, send_buf, buflen, &n);
        }
    }

    // Fuzz curl_ws_recv
    if (Size >= sizeof(size_t)) {
        size_t buflen = safe_read_size_t(Data, Size, 0);
        if (buflen > 256) buflen = 256;
        if (buflen > 0) {
            curl_ws_recv(curl, recv_buf, buflen, &recv_n, &meta);
        }
    }

    cleanup_mime(mime);
    cleanup_handle(curl);

    return 0;
}