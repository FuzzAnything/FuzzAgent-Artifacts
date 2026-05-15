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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 21
//<Quality> {"density":5,"unique_branches":{"curl_multi_get_offt":[[3828,6,3828,24,0,0,4,1],[3830,6,3830,13,0,0,4,1],[3834,3,3834,31,0,0,4,0],[3834,3,3834,31,0,0,4,1],[3836,8,3836,9,0,0,4,0],[3836,13,3836,25,0,0,4,0],[3840,3,3840,31,0,0,4,0],[3840,3,3840,31,0,0,4,1],[3842,8,3842,9,0,0,4,0],[3842,13,3842,74,0,0,4,0],[3846,3,3846,31,0,0,4,1],[3849,3,3849,28,0,0,4,1],[3852,3,3852,29,0,0,4,1],[3855,3,3855,10,0,0,4,0],[3855,3,3855,10,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"Curl_uint32_bset_count":[[82,14,82,30,0,0,4,0],[82,14,82,30,0,0,4,1],[83,8,83,22,0,0,4,0],[83,8,83,22,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_getenv","curl_strequal","curl_easy_option_by_name","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_ws_meta","curl_multi_init","curl_multi_get_offt","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_strequal","curl_easy_option_by_name","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_ws_meta","curl_multi_init","curl_multi_get_offt","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_free","curl_global_cleanup"],"visited":0}
/**/


// Helper function to use curl_mvprintf with arguments
static int  id_000064_call_curl_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Need minimal data for meaningful fuzzing
    }

    // Initialize libcurl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for output (even if we might not use it)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *mimepart = NULL;
    char *env_value = NULL;

    // 1. Use curl_getenv - try to get an environment variable
    // Use first 20 bytes as environment variable name (null-terminated)
    size_t env_name_len = size > 20 ? 20 : size;
    char *env_name = (char *)malloc(env_name_len + 1);
    if (env_name) {
        memcpy(env_name, data, env_name_len);
        env_name[env_name_len] = '\0';
        env_value = curl_getenv(env_name);
        free(env_name);
    }

    // 2. Use curl_strequal - compare two strings
    // Use next 20 bytes for first string and following 20 for second
    size_t offset = 20;
    if (size > offset + 40) {
        char str1[21], str2[21];
        size_t len1 = size - offset > 20 ? 20 : size - offset;
        size_t len2 = size - offset - len1 > 20 ? 20 : size - offset - len1;
        
        memcpy(str1, data + offset, len1);
        str1[len1] = '\0';
        memcpy(str2, data + offset + len1, len2);
        str2[len2] = '\0';
        
        int are_equal = curl_strequal(str1, str2);
        (void)are_equal; // Use result to avoid compiler warnings
    }

    // 3. Use curl_easy_option_by_name
    // Look up an option by name from input data
    offset = 40;
    if (size > offset + 20) {
        char opt_name[21];
        size_t len = size - offset > 20 ? 20 : size - offset;
        memcpy(opt_name, data + offset, len);
        opt_name[len] = '\0';
        
        const struct curl_easyoption *opt = curl_easy_option_by_name(opt_name);
        (void)opt; // Use result
    }

    // 4. Create easy handle and set up MIME
    easy_handle = curl_easy_init();
    if (easy_handle) {
        // 5. Use curl_mime_type
        mime = curl_mime_init(easy_handle);
        if (mime) {
            mimepart = curl_mime_addpart(mime);
            if (mimepart) {
                // Set MIME type using data from input
                offset = 60;
                if (size > offset + 20) {
                    char mime_type[21];
                    size_t len = size - offset > 20 ? 20 : size - offset;
                    memcpy(mime_type, data + offset, len);
                    mime_type[len] = '\0';
                    
                    curl_mime_type(mimepart, mime_type);
                }
            }
        }

        // 6. Use curl_ws_meta - get WebSocket metadata
        const struct curl_ws_frame *ws_meta = curl_ws_meta(easy_handle);
        (void)ws_meta; // Use result
    }

    // 7. Create multi handle and use curl_multi_get_offt
    multi_handle = curl_multi_init();
    if (multi_handle) {
        curl_off_t off_value;
        // Try to get different offset values based on input
        CURLMcode mres;
        
        // Try CURLMINFO_CONTENT_LENGTH_UPLOAD_T (0)
        mres = curl_multi_get_offt(multi_handle, 
                                  static_cast<CURLMinfo_offt>(data[0] % 3),  // Use first byte to choose
                                  &off_value);
        (void)mres; // Use result
    }

    // 8. Use curl_mvprintf - format and print using input data
    offset = 80;
    if (size > offset + 20) {
        char format_str[21];
        size_t len = size - offset > 20 ? 20 : size - offset;
        memcpy(format_str, data + offset, len);
        format_str[len] = '\0';
        
        // Use a simple format string
         id_000064_call_curl_mvprintf("Fuzz: %s\n", format_str);
    }

    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    if (env_value) {
        curl_free(env_value);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}