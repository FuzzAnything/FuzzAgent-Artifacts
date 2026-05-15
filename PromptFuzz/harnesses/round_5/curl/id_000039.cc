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
//<ID> 765
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 2
//<Quality> {"density":26,"unique_branches":{"curl_url_dup":[[1304,8,1304,19,11,0,4,0],[1306,10,1306,23,11,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_url","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_share_init","curl_mime_free","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_share_cleanup","curl_mime_free","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_url_set","curl_url_dup","curl_mime_addpart","curl_mime_filename","curl_share_setopt","curl_url_get","curl_free","curl_multi_add_handle","curl_multi_notify_enable","curl_easy_send","curl_easy_upkeep","curl_easy_reset","curl_multi_perform","curl_url_cleanup","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_url","curl_mime_init","curl_share_init","curl_url_set","curl_url_dup","curl_mime_addpart","curl_mime_filename","curl_share_setopt","curl_url_get","curl_free","curl_multi_add_handle","curl_multi_notify_enable","curl_easy_send","curl_easy_upkeep","curl_easy_reset","curl_multi_perform","curl_url_cleanup","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Create file handles for reading/writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create another URL handle for duplication
    CURLU *url_handle_dup = NULL;
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create share handle
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_mime_free(mime);
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use input data to create a URL string (truncate if too long)
    size_t url_len = size < 100 ? size : 100;
    char *url_str = (char *)malloc(url_len + 1);
    if (!url_str) {
        curl_share_cleanup(share_handle);
        curl_mime_free(mime);
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';
    
    // Set URL in URL handle
    CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_URL, url_str, 0);
    
    // Duplicate URL handle
    url_handle_dup = curl_url_dup(url_handle);
    
    // Create mime part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (mime_part) {
        // Use filename from input data (truncate if too long)
        size_t filename_len = size < 50 ? size : 50;
        char *filename = (char *)malloc(filename_len + 1);
        if (filename) {
            memcpy(filename, data, filename_len);
            filename[filename_len] = '\0';
            curl_mime_filename(mime_part, filename);
            free(filename);
        }
    }
    
    // Set share option
    CURLSHcode share_set_res = curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    
    // Get URL back from URL handle
    char *retrieved_url = NULL;
    if (url_handle_dup) {
        CURLUcode url_get_res = curl_url_get(url_handle_dup, CURLUPART_URL, &retrieved_url, 0);
        if (retrieved_url) {
            curl_free(retrieved_url);
        }
    }
    
    // Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    // Enable notifications on multi handle
    CURLMcode notify_res = curl_multi_notify_enable(multi_handle, 1); // CURLM_NOTIFY_DONE
    
    // Send data using easy handle
    size_t sent = 0;
    if (size > 0) {
        // Use a portion of input data for sending
        size_t send_size = size < 1024 ? size : 1024;
        CURLcode send_res = curl_easy_send(easy_handle, data, send_size, &sent);
    }
    
    // Perform upkeep on easy handle
    CURLcode upkeep_res = curl_easy_upkeep(easy_handle);
    
    // Reset easy handle
    curl_easy_reset(easy_handle);
    
    // Perform multi handle operations
    int running_handles = 0;
    CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);
    
    // Cleanup in reverse order
    if (url_handle_dup) {
        curl_url_cleanup(url_handle_dup);
    }
    
    curl_mime_free(mime);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_url_cleanup(url_handle);
    curl_share_cleanup(share_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    free(url_str);
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}