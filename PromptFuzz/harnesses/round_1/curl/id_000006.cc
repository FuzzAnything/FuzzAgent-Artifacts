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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 55, nr_unique_branch: 55
//<Quality> {"density":11,"unique_branches":{"curl_easy_strerror":[[50,3,50,26,0,0,4,0],[54,3,54,35,0,0,4,0],[63,3,63,32,0,0,4,0],[66,3,66,34,0,0,4,0],[69,3,69,31,0,0,4,0],[72,3,72,32,0,0,4,0],[75,3,75,29,0,0,4,0],[78,3,78,34,0,0,4,0],[87,3,87,31,0,0,4,0],[90,3,90,19,0,0,4,0],[99,3,99,35,0,0,4,0],[108,3,108,25,0,0,4,0],[111,3,111,27,0,0,4,0],[117,3,117,27,0,0,4,0],[132,3,132,31,0,0,4,0],[141,3,141,30,0,0,4,0],[144,3,144,32,0,0,4,0],[153,3,153,30,0,0,4,0],[156,3,156,32,0,0,4,0],[159,3,159,28,0,0,4,0],[162,3,162,34,0,0,4,0],[171,3,171,34,0,0,4,0],[174,3,174,35,0,0,4,0],[180,3,180,24,0,0,4,0],[183,3,183,29,0,0,4,0],[207,3,207,29,0,0,4,0],[210,3,210,30,0,0,4,0],[213,3,213,30,0,0,4,0],[222,3,222,23,0,0,4,0],[225,3,225,30,0,0,4,0],[231,3,231,28,0,0,4,0],[237,3,237,29,0,0,4,0],[246,3,246,19,0,0,4,0],[267,3,267,35,0,0,4,0],[273,3,273,32,0,0,4,0],[279,3,279,19,0,0,4,0],[285,3,285,19,0,0,4,0],[288,3,288,28,0,0,4,0],[291,3,291,32,0,0,4,0],[294,3,294,23,0,0,4,0],[297,3,297,26,0,0,4,0],[301,3,301,10,0,0,4,0]],"free_chain":[[291,9,291,10,0,0,4,1]],"free_formlist":[[157,9,157,20,0,0,4,0],[157,9,157,20,0,0,4,1]],"FormAdd":[[330,9,330,34,0,0,4,1],[553,5,553,27,0,0,4,0],[554,5,554,25,0,0,4,0],[556,10,556,46,0,0,4,0],[556,10,556,46,0,0,4,1],[558,15,558,79,0,0,4,1],[568,6,568,13,0,0,4,1],[571,6,571,12,0,0,4,0],[585,6,585,13,0,0,4,1],[311,26,311,31,30,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_easy_strerror","curl_mime_data","curl_mime_free","curl_easy_cleanup","curl_getenv","curl_free","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_formfree","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_easy_strerror","curl_mime_data","curl_mime_free","curl_easy_cleanup","curl_getenv","curl_free","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_formfree","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<7) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create a form
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Try to add form data from input
    if (size > 0) {
        // Create a temporary file with input data for form file upload
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            assert_file_closed(&in_file);;
            // Add form field with file data
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "fuzzfile",
                        CURLFORM_FILENAME, "input_file",
                        CURLFORM_BUFFER, "input_file",
                        CURLFORM_BUFFERPTR, data,
                        CURLFORM_BUFFERLENGTH, size,
                        CURLFORM_END);
        }
    }
    
    // Create a mime structure and add a part
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part && size > 0) {
                // Use first byte as potential error code for testing
                CURLcode test_error = static_cast<CURLcode>(data[0] % (CURL_LAST + 1));
                const char *error_str = curl_easy_strerror(test_error);
                
                // Add some data to mime part
                curl_mime_data(part, reinterpret_cast<const char*>(data), 
                              size > 1024 ? 1024 : size);
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy);
    }
    
    // Get environment variable
    char *env_var = curl_getenv(fuzz_str_1);
    if (env_var) {
        curl_free(env_var);
    }
    
    // Create multi handle for socket operations
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        CURL *easy2 = curl_easy_init();
        if (easy2) {
            // Set up easy handle for send/recv operations
            curl_easy_setopt(easy2, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(easy2, CURLOPT_CONNECT_ONLY, 1L);
            
            // Add to multi handle
            curl_multi_add_handle(multi_handle, easy2);
            
            int running_handles = 0;
            // Try socket operations (will likely fail but we call the APIs)
            curl_multi_socket_all(multi_handle, &running_handles);
            
            // Try to send some data
            if (size > 0) {
                size_t sent = 0;
                curl_easy_send(easy2, data, size > 128 ? 128 : size, &sent);
                
                // Try to receive (will likely fail but we call the API)
                uint8_t recv_buf[128];
                size_t received = 0;
                curl_easy_recv(easy2, recv_buf, sizeof(recv_buf), &received);
            }
            
            // Remove and clean up easy handle
            curl_multi_remove_handle(multi_handle, easy2);
            curl_easy_cleanup(easy2);
        }
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }
    
    // Clean up form
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Clean up URL handle
    curl_url_cleanup(url_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}