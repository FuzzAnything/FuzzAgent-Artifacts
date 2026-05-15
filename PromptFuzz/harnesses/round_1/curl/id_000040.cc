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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 6
//<Quality> {"density":19,"unique_branches":{"curl_multi_cleanup":[[2897,12,2897,32,0,0,4,1],[2900,12,2900,29,0,0,4,0],[2900,33,2900,43,0,0,4,1],[2909,12,2909,36,0,0,4,0],[2912,12,2912,32,0,0,4,1],[2914,15,2914,69,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_easy_upkeep","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_setopt","curl_share_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_easy_upkeep","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_setopt","curl_share_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified curl APIs:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for testing
    
    // 1. Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
	return 0;
	}
	
    
    // 2. Set global trace configuration
    std::vector<char> config_data(size/4 + 1);
    memcpy(config_data.data(), data, size/4);
    config_data[size/4] = '\0';
    CURLcode trace_res = curl_global_trace(config_data.data());
    
    // 3. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Get curl version
    char *version = curl_version();
    
    // 5. Parse date from input data
    std::vector<char> date_str(size/4 + 1);
    memcpy(date_str.data(), data + size/4, size/4);
    date_str[size/4] = '\0';
    time_t parsed_date = curl_getdate(date_str.data(), NULL);
    
    // 6. Unescape data
    std::vector<char> escape_data(size/4);
    memcpy(escape_data.data(), data + size/2, size/4);
    int outlength = 0;
    char *unescaped = curl_easy_unescape(easy_handle, escape_data.data(), 
                                         escape_data.size(), &outlength);
    if (unescaped) curl_free(unescaped);
    
    // 7. Duplicate easy handle
    CURL *duped_handle = curl_easy_duphandle(easy_handle);
    
    // 8. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        if (duped_handle) curl_easy_cleanup(duped_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 9. Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 5L);
    
    // 10. Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    
    // 11. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
        assert_file_closed(&in_file);;
    }
    
    // 12. Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 13. Add handles to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (duped_handle) {
        curl_multi_add_handle(multi_handle, duped_handle);
    }
    
    // 14. Perform upkeep
    curl_easy_upkeep(easy_handle);
    
    // 15. Perform multi transfers
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // 16. Create share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        // 17. Set share options
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        
        // 18. Cleanup share handle
        curl_share_cleanup(share_handle);
    }
    
    // 19. Cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // 20. Cleanup easy handles
    curl_easy_cleanup(easy_handle);
    if (duped_handle) curl_easy_cleanup(duped_handle);
    
    // 21. Final global cleanup
    curl_global_cleanup();
    
    // Clean up output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}