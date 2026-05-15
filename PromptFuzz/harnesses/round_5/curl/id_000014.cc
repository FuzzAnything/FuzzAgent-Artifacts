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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_waitfds","curl_multi_perform","curl_easy_recv","curl_easy_option_next","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_waitfds","curl_multi_perform","curl_easy_recv","curl_easy_option_next","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

// Helper function for curl_mvaprintf
char*  id_000014_my_curl_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

// Helper function for curl_mvsprintf
int  id_000014_my_curl_mvsprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a file from input data for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
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
    
    // Initialize easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up easy handle options
    // Use a dummy URL since we're focusing on receiving data
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/");
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // Set read function to read from our input file
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, fread);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    
    // Set the size of the data to upload
    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Variables for multi perform
    int still_running = 1;
    int numfds = 0;
    
    // Use curl_multi_waitfds to wait for activity
    struct curl_waitfd ufds[2];
    ufds[0].fd = in_fd;
    ufds[0].events = CURL_WAIT_POLLIN;
    ufds[0].revents = 0;
    
    ufds[1].fd = out_fd;
    ufds[1].events = CURL_WAIT_POLLOUT;
    ufds[1].revents = 0;
    
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
    
    // Try to perform multi operations
    curl_multi_perform(multi_handle, &still_running);
    
    // Now try to receive data using curl_easy_recv
    char buffer[1024];
    size_t n = 0;
    CURLcode recv_result = curl_easy_recv(easy_handle, buffer, sizeof(buffer), &n);
    
    // Use curl_easy_option_next to iterate through options
    const struct curl_easyoption* option = nullptr;
    int option_count = 0;
    while ((option = curl_easy_option_next(option)) != nullptr && option_count < 5) {
        // Use curl_mvaprintf to format option info
        char* formatted =  id_000014_my_curl_mvaprintf("Option %d: %s (ID: %d)", 
                                           option_count + 1, 
                                           option->name, 
                                           option->id);
        if (formatted) {
            // Write to output file
            fwrite(formatted, 1, strlen(formatted), out_file);
            curl_free(formatted);
        }
        
        // Also use curl_mvsprintf
        char local_buffer[256];
         id_000014_my_curl_mvsprintf(local_buffer, "Type: %d, Flags: %u", 
                         option->type, option->flags);
        fwrite(local_buffer, 1, strlen(local_buffer), out_file);
        
        option_count++;
    }
    
    // Write any received data to output file
    if (recv_result == CURLE_OK && n > 0) {
        fwrite(buffer, 1, n, out_file);
    }
    
    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}