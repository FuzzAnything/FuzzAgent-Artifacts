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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["curl_global_init","curl_unescape","curl_url","curl_url_set","curl_url_get","curl_free","curl_free","curl_formadd","curl_formget","curl_easy_init","curl_easy_setopt","curl_easy_getinfo","curl_msnprintf","curl_easy_header","curl_multi_init","curl_multi_add_handle","curl_multi_fdset","curl_multi_remove_handle","curl_url_cleanup","curl_formfree","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_unescape","curl_url","curl_url_set","curl_url_get","curl_free","curl_free","curl_formadd","curl_formget","curl_easy_init","curl_easy_setopt","curl_easy_getinfo","curl_msnprintf","curl_easy_header","curl_multi_init","curl_multi_add_handle","curl_multi_fdset","curl_multi_remove_handle","curl_url_cleanup","curl_formfree","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*To create a fuzzer that exercises the specified curl APIs together in a meaningful way, I'll design a scenario where we:

1. Parse URL-encoded data from input
2. Build an HTTP form from parsed data
3. Process curl headers and information
4. Format output using curl's formatting functions

Here's the complete fuzzer implementation:

*/



// Custom callback for curl_formget
static size_t  id_000034_formget_callback(void *arg, const char *data, size_t size) {
    FILE *out_file = (FILE *)arg;
    if (out_file && data) {
        return fwrite(data, 1, size, out_file);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize variables we'll need
    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    CURLU *url_handle = NULL;
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *last_post = NULL;
    struct curl_header *header = NULL;
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    int ret = 0;
    
    // If we have enough data, create input file
    if (size > 0) {
        in_file = fmemopen((void *)data, size, "rb");
        if (!in_file) {
            goto cleanup;
        }
        
        // Create output file for writing
        out_file = fopen("output_file", "wb");
        if (!out_file) {
            goto cleanup;
        }
        
        // 1. Use curl_unescape on a portion of the input
        // Create a null-terminated string from first part of input
        size_t unescape_len = size > 100 ? 100 : size;
        char *input_str = (char *)malloc(unescape_len + 1);
        if (input_str) {
            memcpy(input_str, data, unescape_len);
            input_str[unescape_len] = '\0';
            
            char *unescaped = curl_unescape(input_str, unescape_len);
            if (unescaped) {
                // Use the unescaped string in URL parsing
                url_handle = curl_url();
                if (url_handle) {
                    // Try to set URL from unescaped data
                    curl_url_set(url_handle, CURLUPART_URL, unescaped, fuzz_uint32_t_1);
                    
                    // Get URL components
                    char *url_part = NULL;
                    curl_url_get(url_handle, CURLUPART_HOST, &url_part, fuzz_uint32_t_2);
                    if (url_part) {
                        curl_free(url_part);
                    }
                }
                curl_free(unescaped);
            }
            free(input_str);
        }
        
        // 2. Create a form with some data from input
        if (size >= 2) {
            // Use first byte as form name length, second as content length
            size_t name_len = data[0] % 50 + 1;
            size_t content_len = data[1] % 50 + 1;
            
            if (name_len + content_len + 2 <= size) {
                char *name = (char *)malloc(name_len + 1);
                char *content = (char *)malloc(content_len + 1);
                
                if (name && content) {
                    memcpy(name, data + 2, name_len);
                    name[name_len] = '\0';
                    memcpy(content, data + 2 + name_len, content_len);
                    content[content_len] = '\0';
                    
                    // Add form field
                    curl_formadd(&formpost, &last_post,
                                CURLFORM_COPYNAME, name,
                                CURLFORM_COPYCONTENTS, content,
                                CURLFORM_END);
                    
                    free(name);
                    free(content);
                }
            }
        }
        
        // 3. Use curl_formget to output form data
        if (formpost) {
            curl_formget(formpost, out_file,  id_000034_formget_callback);
        }
        
        // 4. Create easy handle and get info
        easy_handle = curl_easy_init();
        if (easy_handle) {
            // Set some options
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
            
            // Get information from easy handle
            char *effective_url = NULL;
            curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
            
            // 5. Use curl_mvsprintf to format a string
            char buffer[256];
            va_list args;
            // We need to create va_list for demonstration
            // For simplicity, we'll use a fixed format
            const char *format = "CURL handle created. Size: %zu";
            int printed = curl_msnprintf(buffer, sizeof(buffer), format, size);
            
            // 6. Try to get header (may fail since no transfer occurred)
            curl_easy_header(easy_handle, fuzz_str_3, fuzz_uint64_t_4, 
                            CURLH_HEADER, fuzz_int32_t_5, &header);
        }
        
        // 7. Create multi handle and use fdset
        multi_handle = curl_multi_init();
        if (multi_handle && easy_handle) {
            curl_multi_add_handle(multi_handle, easy_handle);
            
            fd_set read_fds, write_fds, exc_fds;
            int max_fd = -1;
            
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            FD_ZERO(&exc_fds);
            
            // Get file descriptors (won't actually have any without performing)
            curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
            
            // Remove handle from multi
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
        
        // Flush output file
        fflush(out_file);
    }
    
cleanup:
    // 8. Clean up URL handle (matching curl_url_cleanup API usage)
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    // Clean up form
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Clean up handles
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    // Close files
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}