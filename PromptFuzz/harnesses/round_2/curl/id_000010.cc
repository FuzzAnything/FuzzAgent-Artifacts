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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"curl_easy_escape":[[59,13,59,21,0,0,4,1],[60,6,60,13,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_version","curl_easy_option_by_name","curl_easy_setopt","curl_ws_meta","curl_easy_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_escape","curl_easy_unescape","curl_ws_send","curl_free","curl_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_version","curl_easy_option_by_name","curl_easy_setopt","curl_ws_meta","curl_escape","curl_easy_unescape","curl_ws_send","curl_free","curl_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Use curl_version API
    char *version_str = curl_version();
    if (version_str) {
        // Do something with version string (could be used for conditional logic)
        // In fuzzing context, we just ensure it's called
    }

    // Find WebSocket option using curl_easy_option_by_name
    const struct curl_easyoption *ws_opt = curl_easy_option_by_name("WS");
    if (ws_opt) {
        // Try to set the WebSocket option (though connection won't be established)
        curl_easy_setopt(curl, ws_opt->id, 1L);
    }

    // Get WebSocket metadata (will likely return NULL since no WebSocket established)
    const struct curl_ws_frame *meta = curl_ws_meta(curl);
    if (meta) {
        // Do something with meta if available
    }

    // Process input data as strings for escape/unescape operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Read a portion of data for string processing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    size_t read_size = fread(input_str, 1, size, in_file);
    input_str[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Use curl_escape API
    char *escaped = curl_escape(input_str, (int)strlen(input_str));
    if (escaped) {
        // Use curl_easy_unescape API
        int outlength = 0;
        char *unescaped = curl_easy_unescape(curl, escaped, 0, &outlength);
        
        if (unescaped) {
            // Try to send the unescaped data as WebSocket message
            // We'll send a small portion to avoid excessive allocation
            size_t send_size = (outlength < 1024) ? outlength : 1024;
            size_t sent = 0;
            
            // Use curl_ws_send API (will likely fail without proper connection setup)
            unsigned int flags = CURLWS_BINARY;
            curl_ws_send(curl, unescaped, send_size, &sent, 0, flags);
            
            curl_free(unescaped);
        }
        curl_free(escaped);
    }

    // Cleanup
    free(input_str);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}