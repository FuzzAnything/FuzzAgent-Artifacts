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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 20.5, nr_unique_branch: 0
//<Quality> {"density":41,"unique_branches":{},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_set","curl_url_get","curl_easy_init","curl_free","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_free","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_free","curl_url_cleanup","curl_global_cleanup","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_maprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_free","curl_mime_free","curl_easy_cleanup","curl_free","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_multi_add_handle","curl_formadd","curl_multi_socket_action","curl_multi_wait","curl_multi_wakeup","curl_multi_perform","curl_multi_remove_handle","curl_formfree","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_mime_free","curl_url_cleanup","curl_free","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_maprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_notify_enable","curl_multi_add_handle","curl_formadd","curl_multi_socket_action","curl_multi_wait","curl_multi_wakeup","curl_multi_perform","curl_multi_remove_handle","curl_formfree","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_mime_free","curl_url_cleanup","curl_free","curl_free","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified curl APIs to achieve a scenario where a multi-handle transfer is performed with MIME data and URL parsing:

*/


extern "C" {
}

// Helper function to create va_list for curl_mvsnprintf
static int  id_000027_call_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<46) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
	return 0;
	}
	
    
    // 2. Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Use formatted printing with input data
    char buffer[256];
    const char *format_str = "Data: %.*s";
    int printed =  id_000027_call_curl_mvsnprintf(buffer, sizeof(buffer), format_str, 
                                      (int)(size > 100 ? 100 : size), data);
    
    // 4. Parse URL from input data (first null-terminated string)
    const char *url_str = "http://example.com/path";
    CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_URL, fuzz_str_1, fuzz_uint32_t_2);
    
    // 5. Get URL component
    char *url_part = nullptr;
    CURLUcode url_get_res = curl_url_get(url_handle, CURLUPART_HOST, &url_part, fuzz_uint32_t_3);
    
    // 6. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        if (url_part) curl_free(url_part);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 7. Create MIME structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        if (url_part) curl_free(url_part);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 8. Add MIME part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (!mime_part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        if (url_part) curl_free(url_part);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 9. Set MIME data from input
    // Use first half of data for MIME
    size_t mime_data_size = size / 2;
    if (mime_data_size > 0) {
        CURLcode mime_data_res = curl_mime_data(mime_part, 
                                               reinterpret_cast<const char*>(data), 
                                               mime_data_size);
    }
    
    // 10. Create sub-MIME structure
    curl_mime *sub_mime = curl_mime_init(easy_handle);
    if (sub_mime) {
        curl_mimepart *sub_part = curl_mime_addpart(sub_mime);
        if (sub_part) {
            // Use second half of data for sub-MIME
            size_t sub_data_size = size - mime_data_size;
            if (sub_data_size > 0) {
                curl_mime_data(sub_part, 
                             reinterpret_cast<const char*>(data + mime_data_size), 
                             sub_data_size);
            }
            
            // 11. Set MIME subparts
            curl_mime_subparts(mime_part, sub_mime);
        }
    }
    
    // 12. Use curl_maprintf
    char *formatted_str = curl_maprintf("Formatted: size=%zu", size);
    
    // 13. Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, url_str);
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    
    // 14. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (formatted_str) curl_free(formatted_str);
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        if (url_part) curl_free(url_part);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 15. Enable notifications
    curl_multi_notify_enable(multi_handle, fuzz_uint32_t_4);
    
    // 16. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    // 17. Create a form for formfree API
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "field1",
                CURLFORM_COPYCONTENTS, "test",
                CURLFORM_END);
    
    // 18. Create file for socket operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // 19. Multi socket action with file descriptor
        int running_handles = 0;
        curl_multi_socket_action(multi_handle, fd, fuzz_int32_t_5, &running_handles);
        
        // 20. Multi wait
        struct curl_waitfd extra_fds;
        extra_fds.fd = fd;
        extra_fds.events = CURL_WAIT_POLLIN;
        extra_fds.revents = 0;
        
        int ret = 0;
        curl_multi_wait(multi_handle, &extra_fds, 1, fuzz_int32_t_6, &ret);
        
        assert_file_closed(&in_file);;
    }
    
    // 21. Wake up multi handle
    curl_multi_wakeup(multi_handle);
    
    // 22. Perform a short multi operation
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // 23. Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // 24. Clean up form
    curl_formfree(formpost);
    
    // 25. Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // 26. Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // 27. Free MIME structures
    if (sub_mime) curl_mime_free(sub_mime);
    curl_mime_free(mime);
    
    // 28. Clean up URL handle
    curl_url_cleanup(url_handle);
    
    // 29. Free allocated strings
    if (url_part) curl_free(url_part);
    if (formatted_str) curl_free(formatted_str);
    
    // 30. Clean up global curl
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}