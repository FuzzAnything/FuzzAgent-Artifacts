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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.6666665, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_easy_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_easy_cleanup","curl_multi_init","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Global initialization
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        return 0;
    }

    // 2. Global tracing (use part of input as config string)
    size_t config_len = size > 100 ? 100 : size;
    char *config = (char *)malloc(config_len + 1);
    if (!config) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(config, data, config_len);
    config[config_len] = '\0';
    curl_global_trace(config);
    free(config);

    // 3. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // 4. Get curl version
    char *version = curl_version();
    (void)version; // Use version to avoid unused variable warning

    // 5. Parse date from input
    size_t date_len = size > 50 ? 50 : size;
    char *date_str = (char *)malloc(date_len + 1);
    if (date_str) {
        memcpy(date_str, data, date_len);
        date_str[date_len] = '\0';
        time_t parsed_date = curl_getdate(date_str, NULL);
        (void)parsed_date;
        free(date_str);
    }

    // 6. Unescape string (use middle portion of input)
    size_t unescape_offset = size / 3;
    size_t unescape_len = size > unescape_offset + 50 ? 50 : size - unescape_offset;
    if (unescape_len > 0) {
        char *to_unescape = (char *)malloc(unescape_len + 1);
        if (to_unescape) {
            memcpy(to_unescape, data + unescape_offset, unescape_len);
            to_unescape[unescape_len] = '\0';
            int outlength = 0;
            char *unescaped = curl_easy_unescape(easy, to_unescape, unescape_len, &outlength);
            if (unescaped) {
                curl_free(unescaped);
            }
            free(to_unescape);
        }
    }

    // 7. Duplicate easy handle
    CURL *easy_dup = curl_easy_duphandle(easy);
    if (easy_dup) {
        curl_easy_cleanup(easy_dup);
    }

    // 8. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // 9. Set multi handle options
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_NOTHING);

    // 10. Set easy handle options using input data
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Set various easy options
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
        curl_easy_setopt(easy, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
        
        // Use part of input as URL
        size_t url_len = size > 30 ? 30 : size;
        char *url = (char *)malloc(url_len + 1);
        if (url) {
            memcpy(url, data, url_len);
            url[url_len] = '\0';
            curl_easy_setopt(easy, CURLOPT_URL, url);
            free(url);
        }
    }

    // 11. Call easy upkeep
    curl_easy_upkeep(easy);

    // 12. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // 13. Perform multi handle operations
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);

    // 14. Create share handle
    CURLSH *share = curl_share_init();
    if (share) {
        // 15. Set share handle options
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        
        // 16. Cleanup share handle
        curl_share_cleanup(share);
    }

    // Cleanup sequence
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    // 17. Cleanup multi handle
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    
    // Cleanup easy handle
    curl_easy_cleanup(easy);
    
    // 18. Global cleanup
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}