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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"curl_mime_addpart":[[1228,8,1228,22,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_filedata","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_addpart","curl_mime_subparts","curl_mime_free","curl_multi_add_handle","curl_multi_timeout","curl_multi_assign","curl_multi_socket_action","curl_ws_meta","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_addpart","curl_mime_subparts","curl_mime_free","curl_multi_add_handle","curl_multi_timeout","curl_multi_assign","curl_multi_socket_action","curl_ws_meta","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a temporary file with the input data
    FILE *temp_file = fmemopen((void *)data, size, "rb");
    if (!temp_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&temp_file);
	return 0;
    }
    
    // Get file descriptor from the temp file
    curl_socket_t socket_fd = fuzz_fileno(temp_file);
    
    // Create a mime structure for multipart data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        assert_file_closed(&temp_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&temp_file);
	assert_fd_closed(socket_fd);
	return 0;
    }
    
    // Create first mime part
    curl_mimepart *part1 = curl_mime_addpart(mime);
    if (part1) {
        // Set filedata for the first mime part using the temp file
        // We'll write the data to a file first since curl_mime_filedata expects a filename
        FILE *data_file = fopen("input_file", "wb");
        if (data_file) {
            fwrite(data, 1, size, data_file);
            assert_file_closed(&data_file);;
            
            // Use the file as data source for mime part
            curl_mime_filedata(part1, "input_file");
        }
    }
    
    // Create a sub-mime structure
    curl_mime *submime = curl_mime_init(easy_handle);
    if (submime) {
        // Create a part in the submime
        curl_mimepart *subpart = curl_mime_addpart(submime);
        if (subpart) {
            // Add some data to the subpart
            curl_mime_data(subpart, "fuzz data", 9);
        }
        
        // Create another part in the main mime
        curl_mimepart *part2 = curl_mime_addpart(mime);
        if (part2 && part1) {
            // Attach the submime as subparts to part2
            curl_mime_subparts(part2, submime);
        }
        
        // Free the submime (it's now owned by part2)
        curl_mime_free(submime);
    }
    
    // Add the easy handle to the multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Get timeout value from multi handle
    long timeout_ms;
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_res; // Ignore result for fuzzing
    
    // Assign socket data to the multi handle
    void *socket_ptr = malloc(16);
    if (socket_ptr) {
        CURLMcode assign_res = curl_multi_assign(multi_handle, socket_fd, socket_ptr);
        (void)assign_res; // Ignore result for fuzzing
    }
    
    // Try socket operation (this would normally be used with actual sockets)
    int running_handles;
    CURLMcode socket_res = curl_multi_socket(multi_handle, socket_fd, &running_handles);
    (void)socket_res; // Ignore result for fuzzing
    
    // Get WebSocket metadata (will be NULL since we're not in a WebSocket transfer)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(easy_handle);
    (void)ws_meta; // Use variable to avoid unused warning
    
    // Clean up
    curl_mime_free(mime);
    
    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Cleanup handles
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Close temp file
    assert_file_closed(&temp_file);;
    
    // Remove temporary file
    remove("input_file");
    
    // Clean up socket pointer if allocated
    if (socket_ptr) {
        free(socket_ptr);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(socket_fd);
	return 0;
}