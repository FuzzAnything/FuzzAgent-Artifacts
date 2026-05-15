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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 7
//<Quality> {"density":19,"unique_branches":{"curl_multi_get_handles":[[3804,6,3804,7,0,0,4,0],[3808,8,3808,58,0,0,4,0],[3812,12,3812,33,0,0,4,0],[3812,12,3812,33,0,0,4,1],[3814,15,3814,69,0,0,4,0],[3814,15,3814,69,0,0,4,1]],"Curl_uint32_tbl_next":[[195,6,195,57,0,0,4,0]]},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_version_info","curl_msprintf","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_get_handles","curl_free","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data_cb","curl_mime_type","curl_mime_free","curl_easy_setopt","curl_ws_meta","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_version_info","curl_msprintf","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_multi_get_handles","curl_free","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data_cb","curl_mime_type","curl_mime_free","curl_easy_setopt","curl_ws_meta","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Callback structure for mime data
struct FuzzData {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Read callback for mime_data_cb
static size_t  id_000050_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    FuzzData* fuzz_data = static_cast<FuzzData*>(arg);
    size_t total_size = size * nitems;
    size_t available = fuzz_data->size - fuzz_data->pos;
    size_t to_copy = (total_size < available) ? total_size : available;
    
    if (to_copy > 0) {
        memcpy(buffer, fuzz_data->data + fuzz_data->pos, to_copy);
        fuzz_data->pos += to_copy;
        return to_copy;
    }
    return 0;
}

// Seek callback for mime_data_cb
static int  id_000050_seek_callback(void* arg, curl_off_t offset, int origin) {
    FuzzData* fuzz_data = static_cast<FuzzData*>(arg);
    
    if (origin == SEEK_SET) {
        if (offset >= 0 && static_cast<size_t>(offset) <= fuzz_data->size) {
            fuzz_data->pos = offset;
            return CURL_SEEKFUNC_OK;
        }
    } else if (origin == SEEK_CUR) {
        size_t new_pos = fuzz_data->pos + offset;
        if (new_pos <= fuzz_data->size) {
            fuzz_data->pos = new_pos;
            return CURL_SEEKFUNC_OK;
        }
    } else if (origin == SEEK_END) {
        if (offset <= 0 && static_cast<size_t>(-offset) <= fuzz_data->size) {
            fuzz_data->pos = fuzz_data->size + offset;
            return CURL_SEEKFUNC_OK;
        }
    }
    return CURL_SEEKFUNC_CANTSEEK;
}

// Free callback for mime_data_cb
static void  id_000050_free_callback(void* arg) {
    FuzzData* fuzz_data = static_cast<FuzzData*>(arg);
    delete fuzz_data;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // 1. Get environment variable
    char* env_var = curl_getenv("HOME");
    if (env_var) {
        // Do something with env_var
        curl_free(env_var);
    }
    
    // 2. Get version info
    curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
    
    // 3. Use msprintf with version info
    char buffer[256];
    if (version_info && version_info->version) {
        curl_msprintf(buffer, "CURL Version: %s, Protocols: ", version_info->version);
        // Could append more info here
    }
    
    // 4. Create a multi handle for getting handles later
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Create an easy handle for WebSocket metadata
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 6. Get handles from multi handle
    CURL** handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        // Do something with handles (just for demonstration)
        curl_free(handles);
    }
    
    // 7. Create mime structure
    curl_mime* mime = curl_mime_init(easy_handle);
    if (mime) {
        // Create mime part
        curl_mimepart* part = curl_mime_addpart(mime);
        if (part) {
            // 8. Set mime headers
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
            headers = curl_slist_append(headers, "X-Fuzzed: true");
            
            curl_mime_headers(part, headers, 1); // take_ownership = 1
            
            // 9. Set mime data callback
            FuzzData* fuzz_data = new FuzzData{data, size, 0};
            curl_mime_data_cb(part, static_cast<curl_off_t>(size),
                               id_000050_read_callback,
                               id_000050_seek_callback,
                               id_000050_free_callback,
                              fuzz_data);
            
            // Set mime type
            curl_mime_type(part, "application/octet-stream");
        }
        
        // Clean up mime
        curl_mime_free(mime);
    }
    
    // 10. Get WebSocket metadata (even though we don't have a real WS connection)
    // Set some WebSocket options if we have data
    if (size > 0) {
        curl_easy_setopt(easy_handle, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
        
        // Try to get WS metadata
        const struct curl_ws_frame* ws_meta = curl_ws_meta(easy_handle);
        (void)ws_meta; // Use it to avoid unused variable warning
    }
    
    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}