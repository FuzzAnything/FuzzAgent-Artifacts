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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"curl_url_set":[[1833,8,1833,14,0,0,4,0]],"set_url_scheme":[[1642,33,1642,43,0,0,4,0],[1647,46,1647,48,0,0,4,0],[1647,52,1647,59,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_url_set","curl_url_dup","curl_url_get","curl_free","curl_url_cleanup","curl_url_cleanup","curl_pushheader_byname","curl_pushheader_bynum","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_share_init","curl_share_setopt","curl_share_strerror","curl_mfprintf","curl_share_cleanup","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_dup","curl_url_get","curl_free","curl_url_cleanup","curl_url_cleanup","curl_pushheader_byname","curl_pushheader_bynum","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_share_init","curl_share_setopt","curl_share_strerror","curl_mfprintf","curl_share_cleanup","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* to write output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create and duplicate a CURLU handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set some URL parts using data
        if (size > 10) {
            // Use first part of data as URL scheme
            char scheme[11];
            size_t scheme_len = size > 10 ? 10 : size;
            memcpy(scheme, data, scheme_len);
            scheme[scheme_len] = '\0';
            curl_url_set(url_handle, CURLUPART_SCHEME, scheme, fuzz_uint32_t_1);
        }
        
        // Duplicate the URL handle
        CURLU *dup_handle = curl_url_dup(url_handle);
        if (dup_handle) {
            // Use the duplicated handle
            char *url_part = nullptr;
            curl_url_get(dup_handle, CURLUPART_SCHEME, &url_part, fuzz_uint32_t_2);
            if (url_part) {
                curl_free(url_part);
            }
            curl_url_cleanup(dup_handle);
        }
        curl_url_cleanup(url_handle);
    }
    
    // 2. Create a dummy pushheaders structure and use pushheader functions
    // Note: In real usage, pushheaders would come from a push callback
    // For fuzzing, we'll create a minimal simulation
    if (size > 0) {
        // Allocate memory that we'll treat as pushheaders structure
        // This is a simulation since we can't create real pushheaders without actual HTTP/2 push
        void *dummy_pushheaders = malloc(64);
        if (dummy_pushheaders) {
            struct curl_pushheaders *ph = (struct curl_pushheaders *)dummy_pushheaders;
            
            // Try to get header by name (will likely return NULL in simulation)
            char *header_by_name = curl_pushheader_byname(ph, fuzz_str_3);
            // Don't use the result as it's NULL in this simulation
            
            // Try to get header by number
            char *header_by_num = curl_pushheader_bynum(ph, fuzz_uint64_t_4);
            // Don't use the result as it's NULL in this simulation
            
            free(dummy_pushheaders);
        }
    }
    
    // 3. Use curl_mfprintf to write to output file
    if (size > 0) {
        // Write some formatted output using data
        curl_mfprintf(out_file, "Data size: %zu\n", size);
        
        // Write first few bytes as hex
        curl_mfprintf(out_file, "First bytes: ");
        size_t bytes_to_print = size > 16 ? 16 : size;
        for (size_t i = 0; i < bytes_to_print; i++) {
            curl_mfprintf(out_file, "%02x ", data[i]);
        }
        curl_mfprintf(out_file, "\n");
        
        // If we have enough data, try to interpret as string
        if (size > 0 && data[0] != 0) {
            // Find null terminator or use first 100 chars
            size_t str_len = 0;
            while (str_len < size && data[str_len] != 0 && str_len < 100) {
                str_len++;
            }
            if (str_len > 0) {
                char *temp_str = (char*)malloc(str_len + 1);
                if (temp_str) {
                    memcpy(temp_str, data, str_len);
                    temp_str[str_len] = '\0';
                    curl_mfprintf(out_file, "As string: %s\n", temp_str);
                    free(temp_str);
                }
            }
        }
    }
    
    // 4. Create a share handle and generate share error
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        // Try to set an invalid option to generate error
        CURLSHcode share_error = curl_share_setopt(share_handle, CURLSHOPT_SHARE, (long)99);
        
        // Get error string
        const char *error_str = curl_share_strerror(share_error);
        
        // Write error to output file
        curl_mfprintf(out_file, "Share error: %s\n", error_str ? error_str : "Unknown error");
        
        curl_share_cleanup(share_handle);
    }
    
    // 5. Additional curl operations using input data
    // Create a simple CURL handle and perform basic operations
    CURL *curl = curl_easy_init();
    if (curl) {
        // Set some options using input data
        if (size > 0) {
            // Use data as a URL if it looks like one
            size_t url_end = 0;
            while (url_end < size && data[url_end] != 0 && url_end < 255) {
                url_end++;
            }
            if (url_end > 0) {
                char *url = (char*)malloc(url_end + 1);
                if (url) {
                    memcpy(url, data, url_end);
                    url[url_end] = '\0';
                    
                    // Check if it looks like a URL
                    if (strstr(url, "://") || strstr(url, "http") || strstr(url, "ftp")) {
                        curl_easy_setopt(curl, CURLOPT_URL, url);
                    }
                    
                    free(url);
                }
            }
        }
        
        // Set output file for writing
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
        
        // Don't actually perform the transfer to avoid network calls
        // Just set up the handle
        
        curl_easy_cleanup(curl);
    }
    
    // 6. Read some data from input file and process it
    if (size > 0) {
        // Read first few bytes
        uint8_t buffer[256];
        size_t read_size = size > sizeof(buffer) ? sizeof(buffer) : size;
        fread(buffer, 1, read_size, in_file);
        
        // Process the read data
        // Could be used for additional curl operations
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up curl global state
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}