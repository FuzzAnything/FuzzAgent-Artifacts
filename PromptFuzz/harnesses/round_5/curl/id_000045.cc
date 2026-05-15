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
//<ID> 932
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 13
//<Quality> {"density":15,"unique_branches":{"curl_multi_get_offt":[[3834,3,3834,31,0,0,4,0],[3836,8,3836,9,0,0,4,0],[3836,13,3836,25,0,0,4,0],[3840,3,3840,31,0,0,4,0],[3842,8,3842,9,0,0,4,0],[3842,13,3842,74,0,0,4,0],[3846,3,3846,31,0,0,4,0],[3849,3,3849,28,0,0,4,0],[3855,3,3855,10,0,0,4,1]],"Curl_uint32_bset_count":[[82,14,82,30,0,0,4,0],[82,14,82,30,0,0,4,1],[83,8,83,22,0,0,4,0],[83,8,83,22,0,0,4,1]]},"library_calls":["curl_global_init","curl_msnprintf","curl_version","curl_strequal","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_formadd","curl_formget","curl_multi_init","curl_multi_add_handle","curl_multi_get_offt","curl_multi_socket_action","curl_easy_unescape","curl_free","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_msnprintf","curl_version","curl_strequal","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_formadd","curl_formget","curl_multi_init","curl_multi_add_handle","curl_multi_get_offt","curl_multi_socket_action","curl_easy_unescape","curl_free","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom callback for curl_formget
static size_t  id_000045_formget_callback(void *arg, const char *buf, size_t len) {
    // Simple callback that just counts bytes
    size_t* total = static_cast<size_t*>(arg);
    if (total) *total += len;
    return len;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. curl_msnprintf - format some data
    char buffer[256];
    const char* format_str = "Size: %zu, First: 0x%02x";
    int printed = curl_msnprintf(buffer, sizeof(buffer), format_str, size, data[0]);
    
    // 2. curl_version - get version string
    char* version_str = curl_version();
    
    // 3. curl_strequal - compare version with a test string
    int is_equal = curl_strequal(version_str, "curl");
    
    // 4. Create a mime part and set its name using data
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    curl_mime* mime = curl_mime_init(easy);
    curl_mimepart* part = curl_mime_addpart(mime);
    
    // Use first 100 bytes or less as mime name
    size_t name_len = size > 100 ? 100 : size;
    char* mime_name = new char[name_len + 1];
    memcpy(mime_name, data, name_len);
    mime_name[name_len] = '\0';
    
    CURLcode res = curl_mime_name(part, mime_name);
    delete[] mime_name;
    
    // 5. Create form data and use curl_formget
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    
    // Use a portion of input data as form content
    size_t form_data_len = size > 50 ? 50 : size;
    char* form_content = new char[form_data_len + 1];
    memcpy(form_content, data, form_data_len);
    form_content[form_data_len] = '\0';
    
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "testfield",
                CURLFORM_COPYCONTENTS, form_content,
                CURLFORM_END);
    
    delete[] form_content;
    
    // Use curl_formget with our callback
    size_t formget_total = 0;
    curl_formget(formpost, &formget_total,  id_000045_formget_callback);
    
    // 6. Create multi handle and get offset info
    CURLM* multi = curl_multi_init();
    if (multi) {
        curl_multi_add_handle(multi, easy);
        
        // Try to get offset info (use some value from data)
        curl_off_t offset_val = 0;
        CURLMcode mres = curl_multi_get_offt(multi, 
            static_cast<CURLMinfo_offt>(data[0] % 5), &offset_val);
        
        // 7. curl_multi_socket - simulate socket activity
        // Create a temporary file to get a file descriptor
        FILE* tmp_file = fmemopen((void*)data, size > 1024 ? 1024 : size, "rb");
        if (tmp_file) {
            int sock_fd = fuzz_fileno(tmp_file);
            int running = 0;
            curl_multi_socket(multi, sock_fd, &running);
            assert_file_closed(&tmp_file);;
        }
    }
    
    // 8. curl_easy_unescape - unescape some data
    // Create a simple escaped string from input data
    size_t unescape_len = size > 200 ? 200 : size;
    char* to_unescape = new char[unescape_len * 3 + 1];
    size_t j = 0;
    for (size_t i = 0; i < unescape_len && i < size; i++) {
        if (data[i] == '%') {
            to_unescape[j++] = '%';
            to_unescape[j++] = '2';
            to_unescape[j++] = '0';
        } else {
            to_unescape[j++] = data[i];
        }
    }
    to_unescape[j] = '\0';
    
    int outlength = 0;
    char* unescaped = curl_easy_unescape(easy, to_unescape, j, &outlength);
    delete[] to_unescape;
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // 9. curl_pushheader_bynum - This requires HTTP/2 push context
    // We'll create a dummy scenario. In reality, this would be called
    // from within a push callback. We'll just reference the function.
    struct curl_pushheaders* dummy_headers = NULL;
    // This would normally be called from within a push callback:
    // char* push_header = curl_pushheader_bynum(dummy_headers, 0);
    
    // Clean up
    if (formpost) {
        curl_formfree(formpost);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (multi) {
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
    }
    
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    return 0;
}