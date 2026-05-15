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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 111, nr_unique_branch: 27
//<Quality> {"density":37,"unique_branches":{"Curl_getinfo":[[652,3,652,23,0,0,4,0],[654,8,654,21,0,0,4,0]],"getinfo_double":[[495,3,495,27,0,0,4,1],[496,3,496,32,0,0,4,1],[497,3,497,29,0,0,4,1],[498,3,498,32,0,0,4,1],[499,3,499,33,0,0,4,1],[500,3,500,35,0,0,4,1],[501,3,501,30,0,0,4,1],[502,3,502,31,0,0,4,1],[503,3,503,29,0,0,4,1],[512,3,512,10,0,0,4,0],[517,3,517,27,0,0,4,1],[520,3,520,32,0,0,4,1],[523,3,523,29,0,0,4,1],[526,3,526,32,0,0,4,1],[529,3,529,33,0,0,4,1],[532,3,532,35,0,0,4,1],[535,3,535,28,0,0,4,1],[538,3,538,30,0,0,4,1],[541,3,541,31,0,0,4,1],[544,3,544,29,0,0,4,1],[547,3,547,40,0,0,4,0],[548,22,548,50,0,0,4,1],[551,3,551,38,0,0,4,1],[555,3,555,30,0,0,4,1],[559,3,559,10,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_perform","curl_multi_info_read","curl_easy_getinfo","curl_easy_header","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_unescape","curl_free","curl_easy_getinfo","curl_easy_getinfo","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_perform","curl_multi_info_read","curl_easy_getinfo","curl_easy_header","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_unescape","curl_free","curl_easy_getinfo","curl_easy_getinfo","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate an event processing HTTP responses with multipart MIME data:

*/


extern "C" {
}

// Helper function to split input data
static void  id_000014_split_data(const uint8_t* data, size_t size, 
                      const uint8_t** part1, size_t* size1,
                      const uint8_t** part2, size_t* size2) {
    if (size == 0) {
        *part1 = nullptr;
        *size1 = 0;
        *part2 = nullptr;
        *size2 = 0;
        return;
    }
    
    size_t split = size / 2;
    *part1 = data;
    *size1 = split;
    *part2 = data + split;
    *size2 = size - split;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<73) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle for async operations
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a file with the input data for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int running_handles = 0;
    
    // Split input data for different purposes
    const uint8_t* part1;
    const uint8_t* part2;
    size_t size1, size2;
     id_000014_split_data(data, size, &part1, &size1, &part2, &size2);
    
    // Use part1 to create MIME structure with subparts
    curl_mime* mime = curl_mime_init(easy_handle);
    if (mime) {
        // Create main mime part
        curl_mimepart* main_part = curl_mime_addpart(mime);
        if (main_part) {
            // Set some basic properties
            if (size1 > 0) {
                // Use part1 data for main part
                curl_mime_data(main_part, (const char*)part1, size1);
            }
            
            // Create subparts mime structure
            curl_mime* subparts = curl_mime_init(easy_handle);
            if (subparts) {
                // Add a subpart with part2 data
                curl_mimepart* subpart = curl_mime_addpart(subparts);
                if (subpart && size2 > 0) {
                    curl_mime_data(subpart, (const char*)part2, size2);
                    
                    // Set subpart properties
                    curl_mime_name(subpart, fuzz_str_1);
                    curl_mime_type(subpart, fuzz_str_2);
                }
                
                // Use curl_mime_subparts API to attach subparts to main part
                curl_mime_subparts(main_part, subparts);
                
                // Set MIME as POST data
                curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
            }
        }
    }
    
    // Setup easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/upload");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Get file descriptor from input file
    int fd = fuzz_fileno(in_file);
    
    // Simulate socket action with curl_multi_socket_action
    // We use the file descriptor as a socket for demonstration
    curl_multi_socket_action(multi_handle, fd, fuzz_int32_t_3, &running_handles);
    
    // Perform multi handle operations
    int still_running = 1;
    while (still_running) {
        CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
        if (!still_running)
            break;
        
        // Process messages
        int msgs_in_queue;
        CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg && msg->msg == CURLMSG_DONE) {
            // Get info about completed transfer
            long response_code = 0;
            curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
            
            // Try to get a header from response
            struct curl_header* header = nullptr;
            curl_easy_header(easy_handle, fuzz_str_4, fuzz_uint64_t_5, 
                           CURLH_HEADER, fuzz_int32_t_6, &header);
            
            break;
        }
    }
    
    // Reset easy handle to reuse it
    curl_easy_reset(easy_handle);
    
    // Setup for second request
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/test");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    
    // Add back to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Perform another operation
    running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Use curl_unescape on some test data
    if (size > 0) {
        // Take first few bytes as escaped string
        size_t test_len = size > 100 ? 100 : size;
        char* unescaped = curl_unescape((const char*)data, test_len);
        if (unescaped) {
            // Can do something with unescaped data here
            curl_free(unescaped);
        }
    }
    
    // Get more info using curl_easy_getinfo
    double content_length = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);
    
    char* effective_url = nullptr;
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}