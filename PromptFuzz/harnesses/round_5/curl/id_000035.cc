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
#include <mprintf.h>
}
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"curl_easy_unescape":[[167,16,167,29,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_escape","curl_unescape","curl_free","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_easy_duphandle","curl_easy_setopt","curl_ws_meta","curl_easy_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_escape","curl_unescape","curl_free","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_easy_duphandle","curl_easy_setopt","curl_ws_meta","curl_easy_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified curl APIs:

*/


// Include curl headers (assuming they're available)

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Need minimum data
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a main CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // 1. Use curl_escape and curl_unescape with input data
    // Create a string from first portion of data (null-terminated)
    size_t str_len = size > 100 ? 100 : size;
    char *input_str = (char*)malloc(str_len + 1);
    if (!input_str) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    memcpy(input_str, data, str_len);
    input_str[str_len] = '\0';
    
    // Escape the string
    char *escaped = curl_escape(input_str, (int)str_len);
    
    // Unescape it back
    if (escaped) {
        char *unescaped = curl_unescape(escaped, -1);
        if (unescaped) {
            curl_free(unescaped);
        }
        curl_free(escaped);
    }
    
    free(input_str);

    // 2. Create and use a mime part with data callback
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Define callbacks for mime data
            struct CallbackData {
                const uint8_t *data;
                size_t size;
                size_t pos;
            };
            
            CallbackData *cb_data = new CallbackData{data, size, 0};
            
            // Read callback
            auto read_func = [](char *buffer, size_t size, size_t nitems, void *userdata) -> size_t {
                CallbackData *cb = static_cast<CallbackData*>(userdata);
                size_t total_size = size * nitems;
                size_t to_copy = (cb->size - cb->pos) < total_size ? (cb->size - cb->pos) : total_size;
                
                if (to_copy > 0 && cb->pos < cb->size) {
                    memcpy(buffer, cb->data + cb->pos, to_copy);
                    cb->pos += to_copy;
                }
                return to_copy;
            };
            
            // Seek callback
            auto seek_func = [](void *userdata, curl_off_t offset, int origin) -> int {
                CallbackData *cb = static_cast<CallbackData*>(userdata);
                switch (origin) {
                    case SEEK_SET:
                        if (offset >= 0 && offset <= (curl_off_t)cb->size) {
                            cb->pos = offset;
                            return CURL_SEEKFUNC_OK;
                        }
                        break;
                    case SEEK_CUR:
                        if (cb->pos + offset >= 0 && cb->pos + offset <= cb->size) {
                            cb->pos += offset;
                            return CURL_SEEKFUNC_OK;
                        }
                        break;
                    case SEEK_END:
                        if (cb->size + offset >= 0 && cb->size + offset <= cb->size) {
                            cb->pos = cb->size + offset;
                            return CURL_SEEKFUNC_OK;
                        }
                        break;
                }
                return CURL_SEEKFUNC_FAIL;
            };
            
            // Free callback
            auto free_func = [](void *userdata) {
                delete static_cast<CallbackData*>(userdata);
            };
            
            // Set up the mime data callback
            curl_mime_data_cb(part, size, 
                             (curl_read_callback)read_func,
                             (curl_seek_callback)seek_func,
                             (curl_free_callback)free_func,
                             cb_data);
        }
    }

    // 3. Duplicate the CURL handle
    CURL *dup_curl = curl_easy_duphandle(curl);
    if (dup_curl) {
        // We could set some options on the duplicated handle
        // Set a dummy URL option
        curl_easy_setopt(dup_curl, CURLOPT_URL, "http://example.com");
        
        // 4. Get WebSocket metadata (if available)
        // This will typically return NULL unless we're in a WebSocket transfer
        const struct curl_ws_frame *ws_meta = curl_ws_meta(dup_curl);
        (void)ws_meta;  // Use variable to avoid unused warning
        
        // Clean up duplicated handle
        curl_easy_cleanup(dup_curl);
    }

    // 5. Clean up resources
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Clean up main handle
    curl_easy_cleanup(curl);
    
    // Global cleanup
    curl_global_cleanup();

    return 0;
}