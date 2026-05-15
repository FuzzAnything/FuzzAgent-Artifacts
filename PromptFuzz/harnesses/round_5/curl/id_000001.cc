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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_escape","curl_mprintf","curl_free","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_header","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_escape","curl_mprintf","curl_free","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_header","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Write input data to a temporary file for curl_mime_filedata
            FILE *in_file = fopen("input_file", "wb");
            if (in_file) {
                fwrite(data, 1, size, in_file);
                assert_file_closed(&in_file);;
                
                // Use curl_mime_filedata with the input file
                curl_mime_filedata(part, "input_file");
            }
        }
    }
    
    // Use curl_easy_escape with input data (as string)
    // Ensure null-terminated string
    char *str_data = new char[size + 1];
    memcpy(str_data, data, size);
    str_data[size] = '\0';
    char *escaped = curl_easy_escape(easy, str_data, size);
    delete[] str_data;
    
    // Use curl_mprintf with escaped string
    if (escaped) {
        curl_mprintf("Escaped: %s\n", escaped);
        curl_free(escaped);
    }
    
    // Set up easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    if (mime) {
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Use curl_multi_wait
    struct curl_waitfd extra_fds;
    extra_fds.fd = 0; // stdin
    extra_fds.events = CURL_WAIT_POLLIN;
    extra_fds.revents = 0;
    int ret;
    curl_multi_wait(multi, &extra_fds, 1, 1000, &ret);
    
    // Use curl_multi_wakeup
    curl_multi_wakeup(multi);
    
    // Perform some multi operations
    int running_handles;
    curl_multi_perform(multi, &running_handles);
    
    // Use curl_easy_getinfo
    double total_time;
    curl_easy_getinfo(easy, CURLINFO_TOTAL_TIME, &total_time);
    
    // Use curl_easy_header (try to get a header)
    struct curl_header *header = nullptr;
    curl_easy_header(easy, "Content-Type", 0, CURLH_HEADER, 0, &header);
    
    // Clean up multi handle
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    
    // Clean up easy handle
    curl_easy_cleanup(easy);
    
    // Clean up mime
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove temporary file
    remove("input_file");
    
    curl_global_cleanup();
    
    assert_file_name_closed("input_file");
	return 0;
}