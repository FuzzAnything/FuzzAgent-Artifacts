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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"curl_url_strerror":[[427,3,427,25,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_easy_option_next","curl_easy_option_next","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_url_strerror","curl_ws_send","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_easy_option_next","curl_easy_option_next","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_url_strerror","curl_ws_send","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers
extern "C" {
}

// Helper function to use curl_mvfprintf with our data
static int  id_000029_my_curl_mvfprintf(FILE* fd, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

// External C interface for fuzzing
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Variables that need cleanup
    CURL* curl = nullptr;
    CURLSH* share = nullptr;
    curl_mime* mime = nullptr;
    curl_mimepart* part = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Create a shared object
    share = curl_share_init();
    
    // 2. Iterate over easy options (use the API)
    const struct curl_easyoption* opt = curl_easy_option_next(nullptr);
    if (opt) {
        // Just iterate through a few options to use the API
        for (int i = 0; i < 3 && opt; i++) {
            opt = curl_easy_option_next(opt);
        }
    }
    
    // 3. Create a temporary file with input data for curl_mime_filedata
    const char* temp_filename = "input_file";
    FILE* temp_file = fopen(temp_filename, "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
    }
    
    // 4. Create a curl easy handle
    curl = curl_easy_init();
    if (curl) {
        // Create mime structure
        mime = curl_mime_init(curl);
        if (mime) {
            part = curl_mime_addpart(mime);
            if (part) {
                // Use curl_mime_filedata API with our temporary file
                CURLcode res = curl_mime_filedata(part, temp_filename);
                (void)res; // Result intentionally ignored for fuzzing
            }
        }
        
        // 5. Create input file handle from data for other operations
        in_file = fmemopen((void*)data, size, "rb");
        
        // 6. Create output file for writing
        out_file = fopen("output_file", "wb");
        
        if (in_file && out_file) {
            // 7. Use curl_mvfprintf to write something to output file
            // First get an error code to use with curl_url_strerror
            CURLUcode url_error = CURLUE_BAD_HANDLE; // Example error code
            const char* error_str = curl_url_strerror(url_error);
            
            // Use curl_mvfprintf to write error string to output file
             id_000029_my_curl_mvfprintf(out_file, "URL Error: %s\n", error_str);
            
            // 8. Try to use curl_ws_send (even though we don't have a real WS connection)
            // This will likely fail, but we're exercising the API
            if (size > 0) {
                size_t sent = 0;
                // Use a small fragment size for fuzzing
                curl_off_t fragsize = size > 100 ? 100 : static_cast<curl_off_t>(size);
                // Try to send (will fail without proper setup, but that's OK for fuzzing)
                CURLcode ws_res = curl_ws_send(curl, data, 
                    size > 100 ? 100 : size, 
                    &sent, 
                    fragsize, 
                    0);
                (void)ws_res; // Result intentionally ignored for fuzzing
            }
            
            // 9. Also try to perform some operations with the file descriptors
            int in_fd = fuzz_fileno(in_file);
            int out_fd = fuzz_fileno(out_file);
            (void)in_fd; // Mark as used
            (void)out_fd; // Mark as used
            
            // Try to read a bit from the input file
            if (size > 0) {
                uint8_t buffer[256];
                size_t bytes_to_read = size > sizeof(buffer) ? sizeof(buffer) : size;
                fread(buffer, 1, bytes_to_read, in_file);
            }
        }
    }
    
    // Cleanup section
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (curl) {
        curl_easy_cleanup(curl);
    }
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Remove temporary file
    remove(temp_filename);
    
    // Cleanup curl globally
    curl_global_cleanup();
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
}