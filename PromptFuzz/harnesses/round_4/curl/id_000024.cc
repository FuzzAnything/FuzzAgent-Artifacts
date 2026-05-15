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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"Curl_getformdata":[[820,14,820,19,0,0,4,0]],"curl_mime_data":[[1281,8,1281,40,0,0,4,0]],"curl_multi_info_read":[[2985,6,2985,39,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_slist_append","curl_slist_append","curl_escape","curl_unescape","curl_free","curl_free","curl_formadd","curl_formget","curl_formfree","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_slist_free_all","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_slist_append","curl_slist_append","curl_escape","curl_unescape","curl_free","curl_free","curl_formadd","curl_formget","curl_formfree","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_slist_free_all","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to create a va_list for curl_mvfprintf
static void  id_000024_call_curl_mvfprintf(FILE* fd, const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

// Callback for curl_formget
static size_t  id_000024_formget_callback(void* arg, const char* data, size_t size) {
    FILE* out_file = static_cast<FILE*>(arg);
    if (out_file && data && size) {
        fwrite(data, 1, size, out_file);
    }
    return size;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    // 1. Use curl_mvfprintf to write to output file
     id_000024_call_curl_mvfprintf(out_file, "Processing fuzzer input of size: %zu\n", size);
    
    // 2. Create and use curl_share_init
    CURLSH* share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        // Clean up share handle
        curl_share_cleanup(share);
    }
    
    // 3. Create and use linked list with curl_slist
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, fuzz_str_1);
    headers = curl_slist_append(headers, fuzz_str_2);
    
    // Use the list in some operation (simulated)
    if (headers) {
         id_000024_call_curl_mvfprintf(out_file, "Header count in list: at least 2\n");
    }
    
    // 4. Create and use curl_escape/curl_unescape
    // Extract a string from input (first half of data)
    size_t str_len = size / 2;
    if (str_len > 0) {
        char* input_str = (char*)malloc(str_len + 1);
        if (input_str) {
            memcpy(input_str, data, str_len);
            input_str[str_len] = '\0';
            
            // Escape the string
            char* escaped = curl_escape(input_str, (int)str_len);
            if (escaped) {
                 id_000024_call_curl_mvfprintf(out_file, "Escaped string: %s\n", escaped);
                
                // Unescape it back
                char* unescaped = curl_unescape(escaped, 0);
                if (unescaped) {
                     id_000024_call_curl_mvfprintf(out_file, "Unescaped string: %s\n", unescaped);
                    curl_free(unescaped);
                }
                curl_free(escaped);
            }
            free(input_str);
        }
    }
    
    // 5. Create and use curl_formget
    // Create a simple form
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    
    // Add a field to the form using the second half of input data
    if (size > 1) {
        size_t field_len = size - (size / 2);
        if (field_len > 0) {
            char* field_data = (char*)malloc(field_len + 1);
            if (field_data) {
                memcpy(field_data, data + (size / 2), field_len);
                field_data[field_len] = '\0';
                
                // Add form field
                curl_formadd(&formpost, &lastptr,
                           CURLFORM_COPYNAME, "fuzzfield",
                           CURLFORM_COPYCONTENTS, field_data,
                           CURLFORM_END);
                
                // Use curl_formget to write form data to output file
                if (formpost) {
                    rewind(out_file); // Reset file position for form data
                    curl_formget(formpost, out_file,  id_000024_formget_callback);
                    
                    // Clean up form
                    curl_formfree(formpost);
                }
                
                free(field_data);
            }
        }
    }
    
    // 6. Create and use curl_multi_info_read
    // Create a multi handle and easy handle
    CURLM* multi_handle = curl_multi_init();
    CURL* easy_handle = curl_easy_init();
    
    if (multi_handle && easy_handle) {
        // Set some options on the easy handle
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Perform (this won't actually connect in fuzzing context)
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Read messages from multi handle
        int msgs_in_queue = 0;
        CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg) {
             id_000024_call_curl_mvfprintf(out_file, "Multi message type: %d\n", msg->msg);
        }
        
        // Clean up
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
    }
    
    // 7. Clean up curl_slist
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up curl
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}