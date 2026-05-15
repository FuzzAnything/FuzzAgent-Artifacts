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
//<ID> 1010
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 4
//<Quality> {"density":24,"unique_branches":{"multi_posix_poll":[[1505,10,1505,20,0,0,4,1],[1507,10,1507,21,0,0,4,0]],"multi_wait":[[1605,8,1605,46,0,0,4,1],[1609,8,1609,47,0,0,4,0]]},"library_calls":["curl_multi_init","curl_easy_init","curl_multi_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_assign","curl_multi_socket_action","curl_multi_socket_action","curl_multi_wait","curl_easy_header","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup"],"critical_calls":["curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_assign","curl_multi_socket_action","curl_multi_socket_action","curl_multi_wait","curl_easy_header","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful testing
    
    // 1. Initialize curl
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
	return 0;
	}
	
    
    // 2. Create easy handle and set up a simple operation
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    // 3. Use fmemopen to create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create mime structure for multipart form data
    curl_mime *mime = curl_mime_init(easy);
    curl_mimepart *part = curl_mime_addpart(mime);
    
    // Use part of input data as mime name
    size_t name_len = size > 50 ? 50 : size;
    char *mime_name = (char *)malloc(name_len + 1);
    if (mime_name) {
        memcpy(mime_name, data, name_len);
        mime_name[name_len] = '\0';
        curl_mime_name(part, mime_name); // API 1
        free(mime_name);
    }
    
    // Use another part as mime data
    size_t data_offset = name_len;
    size_t data_len = size - data_offset;
    if (data_len > 0) {
        char *mime_data = (char *)malloc(data_len + 1);
        if (mime_data) {
            memcpy(mime_data, data + data_offset, data_len);
            mime_data[data_len] = '\0';
            curl_mime_data(part, mime_data, data_len);
            free(mime_data);
        }
    }
    
    // 6. Set up easy handle with mime data
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    
    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy);
    
    // 8. Use file descriptors for wait/assign operations
    curl_socket_t in_fd = fuzz_fileno(in_file);
    curl_socket_t out_fd = fuzz_fileno(out_file);
    
    // Create waitfd structures
    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[1].fd = out_fd;
    extra_fds[1].events = CURL_WAIT_POLLOUT;
    
    int running_handles = 0;
    
    // 9. Use curl_multi_assign (API 3)
    curl_multi_assign(multi_handle, in_fd, (void *)"input_descriptor");
    curl_multi_assign(multi_handle, out_fd, (void *)"output_descriptor");
    
    // 10. Use curl_multi_socket (API 5)
    curl_multi_socket(multi_handle, in_fd, &running_handles);
    curl_multi_socket(multi_handle, out_fd, &running_handles);
    
    // 11. Use curl_multi_wait (API 2)
    int numfds = 0;
    int timeout_ms = 100; // Short timeout for fuzzing
    curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &numfds);
    
    // 12. Try to get headers using curl_easy_header (API 4)
    struct curl_header *header = nullptr;
    // Try to get Content-Type header if it exists
    curl_easy_header(easy, "Content-Type", 0, CURLH_HEADER, 1, &header);
    
    // 13. Perform some multi operations
    curl_multi_perform(multi_handle, &running_handles);
    
    // 14. Clean up
    curl_multi_remove_handle(multi_handle, easy);
    
    // API 6: Free mime structure
    curl_mime_free(mime);
    
    // Final cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}