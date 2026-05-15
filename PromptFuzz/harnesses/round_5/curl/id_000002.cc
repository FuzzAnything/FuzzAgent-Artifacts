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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_escape","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_perform","curl_multi_wakeup","curl_easy_getinfo","curl_easy_header","curl_mprintf","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_escape","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_perform","curl_multi_wakeup","curl_easy_getinfo","curl_easy_header","curl_mprintf","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE pointer to write output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create easy handle for HTTP operations
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create multi handle for concurrent operations
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use curl_easy_escape to escape part of input data
    if (size > 0) {
        // Create null-terminated string from first part of data
        size_t escape_len = size > 100 ? 100 : size;
        std::vector<char> input_str(escape_len + 1);
        memcpy(input_str.data(), data, escape_len);
        input_str[escape_len] = '\0';
        
        char *escaped = curl_easy_escape(easy, input_str.data(), escape_len);
        if (escaped) {
            // Use the escaped string somehow (e.g., in URL)
            curl_free(escaped);
        }
    }
    
    // 2. Create mime structure and set filedata
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use the input file for mime filedata
            // First write input data to a temporary file
            FILE *tmp_file = fopen("input_file", "wb");
            if (tmp_file) {
                fwrite(data, 1, size, tmp_file);
                assert_file_closed(&tmp_file);;
                
                // Set mime filedata to the temporary file
                curl_mime_filedata(part, "input_file");
            }
        }
    }
    
    // 3. Set up easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    
    // 4. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 5. Use curl_multi_wait with file descriptors
    int file_descriptor = fuzz_fileno(in_file);
    if (file_descriptor >= 0) {
        struct curl_waitfd wait_fd;
        wait_fd.fd = file_descriptor;
        wait_fd.events = CURL_WAIT_POLLIN;
        wait_fd.revents = 0;
        
        int ret = 0;
        int numfds = 0;
        curl_multi_wait(multi, &wait_fd, 1, 1000, &ret);
    }
    
    // 6. Perform operations
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);
    
    // 7. Wake up multi handle (useful if we had async operations)
    curl_multi_wakeup(multi);
    
    // 8. Get information from easy handle
    long http_code = 0;
    curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &http_code);
    
    // 9. Try to get headers using curl_easy_header
    struct curl_header *header = nullptr;
    curl_easy_header(easy, "Content-Type", 0, CURLH_HEADER, 1, &header);
    
    // 10. Use curl_mprintf for formatted output
    char buffer[256];
    curl_mprintf(buffer, "HTTP Response: %ld, Input size: %zu", http_code, size);
    
    // 11. Clean up mime structure
    if (mime) {
        curl_mime_free(mime);
    }
    
    // 12. Remove easy handle from multi
    curl_multi_remove_handle(multi, easy);
    
    // 13. Clean up multi handle
    curl_multi_cleanup(multi);
    
    // 14. Clean up easy handle
    curl_easy_cleanup(easy);
    
    // 15. Close file handles
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // 16. Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    // 17. Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(file_descriptor);
	return 0;
}