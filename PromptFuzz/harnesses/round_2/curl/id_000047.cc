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
//<ID> 865
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["curl_global_init","curl_easy_strerror","curl_share_init","curl_share_setopt","curl_share_strerror","curl_easy_init","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_assign","curl_multi_socket_action","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_ssls_export","curl_multi_wait","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_mime_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_strerror","curl_share_init","curl_share_setopt","curl_share_strerror","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_assign","curl_multi_socket_action","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_ssls_export","curl_multi_wait","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_mime_free","curl_global_cleanup"],"visited":0}
/*Here's a comprehensive fuzz driver that uses all the specified curl APIs to simulate a complex scenario:

*/


extern "C" {
}

// Forward declaration for callback
extern "C" size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

// SSL export callback
static CURLcode  id_000047_ssl_export_callback(CURL *handle, void *userptr,
                                     const char *session_key,
                                     const unsigned char *shmac, size_t shmac_len,
                                     const unsigned char *sdata, size_t sdata_len,
                                     curl_off_t expires, int error,
                                     const char *cipher, size_t cipher_len) {
    return CURLE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use at least 1 byte for initialization
    if (size < 1) {
	return 0;
	}
	
    
    // 1. Global initialization
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        const char* error_str = curl_easy_strerror(res);
        // Use error string to avoid unused variable warning
        if (error_str[0] == 0) return 0;
    }
    
    // 2. Initialize share object
    CURLSH *share = curl_share_init();
    if (share) {
        // Set share options
        CURLSHcode share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        if (share_res != CURLSHE_OK) {
            const char* share_error = curl_share_strerror(share_res);
            if (share_error[0] == 0) {
                // Just to use the variable
            }
        }
    }
    
    // 3. Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Set easy handle options using input data
    // Use first part of data for URL/options
    size_t url_len = size > 100 ? 100 : size;
    char *url = (char*)malloc(url_len + 1);
    if (url) {
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        
        // Set URL option
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        
        // Set share option
        if (share) {
            curl_easy_setopt(easy, CURLOPT_SHARE, share);
        }
        
        // Set other options
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, NULL);
        
        free(url);
    }
    
    // 5. Create MIME structure for multipart form data
    curl_mime *mime = NULL;
    if (size > 10) {
        mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use part of input data for mime data
                size_t mime_data_size = size > 50 ? 50 : size;
                curl_mime_data(part, (const char*)data, mime_data_size);
            }
            
            // Set MIME option
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
        }
    }
    
    // 6. Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        if (mime) curl_mime_free(mime);
        curl_easy_cleanup(easy);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // 7. Add easy handle to multi handle
    CURLMcode mres = curl_multi_add_handle(multi, easy);
    
    // 8. Disable notifications
    curl_multi_notify_disable(multi, 0);
    
    // 9. Create a file from input data for socket operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        
        // 10. Assign socket to multi handle (dummy assignment)
        curl_multi_assign(multi, (curl_socket_t)fd, NULL);
        
        // 11. Perform socket action (with timeout since it's not a real socket)
        int running_handles = 0;
        curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &running_handles);
        
        assert_file_closed(&in_file);;
    }
    
    // 12. Escape/unescape operations using input data
    if (size > 20) {
        // Take a portion for escaping
        char *escaped = curl_easy_escape(easy, (const char*)data, 20);
        if (escaped) {
            int outlength = 0;
            char *unescaped = curl_easy_unescape(easy, escaped, strlen(escaped), &outlength);
            
            // Free allocated strings
            if (escaped) curl_free(escaped);
            if (unescaped) curl_free(unescaped);
        }
    }
    
    // 13. Try SSL export (will likely fail but we call it)
    curl_easy_ssls_export(easy,  id_000047_ssl_export_callback, NULL);
    
    // 14. Multi wait (non-blocking)
    struct curl_waitfd extra_fds[1];
    int numfds = 0;
    curl_multi_wait(multi, extra_fds, 0, 0, &numfds);
    
    // 15. Try to receive data (non-blocking)
    if (size > 30) {
        char buffer[30];
        size_t n = 0;
        curl_easy_recv(easy, buffer, sizeof(buffer), &n);
    }
    
    // 16. Clean up in reverse order
    
    // Remove handle from multi
    curl_multi_remove_handle(multi, easy);
    
    // Clean up multi
    curl_multi_cleanup(multi);
    
    // Clean up easy handle
    curl_easy_cleanup(easy);
    
    // Clean up share
    if (share) curl_share_cleanup(share);
    
    // Clean up mime
    if (mime) curl_mime_free(mime);
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}

// Write callback implementation
extern "C" size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Just discard the data
    return size * nmemb;
}