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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 126, nr_unique_branch: 13
//<Quality> {"density":21,"unique_branches":{"Curl_cpool_upkeep":[[752,6,752,12,0,0,4,1],[43,8,43,9,1,0,4,0],[54,8,54,9,2,0,4,0],[41,4,41,7,3,0,4,1],[41,4,41,7,8,0,4,1]],"setopt_long_bool":[[465,3,465,22,0,0,4,0],[471,8,471,22,0,0,4,0]],"curl_easy_upkeep":[[1323,6,1323,23,0,0,4,1],[1326,6,1326,31,0,0,4,1],[134,5,134,8,1,0,4,0],[134,12,134,49,1,0,4,0]],"Curl_pretransfer":[[510,11,510,48,0,0,4,1]],"Curl_init_do":[[3524,6,3524,23,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_encoder","curl_mime_type","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_easy_upkeep","curl_easy_header","curl_easy_nextheader","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_encoder","curl_mime_type","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_easy_upkeep","curl_easy_header","curl_easy_nextheader","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for processing
    
    // Initialize libcurl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use curl_strnequal to compare parts of the input
    // Split input into two parts for comparison
    size_t half_size = size / 2;
    const char *s1 = reinterpret_cast<const char*>(data);
    const char *s2 = reinterpret_cast<const char*>(data + half_size);
    size_t compare_len = half_size < 100 ? half_size : 100;
    
    int str_equal = curl_strnequal(s1, s2, compare_len);
    
    // 2. Create MIME part and set encoder
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input as mime data
            size_t mime_data_size = size > 200 ? 200 : size;
            char *mime_data = (char*)malloc(mime_data_size + 1);
            if (mime_data) {
                memcpy(mime_data, data, mime_data_size);
                mime_data[mime_data_size] = '\0';
                
                curl_mime_data(part, mime_data, mime_data_size);
                curl_mime_encoder(part, "base64");  // Use base64 encoding
                
                free(mime_data);
            }
            
            // Set MIME type using part of input
            if (size > 50) {
                char mime_type[50];
                size_t mime_type_len = size > 100 ? 50 : size/2;
                if (mime_type_len > 49) mime_type_len = 49;
                memcpy(mime_type, data + 10, mime_type_len);
                mime_type[mime_type_len] = '\0';
                curl_mime_type(part, mime_type);
            }
        }
        
        // Set MIME data to easy handle
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }
    
    // 3. Set up easy handle with options
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);  // HEAD request only
    
    // 4. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 5. Get multi timeout
    long timeout_ms = 0;
    CURLMcode timeout_res = curl_multi_timeout(multi, &timeout_ms);
    
    // 6. Perform upkeep on easy handle
    curl_easy_upkeep(easy);
    
    // 7. Process headers
    unsigned int origin = CURLH_HEADER;
    struct curl_header *header = NULL;
    
    // Try to get first header (will likely be NULL without actual request)
    curl_easy_header(easy, "Content-Type", 0, origin, 0, &header);
    
    // Get next header
    struct curl_header *next_header = curl_easy_nextheader(easy, origin, 0, header);
    
    // Try to perform a single read to trigger header processing
    int still_running = 0;
    curl_multi_perform(multi, &still_running);
    
    // 8. Clean up resources
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove and cleanup handles
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}