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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 34.666668, nr_unique_branch: 5
//<Quality> {"density":26,"unique_branches":{"multi_addtimeout":[[3589,10,3589,18,0,0,4,0]],"setopt_func":[[2552,3,2552,29,0,0,4,0],[2557,8,2557,23,0,0,4,1],[2568,3,2568,28,0,0,4,1]],"Curl_llist_insert_next":[[91,8,91,10,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_version","curl_getdate","curl_easy_unescape","curl_easy_duphandle","curl_multi_init","curl_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_setopt","curl_share_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_free"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_version","curl_getdate","curl_easy_unescape","curl_easy_duphandle","curl_multi_init","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_setopt","curl_share_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_free"],"visited":2}
/*Here's a C++ fuzz driver that implements the specified event using the curl library APIs:

*/


// Callback function for reading data
static size_t  id_000038_read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    FILE *readhere = (FILE *)userp;
    size_t retcode = fread(ptr, size, nmemb, readhere);
    return retcode;
}

// Callback function for writing data
static size_t  id_000038_write_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    FILE *writehere = (FILE *)userp;
    size_t retcode = fwrite(ptr, size, nmemb, writehere);
    return retcode;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // Create file pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create file pointer for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Initialize global curl
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Configure global trace (simulating with dummy config)
    curl_global_trace(fuzz_str_1);
    
    // 3. Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Get curl version info
    char *version_info = curl_version();
    (void)version_info; // Use to avoid unused variable warning
    
    // 5. Parse date from input data (first 100 bytes or size if smaller)
    size_t date_len = size > 100 ? 100 : size;
    char date_str[101];
    memcpy(date_str, data, date_len);
    date_str[date_len] = '\0';
    time_t parsed_time = curl_getdate(date_str, NULL);
    (void)parsed_time; // Use to avoid unused variable warning
    
    // 6. Unescape some data (use a portion of input)
    size_t unescape_len = size > 50 ? 50 : size;
    char *unescaped = nullptr;
    if (unescape_len > 0) {
        char *to_unescape = (char *)malloc(unescape_len + 1);
        if (to_unescape) {
            memcpy(to_unescape, data, unescape_len);
            to_unescape[unescape_len] = '\0';
            int outlength = 0;
            unescaped = curl_easy_unescape(easy_handle, to_unescape, unescape_len, &outlength);
            free(to_unescape);
        }
    }
    
    // 7. Duplicate easy handle
    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    
    // 8. Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (unescaped) curl_free(unescaped);
        curl_easy_cleanup(easy_handle);
        if (duplicated_handle) curl_easy_cleanup(duplicated_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 9. Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
    
    // 10. Set easy handle options
    // Set read callback and data
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION,  id_000038_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    
    // Set write callback and data
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION,  id_000038_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    
    // Set a dummy URL (required for some operations)
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/test");
    
    // Disable signals and timeout to make it safe
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 5L);
    
    // 11. Perform upkeep on easy handle
    curl_easy_upkeep(easy_handle);
    
    // 12. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // If duplicated handle exists, also add it
    if (duplicated_handle) {
        curl_multi_add_handle(multi_handle, duplicated_handle);
    }
    
    // 13. Perform multi handle operations
    int still_running = 0;
    int numfds = 0;
    
    // Perform initial multi perform
    CURLMcode multi_code = curl_multi_perform(multi_handle, &still_running);
    
    // If there are transfers running, wait for activity
    if (still_running) {
        // Wait for activity with a timeout
        multi_code = curl_multi_wait(multi_handle, NULL, 0, fuzz_int32_t_2, &numfds);
        
        // Perform again
        if (multi_code == CURLM_OK) {
            curl_multi_perform(multi_handle, &still_running);
        }
    }
    
    // 14. Initialize share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        // 15. Set share handle options
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        
        // 16. Clean up share handle
        curl_share_cleanup(share_handle);
    }
    
    // 17. Clean up multi handle
    if (duplicated_handle) {
        curl_multi_remove_handle(multi_handle, duplicated_handle);
    }
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // 18. Clean up global curl
    curl_global_cleanup();
    
    // Clean up easy handles
    curl_easy_cleanup(easy_handle);
    if (duplicated_handle) {
        curl_easy_cleanup(duplicated_handle);
    }
    
    // Free unescaped data if allocated
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}