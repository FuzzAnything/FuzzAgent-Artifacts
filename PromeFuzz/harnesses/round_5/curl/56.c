// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use fuzzer data to determine which operations to perform
    uint8_t op_selector = Data[0];
    size_t data_offset = 1;
    
    // Set various options using fuzzer data
    if (data_offset < Size) {
        long opt_val = (long)Data[data_offset % Size];
        curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_val);
        data_offset++;
    }
    
    if (data_offset < Size) {
        long opt_val = (long)Data[data_offset % Size];
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, opt_val);
        data_offset++;
    }
    
    // Set URL to dummy file
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    
    // Try to perform the transfer
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Try to get info after perform
    if (data_offset < Size) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        
        double total_time = 0.0;
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        
        const char *effective_url = NULL;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        data_offset++;
    }
    
    // Try CONNECT_ONLY operations based on fuzzer input
    if ((op_selector & 0x1) && data_offset < Size) {
        // Set CONNECT_ONLY mode
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
        
        // Perform again (will likely fail without real connection, but we try)
        curl_easy_perform(curl);
        
        // Try easy_recv
        if (data_offset + 4 < Size) {
            char recv_buf[256];
            size_t n = 0;
            size_t buflen = Data[data_offset] % sizeof(recv_buf);
            curl_easy_recv(curl, recv_buf, buflen, &n);
            data_offset++;
        }
        
        // Try ws_recv
        if (data_offset + 4 < Size) {
            char ws_buf[256];
            size_t recv = 0;
            const struct curl_ws_frame *meta = NULL;
            size_t buflen = Data[data_offset] % sizeof(ws_buf);
            curl_ws_recv(curl, ws_buf, buflen, &recv, &meta);
            data_offset++;
        }
        
        // Try ws_send
        if (data_offset + 8 < Size) {
            char send_buf[256];
            size_t sent = 0;
            size_t buflen = Data[data_offset] % sizeof(send_buf);
            data_offset++;
            curl_off_t fragsize = (curl_off_t)Data[data_offset];
            data_offset++;
            unsigned int flags = Data[data_offset] & 0xFF;
            data_offset++;
            
            // Use some fuzzer data as send buffer if available
            size_t copy_len = (Size - data_offset) < buflen ? (Size - data_offset) : buflen;
            if (copy_len > 0) {
                memcpy(send_buf, Data + data_offset, copy_len);
                curl_ws_send(curl, send_buf, copy_len, &sent, fragsize, flags);
            }
        }
    }
    
    // Try more getinfo calls with different info types
    if (data_offset < Size) {
        CURLINFO info_types[] = {
            CURLINFO_CONTENT_TYPE,
            CURLINFO_PRIVATE,
            CURLINFO_HTTP_CONNECTCODE,
            CURLINFO_FILETIME,
            CURLINFO_REDIRECT_COUNT
        };
        
        uint8_t info_idx = Data[data_offset] % (sizeof(info_types)/sizeof(info_types[0]));
        CURLINFO selected_info = info_types[info_idx];
        
        // Use appropriate type for each info
        if (selected_info == CURLINFO_CONTENT_TYPE) {
            const char *content_type = NULL;
            curl_easy_getinfo(curl, selected_info, &content_type);
        } else if (selected_info == CURLINFO_FILETIME) {
            long filetime = 0;
            curl_easy_getinfo(curl, selected_info, &filetime);
        } else if (selected_info == CURLINFO_REDIRECT_COUNT) {
            long redirect_count = 0;
            curl_easy_getinfo(curl, selected_info, &redirect_count);
        } else {
            // Generic pointer for other types
            void *info_ptr = NULL;
            curl_easy_getinfo(curl, selected_info, &info_ptr);
        }
        
        data_offset++;
    }
    
    // Try setting more options with fuzzer data
    if (data_offset < Size) {
        CURLoption opt_types[] = {
            CURLOPT_FOLLOWLOCATION,
            CURLOPT_MAXREDIRS,
            CURLOPT_TIMEOUT,
            CURLOPT_CONNECTTIMEOUT
        };
        
        uint8_t opt_idx = Data[data_offset] % (sizeof(opt_types)/sizeof(opt_types[0]));
        CURLoption selected_opt = opt_types[opt_idx];
        data_offset++;
        
        if (data_offset < Size) {
            long opt_value = (long)Data[data_offset];
            curl_easy_setopt(curl, selected_opt, opt_value);
            data_offset++;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}