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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 10
//<Quality> {"density":6,"unique_branches":{"setopt_long_net":[[923,3,923,28,0,0,4,0],[924,8,924,15,0,0,4,1],[924,19,924,26,0,0,4,1]],"Curl_getconnectinfo":[[279,6,279,38,0,0,4,1]],"easy_connection":[[1203,6,1203,29,0,0,4,1],[1210,6,1210,28,0,0,4,0]],"Curl_connect_only_attach":[[1254,6,1254,12,0,0,4,0]],"curl_ws_send":[[1792,21,1792,43,0,0,4,0],[1794,8,1794,14,0,0,4,0]],"curl_easy_unescape":[[167,16,167,29,0,0,4,1]]},"library_calls":["curl_global_init","curl_version","curl_global_cleanup","curl_global_cleanup","curl_escape","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_cleanup","curl_free","curl_easy_option_by_name","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_ws_meta","curl_ws_send","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_escape","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_cleanup","curl_free","curl_easy_option_by_name","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_ws_meta","curl_ws_send","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified curl APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Too small to do anything meaningful
    }

    // Initialize curl
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_result != CURLE_OK) {
        return 0;
    }

    // 1. Get curl version
    char *version = curl_version();
    if (version) {
        // Use version info (can be printed or processed)
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Escape and unescape operations using input data
    // Extract a string from the input data (first half)
    size_t str_len = size / 2;
    char *input_str = (char *)malloc(str_len + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_str, 1, str_len, in_file);
    input_str[str_len] = '\0';

    // Escape the string
    char *escaped = curl_escape(input_str, (int)str_len);
    
    // Unescape using easy interface (we'll create a dummy handle)
    CURL *dummy_handle = curl_easy_init();
    if (dummy_handle && escaped) {
        int outlength = 0;
        char *unescaped = curl_easy_unescape(dummy_handle, escaped, -1, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
        curl_easy_cleanup(dummy_handle);
    }
    
    if (escaped) {
        curl_free(escaped);
    }

    // 3. Look up an easy option by name (using input data as option name)
    // Use first 50 bytes or less as option name
    size_t opt_name_len = size < 50 ? size : 50;
    char *opt_name = (char *)malloc(opt_name_len + 1);
    if (opt_name) {
        // Read option name from file (we've already read some, so seek back)
        fseek(in_file, 0, SEEK_SET);
        fread(opt_name, 1, opt_name_len, in_file);
        opt_name[opt_name_len] = '\0';
        
        const struct curl_easyoption *option = curl_easy_option_by_name(opt_name);
        // option might be NULL if name is invalid, that's OK
        free(opt_name);
    }

    // 4. Create a CURL handle for WebSocket operations
    CURL *curl = curl_easy_init();
    if (curl) {
        // Set up basic options (no actual connection needed for fuzzing)
        curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080");
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // WebSocket mode
        
        // Try to get WebSocket metadata (will return NULL since no connection)
        const struct curl_ws_frame *meta = curl_ws_meta(curl);
        // meta will be NULL, that's expected
        
        // 5. Try WebSocket send with input data
        // Use the second half of input data as buffer
        size_t buffer_size = size - str_len;
        if (buffer_size > 0) {
            uint8_t *ws_buffer = (uint8_t *)malloc(buffer_size);
            if (ws_buffer) {
                fread(ws_buffer, 1, buffer_size, in_file);
                
                size_t sent = 0;
                // Try to send WebSocket data (will fail since no connection, but API is called)
                CURLcode ws_result = curl_ws_send(curl, ws_buffer, buffer_size, &sent, 0, CURLWS_BINARY);
                // Result will be error, but API is exercised
                
                free(ws_buffer);
            }
        }
        
        curl_easy_cleanup(curl);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    free(input_str);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}