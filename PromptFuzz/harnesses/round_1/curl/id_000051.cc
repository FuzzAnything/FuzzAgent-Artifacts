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
//<ID> 794
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_share_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_add_handle","curl_multi_timeout","curl_multi_poll","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_init","curl_share_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_add_handle","curl_multi_timeout","curl_multi_poll","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global CURL environment
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Set up tracing if we have enough data for a config string
    if (size > 10) {
        char config[11];
        memcpy(config, data, 10);
        config[10] = '\0';
        curl_global_trace(config);
    }
    
    // 3. Create an easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Get curl version
    char *version = curl_version();
    (void)version; // Use version to avoid unused variable warning
    
    // 5. Parse date from input
    if (size > 0) {
        char date_str[256];
        size_t date_len = size > 255 ? 255 : size;
        memcpy(date_str, data, date_len);
        date_str[date_len] = '\0';
        time_t unused = 0;
        time_t parsed_date = curl_getdate(date_str, &unused);
        (void)parsed_date; // Use parsed_date to avoid unused variable warning
    }
    
    // 6. Unescape URL-encoded data
    int outlength = 0;
    char *unescaped = nullptr;
    if (size > 0) {
        char *input_str = (char *)data;
        // Ensure null termination for the portion we want to unescape
        size_t unescape_len = size > 100 ? 100 : size;
        char temp[101];
        memcpy(temp, data, unescape_len);
        temp[unescape_len] = '\0';
        unescaped = curl_easy_unescape(curl, temp, unescape_len, &outlength);
    }
    
    // 7. Free the unescaped string if allocated
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // 8. Duplicate the easy handle
    CURL *dup_curl = curl_easy_duphandle(curl);
    
    // 9. Create a multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        if (dup_curl) curl_easy_cleanup(dup_curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    // 10. Create a share handle
    CURLSH *share = curl_share_init();
    
    // 11. Set options on easy handles
    // Use input data for URL if we have enough data
    if (size > 20) {
        char url[256];
        size_t url_len = size > 255 ? 255 : size;
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        if (dup_curl) {
            curl_easy_setopt(dup_curl, CURLOPT_URL, url);
        }
    }
    
    // Set other options
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    // 12. Set multi handle options
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    
    // 13. Perform upkeep on easy handle
    curl_easy_upkeep(curl);
    
    // 14. Add easy handles to multi handle
    curl_multi_add_handle(multi, curl);
    if (dup_curl) {
        curl_multi_add_handle(multi, dup_curl);
    }
    
    // 15. Get timeout value
    long timeout = 0;
    curl_multi_timeout(multi, &timeout);
    
    // 16. Perform polling
    int ret = 0;
    int max_timeout = timeout > 0 ? (int)timeout : 1000;
    curl_multi_poll(multi, nullptr, 0, max_timeout, &ret);
    
    // 17. Perform transfers
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);
    
    // 18. Create a FILE* for input data if needed
    FILE *in_file = nullptr;
    if (size > 0) {
        in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Get file descriptor
            int fd = fuzz_fileno(in_file);
            (void)fd; // Use fd to avoid unused variable warning
            
            // Read some data from the file
            char buffer[1024];
            size_t read_bytes = fread(buffer, 1, sizeof(buffer), in_file);
            (void)read_bytes; // Use read_bytes to avoid unused variable warning
            
            assert_file_closed(&in_file);;
        }
    }
    
    // 19. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (size > 0) {
            fwrite(data, 1, size > 1000 ? 1000 : size, out_file);
        }
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use out_fd to avoid unused variable warning
        assert_file_closed(&out_file);;
    }
    
    // 20. Clean up multi handle
    curl_multi_remove_handle(multi, curl);
    if (dup_curl) {
        curl_multi_remove_handle(multi, dup_curl);
    }
    curl_multi_cleanup(multi);
    
    // 21. Clean up share handle
    if (share) {
        curl_share_cleanup(share);
    }
    
    // 22. Clean up easy handles
    curl_easy_cleanup(curl);
    if (dup_curl) {
        curl_easy_cleanup(dup_curl);
    }
    
    // 23. Clean up global state
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}