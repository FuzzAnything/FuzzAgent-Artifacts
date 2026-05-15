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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_maprintf","curl_free","curl_mfprintf","curl_ws_start_frame","curl_ws_meta","curl_mfprintf","curl_easy_getinfo","curl_mfprintf","curl_easy_getinfo","curl_mfprintf","curl_maprintf","curl_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_maprintf","curl_free","curl_mfprintf","curl_ws_start_frame","curl_ws_meta","curl_mfprintf","curl_easy_getinfo","curl_mfprintf","curl_easy_getinfo","curl_mfprintf","curl_maprintf","curl_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Use curl_mprintf - format and print to string allocated with malloc
    char *formatted_str = curl_maprintf("Input size: %zu bytes", size);
    if (formatted_str) {
        // Write formatted string to output file
        fwrite(formatted_str, 1, strlen(formatted_str), out_file);
        fwrite("\n", 1, 1, out_file);
        curl_free(formatted_str);
    }

    // Use curl_mprintf with fprintf equivalent
    curl_mfprintf(out_file, "Data processed: %zu bytes\n", size);

    // Try to start a WebSocket frame (may fail without actual connection)
    unsigned int ws_flags = 0;
    curl_off_t frame_len = (curl_off_t)(size % 1024); // Limit frame length
    CURLcode ws_start_res = curl_ws_start_frame(curl, ws_flags, frame_len);
    
    // Get WebSocket metadata (will be NULL without actual WebSocket connection)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(curl);
    if (ws_meta) {
        curl_mfprintf(out_file, "WS Frame age: %d\n", ws_meta->age);
    }

    // Get some info from the easy handle
    char *effective_url = nullptr;
    CURLcode info_res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    if (info_res == CURLE_OK && effective_url) {
        curl_mfprintf(out_file, "Effective URL: %s\n", effective_url);
    }

    // Get more info - response code
    long response_code = 0;
    info_res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (info_res == CURLE_OK) {
        curl_mfprintf(out_file, "Response code: %ld\n", response_code);
    }

    // Process input data using curl_maprintf with the actual data
    // Ensure we don't overflow by limiting the size we use
    size_t data_to_use = size > 100 ? 100 : size;
    char *data_str = (char *)malloc(data_to_use + 1);
    if (data_str) {
        memcpy(data_str, data, data_to_use);
        data_str[data_to_use] = '\0';
        
        // Use curl_maprintf with the data
        char *data_formatted = curl_maprintf("Data preview: %s", data_str);
        if (data_formatted) {
            fwrite(data_formatted, 1, strlen(data_formatted), out_file);
            fwrite("\n", 1, 1, out_file);
            curl_free(data_formatted);
        }
        free(data_str);
    }

    // Write some of the input data to output file
    if (size > 0) {
        size_t write_size = size > 512 ? 512 : size;
        fwrite(data, 1, write_size, out_file);
    }

    // Clean up
    curl_easy_cleanup(curl);
    assert_file_closed(&out_file);;
    curl_global_cleanup();

    assert_file_closed(&out_file);
	return 0;
}