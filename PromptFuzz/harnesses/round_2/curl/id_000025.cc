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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 99.666664, nr_unique_branch: 48
//<Quality> {"density":23,"unique_branches":{"urlget_url":[[1473,15,1473,41,0,0,4,0],[1474,10,1474,41,0,0,4,1]],"Curl_socket_check":[[129,6,129,34,0,0,4,0],[129,38,129,66,0,0,4,0],[130,6,130,34,0,0,4,1],[141,6,141,32,0,0,4,1],[147,6,147,32,0,0,4,1],[153,6,153,32,0,0,4,0],[161,6,161,12,0,0,4,1],[166,6,166,32,0,0,4,1],[173,6,173,32,0,0,4,1],[180,6,180,32,0,0,4,0],[181,8,181,49,0,0,4,0],[183,8,183,67,0,0,4,0]],"curl_easy_strerror":[[57,3,57,34,0,0,4,0]],"setopt_long_proto":[[1216,3,1216,26,0,0,4,1],[1221,3,1221,10,0,0,4,0]],"setopt_long_misc":[[1233,3,1233,29,0,0,4,1],[1238,3,1238,25,0,0,4,1],[1241,3,1241,29,0,0,4,0],[1242,8,1242,16,0,0,4,1],[1244,8,1244,30,0,0,4,0],[1245,8,1245,54,0,0,4,0],[1251,3,1251,26,0,0,4,1],[1256,3,1256,27,0,0,4,1],[1261,3,1261,28,0,0,4,1],[1265,3,1265,28,0,0,4,1],[1283,3,1283,27,0,0,4,1],[1292,3,1292,10,0,0,4,1]],"cf_tcp_connect":[[1258,6,1258,34,0,0,4,1],[1292,6,1292,13,0,0,4,1],[1296,11,1296,33,0,0,4,1],[1296,37,1296,64,0,0,4,1],[1307,11,1307,32,0,0,4,0],[1314,8,1314,18,0,0,4,0],[319,4,319,10,26,0,4,0],[319,14,319,33,26,0,4,1]],"cf_dns_connect":[[226,18,226,33,0,0,4,1]],"setopt_long_bool":[[563,3,563,20,0,0,4,0],[567,8,567,15,0,0,4,0]],"curl_multi_wakeup":[[1693,6,1693,45,0,0,4,1]],"verifyconnect":[[805,6,805,70,0,0,4,1],[814,6,814,16,0,0,4,1],[814,20,814,40,0,0,4,1],[820,6,820,11,0,0,4,0]],"cf_ip_happy_connect":[[783,6,783,24,0,0,4,1]],"curl_url_get":[[1600,15,1600,41,0,0,4,0],[1601,10,1601,41,0,0,4,1]]},"library_calls":["curl_global_init_mem","curl_getenv","curl_msnprintf","curl_free","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_unescape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_easy_strerror","curl_multi_wakeup","curl_multi_info_read","curl_easy_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_getenv","curl_msnprintf","curl_free","curl_multi_init","curl_easy_init","curl_unescape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_easy_strerror","curl_multi_wakeup","curl_multi_info_read","curl_easy_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" {
}

// Custom memory callbacks for curl_global_init_mem
static void * id_000025_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000025_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000025_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000025_custom_strdup(const char *str) {
    return strdup(str);
}

static void * id_000025_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl with custom memory functions
    CURLcode global_init_result = curl_global_init_mem(CURL_GLOBAL_ALL,
                                                        id_000025_custom_malloc,
                                                        id_000025_custom_free,
                                                        id_000025_custom_realloc,
                                                        id_000025_custom_strdup,
                                                        id_000025_custom_calloc);
    if (global_init_result != CURLE_OK) {
        return 0;
    }

    // Get environment variable
    char *env_var = curl_getenv("HOME");
    if (env_var) {
        // Format a string using curl_msnprintf
        char formatted[256];
        curl_msnprintf(formatted, sizeof(formatted), 
                      "HOME is: %s, input size: %zu", env_var, size);
        
        // Clean up environment variable
        curl_free(env_var);
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set up easy handle options using input data
    if (size > 0) {
        // Use input data as URL (first part as URL string)
        size_t url_len = size > 100 ? 100 : size;
        char *url = (char *)malloc(url_len + 1);
        if (url) {
            memcpy(url, data, url_len);
            url[url_len] = '\0';
            
            // Unescape the URL string (assuming it might be escaped)
            char *unescaped = curl_unescape(url, url_len);
            if (unescaped) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, unescaped);
                curl_free(unescaped);
            } else {
                curl_easy_setopt(easy_handle, CURLOPT_URL, url);
            }
            
            free(url);
        }

        // Set other options
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); // HEAD request
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1000L);
        
        // Use input data as POST data if we have enough
        if (size > 10) {
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                curl_easy_setopt(easy_handle, CURLOPT_POST, 1L);
                curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
                curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, (long)size);
                
                // Add easy handle to multi handle
                curl_multi_add_handle(multi_handle, easy_handle);
                
                int still_running = 0;
                do {
                    CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
                    if (mc != CURLM_OK) {
                        // Get error string
                        const char *error_str = curl_easy_strerror((CURLcode)mc);
                        break;
                    }
                    
                    // Wake up multi handle
                    curl_multi_wakeup(multi_handle);
                    
                    // Read multi info
                    int msgs_in_queue = 0;
                    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
                    if (msg && msg->msg == CURLMSG_DONE) {
                        // Get error string for completed transfer
                        const char *error_str = curl_easy_strerror(msg->data.result);
                        break;
                    }
                    
                } while (still_running);
                
                assert_file_closed(&in_file);;
            }
        }
    }

    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    curl_global_cleanup();
    return 0;
}