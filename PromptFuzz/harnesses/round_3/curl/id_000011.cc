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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"curl_easy_option_next":[[68,6,68,10,0,0,4,1],[73,11,73,16,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_data","curl_easy_upkeep","curl_share_init","curl_share_setopt","curl_share_strerror","curl_share_cleanup","curl_easy_header","curl_easy_option_by_name","curl_easy_option_next","curl_easy_unescape","curl_free","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_upkeep","curl_share_init","curl_share_setopt","curl_share_strerror","curl_share_cleanup","curl_easy_header","curl_easy_option_by_name","curl_easy_option_next","curl_easy_unescape","curl_free","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create an easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Use curl_mime_data with input data
    // Ensure we don't exceed input bounds
    size_t mime_data_size = size > 1024 ? 1024 : size;
    curl_mime_data(part, reinterpret_cast<const char*>(data), mime_data_size);

    // Use curl_easy_upkeep on the easy handle
    curl_easy_upkeep(easy);

    // Create a share handle for error testing
    CURLSH *share = curl_share_init();
    if (share) {
        // Try to set an invalid option to generate an error
        CURLSHcode share_error = curl_share_setopt(share, static_cast<CURLSHoption>(9999), nullptr);
        
        // Use curl_share_strerror to get error string
        const char *error_str = curl_share_strerror(share_error);
        // We don't need to do anything with the error string in fuzzing context
        
        curl_share_cleanup(share);
    }

    // Use curl_easy_header - try to get a header (will likely fail since no transfer)
    struct curl_header *header_out = nullptr;
    size_t header_index = data[0] % 10;  // Use first byte for index
    CURLHcode header_code = curl_easy_header(easy, fuzz_str_1, header_index, 
                                             CURLH_HEADER, fuzz_int32_t_2, &header_out);

    // Use curl_easy_option_by_name
    const struct curl_easyoption *option_by_name = curl_easy_option_by_name(fuzz_str_3);
    
    // Use curl_easy_option_next to iterate through options
    const struct curl_easyoption *current_option = nullptr;
    int option_count = 0;
    while ((current_option = curl_easy_option_next(current_option)) != nullptr && 
           option_count < 10) {  // Limit iterations for performance
        option_count++;
    }

    // Use curl_easy_unescape with input data
    // Create a test escaped string using part of input data
    size_t escape_data_size = size > 512 ? 512 : size;
    char *test_string = static_cast<char*>(malloc(escape_data_size + 1));
    if (test_string) {
        memcpy(test_string, data, escape_data_size);
        test_string[escape_data_size] = '\0';
        
        int outlength = 0;
        char *unescaped = curl_easy_unescape(easy, test_string, escape_data_size, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
        free(test_string);
    }

    // Clean up
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    return 0;
}