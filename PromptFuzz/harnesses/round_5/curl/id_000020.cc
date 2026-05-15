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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 8
//<Quality> {"density":19,"unique_branches":{"file_upload":[[310,6,310,34,0,0,4,0],[328,20,328,24,0,0,4,1]],"cr_in_read":[[663,6,663,25,0,0,4,0],[726,8,726,27,0,0,4,0]],"curlx_sotouz_range":[[288,6,288,15,0,0,4,1],[1290,24,1290,33,1,0,4,0],[1290,25,1290,26,1,0,4,0],[1290,37,1290,38,1,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_info_read","curl_easy_strerror","curl_strnequal","curl_mprintf","curl_msprintf","curl_msnprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_info_read","curl_easy_strerror","curl_strnequal","curl_mprintf","curl_msprintf","curl_msnprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom callback to read from memory
static size_t  id_000020_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    FILE *f = (FILE *)instream;
    return fread(buffer, size, nitems, f);
}

// Custom callback to write to memory
static size_t  id_000020_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *f = (FILE *)userdata;
    return fwrite(ptr, size, nmemb, f);
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    if (!in_file || !out_file) {
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
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
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set curl options
    curl_easy_setopt(curl, CURLOPT_URL, "file:///dev/null"); // Dummy URL
    curl_easy_setopt(curl, CURLOPT_READFUNCTION,  id_000020_read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, in_file);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  id_000020_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, curl);
    
    // Perform the transfer
    int running_handles = 0;
    CURLMcode mres = curl_multi_perform(multi_handle, &running_handles);
    
    // Process messages
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // Use curl_easy_strerror to get error string
    const char *error_str = curl_easy_strerror(CURLE_OK);
    
    // Use curl_strnequal for string comparison
    int str_equal = curl_strnequal("test", "test", 4);
    
    // Use curl_mprintf family for formatted output
    char buffer[256];
    curl_mprintf("Fuzzing curl\n");
    curl_msprintf(buffer, "Messages in queue: %d\n", msgs_in_queue);
    curl_msnprintf(buffer, sizeof(buffer), "Error string: %s\n", error_str);
    
    // Clean up multi handle
    curl_multi_remove_handle(multi_handle, curl);
    curl_multi_cleanup(multi_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(curl);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up global curl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}