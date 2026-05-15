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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.3333333, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"curl_share_strerror":[[395,3,395,22,0,0,4,0],[398,3,398,23,0,0,4,0],[401,3,401,21,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_pushheader_byname","curl_pushheader_bynum","curl_mfprintf","curl_share_strerror","curl_mfprintf","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_cleanup","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_pushheader_byname","curl_pushheader_bynum","curl_mfprintf","curl_share_strerror","curl_mfprintf","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_cleanup","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create a CURLU handle and set some URL parts
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set some URL parts using data from input
        // We'll use the first min(100, size) bytes as URL parts
        size_t url_part_size = size > 100 ? 100 : size;
        char *url_part = (char *)malloc(url_part_size + 1);
        if (url_part) {
            fread(url_part, 1, url_part_size, in_file);
            url_part[url_part_size] = '\0';
            
            // Set URL scheme
            curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_1, fuzz_uint32_t_2);
            
            // Duplicate the URL handle - FIRST REQUIRED API
            CURLU *dup_url = curl_url_dup(url_handle);
            
            // Clean up duplicated handle
            if (dup_url) curl_url_cleanup(dup_url);
            
            free(url_part);
        }
        
        // Clean up original URL handle
        curl_url_cleanup(url_handle);
    }
    
    // Reset file pointer
    rewind(in_file);
    
    // Simulate push headers (normally provided by libcurl during HTTP/2 push)
    // We'll create a simple structure to mimic curl_pushheaders
    struct curl_pushheaders *push_headers = nullptr;
    
    // Call pushheader APIs - SECOND and THIRD REQUIRED APIs
    char *header_by_name = curl_pushheader_byname(push_headers, fuzz_str_3);
    char *header_by_num = curl_pushheader_bynum(push_headers, fuzz_uint64_t_4);
    
    // Use curl_mfprintf to write to file - FOURTH REQUIRED API
    // Read some data from input to use as format string
    char format_buf[256];
    size_t format_len = size > 255 ? 255 : size;
    fread(format_buf, 1, format_len, in_file);
    format_buf[format_len] = '\0';
    
    // Write formatted output to file
    curl_mfprintf(out_file, "Input data (first %zu bytes): %.*s\n", 
                  format_len, (int)format_len, format_buf);
    
    // Generate a random share error code and get its string - FIFTH REQUIRED API
    CURLSHcode share_error = CURLSHE_OK;
    if (size > 0) {
        // Use first byte to determine error code (modulo to valid range)
        uint8_t error_index = data[0] % 5;
        switch (error_index) {
            case 0: share_error = CURLSHE_OK; break;
            case 1: share_error = CURLSHE_BAD_OPTION; break;
            case 2: share_error = CURLSHE_IN_USE; break;
            case 3: share_error = CURLSHE_INVALID; break;
            case 4: share_error = CURLSHE_NOMEM; break;
        }
    }
    const char *error_str = curl_share_strerror(share_error);
    if (error_str) {
        curl_mfprintf(out_file, "Share error string: %s\n", error_str);
    }
    
    // Additional curl usage to consume more input data
    // Create a simple HTTP POST with form data
    CURL *curl = curl_easy_init();
    if (curl) {
        // Read remaining data for POST fields
        long bytes_remaining = size - ftell(in_file);
        if (bytes_remaining > 0) {
            char *post_data = (char *)malloc(bytes_remaining + 1);
            if (post_data) {
                fread(post_data, 1, bytes_remaining, in_file);
                post_data[bytes_remaining] = '\0';
                
                // Use the data for POST field
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
                
                free(post_data);
            }
        }
        
        // Set some easy options
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
        
        // Clean up curl handle
        curl_easy_cleanup(curl);
    }
    
    // Create and use curl share handle
    CURLSH *share = curl_share_init();
    if (share) {
        // Set share options
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        
        // Clean up share handle
        curl_share_cleanup(share);
    }
    
    // Clean up all resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}