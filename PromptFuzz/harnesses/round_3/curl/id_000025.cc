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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 2
//<Quality> {"density":60,"unique_branches":{"setopt_func":[[2588,3,2588,28,0,0,4,0]],"setopt_cptr":[[2045,3,2045,24,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_multi_notify_enable","curl_multi_cleanup","curl_global_cleanup","curl_share_init","curl_multi_cleanup","curl_global_cleanup","curl_share_setopt","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_init","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_url","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_mime_init","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_url_get","curl_strequal","curl_unescape","curl_free","curl_free","curl_multi_remove_handle","curl_mime_free","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_url","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_url_get","curl_strequal","curl_unescape","curl_free","curl_free","curl_multi_remove_handle","curl_mime_free","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback functions needed by curl
static size_t  id_000025_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    FILE *in_file = (FILE *)instream;
    return fread(buffer, size, nitems, in_file);
}

static int  id_000025_seek_callback(void *instream, curl_off_t offset, int origin) {
    FILE *in_file = (FILE *)instream;
    return fseek(in_file, (long)offset, origin);
}

static void  id_000025_free_callback(void *ptr) {
    // Nothing to free for FILE*
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<70) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Enable notifications on multi handle
    CURLMcode notify_res = curl_multi_notify_enable(multi_handle, fuzz_uint32_t_1);
    if (notify_res != CURLM_OK) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create share handle
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set share options
    CURLSHcode share_res = curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    if (share_res != CURLSHE_OK) {
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_easy_cleanup(easy_handle);
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Parse input data as URL string (use first part of data as URL)
    size_t url_len = size > 256 ? 256 : size;
    char *url_str = (char *)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_easy_cleanup(easy_handle);
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';
    
    // Set URL in URL handle
    CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_2);
    
    // Get URL back from URL handle
    char *retrieved_url = nullptr;
    CURLUcode url_get_res = curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_3);
    
    // Set options on easy handle
    if (retrieved_url) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, retrieved_url);
        curl_free(retrieved_url);
    }
    
    // Use share handle
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_easy_cleanup(easy_handle);
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_easy_cleanup(easy_handle);
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set mime data from remaining input
    if (size > url_len) {
        size_t mime_data_size = size - url_len;
        const char *mime_data = (const char *)(data + url_len);
        CURLcode mime_data_res = curl_mime_data(part, mime_data, mime_data_size);
        
        // Set mime name
        curl_mime_name(part, fuzz_str_4);
    }
    
    // Create subparts mime
    curl_mime *subparts = curl_mime_init(easy_handle);
    if (subparts) {
        curl_mimepart *subpart = curl_mime_addpart(subparts);
        if (subpart) {
            curl_mime_data(subpart, fuzz_str_5, static_cast<size_t>(fuzz_str_sz_5));
            curl_mime_name(subpart, fuzz_str_6);
        }
        
        // Attach subparts to main part
        curl_mime_subparts(part, subparts);
    }
    
    // Set mime to easy handle
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    
    // Create FILE* from input data for callback
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Set up read callback for POST data
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION,  id_000025_read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
        curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION,  id_000025_seek_callback);
        curl_easy_setopt(easy_handle, CURLOPT_SEEKDATA, in_file);
    }
    
    // Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    // Perform multi poll
    int running_handles = 0;
    if (add_res == CURLM_OK) {
        curl_multi_poll(multi_handle, nullptr, 0, fuzz_int32_t_7, &running_handles);
        
        // Get URL component for string comparison
        char *host_part = nullptr;
        CURLUcode host_get_res = curl_url_get(url_handle, CURLUPART_HOST, &host_part, fuzz_uint32_t_8);
        
        // Use string comparison
        if (host_part) {
            int cmp_res = curl_strequal(host_part, fuzz_str_9);
            (void)cmp_res; // Prevent unused variable warning
            
            // Use unescape
            char *unescaped = curl_unescape(host_part, -1);
            if (unescaped) {
                curl_free(unescaped);
            }
            
            curl_free(host_part);
        }
        
        // Remove handle from multi
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Clean up
    if (subparts) {
        curl_mime_free(subparts);
    }
    curl_mime_free(mime);
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    free(url_str);
    curl_url_cleanup(url_handle);
    curl_easy_cleanup(easy_handle);
    curl_share_cleanup(share_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}