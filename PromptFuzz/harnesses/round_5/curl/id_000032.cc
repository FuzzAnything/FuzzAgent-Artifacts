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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 88, nr_unique_branch: 6
//<Quality> {"density":22,"unique_branches":{"Curl_poll":[[259,8,259,33,0,0,4,0],[261,8,261,33,0,0,4,0]],"cpfds_add_sock":[[386,32,386,38,0,0,4,0],[387,10,387,35,0,0,4,1]],"Curl_pollfds_add_ps":[[419,8,419,37,0,0,4,1],[421,8,421,38,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_escape","curl_mprintf","curl_msnprintf","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_header","curl_multi_wait","curl_easy_recv","curl_url_strerror","curl_free","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_escape","curl_mprintf","curl_msnprintf","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_header","curl_multi_wait","curl_easy_recv","curl_url_strerror","curl_free","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function for curl_mvprintf
static int  id_000032_call_curl_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    // 1. Initialize curl globally
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }
    
    // 2. Create multi and easy handles
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Create a temporary file with input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create MIME structure and add part
    curl_mime* mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart* part = curl_mime_addpart(mime);
        if (part) {
            // Use the input file for mime filedata
            assert_file_closed(&in_file);; // Close to write to disk
            FILE* tmp_file = fopen("input_file", "wb");
            if (tmp_file) {
                fwrite(data, 1, size, tmp_file);
                assert_file_closed(&tmp_file);;
                
                curl_mime_filedata(part, "input_file");
            }
        }
    }
    
    // 5. Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:8080");
    if (mime) {
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }
    
    // 6. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 7. Use curl_easy_escape with input data
    char* escaped = nullptr;
    if (size > 0) {
        // Use first 100 bytes or less for escaping
        size_t escape_len = size > 100 ? 100 : size;
        escaped = curl_easy_escape(easy_handle, (const char*)data, escape_len);
    }
    
    // 8. Use printf functions with input data
    if (size > 0) {
        // Use a small portion for formatting
        size_t fmt_len = size > 50 ? 50 : size;
        char fmt_buf[51];
        memcpy(fmt_buf, data, fmt_len);
        fmt_buf[fmt_len] = '\0';
        
        // curl_mprintf
        curl_mprintf("Test: %s\n", fmt_buf);
        
        // curl_msnprintf
        char buffer[100];
        curl_msnprintf(buffer, sizeof(buffer), "Buffer: %s", fmt_buf);
        
        // curl_mvprintf via helper
         id_000032_call_curl_mvprintf("Varargs: %s\n", fmt_buf);
    }
    
    // 9. Wake up multi handle
    curl_multi_wakeup(multi_handle);
    
    // 10. Perform multi operations
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // 11. Get info from easy handle
    long http_code = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &http_code);
    
    // 12. Try to get headers
    struct curl_header* header_out = nullptr;
    curl_easy_header(easy_handle, "Content-Type", 0, CURLH_HEADER, 0, &header_out);
    
    // 13. Wait on multi handle
    int numfds = 0;
    curl_multi_wait(multi_handle, nullptr, 0, 100, &numfds);
    
    // 14. Try to receive data (will likely fail since we're not really connected)
    char recv_buffer[1024];
    size_t bytes_received = 0;
    curl_easy_recv(easy_handle, recv_buffer, sizeof(recv_buffer), &bytes_received);
    
    // 15. Use curl_url_strerror (just to call it)
    const char* error_str = curl_url_strerror(CURLUE_OK);
    
    // 16. Clean up
    if (escaped) {
        curl_free(escaped);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove the input file if it exists
    remove("input_file");
    
    // 17. Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // 18. Clean up handles
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}