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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 4
//<Quality> {"density":18,"unique_branches":{"setopt_set_timeout_ms":[[69,6,69,12,0,0,4,1]],"setopt_long_bool":[[465,3,465,22,0,0,4,0],[471,8,471,22,0,0,4,0]],"setopt_long_net":[[883,3,883,26,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_escape","curl_unescape","curl_free","curl_free","curl_easy_duphandle","curl_easy_setopt","curl_easy_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_free","curl_ws_meta","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_escape","curl_unescape","curl_free","curl_free","curl_easy_duphandle","curl_easy_setopt","curl_easy_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_free","curl_ws_meta","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Custom data structure for callback context
struct mime_callback_data {
    const uint8_t* data;
    size_t size;
    size_t position;
};

// Read callback for mime data
static size_t  id_000036_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    mime_callback_data* cb_data = (mime_callback_data*)arg;
    size_t total_size = size * nitems;
    size_t bytes_to_read = total_size;
    
    if (cb_data->position >= cb_data->size) {
        return 0; // EOF
    }
    
    if (cb_data->position + bytes_to_read > cb_data->size) {
        bytes_to_read = cb_data->size - cb_data->position;
    }
    
    if (bytes_to_read > 0) {
        memcpy(buffer, cb_data->data + cb_data->position, bytes_to_read);
        cb_data->position += bytes_to_read;
    }
    
    return bytes_to_read;
}

// Seek callback for mime data
static int  id_000036_seek_callback(void* arg, curl_off_t offset, int origin) {
    mime_callback_data* cb_data = (mime_callback_data*)arg;
    
    switch (origin) {
        case SEEK_SET:
            if (offset >= 0 && (size_t)offset <= cb_data->size) {
                cb_data->position = (size_t)offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_CUR:
            if (cb_data->position + offset >= 0 && 
                cb_data->position + offset <= cb_data->size) {
                cb_data->position += (size_t)offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_END:
            if (cb_data->size + offset >= 0 && 
                cb_data->size + offset <= cb_data->size) {
                cb_data->position = cb_data->size + (size_t)offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
    }
    
    return CURL_SEEKFUNC_FAIL;
}

// Free callback for mime data
static void  id_000036_free_callback(void* arg) {
    // Clean up the callback data structure
    free(arg);
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize CURL global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create main CURL handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // 1. Use curl_escape and curl_unescape on the input data
    // Convert data to a null-terminated string for escaping
    char* input_str = (char*)malloc(size + 1);
    if (input_str) {
        if (size > 0) {
            memcpy(input_str, data, size);
        }
        input_str[size] = '\0';
        
        // Escape the string
        char* escaped = curl_escape(input_str, (int)size);
        if (escaped) {
            // Unescape the escaped string
            char* unescaped = curl_unescape(escaped, (int)strlen(escaped));
            
            // Free the escaped string
            curl_free(escaped);
            
            if (unescaped) {
                // Free the unescaped string
                curl_free(unescaped);
            }
        }
        
        free(input_str);
    }
    
    // 2. Use curl_easy_duphandle to create a duplicate handle
    CURL* dup_curl = curl_easy_duphandle(curl);
    if (dup_curl) {
        // We could set some options on the duplicate handle
        curl_easy_setopt(dup_curl, CURLOPT_VERBOSE, 0L);
        
        // Clean up the duplicate handle
        curl_easy_cleanup(dup_curl);
    }
    
    // 3. Use curl_mime_data_cb with the input data
    // Create a mime structure
    curl_mime* mime = curl_mime_init(curl);
    if (mime) {
        // Create a mime part
        curl_mimepart* part = curl_mime_addpart(mime);
        if (part) {
            // Set up callback data structure
            mime_callback_data* cb_data = (mime_callback_data*)malloc(sizeof(mime_callback_data));
            if (cb_data) {
                cb_data->data = data;
                cb_data->size = size;
                cb_data->position = 0;
                
                // Set the data callback for the mime part
                CURLcode res = curl_mime_data_cb(
                    part, 
                    (curl_off_t)size, 
                    (curl_read_callback) id_000036_read_callback,
                    (curl_seek_callback) id_000036_seek_callback,
                     id_000036_free_callback,
                    cb_data
                );
                
                // Also set a mime name for the part
                if (res == CURLE_OK) {
                    curl_mime_name(part, "fuzzed_data");
                }
            }
        }
        
        // Clean up the mime structure
        curl_mime_free(mime);
    }
    
    // 4. Use curl_ws_meta (this would normally be used during WebSocket transfers)
    // Since we're not actually performing a WebSocket transfer, we'll just 
    // call the function to exercise it (it will return NULL in this context)
    const struct curl_ws_frame* ws_meta = curl_ws_meta(curl);
    // Note: ws_meta will be NULL since no WebSocket transfer is active
    
    // Set some basic options on the main handle
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // HEAD request only
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L); // Short timeout
    
    // Create a FILE pointer for output (though we won't actually write much)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // We could set CURLOPT_WRITEDATA to out_file if we were performing a transfer
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Clean up resources
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}