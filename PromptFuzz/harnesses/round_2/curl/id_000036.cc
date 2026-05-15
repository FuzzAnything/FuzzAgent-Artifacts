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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 5
//<Quality> {"density":20,"unique_branches":{"Curl_senddata":[[1280,6,1280,12,0,0,4,0]],"curl_easy_send":[[1306,6,1306,23,0,0,4,1],[1308,6,1308,31,0,0,4,1],[134,5,134,8,1,0,4,0],[134,12,134,49,1,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_version","curl_msprintf","curl_slist_append","curl_msprintf","curl_slist_append","curl_mime_headers","curl_mime_data","curl_mime_type","curl_url","curl_url_set","curl_url_cleanup","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_fdset","curl_easy_send","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_version","curl_msprintf","curl_slist_append","curl_msprintf","curl_slist_append","curl_mime_headers","curl_mime_data","curl_mime_type","curl_url","curl_url_set","curl_url_cleanup","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_fdset","curl_easy_send","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Create a mime structure for multipart data
    CURL *easy_handle = curl_easy_init();
    curl_mime *mime = curl_mime_init(easy_handle);
    
    // 3. Add a mime part using fuzzer data as content
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
        // Create headers list
        struct curl_slist *headers = NULL;
        char header_buffer[256];
        
        // 4. Use curl_version() and curl_msprintf() to create a header
        char *version = curl_version();
        curl_msprintf(header_buffer, "X-Curl-Version: %s", version);
        headers = curl_slist_append(headers, header_buffer);
        
        // Add another header with data info
        curl_msprintf(header_buffer, "Content-Length: %zu", size);
        headers = curl_slist_append(headers, header_buffer);
        
        // 5. Set headers for the mime part (take ownership)
        curl_mime_headers(part, headers, 1);
        
        // Set mime data from fuzzer input
        curl_mime_data(part, (const char*)data, size);
        curl_mime_type(part, "application/octet-stream");
    }
    
    // 6. Create URL handle and clean it up (demonstrate usage)
    CURLU *url_handle = curl_url();
    if (url_handle) {
        curl_url_set(url_handle, CURLUPART_URL, "http://example.com", 0);
        curl_url_cleanup(url_handle);
    }
    
    // 7. Create multi handle for non-blocking operations
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle && easy_handle) {
        // Setup easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/upload");
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // 8. Use curl_multi_fdset() to monitor file descriptors
        fd_set read_fds, write_fds, exc_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&exc_fds);
        int max_fd = -1;
        
        curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
        
        // 9. Demonstrate curl_easy_send() with a small test buffer
        if (size > 0) {
            size_t sent = 0;
            const char *test_data = "Test";
            // Note: curl_easy_send requires an active connection, which we don't have in fuzzing.
            // We'll call it but ignore the expected failure.
            CURLcode send_result = curl_easy_send(easy_handle, test_data, 4, &sent);
            // Result is ignored as connection isn't established in fuzzing context
        }
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }
    
    // 10. Cleanup all resources
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    curl_global_cleanup();
    
    return 0;
}