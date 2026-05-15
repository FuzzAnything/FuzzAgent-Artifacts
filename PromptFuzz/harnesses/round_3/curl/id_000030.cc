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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["curl_global_init","curl_global_sslset","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_filedata","curl_mime_free","curl_formadd","curl_formget","curl_formfree","curl_easy_pause","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_sslset","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_filedata","curl_mime_free","curl_formadd","curl_formget","curl_formfree","curl_easy_pause","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Try to set SSL backend (though we likely won't have multiple backends in fuzzing)
    const curl_ssl_backend ***avail = nullptr;
    curl_global_sslset(CURLSSLBACKEND_NONE, nullptr, avail);
    
    // Create a multi handle and set some options
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 10L);
    }
    
    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Use the input data as a string for various operations
        if (size > 0) {
            // Create a null-terminated copy of the input data
            char *input_str = (char *)malloc(size + 1);
            if (input_str) {
                memcpy(input_str, data, size);
                input_str[size] = '\0';
                
                // Create a FILE* from the input data
                FILE *in_file = fmemopen((void *)data, size, "rb");
                if (in_file) {
                    // Create a mime structure
                    curl_mime *mime = curl_mime_init(easy_handle);
                    if (mime) {
                        curl_mimepart *part = curl_mime_addpart(mime);
                        if (part) {
                            // Use the APIs with the input data
                            curl_mime_name(part, fuzz_str_1);
                            curl_mime_filename(part, input_str); // Using input as filename
                            
                            // Create headers for mime part
                            struct curl_slist *headers = nullptr;
                            headers = curl_slist_append(headers, fuzz_str_2);
                            curl_mime_headers(part, headers, 1); // Takes ownership
                            
                            // Try to use filedata with a temp file
                            FILE *out_file = fopen("output_file", "wb");
                            if (out_file) {
                                fwrite(data, 1, size, out_file);
                                assert_file_closed(&out_file);;
                                curl_mime_filedata(part, fuzz_str_3);
                            }
                        }
                        curl_mime_free(mime);
                    }
                    
                    // Create form data using the older API
                    struct curl_httppost *formpost = nullptr;
                    struct curl_httppost *last_post = nullptr;
                    
                    curl_formadd(&formpost, &last_post,
                                CURLFORM_COPYNAME, "field1",
                                CURLFORM_COPYCONTENTS, input_str,
                                CURLFORM_END);
                    
                    // Use curl_formget to write form data to a buffer
                    if (formpost) {
                        // Create a callback that writes to a file
                        FILE *form_out = fopen("form_output", "wb");
                        if (form_out) {
                            auto append_callback = [](void *arg, const char *buf, size_t len) -> size_t {
                                return fwrite(buf, 1, len, (FILE*)arg);
                            };
                            
                            curl_formget(formpost, form_out, (curl_formget_callback)append_callback);
                            assert_file_closed(&form_out);;
                        }
                        curl_formfree(formpost);
                    }
                    
                    assert_file_closed(&in_file);;
                }
                
                free(input_str);
            }
        }
        
        // Use curl_easy_pause (even though we're not actively transferring)
        curl_easy_pause(easy_handle, CURLPAUSE_ALL);
        
        curl_easy_cleanup(easy_handle);
    }
    
    // Clean up multi handle
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    // Clean up global state
    curl_global_cleanup();
    
    // Remove temporary files
    remove("output_file");
    remove("form_output");
    
    assert_file_name_closed("output_file");
	return 0;
}