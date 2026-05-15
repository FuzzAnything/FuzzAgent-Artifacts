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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 304.5, nr_unique_branch: 61
//<Quality> {"density":29,"unique_branches":{"multi_runsingle":[[2538,10,2538,34,0,0,4,0]],"setopt_long_net":[[887,3,887,33,0,0,4,0]],"set_url_scheme":[[1642,6,1642,29,0,0,4,1],[1642,33,1642,43,0,0,4,1],[1647,6,1647,41,0,0,4,0],[1647,46,1647,48,0,0,4,1],[1647,52,1647,59,0,0,4,1],[1649,6,1649,8,0,0,4,1]],"cf_ip_happy_init":[[703,6,703,32,0,0,4,0]],"share_destroy":[[37,6,37,54,0,0,4,0]],"cf_ip_ballers_run":[[507,8,507,26,0,0,4,0],[1290,24,1290,33,24,0,4,0]],"create_dnscache_id":[[85,6,85,24,0,0,4,0]],"cf_ip_happy_connect":[[798,8,798,14,0,0,4,0]],"multi_deltimeout":[[3551,41,3551,42,0,0,4,0],[3553,8,3553,21,0,0,4,1]],"Curl_expire_ex":[[3633,6,3633,25,0,0,4,0],[3640,8,3640,16,0,0,4,1],[3650,8,3650,10,0,0,4,1]],"curl_dbg_free":[[369,8,369,14,0,0,4,1]],"Curl_timeleft_now_ms":[[115,30,115,60,0,0,4,0],[119,8,119,21,0,0,4,0],[129,8,129,20,0,0,4,0]],"Curl_cpool_destroy":[[41,4,41,7,6,0,4,0],[41,11,41,59,6,0,4,0],[41,4,41,7,11,0,4,0],[41,11,41,59,11,0,4,0]],"multi_addtimeout":[[3584,6,3584,7,0,0,4,0],[3586,43,3586,44,0,0,4,0],[3586,43,3586,44,0,0,4,1],[3589,10,3589,18,0,0,4,1]],"curl_mime_data_cb":[[1421,6,1421,11,0,0,4,1],[1426,6,1426,14,0,0,4,0]],"curl_url_set":[[1831,3,1831,24,0,0,4,0],[1833,8,1833,14,0,0,4,1],[1857,3,1857,22,0,0,4,0],[1884,8,1884,20,0,0,4,0],[1884,24,1884,40,0,0,4,1]],"setopt_long_bool":[[750,3,750,24,0,0,4,0]],"curl_share_setopt":[[204,6,204,25,0,0,4,1],[213,3,213,23,0,0,4,0],[218,5,218,28,0,0,4,1],[221,5,221,31,0,0,4,1],[233,5,233,29,0,0,4,1],[245,5,245,36,0,0,4,1],[261,5,261,32,0,0,4,0],[263,10,263,35,0,0,4,0],[268,5,268,28,0,0,4,1],[274,5,274,12,0,0,4,1],[277,8,277,12,0,0,4,0],[281,3,281,25,0,0,4,1],[331,3,331,26,0,0,4,1],[336,3,336,28,0,0,4,1],[341,3,341,26,0,0,4,1],[346,3,346,10,0,0,4,1],[38,31,38,34,1,0,4,0],[38,38,38,67,1,0,4,0]],"thrdq_tpool_take":[[115,13,115,14,0,0,4,1],[119,12,119,26,0,0,4,0],[138,6,138,14,0,0,4,0]],"curl_easy_strerror":[[120,3,120,32,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_filename","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_waitfds","curl_multi_perform","curl_multi_remove_handle","curl_share_init","curl_share_setopt","curl_share_strerror","curl_mfprintf","curl_url_get","curl_mfprintf","curl_free","curl_mfprintf","curl_mfprintf","curl_share_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_filename","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_waitfds","curl_multi_perform","curl_multi_remove_handle","curl_share_init","curl_share_setopt","curl_share_strerror","curl_mfprintf","curl_url_get","curl_mfprintf","curl_free","curl_mfprintf","curl_mfprintf","curl_share_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Custom data structure to manage the input data for callbacks
struct ReadCallbackData {
    const uint8_t* data;
    size_t size;
    size_t position;
    FILE* file;
};

// Read callback for mime data
static size_t  id_000003_read_callback(char* buffer, size_t size, size_t nitems, void* instream) {
    ReadCallbackData* cb_data = static_cast<ReadCallbackData*>(instream);
    size_t bytes_to_read = size * nitems;
    
    if (cb_data->position >= cb_data->size) {
        return 0; // EOF
    }
    
    if (bytes_to_read > cb_data->size - cb_data->position) {
        bytes_to_read = cb_data->size - cb_data->position;
    }
    
    memcpy(buffer, cb_data->data + cb_data->position, bytes_to_read);
    cb_data->position += bytes_to_read;
    
    return bytes_to_read;
}

