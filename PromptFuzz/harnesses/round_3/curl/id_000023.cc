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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 95, nr_unique_branch: 35
//<Quality> {"density":19,"unique_branches":{"Curl_getinfo":[[642,3,642,23,0,0,4,1],[647,3,647,21,0,0,4,0],[649,8,649,19,0,0,4,0]],"getinfo_long":[[205,3,205,27,0,0,4,1],[214,3,214,28,0,0,4,1],[215,3,215,29,0,0,4,1],[224,3,224,10,0,0,4,0],[230,3,230,30,0,0,4,0],[233,3,233,33,0,0,4,1],[236,3,236,25,0,0,4,1],[246,3,246,28,0,0,4,1],[249,3,249,29,0,0,4,1],[252,3,252,33,0,0,4,1],[255,3,255,39,0,0,4,1],[262,3,262,31,0,0,4,1],[265,3,265,31,0,0,4,1],[269,3,269,32,0,0,4,1],[273,3,273,30,0,0,4,1],[277,3,277,31,0,0,4,1],[281,3,281,25,0,0,4,1],[284,3,284,29,0,0,4,1],[292,3,292,27,0,0,4,1],[304,3,304,29,0,0,4,1],[311,3,311,27,0,0,4,1],[318,3,318,28,0,0,4,1],[321,3,321,32,0,0,4,1],[329,3,329,33,0,0,4,1],[332,3,332,33,0,0,4,1],[335,3,335,31,0,0,4,1],[345,3,345,29,0,0,4,1],[364,3,364,25,0,0,4,1],[367,3,367,27,0,0,4,1],[376,3,376,10,0,0,4,1]],"curl_global_cleanup":[[264,6,264,19,0,0,4,0]],"curl_global_init_mem":[[225,6,225,17,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_url","curl_url_set","curl_url_set","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_easy_getinfo","curl_easy_nextheader","curl_msnprintf","curl_url_get","curl_free","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_init_mem","curl_multi_init","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_url","curl_url_set","curl_url_set","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_easy_getinfo","curl_easy_nextheader","curl_msnprintf","curl_url_get","curl_free","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize curl with custom memory functions
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Custom memory callbacks (using standard ones for simplicity)
    curl_global_init_mem(CURL_GLOBAL_ALL, 
                        (curl_malloc_callback)malloc,
                        (curl_free_callback)free,
                        (curl_realloc_callback)realloc,
                        (curl_strdup_callback)strdup,
                        (curl_calloc_callback)calloc);

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Enable notifications
    curl_multi_notify_enable(multi_handle, fuzz_uint32_t_1);

    // Create share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        if (share_handle) curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create URL handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Use first part of data for URL scheme
        size_t scheme_len = size > 10 ? 10 : size;
        char scheme[11];
        memcpy(scheme, data, scheme_len);
        scheme[scheme_len] = '\0';
        curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_2, fuzz_uint32_t_3);
        
        // Use next part for URL path
        if (size > 20) {
            size_t path_len = size > 50 ? 30 : size - 20;
            char *path = new char[path_len + 1];
            memcpy(path, data + 10, path_len);
            path[path_len] = '\0';
            curl_url_set(url_handle, CURLUPART_PATH, path, fuzz_uint32_t_4);
            delete[] path;
        }
    }

    // Create MIME structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use data for mime name and data
            size_t name_len = size > 20 ? 10 : size / 2;
            char *name = new char[name_len + 1];
            memcpy(name, data, name_len);
            name[name_len] = '\0';
            curl_mime_name(part, name);
            delete[] name;

            // Use remaining data for mime data
            if (size > name_len) {
                size_t data_len = size - name_len;
                curl_mime_data(part, (const char*)(data + name_len), data_len);
            }
        }
    }

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    if (share_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    }

    // Add handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Perform multi operations
    int running_handles = 0;
    int ret = 0;
    struct curl_waitfd extra_fds;
    extra_fds.fd = 0;
    extra_fds.events = CURL_WAIT_POLLIN;
    extra_fds.revents = 0;
    
    curl_multi_poll(multi_handle, &extra_fds, 1, fuzz_int32_t_5, &ret);
    curl_multi_perform(multi_handle, &running_handles);

    // Get info from easy handle
    long http_code = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &http_code);

    // Get headers
    struct curl_header *header = NULL;
    header = curl_easy_nextheader(easy_handle, CURLH_HEADER, fuzz_int32_t_6, header);

    // Format string operations
    char buffer[256];
    int printed = curl_msnprintf(buffer, sizeof(buffer), "Size: %zu", size);
    
    if (url_handle) {
        char *url_part = NULL;
        if (curl_url_get(url_handle, CURLUPART_URL, &url_part, fuzz_uint32_t_7) == CURLUE_OK) {
            if (url_part) curl_free(url_part);
        }
    }

    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(easy_handle);
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    curl_multi_cleanup(multi_handle);
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    curl_global_cleanup();

    return 0;
}