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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 6
//<Quality> {"density":20,"unique_branches":{"curl_multi_get_handles":[[3804,6,3804,7,0,0,4,0],[3808,8,3808,58,0,0,4,0],[3812,12,3812,33,0,0,4,0],[3812,12,3812,33,0,0,4,1],[3814,15,3814,69,0,0,4,0],[3814,15,3814,69,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_setopt","curl_multi_setopt","curl_multi_setopt","curl_multi_fdset","curl_multi_wait","curl_easy_escape","curl_easy_nextheader","curl_easy_nextheader","curl_free","curl_multi_get_handles","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_multi_setopt","curl_multi_setopt","curl_multi_setopt","curl_multi_fdset","curl_multi_wait","curl_easy_escape","curl_easy_nextheader","curl_easy_nextheader","curl_free","curl_multi_get_handles","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create easy handle and add to multi handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set up file descriptor sets for curl_multi_fdset
    fd_set read_fds, write_fds, exc_fds;
    int max_fd = -1;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exc_fds);

    // Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = -1;
    if (out_file) {
        out_fd = fuzz_fileno(out_file);
    }

    // Set up extra file descriptors for curl_multi_wait
    struct curl_waitfd extra_fds[2];
    int extra_nfds = 0;
    
    // Add input file descriptor to extra_fds
    if (in_fd >= 0) {
        extra_fds[extra_nfds].fd = in_fd;
        extra_fds[extra_nfds].events = CURL_WAIT_POLLIN;
        extra_fds[extra_nfds].revents = 0;
        extra_nfds++;
    }

    // Add output file descriptor to extra_fds
    if (out_fd >= 0) {
        extra_fds[extra_nfds].fd = out_fd;
        extra_fds[extra_nfds].events = CURL_WAIT_POLLOUT;
        extra_fds[extra_nfds].revents = 0;
        extra_nfds++;
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 5L);

    // Get file descriptor set from multi handle
    curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);

    // Wait for activity
    int numfds = 0;
    int ret = 0;
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, fuzz_int32_t_1, &numfds);

    // Process input data as string for escaping
    char *input_string = (char *)malloc(size + 1);
    if (input_string) {
        memcpy(input_string, data, size);
        input_string[size] = '\0';
        
        // Escape the string
        char *escaped_string = curl_easy_escape(easy_handle, input_string, size);
        if (escaped_string) {
            // Get headers from easy handle
            struct curl_header *header = NULL;
            unsigned int origin = 0; // CURLH_HEADER
            int request = 0;
            
            // Iterate through headers
            header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_2, fuzz_int32_t_3, header);
            while (header) {
                // Process each header
                header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_4, fuzz_int32_t_5, header);
            }
            
            // Free the escaped string
            curl_free(escaped_string);
        }
        free(input_string);
    }

    // Get all handles from multi handle
    CURL **handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        // Process handles if needed
        curl_free(handles);
    }

    // Clean up
    if (out_file) assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}