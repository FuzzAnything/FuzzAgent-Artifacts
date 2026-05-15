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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 49
//<Quality> {"density":5,"unique_branches":{"trc_opt":[[630,9,630,49,0,0,4,0],[630,9,630,49,0,0,4,1],[634,8,634,21,0,0,4,0],[634,8,634,21,0,0,4,1],[638,13,638,26,0,0,4,0],[638,13,638,26,0,0,4,1],[641,8,641,42,0,0,4,0],[641,8,641,42,0,0,4,1],[643,13,643,52,0,0,4,1],[645,13,645,51,0,0,4,1],[647,13,647,49,0,0,4,1],[649,13,649,47,0,0,4,1],[656,8,656,38,0,0,4,0],[656,8,656,38,0,0,4,1]],"curl_ws_meta":[[1859,32,1859,57,0,0,4,1],[134,3,135,31,1,0,4,0],[134,5,134,8,1,0,4,0],[134,12,134,49,1,0,4,0]],"trc_apply_level_by_name":[[599,14,599,32,0,0,4,0],[599,14,599,32,0,0,4,1],[600,8,600,59,0,0,4,0],[600,8,600,59,0,0,4,1],[605,14,605,32,0,0,4,0],[605,14,605,32,0,0,4,1],[606,8,606,61,0,0,4,0],[606,8,606,61,0,0,4,1]],"curlx_str_single":[[137,6,137,21,0,0,4,0],[137,6,137,21,0,0,4,1]],"curlx_str_casecompare":[[250,17,250,22,0,0,4,0],[251,11,251,29,0,0,4,0],[251,11,251,29,0,0,4,1],[251,33,251,70,0,0,4,0],[251,33,251,70,0,0,4,1]],"Curl_trc_opt":[[664,21,664,27,0,0,4,0]],"trc_apply_level_by_category":[[617,14,617,32,0,0,4,0],[617,14,617,32,0,0,4,1],[618,8,618,17,0,0,4,0],[621,14,621,32,0,0,4,0],[621,14,621,32,0,0,4,1],[622,8,622,17,0,0,4,0]],"curlx_str_nudge":[[269,6,269,21,0,0,4,0]],"curlx_str_until":[[57,9,57,11,0,0,4,0],[57,9,57,11,0,0,4,1],[57,15,57,28,0,0,4,0],[57,15,57,28,0,0,4,1],[59,8,59,19,0,0,4,0],[59,8,59,19,0,0,4,1],[63,6,63,10,0,0,4,0],[63,6,63,10,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_global_trace","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_ws_meta","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_ws_meta","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Custom types from the provided list
struct curl_mime;
struct Curl_URL;
typedef struct Curl_URL CURLU;
typedef void CURL;
typedef void CURLM;

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize global curl with memory tracking
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create file for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use curl_global_trace with config from input data
    // Take first 100 bytes or less for config string
    size_t config_len = size > 100 ? 100 : size;
    char *config_str = (char *)malloc(config_len + 1);
    if (config_str) {
        memcpy(config_str, data, config_len);
        config_str[config_len] = '\0';
        CURLcode trace_result = curl_global_trace(config_str);
        free(config_str);
    }

    // 2. Create and duplicate CURLU handle
    CURLU *url_handle = curl_url();
    CURLU *url_dup = nullptr;
    if (url_handle) {
        // Use part of input data as URL component
        size_t url_len = size > 50 ? 50 : size;
        char *url_part = (char *)malloc(url_len + 1);
        if (url_part) {
            // Use data from the middle of input for URL
            size_t offset = size > 100 ? 50 : 0;
            size_t copy_len = (offset + url_len <= size) ? url_len : size - offset;
            if (copy_len > 0) {
                memcpy(url_part, data + offset, copy_len);
                url_part[copy_len] = '\0';
                curl_url_set(url_handle, CURLUPART_URL, url_part, fuzz_uint32_t_1);
            }
            free(url_part);
        }
        
        // Duplicate the URL handle
        url_dup = curl_url_dup(url_handle);
    }

    // 3. Create and free MIME structure
    CURL *easy_handle = curl_easy_init();
    curl_mime *mime = nullptr;
    if (easy_handle) {
        mime = curl_mime_init(easy_handle);
        if (mime) {
            // Create a mime part
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use input data for mime data
                size_t mime_data_len = size > 200 ? 200 : size;
                char *mime_data = (char *)malloc(mime_data_len + 1);
                if (mime_data) {
                    // Use last part of input for mime data
                    size_t mime_offset = size > mime_data_len ? size - mime_data_len : 0;
                    memcpy(mime_data, data + mime_offset, mime_data_len);
                    mime_data[mime_data_len] = '\0';
                    
                    // Set mime data and name
                    curl_mime_data(part, mime_data, mime_data_len);
                    curl_mime_name(part, fuzz_str_2);
                    
                    free(mime_data);
                }
            }
        }
    }

    // 4. Get WebSocket metadata (requires WebSocket connection setup)
    const struct curl_ws_frame *ws_meta = nullptr;
    if (easy_handle) {
        // Configure for WebSocket (will fail without proper setup, but API will be called)
        curl_easy_setopt(easy_handle, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
        ws_meta = curl_ws_meta(easy_handle);
    }

    // 5. Create multi handle and get timeout
    CURLM *multi_handle = curl_multi_init();
    long timeout_ms = 0;
    if (multi_handle && easy_handle) {
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Get timeout
        CURLMcode timeout_result = curl_multi_timeout(multi_handle, &timeout_ms);
        
        // Perform some multi operations
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Remove handle
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // Use the input file descriptor for something
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Write some data to output file
    if (size > 0 && out_file) {
        // Write first 100 bytes or less to output
        size_t write_len = size > 100 ? 100 : size;
        fwrite(data, 1, write_len, out_file);
        fflush(out_file);
    }

    // Clean up resources in reverse order
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (url_dup) {
        curl_url_cleanup(url_dup);
    }
    
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}