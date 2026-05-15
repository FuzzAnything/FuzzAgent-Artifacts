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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 12
//<Quality> {"density":9,"unique_branches":{"curl_url_set":[[1884,24,1884,40,0,0,4,0],[1886,10,1886,16,0,0,4,1]],"parsefmt":[[417,7,417,15,0,0,4,0],[418,12,418,34,0,0,4,1],[420,17,420,35,0,0,4,1]],"dollarstring":[[141,6,141,48,0,0,4,1],[142,6,142,31,0,0,4,0]],"out_number":[[735,11,735,28,0,0,4,0],[737,14,737,35,0,0,4,1],[771,3,771,10,0,0,4,0],[772,11,772,18,0,0,4,0],[772,11,772,18,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_url_set","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_type","curl_easy_setopt","curl_mime_free","curl_easy_cleanup","curl_multi_init","curl_multi_waitfds","curl_multi_cleanup","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_share_init","curl_share_setopt","curl_share_strerror","curl_mfprintf","curl_share_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_type","curl_easy_setopt","curl_mime_free","curl_easy_cleanup","curl_multi_init","curl_multi_waitfds","curl_multi_cleanup","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_share_init","curl_share_setopt","curl_share_strerror","curl_mfprintf","curl_share_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Define callback functions for curl_mime_data_cb
static size_t  id_000004_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *file = (FILE *)arg;
    return fread(buffer, size, nitems, file);
}

static int  id_000004_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *file = (FILE *)arg;
    return fseek(file, (long)offset, origin);
}

static void  id_000004_free_callback(void *arg) {
    // No-op since we'll close the file separately
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create and configure CURLU handle for URL manipulation
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Use part of input data as URL path (ensure null termination)
        size_t url_len = size > 100 ? 100 : size;
        char *url_part = (char *)malloc(url_len + 1);
        if (url_part) {
            memcpy(url_part, data, url_len);
            url_part[url_len] = '\0';
            
            // Use curl_url_set to set URL path
            curl_url_set(url_handle, CURLUPART_PATH, url_part, 0);
            free(url_part);
        }
    }
    
    // 2. Create curl_mime structure and set up data callback
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Reset file position for reading
                fseek(in_file, 0, SEEK_SET);
                
                // Use curl_mime_data_cb with our callbacks
                // Use part of size as datasize (modulo to avoid overflow)
                curl_off_t datasize = size % 1024;
                curl_mime_data_cb(part, datasize, 
                                  id_000004_read_callback,
                                  id_000004_seek_callback,
                                  id_000004_free_callback,
                                 in_file);
                
                // Also set some basic properties
                curl_mime_name(part, "fuzz_data");
                curl_mime_type(part, "application/octet-stream");
            }
            
            // Set the mime data to easy handle
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
            
            // Clean up mime structure
            curl_mime_free(mime);
        }
        
        // Clean up easy handle
        curl_easy_cleanup(easy);
    }
    
    // 3. Create multi handle and use curl_multi_waitfds
    CURLM *multi = curl_multi_init();
    if (multi) {
        struct curl_waitfd ufds[2];
        unsigned int fd_count = 0;
        
        // Get file descriptor from input file
        int in_fd = fuzz_fileno(in_file);
        if (in_fd >= 0) {
            ufds[fd_count].fd = (curl_socket_t)in_fd;
            ufds[fd_count].events = CURL_WAIT_POLLIN;
            ufds[fd_count].revents = 0;
            fd_count++;
        }
        
        // Get file descriptor from output file
        int out_fd = fuzz_fileno(out_file);
        if (out_fd >= 0) {
            ufds[fd_count].fd = (curl_socket_t)out_fd;
            ufds[fd_count].events = CURL_WAIT_POLLOUT;
            ufds[fd_count].revents = 0;
            fd_count++;
        }
        
        // Use curl_multi_waitfds
        if (fd_count > 0) {
            unsigned int actual_fd_count = 0;
            curl_multi_waitfds(multi, ufds, fd_count, &actual_fd_count);
        }
        
        curl_multi_cleanup(multi);
    }
    
    // 4. Use curl_mfprintf to write to output file
    // Write some information about the input
    curl_mfprintf(out_file, "Input size: %zu bytes\n", size);
    curl_mfprintf(out_file, "First %d bytes: ", size > 10 ? 10 : (int)size);
    
    // Write first few bytes as hex
    size_t bytes_to_print = size > 10 ? 10 : size;
    for (size_t i = 0; i < bytes_to_print; i++) {
        curl_mfprintf(out_file, "%02x ", data[i]);
    }
    curl_mfprintf(out_file, "\n");
    
    // 5. Use curl_share_strerror for error reporting
    // Create a share handle to get an error
    CURLSH *share = curl_share_init();
    if (share) {
        // Try to set an option that might fail
        CURLSHcode share_result = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
        
        // Use curl_share_strerror to get error string
        const char *error_str = curl_share_strerror(share_result);
        curl_mfprintf(out_file, "Share error: %s\n", error_str);
        
        curl_share_cleanup(share);
    }
    
    // Clean up resources
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