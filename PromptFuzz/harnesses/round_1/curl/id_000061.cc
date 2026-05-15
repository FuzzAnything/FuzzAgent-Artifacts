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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 65.333336, nr_unique_branch: 13
//<Quality> {"density":28,"unique_branches":{"setopt_set_timeout_ms":[[69,6,69,12,0,0,4,1]],"Curl_timeleft_now_ms":[[119,8,119,21,0,0,4,0],[129,8,129,20,0,0,4,0]],"setopt_long_net":[[883,3,883,26,0,0,4,0],[887,3,887,33,0,0,4,0]],"multi_runsingle":[[2515,8,2515,58,0,0,4,0]],"multi_handle_timeout":[[1806,6,1806,20,0,0,4,0],[1809,8,1809,32,0,0,4,0],[1813,8,1813,41,0,0,4,1],[1819,10,1819,23,0,0,4,1],[1834,8,1834,18,0,0,4,0],[1836,10,1836,34,0,0,4,1]],"curl_multi_add_handle":[[494,6,494,22,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_unescape","curl_free","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_unescape","curl_free","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Helper function for curl_mvsnprintf
static int  id_000061_helper_mvsnprintf(char *buffer, size_t maxlen, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlen, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi and easy handles
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    
    // Create mime structure for multipart data
    curl_mime *mime = curl_mime_init(easy_handle);
    curl_mime *submime = curl_mime_init(easy_handle);
    
    if (mime && submime) {
        // Create main mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input data for mime data
            if (size > 0) {
                char *data_copy = (char *)malloc(size);
                if (data_copy) {
                    memcpy(data_copy, data, size);
                    curl_mime_data(part, data_copy, size);
                    free(data_copy);
                }
            }
            
            // Create subpart for nested mime structure
            curl_mimepart *subpart = curl_mime_addpart(submime);
            if (subpart && size > 0) {
                curl_mime_data(subpart, (const char *)data, 
                              size > 100 ? 100 : size); // Use first 100 bytes
            }
            
            // Attach submime to main part
            curl_mime_subparts(part, submime);
        }
        
        // Set easy handle options
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    }
    
    // Perform easy transfer
    curl_easy_perform(easy_handle);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Get file descriptor for assignment
    int fd = fuzz_fileno(in_file);
    
    // Assign socket to multi handle (using file descriptor as socket)
    curl_multi_assign(multi_handle, (curl_socket_t)fd, nullptr);
    
    // Perform socket action with the assigned socket
    int running_handles = 0;
    curl_multi_socket_action(multi_handle, (curl_socket_t)fd, fuzz_int32_t_1, &running_handles);
    
    // Unescape some data (use first 100 bytes or full size if smaller)
    if (size > 0) {
        size_t unescape_len = size > 100 ? 100 : size;
        char *unescaped = curl_unescape((const char *)data, (int)unescape_len);
        if (unescaped) {
            // Use curl_mvsnprintf with the unescaped data
            char buffer[256];
             id_000061_helper_mvsnprintf(buffer, sizeof(buffer), "Unescaped: %s", unescaped);
            curl_free(unescaped);
        }
    }
    
    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up resources
    if (out_file) assert_file_closed(&out_file);;
    if (in_file) assert_file_closed(&in_file);;
    if (mime) curl_mime_free(mime);
    // Note: submime is freed automatically when parent mime is freed
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}