// Seek callback for mime data
static int  id_000003_seek_callback(void* instream, curl_off_t offset, int origin) {
    ReadCallbackData* cb_data = static_cast<ReadCallbackData*>(instream);
    
    if (origin == SEEK_SET) {
        if (offset < 0 || static_cast<size_t>(offset) > cb_data->size) {
            return CURL_SEEKFUNC_FAIL;
        }
        cb_data->position = static_cast<size_t>(offset);
    } else if (origin == SEEK_CUR) {
        if (cb_data->position + offset < 0 || 
            cb_data->position + offset > cb_data->size) {
            return CURL_SEEKFUNC_FAIL;
        }
        cb_data->position += static_cast<size_t>(offset);
    } else if (origin == SEEK_END) {
        if (cb_data->size + offset < 0 || 
            cb_data->size + offset > cb_data->size) {
            return CURL_SEEKFUNC_FAIL;
        }
        cb_data->position = cb_data->size + static_cast<size_t>(offset);
    } else {
        return CURL_SEEKFUNC_FAIL;
    }
    
    return CURL_SEEKFUNC_OK;
}

// Free callback for mime data
static void  id_000003_free_callback(void* ptr) {
    // Nothing special to free since we manage memory elsewhere
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Need minimum data
    }

    // Initialize libcurl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create input and output files
    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");
    if (!in_file || !out_file) {
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use curl_url_set to parse and set URL components
    CURLU* url_handle = curl_url();
    if (url_handle) {
        // Extract a string from the input data (first null-terminated string)
        const char* url_part = reinterpret_cast<const char*>(data);
        size_t url_len = strnlen(url_part, size);
        
        if (url_len > 0 && url_len < size) {
            // Set URL scheme
            curl_url_set(url_handle, CURLUPART_SCHEME, "http", 0);
            
            // Set URL host from input data
            char* host_part = static_cast<char*>(malloc(url_len + 1));
            if (host_part) {
                memcpy(host_part, url_part, url_len);
                host_part[url_len] = '\0';
                curl_url_set(url_handle, CURLUPART_HOST, host_part, 0);
                free(host_part);
            }
            
            // Set URL path
            curl_url_set(url_handle, CURLUPART_PATH, "/test", 0);
        }
    }

    // 2. Use curl_mime_data_cb for MIME data with callbacks
    CURL* easy_handle = curl_easy_init();
    curl_mime* mime = NULL;
    curl_mimepart* part = NULL;
    ReadCallbackData cb_data = {data, size, 0, in_file};
    
    if (easy_handle) {
        mime = curl_mime_init(easy_handle);
        if (mime) {
            part = curl_mime_addpart(mime);
            if (part) {
                // Use curl_mime_data_cb with our callbacks
                curl_mime_data_cb(part, 
                                  static_cast<curl_off_t>(size),
                                   id_000003_read_callback,
                                   id_000003_seek_callback,
                                   id_000003_free_callback,
                                  &cb_data);
                curl_mime_name(part, "file");
                curl_mime_filename(part, "input.bin");
                
                // Set the mime data to the easy handle
                curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
            }
        }
        
        // Get the URL from url_handle if available
        char* full_url = NULL;
        if (url_handle) {
            curl_url_get(url_handle, CURLUPART_URL, &full_url, 0);
        }
        
        if (full_url) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, full_url);
            curl_free(full_url);
        } else {
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:8080/");
        }
        
        // Don't actually perform the transfer
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    }

    // 3. Use curl_multi_waitfds for multi interface
    CURLM* multi_handle = curl_multi_init();
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    if (multi_handle && easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Prepare waitfd structure
        ufds[0].fd = fuzz_fileno(in_file);
        ufds[0].events = CURL_WAIT_POLLIN;
        ufds[0].revents = 0;
        
        ufds[1].fd = fuzz_fileno(out_file);
        ufds[1].events = CURL_WAIT_POLLOUT;
        ufds[1].revents = 0;
        
        // Use curl_multi_waitfds
        curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
        
        // Perform a single multi step
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // 4. Use curl_share_strerror for error reporting
    CURLSH* share_handle = curl_share_init();
    if (share_handle) {
        // Try to set an option that might fail
        CURLSHcode share_result = curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
        
        // Use curl_share_strerror to get error message
        const char* share_error = curl_share_strerror(share_result);
        
        // 5. Use curl_mfprintf to write to output file
        if (share_error) {
            curl_mfprintf(out_file, "Share error: %s\n", share_error);
        }
        
        // Also write some URL info if available
        if (url_handle) {
            char* host = NULL;
            if (curl_url_get(url_handle, CURLUPART_HOST, &host, 0) == CURLUE_OK && host) {
                curl_mfprintf(out_file, "Host from URL: %s\n", host);
                curl_free(host);
            }
        }
        
        curl_mfprintf(out_file, "Processed %zu bytes of input data\n", size);
        curl_mfprintf(out_file, "File descriptors ready: %u\n", fd_count);
        
        curl_share_cleanup(share_handle);
    }

    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}