// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file for potential use
    write_dummy_file(Data, Size);
    
    // Initialize curl handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        remove_dummy_file();
        return 0;
    }
    
    // Use fuzz data to determine which options to set
    uint8_t option_selector = Data[0];
    size_t data_offset = 1;
    
    // Set various options based on fuzz input
    if (data_offset < Size) {
        // Set URL option
        curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    }
    
    if (data_offset + 1 < Size && (option_selector & 0x01)) {
        // Set verbose option
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[data_offset] & 0x01));
        data_offset++;
    }
    
    if (data_offset + 1 < Size && (option_selector & 0x02)) {
        // Set follow location option
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(Data[data_offset] & 0x01));
        data_offset++;
    }
    
    // Perform the transfer
    if (option_selector & 0x04) {
        curl_easy_perform(curl);
    }
    
    // Test duphandle
    CURL *dup_curl = NULL;
    if (option_selector & 0x08) {
        dup_curl = curl_easy_duphandle(curl);
        if (dup_curl) {
            // Set options on duplicated handle
            curl_easy_setopt(dup_curl, CURLOPT_URL, "file://./dummy_file");
            
            // Perform with duplicated handle
            if (option_selector & 0x10) {
                curl_easy_perform(dup_curl);
            }
            
            // Test send on duplicated handle (though it won't work without CONNECT_ONLY)
            if (data_offset + 4 < Size && (option_selector & 0x20)) {
                size_t bytes_sent = 0;
                const void *send_buffer = Data + data_offset;
                size_t send_size = Size - data_offset;
                if (send_size > 1024) send_size = 1024;
                
                curl_easy_send(dup_curl, send_buffer, send_size, &bytes_sent);
            }
            
            curl_easy_cleanup(dup_curl);
        }
    }
    
    // Test reset
    if (option_selector & 0x40) {
        curl_easy_reset(curl);
        
        // Set options again after reset
        if (data_offset < Size) {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
        }
        
        // Perform after reset
        if (option_selector & 0x80) {
            curl_easy_perform(curl);
        }
    }
    
    // Test send on original handle
    if (data_offset + 4 < Size && (option_selector & 0x20)) {
        size_t bytes_sent = 0;
        const void *send_buffer = Data + data_offset;
        size_t send_size = Size - data_offset;
        if (send_size > 1024) send_size = 1024;
        
        curl_easy_send(curl, send_buffer, send_size, &bytes_sent);
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    remove_dummy_file();
    
    return 0;
}