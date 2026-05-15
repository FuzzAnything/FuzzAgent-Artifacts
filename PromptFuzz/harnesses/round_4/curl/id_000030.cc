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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 11
//<Quality> {"density":23,"unique_branches":{"mev_sh_entry_update":[[232,6,232,31,0,0,4,0]],"mev_pollset_diff":[[314,8,314,14,0,0,4,1],[322,13,322,17,0,0,4,1],[335,8,335,18,0,0,4,1],[352,18,352,32,0,0,4,0],[353,12,353,36,0,0,4,0],[371,16,371,25,0,0,4,0],[372,10,372,29,0,0,4,0],[378,8,378,17,0,0,4,0]],"setopt_func":[[2515,3,2515,32,0,0,4,0],[2520,8,2520,20,0,0,4,0]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_socket_action","curl_easy_setopt","curl_easy_setopt","curl_easy_send","curl_ws_send","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_socket_action","curl_easy_setopt","curl_easy_setopt","curl_easy_send","curl_ws_send","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Helper function to read data from input
static size_t  id_000030_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    FILE *f = (FILE *)instream;
    return fread(buffer, size, nitems, f);
}

// Helper function to write data to output
static size_t  id_000030_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *f = (FILE *)userdata;
    return fwrite(ptr, size, nmemb, f);
}

// Progress callback
static int  id_000030_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    return 0;
}

// Socket callback for multi socket action
static int  id_000030_socket_callback(CURL *easy, curl_socket_t s, int what, void *cbp, void *sockp) {
    return 0;
}

// Timer callback for multi
static int  id_000030_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<45) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create URL handle
    CURLU *url = curl_url();
    if (!url) {
        curl_global_cleanup();
        return 0;
    }

    // Use part of input data as URL components
    size_t url_len = size > 100 ? 100 : size;
    char *url_part = (char *)malloc(url_len + 1);
    if (!url_part) {
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_part, data, url_len);
    url_part[url_len] = '\0';

    // Set URL parts
    CURLUcode url_res = curl_url_set(url, CURLUPART_URL, fuzz_str_1, fuzz_uint32_t_2);
    if (url_res == CURLUE_OK) {
        // Get URL parts back
        char *retrieved_part = nullptr;
        CURLUcode get_res = curl_url_get(url, CURLUPART_HOST, &retrieved_part, fuzz_uint32_t_3);
        if (get_res == CURLUE_OK && retrieved_part) {
            curl_free(retrieved_part);
        }
    }

    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        free(url_part);
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }

    // Set easy options
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(easy, CURLOPT_PROGRESSFUNCTION,  id_000030_progress_callback);
    curl_easy_setopt(easy, CURLOPT_READFUNCTION,  id_000030_read_callback);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION,  id_000030_write_callback);

    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        free(url_part);
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }

    // Set multi options
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION,  id_000030_socket_callback);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION,  id_000030_timer_callback);
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 10L);

    // Add easy handle to multi
    CURLMcode multi_add_res = curl_multi_add_handle(multi, easy);
    if (multi_add_res == CURLM_OK) {
        int running_handles = 0;
        
        // Perform initial multi perform
        curl_multi_perform(multi, &running_handles);
        
        // Setup waitfd for multi wait
        struct curl_waitfd wait_fd;
        wait_fd.fd = 0; // stdin
        wait_fd.events = CURL_WAIT_POLLIN;
        wait_fd.revents = 0;
        
        int numfds = 0;
        curl_multi_wait(multi, &wait_fd, 1, fuzz_int32_t_4, &numfds);
        
        // Try socket action
        curl_multi_socket_action(multi, CURL_SOCKET_BAD, fuzz_int32_t_5, &running_handles);
        
        // Create FILE* for input data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                // Setup callbacks with file handles
                curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
                curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
                
                // Try to send data
                size_t sent = 0;
                size_t ws_sent = 0;
                
                // Regular send (though connection isn't actually established)
                curl_easy_send(easy, data, size > 1024 ? 1024 : size, &sent);
                
                // WebSocket send (simulated)
                // Note: This would normally require an established WS connection
                curl_ws_send(easy, data, size > 1024 ? 1024 : size, &ws_sent, fuzz_int64_t_6, CURLWS_BINARY);
                
                assert_file_closed(&out_file);;
            }
            assert_file_closed(&in_file);;
        }
        
        // Remove handle from multi
        curl_multi_remove_handle(multi, easy);
    }

    // Cleanup
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    free(url_part);
    curl_url_cleanup(url);
    curl_global_cleanup();

    return 0;
}