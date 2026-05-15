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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["curl_global_init","curl_version","curl_easy_option_by_name","curl_formadd","curl_escape","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_getinfo","curl_multi_init","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_formfree","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_easy_option_by_name","curl_formadd","curl_escape","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_getinfo","curl_multi_init","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_formfree","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Get curl version
    char *version = curl_version();
    if (version) {
        // Use the version string in some way
        // (just accessing it is enough for fuzzing)
    }
    
    // 2. Look up an easy option by name
    const struct curl_easyoption *opt = curl_easy_option_by_name("URL");
    if (opt) {
        // Option found - we'll use this later
    }
    
    // 3. Create a form using part of the input data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use first third of data for form name
    size_t name_len = size / 3;
    char *form_name = (char*)malloc(name_len + 1);
    if (form_name) {
        memcpy(form_name, data, name_len);
        form_name[name_len] = '\0';
        
        // Use second third for form content
        size_t content_len = size / 3;
        char *form_content = (char*)malloc(content_len + 1);
        if (form_content) {
            memcpy(form_content, data + name_len, content_len);
            form_content[content_len] = '\0';
            
            // Add form field
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, form_name,
                        CURLFORM_COPYCONTENTS, form_content,
                        CURLFORM_END);
            
            free(form_content);
        }
        free(form_name);
    }
    
    // 4. Escape a string using part of the input data
    size_t escape_len = size / 4;
    if (escape_len > 0) {
        char *to_escape = (char*)malloc(escape_len + 1);
        if (to_escape) {
            memcpy(to_escape, data + (size / 2), escape_len);
            to_escape[escape_len] = '\0';
            
            char *escaped = curl_escape(to_escape, (int)escape_len);
            if (escaped) {
                // Use escaped string
                curl_free(escaped);
            }
            free(to_escape);
        }
    }
    
    // 5. Create easy handle and get info
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set some options
        curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null");
        
        // Get info from the easy handle
        long response_code = 0;
        curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &response_code);
        
        // 6. Create multi handle and poll
        CURLM *multi = curl_multi_init();
        if (multi) {
            curl_multi_add_handle(multi, easy);
            
            // Create waitfd structure
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                struct curl_waitfd wait_fd;
                wait_fd.fd = fuzz_fileno(in_file);
                wait_fd.events = CURL_WAIT_POLLIN;
                wait_fd.revents = 0;
                
                int still_running = 0;
                int ret = 0;
                
                // Poll with timeout
                curl_multi_poll(multi, &wait_fd, 1, 100, &ret);
                
                // Perform (though this won't actually transfer due to file:// URL)
                curl_multi_perform(multi, &still_running);
                
                assert_file_closed(&in_file);;
            }
            
            // Clean up multi
            curl_multi_remove_handle(multi, easy);
            curl_multi_cleanup(multi);
        }
        
        curl_easy_cleanup(easy);
    }
    
    // Clean up form
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}