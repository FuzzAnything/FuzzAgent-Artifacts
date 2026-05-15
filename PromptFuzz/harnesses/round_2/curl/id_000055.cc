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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"setopt_set_timeout_sec":[[55,6,55,14,0,0,4,1],[58,6,58,36,0,0,4,1]],"setopt_long_net":[[881,3,881,23,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_getenv","curl_free","curl_multi_init","curl_easy_init","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_msnprintf","curl_unescape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_strerror","curl_multi_perform","curl_multi_wakeup","curl_easy_recv","curl_easy_strerror","curl_easy_pause","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_getenv","curl_free","curl_multi_init","curl_easy_init","curl_msnprintf","curl_unescape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_strerror","curl_multi_perform","curl_multi_wakeup","curl_easy_recv","curl_easy_strerror","curl_easy_pause","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom memory callbacks
static void * id_000055_custom_malloc(size_t size) {
    return malloc(size);
}

static void * id_000055_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

static void * id_000055_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static void  id_000055_custom_free(void *ptr) {
    free(ptr);
}

static char * id_000055_custom_strdup(const char *str) {
    return strdup(str);
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl with custom memory functions
    curl_global_init_mem(CURL_GLOBAL_ALL,
                         id_000055_custom_malloc,
                         id_000055_custom_free,
                         id_000055_custom_realloc,
                         id_000055_custom_strdup,
                         id_000055_custom_calloc);
    
    // Get an environment variable
    char *env_var = curl_getenv("PATH");
    if (env_var) {
        curl_free(env_var);
    }
    
    // Initialize handles
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (!multi_handle || !easy_handle) {
        if (multi_handle) curl_multi_cleanup(multi_handle);
        if (easy_handle) curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    if (!in_file || !out_file) {
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Format a message using msnprintf
    char buffer[256];
    int printed = curl_msnprintf(buffer, sizeof(buffer),
                                "Processing %zu bytes of data",
                                size);
    
    // Process input data
    char *unescaped_str = nullptr;
    if (size > 0) {
        // Take a portion of input data for unescaping
        size_t unescape_len = size > 100 ? 100 : size;
        unescaped_str = curl_unescape((const char*)data, (int)unescape_len);
        
        // Use the unescaped string if available
        if (unescaped_str) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
            curl_free(unescaped_str);
        }
    }
    
    // Set additional options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 5L);
    
    // Set up file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Add handle to multi
    CURLMcode mcode = curl_multi_add_handle(multi_handle, easy_handle);
    if (mcode != CURLM_OK) {
        const char *err_str = curl_easy_strerror((CURLcode)mcode);
        // Error string can be used for debugging
    }
    
    // Perform multi operations
    int running_handles = 0;
    mcode = curl_multi_perform(multi_handle, &running_handles);
    
    // Wake up multi handle (even if not strictly needed)
    curl_multi_wakeup(multi_handle);
    
    // Try to receive data
    char recv_buffer[1024];
    size_t nrecv = 0;
    if (size > 0) {
        // Use a portion of input data for simulated receive
        size_t recv_size = size > sizeof(recv_buffer) ? sizeof(recv_buffer) : size;
        CURLcode recv_code = curl_easy_recv(easy_handle, recv_buffer, recv_size, &nrecv);
        
        // Convert error code to string for debugging
        if (recv_code != CURLE_OK) {
            const char *err_str = curl_easy_strerror(recv_code);
            // Error string can be used for debugging
        }
    }
    
    // Pause the handle
    curl_easy_pause(easy_handle, CURLPAUSE_ALL);
    
    // Write received data to output file if any was received
    if (nrecv > 0 && out_file) {
        fwrite(recv_buffer, 1, nrecv, out_file);
    }
    
    // Cleanup
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove the easy handle from multi
    if (multi_handle && easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    if (easy_handle) curl_easy_cleanup(easy_handle);
    if (multi_handle) curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}