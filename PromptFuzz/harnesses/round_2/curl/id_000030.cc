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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 93, nr_unique_branch: 10
//<Quality> {"density":31,"unique_branches":{"curl_url_strerror":[[424,3,424,17,0,0,4,0]],"out_string":[[875,16,875,20,0,0,4,1]],"formatf":[[1003,13,1003,33,0,0,4,0]],"Curl_mime_set_subparts":[[1460,8,1460,12,0,0,4,0],[1461,13,1461,25,0,0,4,1],[1463,10,1463,26,0,0,4,1]],"parsefmt":[[252,14,252,25,0,0,4,1],[272,16,272,22,0,0,4,1],[274,16,274,53,0,0,4,1],[277,16,277,22,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_global_sslset","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_escape","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_slist_append","curl_free","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_mime_free","curl_easy_setopt","curl_multi_socket_all","curl_easy_perform","curl_easy_header","curl_slist_free_all","curl_multi_remove_handle","curl_url","curl_url_set","curl_url_strerror","curl_multi_cleanup","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_global_sslset","curl_easy_init","curl_multi_init","curl_multi_add_handle","curl_easy_escape","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_slist_append","curl_free","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_multi_socket_all","curl_easy_perform","curl_easy_header","curl_slist_free_all","curl_multi_remove_handle","curl_url","curl_url_set","curl_url_strerror","curl_multi_cleanup","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Helper function for curl_mvsnprintf
static int  id_000030_my_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Initialize global curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 2. Create formatted string from input data using curl_mvsnprintf
    char formatted_buffer[256];
    const char *format_str = "Data: %.10s... (size: %zu)";
     id_000030_my_curl_mvsnprintf(formatted_buffer, sizeof(formatted_buffer), 
                      format_str, data, size);
    
    // 3. Set global trace configuration
    const char *trace_config = "http/2,ssl";
    curl_global_trace(trace_config);
    
    // 4. Try to set SSL backend (this might fail but we try it)
    const curl_ssl_backend ***avail = nullptr;
    curl_global_sslset(CURLSSLBACKEND_NONE, "test-backend", avail);
    
    // 5. Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 8. Escape some data from input
    const char *string_to_escape = reinterpret_cast<const char*>(data);
    int escape_len = size > 100 ? 100 : size;
    char *escaped = curl_easy_escape(easy_handle, string_to_escape, escape_len);
    
    // 9. Set various options on easy handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use in some context if needed
        
        // Use the input data in some way - here we could set it as POST data
        if (size > 0) {
            // Set postfield size
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, (long)size);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 10. Create and append to slist
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    headers = curl_slist_append(headers, formatted_buffer);
    if (escaped) {
        char header_buf[512];
        snprintf(header_buf, sizeof(header_buf), "X-Escaped: %s", escaped);
        headers = curl_slist_append(headers, header_buf);
        curl_free(escaped);
    }
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    
    // 11. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Could set this as write function, but for simplicity just use it
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    }
    
    // 12. Setup MIME for multipart data
    curl_mime *mime = curl_mime_init(easy_handle);
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part && size > 0) {
        curl_mime_data(part, reinterpret_cast<const char*>(data), 
                      size > 1024 ? 1024 : size);
        curl_mime_name(part, "fuzzdata");
        
        // Create subparts for nested mime structure
        curl_mime *submime = curl_mime_init(easy_handle);
        curl_mimepart *subpart = curl_mime_addpart(submime);
        if (subpart && size > 10) {
            curl_mime_data(subpart, "subdata", 7);
            curl_mime_subparts(part, submime);
        } else {
            curl_mime_free(submime);
        }
        
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }
    
    // 13. Try to perform with multi handle (non-blocking)
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // 14. Also try easy perform (this will likely fail but we call it)
    CURLcode res = curl_easy_perform(easy_handle);
    
    // 15. Try to get a header (even though transfer might have failed)
    struct curl_header *header_out = nullptr;
    curl_easy_header(easy_handle, "Content-Type", 0, 
                    CURLH_HEADER, 1, &header_out);
    
    // 16. Cleanup slist
    curl_slist_free_all(headers);
    
    // 17. Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // 18. Use curl_url_strerror for demonstration
    // Create a URL handle and get error string
    CURLU *url_handle = curl_url();
    CURLUcode url_res = curl_url_set(url_handle, CURLUPART_URL, 
                                     "http://example.com", 0);
    const char *url_error = curl_url_strerror(url_res);
    (void)url_error; // Use if needed
    
    // 19. Cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // 20. Cleanup mime
    if (mime) {
        curl_mime_free(mime);
    }
    
    // 21. Cleanup URL handle
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    // 22. Cleanup easy handle
    curl_easy_cleanup(easy_handle);
    
    // 23. Close output file if opened
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // 24. Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}