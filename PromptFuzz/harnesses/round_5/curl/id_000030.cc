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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 41
//<Quality> {"density":11,"unique_branches":{"curl_multi_setopt":[[3234,6,3234,24,0,0,4,1],[3237,6,3237,24,0,0,4,1],[3243,3,3243,31,0,0,4,1],[3246,3,3246,27,0,0,4,1],[3249,3,3249,29,0,0,4,1],[3252,3,3252,25,0,0,4,1],[3255,3,3255,27,0,0,4,1],[3258,3,3258,30,0,0,4,1],[3261,3,3261,26,0,0,4,1],[3264,3,3264,28,0,0,4,0],[3266,8,3266,24,0,0,4,0],[3269,3,3269,37,0,0,4,1],[3273,3,3273,38,0,0,4,1],[3278,3,3278,36,0,0,4,1],[3280,3,3280,44,0,0,4,1],[3282,3,3282,42,0,0,4,1],[3284,3,3284,35,0,0,4,1],[3286,3,3286,37,0,0,4,1],[3288,3,3288,39,0,0,4,1],[3295,3,3295,32,0,0,4,1],[3310,3,3310,31,0,0,4,1],[3313,3,3313,27,0,0,4,1],[3316,3,3316,36,0,0,4,1],[3341,3,3341,27,0,0,4,1],[3344,3,3344,10,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"global_init":[[129,6,129,17,0,0,4,1]],"curl_multi_get_handles":[[3804,6,3804,7,0,0,4,0],[3808,8,3808,58,0,0,4,0],[3812,12,3812,33,0,0,4,0],[3812,12,3812,33,0,0,4,1],[3814,15,3814,69,0,0,4,0],[3814,15,3814,69,0,0,4,1]],"Curl_uint32_tbl_next":[[195,6,195,57,0,0,4,0]],"curl_global_init_mem":[[220,6,220,8,0,0,4,1],[220,12,220,14,0,0,4,1],[220,18,220,20,0,0,4,1],[220,24,220,26,0,0,4,1],[220,30,220,32,0,0,4,1],[225,6,225,17,0,0,4,1]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_setopt","curl_multi_get_handles","curl_easy_pause","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_multi_setopt","curl_multi_get_handles","curl_easy_pause","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Helper function to create formatted string using curl_mvsprintf
static int  id_000030_curl_format_string(char* buffer, size_t bufsize, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize curl with custom memory functions
    CURLcode global_init_result = curl_global_init_mem(
        CURL_GLOBAL_ALL,
        malloc,    // curl_malloc_callback
        free,      // curl_free_callback
        realloc,   // curl_realloc_callback
        strdup,    // curl_strdup_callback
        calloc     // curl_calloc_callback
    );
    
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create a multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Set multi handle options using input data
    // Use first byte to determine which option to set
    CURLMoption option = CURLMOPT_MAXCONNECTS;
    long option_value = static_cast<long>(data[0] % 100);
    
    curl_multi_setopt(multi_handle, option, option_value);
    
    // Get handles from multi handle
    CURL** handles = curl_multi_get_handles(multi_handle);
    
    // Use curl_mvsprintf to format a string with input data
    // Create buffer for formatted output
    char format_buffer[256];
    const char* format_str = "Data size: %zu, First byte: 0x%02x";
    
    // Format the string using curl_mvsprintf
     id_000030_curl_format_string(format_buffer, sizeof(format_buffer), 
                      format_str, size, data[0]);
    
    // Pause the easy handle using input data to determine bitmask
    // Use second byte (if available) to determine pause action
    int bitmask = 0;
    if (size > 1) {
        bitmask = data[1] & 0x03; // Use only lower 2 bits
        if (bitmask & 0x01) bitmask |= CURLPAUSE_RECV;
        if (bitmask & 0x02) bitmask |= CURLPAUSE_SEND;
    }
    
    curl_easy_pause(easy_handle, bitmask);
    
    // Clean up - remove handles in reverse order
    if (handles) {
        // Free the array returned by curl_multi_get_handles
        // Note: The documentation doesn't specify how to free this,
        // but we should use curl_free if it was allocated by curl
        curl_free(handles);
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}