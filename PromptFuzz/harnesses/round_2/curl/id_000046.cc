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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"out_number":[[747,8,747,14,0,0,4,0],[802,6,802,12,0,0,4,0],[44,8,44,41,9,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_data","curl_multi_timeout","curl_free","curl_easy_option_by_id","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_data","curl_multi_timeout","curl_free","curl_easy_option_by_id","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to create va_list for formatting functions
static int  id_000046_call_curl_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvprintf(format, args);
    va_end(args);
    return result;
}

static char*  id_000046_call_curl_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

static int  id_000046_call_curl_mvsprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a multi handle for timeout operations
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle for MIME operations
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for output
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

    // Create a MIME structure
    curl_mime* mime = curl_mime_init(easy_handle);
    if (mime) {
        // Add a MIME part
        curl_mimepart* part = curl_mime_addpart(mime);
        if (part) {
            // Use curl_mime_type API with formatted string from input data
            // Use first 50 bytes as MIME type string (or less if input is smaller)
            size_t mime_type_len = size > 50 ? 50 : size;
            char* mime_type_buf = (char*)malloc(mime_type_len + 1);
            if (mime_type_buf) {
                memcpy(mime_type_buf, data, mime_type_len);
                mime_type_buf[mime_type_len] = '\0';
                
                // Call curl_mime_type with the extracted string
                curl_mime_type(part, mime_type_buf);
                free(mime_type_buf);
            }

            // Add some data to the MIME part using the input file
            fseek(in_file, 0, SEEK_SET);
            char buffer[1024];
            size_t read_size = fread(buffer, 1, sizeof(buffer), in_file);
            if (read_size > 0) {
                curl_mime_data(part, buffer, read_size);
            }
        }
    }

    // Use curl_multi_timeout API
    long timeout = 0;
    CURLMcode timeout_result = curl_multi_timeout(multi_handle, &timeout);

    // Use formatting APIs with the timeout value
    if (timeout_result == CURLM_OK) {
        // Use curl_mvprintf to print to stdout
         id_000046_call_curl_mvprintf("Multi timeout: %ld\n", timeout);
        
        // Use curl_mvaprintf to create formatted string
        char* formatted_str =  id_000046_call_curl_mvaprintf("Timeout value is: %ld", timeout);
        if (formatted_str) {
            // Write formatted string to output file
            fwrite(formatted_str, 1, strlen(formatted_str), out_file);
            fwrite("\n", 1, 1, out_file);
            curl_free(formatted_str);
        }
        
        // Use curl_mvsprintf with a buffer
        char buffer[256];
         id_000046_call_curl_mvsprintf(buffer, "CURLM timeout: %ld milliseconds", timeout);
        fwrite(buffer, 1, strlen(buffer), out_file);
        fwrite("\n", 1, 1, out_file);
    }

    // Use curl_easy_option_by_id API
    // Get option for CURLOPT_URL (this is ID 10002)
    const struct curl_easyoption* url_option = curl_easy_option_by_id((CURLoption)10002);
    if (url_option) {
        char option_info[256];
        snprintf(option_info, sizeof(option_info), 
                 "Found option: %s with ID: %d\n", 
                 url_option->name, url_option->id);
        fwrite(option_info, 1, strlen(option_info), out_file);
        
        // Set the URL option on the easy handle
        if (size > 20) {
            // Use part of input as URL (ensuring it's null-terminated)
            size_t url_len = size > 100 ? 100 : size;
            char* url = (char*)malloc(url_len + 1);
            if (url) {
                memcpy(url, data + 10, url_len - 10);
                url[url_len - 10] = '\0';
                curl_easy_setopt(easy_handle, CURLOPT_URL, url);
                free(url);
            }
        }
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Perform a simple multi perform to trigger timeout logic
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Clean up MIME structure
    if (mime) {
        curl_mime_free(mime);
    }

    // Clean up handles
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Cleanup global state
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}