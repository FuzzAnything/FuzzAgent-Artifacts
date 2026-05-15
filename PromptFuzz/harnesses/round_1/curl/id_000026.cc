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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 7
//<Quality> {"density":19,"unique_branches":{"curl_ws_start_frame":[[1877,6,1877,23,0,0,4,1],[1879,6,1879,27,0,0,4,1],[1887,6,1887,17,0,0,4,0],[134,5,134,8,1,0,4,0],[134,12,134,49,1,0,4,0],[319,4,319,10,16,0,4,0],[319,14,319,33,16,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data_cb","curl_mime_free","curl_getdate","curl_maprintf","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_ws_start_frame","curl_ws_meta","curl_ws_recv","curl_easy_pause","curl_multi_notify_disable","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data_cb","curl_mime_free","curl_getdate","curl_maprintf","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_ws_start_frame","curl_ws_meta","curl_ws_recv","curl_easy_pause","curl_multi_notify_disable","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*I'll create a fuzz driver that uses the specified curl APIs to simulate a WebSocket client that receives and processes data with MIME encoding.

*/


// Callback function for MIME data
static size_t  id_000026_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *file = (FILE *)arg;
    return fread(buffer, size, nitems, file);
}

static int  id_000026_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *file = (FILE *)arg;
    if (fseek(file, offset, origin) != 0) {
        return CURL_SEEKFUNC_CANTSEEK;
    }
    return CURL_SEEKFUNC_OK;
}

static void  id_000026_free_callback(void *arg) {
    // Nothing to free as we're using fmemopen
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a memory file for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize easy handle for WebSocket
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create MIME structure and set up encoder
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Set encoding for MIME part
            curl_mime_encoder(part, fuzz_str_1);
            
            // Set up data callback for MIME part using input data
            curl_mime_data_cb(part, size, 
                             id_000026_read_callback, 
                             id_000026_seek_callback, 
                             id_000026_free_callback, 
                            in_file);
        }
        
        // Clean up MIME
        curl_mime_free(mime);
    }
    
    // 2. Parse date from a portion of the input data
    if (size > 50) {
        // Use first 50 bytes as date string (if available)
        char date_str[51];
        size_t date_len = size < 50 ? size : 50;
        memcpy(date_str, data, date_len);
        date_str[date_len] = '\0';
        
        time_t parsed_time = curl_getdate(date_str, NULL);
        // We don't need to use parsed_time, just called the function
    }
    
    // 3. Format a string using maprintf
    char *formatted = curl_maprintf("Received %zu bytes at %ld", size, time(NULL));
    if (formatted) {
        // Write formatted string to output file
        fwrite(formatted, strlen(formatted), 1, out_file);
        curl_free(formatted);
    }
    
    // 4. Set up WebSocket connection and frames
    if (curl) {
        // Configure for WebSocket
        curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com/ws");
        
        // Add to multi handle
        curl_multi_add_handle(multi_handle, curl);
        
        // 5. Start a WebSocket frame
        size_t frame_len = size < 1000 ? size : 1000;
        curl_ws_start_frame(curl, CURLWS_TEXT, frame_len);
        
        // 6. Receive WebSocket data (simulated - use input buffer)
        char recv_buffer[1024];
        size_t bytes_received = 0;
        const struct curl_ws_frame *meta = NULL;
        
        // Simulate receiving by copying first part of input
        size_t to_copy = size < sizeof(recv_buffer) ? size : sizeof(recv_buffer);
        memcpy(recv_buffer, data, to_copy);
        bytes_received = to_copy;
        
        // Get WebSocket metadata
        meta = curl_ws_meta(curl);
        
        // 7. Actually try to receive (this will likely fail without real connection)
        // but we call the API anyway
        curl_ws_recv(curl, recv_buffer, sizeof(recv_buffer), &bytes_received, &meta);
        
        // 8. Pause the connection
        curl_easy_pause(curl, CURLPAUSE_RECV);
        
        // 9. Disable notifications on multi handle
        curl_multi_notify_disable(multi_handle, fuzz_uint32_t_2);
        
        // Perform the multi handle (won't actually connect without network)
        int still_running = 0;
        curl_multi_perform(multi_handle, &still_running);
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, curl);
    }
    
    // Clean up
    if (curl) curl_easy_cleanup(curl);
    if (multi_handle) curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}