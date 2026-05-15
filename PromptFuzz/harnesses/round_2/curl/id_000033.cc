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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 27
//<Quality> {"density":14,"unique_branches":{"getinfo_double":[[495,3,495,27,0,0,4,1],[496,3,496,32,0,0,4,1],[497,3,497,29,0,0,4,0],[498,3,498,32,0,0,4,1],[499,3,499,33,0,0,4,1],[500,3,500,35,0,0,4,1],[501,3,501,30,0,0,4,1],[502,3,502,31,0,0,4,1],[503,3,503,29,0,0,4,1],[505,8,505,14,0,0,4,1],[512,3,512,10,0,0,4,1],[517,3,517,27,0,0,4,1],[520,3,520,32,0,0,4,1],[523,3,523,29,0,0,4,0],[526,3,526,32,0,0,4,1],[529,3,529,33,0,0,4,1],[532,3,532,35,0,0,4,1],[535,3,535,28,0,0,4,1],[538,3,538,30,0,0,4,1],[541,3,541,31,0,0,4,1],[544,3,544,29,0,0,4,1],[547,3,547,40,0,0,4,1],[551,3,551,38,0,0,4,1],[555,3,555,30,0,0,4,1],[559,3,559,10,0,0,4,1]],"Curl_getinfo":[[652,3,652,23,0,0,4,0],[654,8,654,21,0,0,4,0]]},"library_calls":["curl_easy_init","curl_easy_getinfo","curl_ws_start_frame","curl_ws_meta","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_mfprintf","curl_mfprintf","curl_maprintf","curl_free","curl_maprintf","curl_free","curl_easy_cleanup"],"critical_calls":["curl_easy_init","curl_easy_getinfo","curl_ws_start_frame","curl_ws_meta","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_mfprintf","curl_mfprintf","curl_maprintf","curl_free","curl_maprintf","curl_free","curl_easy_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Event: Process WebSocket data and log information about frames
    
    // 1. Use curl_easy_getinfo to get information about the connection
    CURLINFO info;
    double connect_time;
    // Get the connection time info
    CURLcode res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect_time);
    
    // 2. Start a WebSocket frame using the first few bytes of input
    unsigned int ws_flags = 0;
    curl_off_t frame_len = 0;
    
    // Use some input data to determine flags and frame length
    if (size >= 2) {
        ws_flags = data[0] % 8;  // Use first byte for flags (limited to valid flag range)
        frame_len = (curl_off_t)(data[1] % 256);  // Use second byte for frame length
    }
    
    // Start a WebSocket frame
    CURLcode ws_res = curl_ws_start_frame(curl, ws_flags, frame_len);
    
    // 3. Get WebSocket metadata
    const struct curl_ws_frame *ws_meta = curl_ws_meta(curl);
    
    // 4. Use curl_mprintf to format and print information about the frame
    if (ws_meta) {
        // Get file descriptor for output file
        int out_fd = fuzz_fileno(out_file);
        
        // Format and print to stdout using curl_mprintf
        curl_mprintf("WebSocket Frame Info:\n");
        curl_mprintf("  Age: %d\n", ws_meta->age);
        curl_mprintf("  Flags: %d\n", ws_meta->flags);
        curl_mprintf("  Offset: %ld\n", (long)ws_meta->offset);
        curl_mprintf("  Bytes left: %ld\n", (long)ws_meta->bytesleft);
        curl_mprintf("  Length: %zu\n", ws_meta->len);
        
        // Format and print to file using curl_mfprintf
        curl_mfprintf(out_file, "WebSocket frame started with result: %d\n", ws_res);
        curl_mfprintf(out_file, "Connection time: %f\n", connect_time);
        
        // 5. Use curl_maprintf to create allocated strings with frame information
        char *frame_info = curl_maprintf(
            "Frame Metadata: age=%d flags=%d offset=%ld bytesleft=%ld len=%zu",
            ws_meta->age,
            ws_meta->flags,
            (long)ws_meta->offset,
            (long)ws_meta->bytesleft,
            ws_meta->len
        );
        
        if (frame_info) {
            // Write the allocated string to output file
            fputs(frame_info, out_file);
            fputc('\n', out_file);
            
            // Free the allocated string
            curl_free(frame_info);
        }
        
        // Also create another formatted string using data from input
        if (size > 10) {
            // Create a safe string from input data (truncate at 100 chars)
            char input_preview[101];
            size_t preview_len = size > 100 ? 100 : size;
            memcpy(input_preview, data, preview_len);
            input_preview[preview_len] = '\0';
            
            char *data_info = curl_maprintf("Input preview: %s", input_preview);
            if (data_info) {
                fputs(data_info, out_file);
                fputc('\n', out_file);
                curl_free(data_info);
            }
        }
    }
    
    // Read and process some data from the input file to consume it
    uint8_t buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    if (bytes_read > 0) {
        // Process the read data if needed
        // In this case, we just write it to output file
        fwrite(buffer, 1, bytes_read, out_file);
    }
    
    // Clean up all allocated resources
    curl_easy_cleanup(curl);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}