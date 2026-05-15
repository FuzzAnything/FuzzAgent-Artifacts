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
//<ID> 855
//<Prompt> []
/*<Combination>: [
*/
//<score> 97.5, nr_unique_branch: 7
//<Quality> {"density":39,"unique_branches":{"dnscache_unlock":[[163,6,163,17,0,0,4,0],[163,21,163,55,0,0,4,1]],"dnscache_get":[[146,6,146,17,0,0,4,0],[146,21,146,71,0,0,4,1]],"Curl_mntfy_enable":[[150,6,150,34,0,0,4,0]],"dnscache_lock":[[156,6,156,17,0,0,4,0],[156,21,156,55,0,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_share_setopt","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_url","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_url_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_unescape","curl_free","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_multi_notify_disable","curl_multi_notify_enable","curl_multi_wait","curl_easy_ssls_export","curl_easy_strerror","curl_share_strerror","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_url","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_url_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_unescape","curl_free","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_multi_notify_disable","curl_multi_notify_enable","curl_multi_wait","curl_easy_ssls_export","curl_easy_strerror","curl_share_strerror","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Initialize share object
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set share options
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    
    // Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Set share on easy handle
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    
    // Initialize mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime part and add input data
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
        curl_mime_data(part, reinterpret_cast<const char*>(data), size);
        
        // Add headers to mime part
        struct curl_slist *headers = curl_slist_append(NULL, "Content-Type: application/octet-stream");
        if (headers) {
            curl_mime_headers(part, headers, 1); // take ownership
        }
    }
    
    // Set mime data as POST fields
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    
    // Create URL handle and parse URL
    CURLU *url = curl_url();
    if (url) {
        // Set URL from input data (use first part as URL string)
        size_t url_len = size > 100 ? 100 : size; // Limit URL length
        char *url_str = reinterpret_cast<char*>(malloc(url_len + 1));
        if (url_str) {
            memcpy(url_str, data, url_len);
            url_str[url_len] = '\0';
            curl_url_set(url, CURLUPART_URL, url_str, 0);
            
            // Get URL component
            char *host = nullptr;
            if (curl_url_get(url, CURLUPART_HOST, &host, 0) == CURLUE_OK) {
                curl_easy_setopt(easy, CURLOPT_URL, host);
                curl_free(host);
            }
            free(url_str);
        }
        curl_url_cleanup(url);
    }
    
    // Set additional easy options
    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
    
    // Unescape a string (using a fixed test string)
    int outlength = 0;
    const char *test_str = "hello%20world";
    char *unescaped = curl_easy_unescape(easy, test_str, strlen(test_str), &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Create file descriptors for input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // Assign socket to multi handle
        curl_multi_assign(multi, fd, nullptr);
        
        // Perform socket action (simulate read event)
        int running_handles = 0;
        curl_multi_socket_action(multi, fd, CURL_CSELECT_IN, &running_handles);
        
        // Disable and enable notifications
        curl_multi_notify_disable(multi, CURLMOPT_PUSHFUNCTION);
        curl_multi_notify_enable(multi, CURLMOPT_PUSHFUNCTION);
        
        // Wait for activity
        struct curl_waitfd extra_fds[1];
        extra_fds[0].fd = fd;
        extra_fds[0].events = CURL_WAIT_POLLIN;
        extra_fds[0].revents = 0;
        int numfds = 0;
        curl_multi_wait(multi, extra_fds, 1, 1000, &numfds);
        
        assert_file_closed(&in_file);;
    }
    
    // Try SSL session export (will likely fail but we call it)
    curl_easy_ssls_export(easy, nullptr, nullptr);
    
    // Get error strings (for demonstration)
    const char *easy_error = curl_easy_strerror(CURLE_OK);
    const char *share_error = curl_share_strerror(CURLSHE_OK);
    (void)easy_error; // Suppress unused warning
    (void)share_error;
    
    // Clean up in reverse order
    curl_multi_remove_handle(multi, easy);
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}