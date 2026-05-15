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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_version","curl_msnprintf","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_multi_init","curl_multi_add_handle","curl_multi_poll","curl_easy_recv","curl_easy_ssls_export","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_nextheader","curl_free","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_msnprintf","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_multi_init","curl_multi_add_handle","curl_multi_poll","curl_easy_recv","curl_easy_ssls_export","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_nextheader","curl_free","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Helper function for curl_mvaprintf
static char*  id_000030_my_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 1: Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Step 2: Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Get curl version
    char *version_str = curl_version();
    if (version_str) {
        // Use curl_msnprintf
        char buffer[256];
        curl_msnprintf(buffer, sizeof(buffer), "Curl Version: %s", version_str);
        // No need to free version_str - it's static
    }
    
    // Step 4: Create easy handle and initialize
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Create mime structure and set filedata using input
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Write input data to a temporary file for curl_mime_filedata
            FILE *tmp_file = fopen("input_file", "wb");
            if (tmp_file) {
                fwrite(data, 1, size, tmp_file);
                assert_file_closed(&tmp_file);;
                
                // Use curl_mime_filedata API
                curl_mime_filedata(part, "input_file");
                
                // Clean up temp file
                remove("input_file");
            }
        }
    }
    
    // Step 6: Create multi handle
    CURLM *multi = curl_multi_init();
    if (multi && easy) {
        curl_multi_add_handle(multi, easy);
        
        // Step 7: Use curl_multi_poll with input file descriptor
        struct curl_waitfd extra_fd;
        extra_fd.fd = fuzz_fileno(in_file);
        extra_fd.events = CURL_WAIT_POLLIN;
        extra_fd.revents = 0;
        
        int running_handles = 0;
        int ret = 0;
        curl_multi_poll(multi, &extra_fd, 1, fuzz_int32_t_1, &ret);
        
        // Step 8: Try to receive data (even though we're not connected)
        char recv_buffer[1024];
        size_t n = 0;
        CURLcode recv_result = curl_easy_recv(easy, recv_buffer, sizeof(recv_buffer), &n);
        
        // Step 9: Try SSL export (will likely fail but we call it)
        CURLcode ssl_result = curl_easy_ssls_export(easy, NULL, NULL);
        
        // Step 10: Clean up multi handle
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
    }
    
    // Step 11: Use curl_easy_nextheader (no headers in this context, but we call it)
    struct curl_header *header = curl_easy_nextheader(easy, CURLH_HEADER, fuzz_int32_t_2, NULL);
    
    // Step 12: Use curl_mvaprintf with data content
    if (size > 0 && size < 100) {
        // Create a format string that includes some of the input data
        char* formatted =  id_000030_my_mvaprintf("Data[0]=%02x, Data[%zu]=%02x", 
                                      data[0], size-1, data[size-1]);
        if (formatted) {
            curl_free(formatted);
        }
    }
    
    // Step 13: Reset easy handle
    curl_easy_reset(easy);
    
    // Step 14: Clean up resources
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(easy);
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}