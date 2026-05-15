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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 58.5, nr_unique_branch: 2
//<Quality> {"density":39,"unique_branches":{"xfer_may_multiplex":[[566,8,566,42,0,0,4,1]],"Curl_multiplex_wanted":[[932,19,932,38,0,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_formget","curl_easy_perform","curl_slist_free_all","curl_easy_cleanup","curl_multi_socket_action","curl_multi_info_read","curl_multi_perform","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_formget","curl_easy_perform","curl_slist_free_all","curl_easy_cleanup","curl_multi_socket_action","curl_multi_info_read","curl_multi_perform","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Data structure for mime callback
struct mime_data {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Formget callback
static size_t  id_000039_formget_callback(void *arg, const char *buf, size_t len) {
    // Just count the bytes, do nothing with them
    return len;
}

// Mime read callback
static size_t  id_000039_mime_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    mime_data* mdata = static_cast<mime_data*>(arg);
    size_t total = size * nitems;
    size_t available = mdata->size - mdata->pos;
    
    if (total > available) {
        total = available;
    }
    
    if (total > 0) {
        memcpy(buffer, mdata->data + mdata->pos, total);
        mdata->pos += total;
    }
    
    return total;
}

// Mime seek callback
static int  id_000039_mime_seek_callback(void *arg, curl_off_t offset, int origin) {
    mime_data* mdata = static_cast<mime_data*>(arg);
    
    switch (origin) {
        case SEEK_SET:
            if (offset >= 0 && static_cast<size_t>(offset) <= mdata->size) {
                mdata->pos = offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_CUR:
            if (mdata->pos + offset >= 0 && 
                static_cast<size_t>(mdata->pos + offset) <= mdata->size) {
                mdata->pos += offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_END:
            if (mdata->size + offset >= 0 && 
                static_cast<size_t>(mdata->size + offset) <= mdata->size) {
                mdata->pos = mdata->size + offset;
                return CURL_SEEKFUNC_OK;
            }
            break;
    }
    
    return CURL_SEEKFUNC_FAIL;
}

// Mime free callback
static void  id_000039_mime_free_callback(void *arg) {
    // Nothing to free since we don't allocate mime_data dynamically
    (void)arg;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create share object
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Set multi options
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, 1L);
    
    // Create easy handle for multi
    CURL* easy1 = curl_easy_init();
    CURL* easy2 = nullptr;
    
    if (!easy1) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime* mime = curl_mime_init(easy1);
    if (!mime) {
        curl_easy_cleanup(easy1);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Add mime part
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy1);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Set up mime data structure
    mime_data mdata = {data, size, 0};
    
    // Set mime data with callback
    curl_mime_data_cb(part, static_cast<curl_off_t>(size),
                      id_000039_mime_read_callback,
                      id_000039_mime_seek_callback,
                      id_000039_mime_free_callback,
                     &mdata);
    
    // Set mime name
    curl_mime_name(part, fuzz_str_1);
    
    // Set easy options for first handle
    curl_easy_setopt(easy1, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy1, CURLOPT_SHARE, share);
    curl_easy_setopt(easy1, CURLOPT_URL, "http://example.com");
    
    // Add first handle to multi
    curl_multi_add_handle(multi, easy1);
    
    // Create old-style form
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    
    // Add form field with input data
    if (size > 0) {
        // Use a copy of the data since formadd expects null-terminated string
        char* data_copy = static_cast<char*>(malloc(size + 1));
        if (data_copy) {
            memcpy(data_copy, data, size);
            data_copy[size] = '\0';
            
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "fuzz_field",
                        CURLFORM_COPYCONTENTS, data_copy,
                        CURLFORM_CONTENTSLENGTH, static_cast<long>(size),
                        CURLFORM_END);
            
            free(data_copy);
        }
    }
    
    // Create another easy handle for form operations
    easy2 = curl_easy_init();
    if (easy2) {
        // Set form data
        curl_easy_setopt(easy2, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(easy2, CURLOPT_URL, "http://example.com");
        
        // Create header list
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, fuzz_str_2);
        
        if (headers) {
            curl_easy_setopt(easy2, CURLOPT_HTTPHEADER, headers);
        }
        
        // Get form data using curl_formget
        if (formpost) {
            curl_formget(formpost, nullptr,  id_000039_formget_callback);
        }
        
        // Perform with second easy handle
        curl_easy_perform(easy2);
        
        // Clean up second handle
        if (headers) {
            curl_slist_free_all(headers);
        }
        curl_easy_cleanup(easy2);
    }
    
    // Simulate socket activity for multi handle
    int running_handles = 0;
    curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, fuzz_int32_t_3, &running_handles);
    
    // Try to read multi info
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi, &msgs_in_queue);
    (void)msg; // Not used, just to satisfy API call
    
    // Perform with multi handle
    curl_multi_perform(multi, &running_handles);
    
    // Clean up form
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Clean up mime
    curl_mime_free(mime);
    
    // Remove first handle from multi
    curl_multi_remove_handle(multi, easy1);
    
    // Clean up easy handles
    curl_easy_cleanup(easy1);
    
    // Clean up multi handle
    curl_multi_cleanup(multi);
    
    // Clean up share
    curl_share_cleanup(share);
    
    // Final cleanup
    curl_global_cleanup();
    
    return 0;
}