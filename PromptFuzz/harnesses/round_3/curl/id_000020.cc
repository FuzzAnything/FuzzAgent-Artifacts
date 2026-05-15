#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <multi.h>
}
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"parsedate":[[541,6,541,17,0,0,4,0],[541,21,541,55,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_getenv","curl_getdate","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_encoder","curl_easy_strerror","curl_ws_start_frame","curl_ws_send","curl_easy_strerror","curl_mime_free","curl_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_getenv","curl_getdate","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_encoder","curl_easy_strerror","curl_ws_start_frame","curl_ws_send","curl_easy_strerror","curl_mime_free","curl_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create a CURL handle for WebSocket operations
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // 1. Use curl_getenv to get an environment variable
    // We'll try to get a common variable, but use input data as fallback name
    char *env_var_name = nullptr;
    if (size > 0) {
        // Use first byte modulo 26 to choose from common env vars
        const char* common_envs[] = {
            "PATH", "HOME", "USER", "SHELL", "TERM", "TMPDIR", 
            "PWD", "LANG", "LC_ALL", "DISPLAY", "EDITOR"
        };
        int idx = data[0] % (sizeof(common_envs)/sizeof(common_envs[0]));
        env_var_name = curl_getenv(common_envs[idx]);
    }

    // 2. Use curl_getdate to parse a date from input data
    time_t parsed_time = 0;
    if (size > 1) {
        // Create a null-terminated string from input data for date parsing
        size_t date_len = size < 100 ? size : 100;
        char *date_str = (char *)malloc(date_len + 1);
        if (date_str) {
            memcpy(date_str, data, date_len);
            date_str[date_len] = '\0';
            parsed_time = curl_getdate(date_str, nullptr);
            free(date_str);
        }
    }

    // 3. Create a mime structure and use curl_mime_encoder
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Set some data to the mime part
            if (size > 0) {
                curl_mime_data(part, (const char *)data, 
                              size < 1024 ? size : 1024);
            }
            
            // Try to set an encoder - use input to choose encoding
            const char* encodings[] = {"base64", "quoted-printable", "8bit", "7bit", "binary"};
            if (size > 2) {
                int enc_idx = data[1] % (sizeof(encodings)/sizeof(encodings[0]));
                CURLcode enc_res = curl_mime_encoder(part, encodings[enc_idx]);
                
                // 5. Use curl_easy_strerror to get error string
                if (enc_res != CURLE_OK) {
                    const char *err_str = curl_easy_strerror(enc_res);
                    // Error string is available for debugging
                }
            }
        }
    }

    // 4. Prepare for WebSocket operations
    // Note: In reality, we'd need a WebSocket connection first,
    // but for fuzzing we'll simulate the API calls
    
    // Start a WebSocket frame
    if (size > 10) {
        unsigned int flags = CURLWS_BINARY;
        if (data[2] & 0x01) flags |= CURLWS_PING;
        if (data[2] & 0x02) flags |= CURLWS_PONG;
        if (data[2] & 0x04) flags |= CURLWS_CLOSE;
        
        curl_off_t frame_len = 0;
        if (size > 20) {
            // Use some bytes from input to determine frame length
            frame_len = (curl_off_t)(data[3] * 256 + data[4]);
            if (frame_len > size - 10) {
                frame_len = size - 10;
            }
        }
        
        CURLcode frame_res = curl_ws_start_frame(curl, flags, fuzz_int64_t_1);
        
        // Send WebSocket data if we successfully started a frame
        if (frame_res == CURLE_OK && frame_len > 0 && size > 10) {
            size_t sent = 0;
            const void *send_data = data + 10;
            size_t send_size = size - 10;
            if (send_size > frame_len) {
                send_size = (size_t)frame_len;
            }
            
            CURLcode send_res = curl_ws_send(curl, send_data, send_size, 
                                           &sent, fuzz_int64_t_2, flags);
            
            // Get error string for send operation
            if (send_res != CURLE_OK) {
                const char *send_err = curl_easy_strerror(send_res);
                // Error string is available for debugging
            }
        }
    }

    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (env_var_name) {
        curl_free(env_var_name);
    }
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}