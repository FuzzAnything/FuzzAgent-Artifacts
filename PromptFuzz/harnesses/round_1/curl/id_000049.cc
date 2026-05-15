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
//<ID> 759
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 16
//<Quality> {"density":4,"unique_branches":{"formatf":[[1030,5,1030,19,0,0,4,0],[1031,10,1031,68,0,0,4,1]],"out_pointer":[[895,6,895,9,0,0,4,0],[900,8,900,57,0,0,4,1]],"out_number":[[781,6,781,12,0,0,4,0],[781,16,781,25,0,0,4,1],[792,6,792,12,0,0,4,0],[792,16,792,26,0,0,4,0],[809,6,809,12,0,0,4,0],[809,16,809,26,0,0,4,0],[811,8,811,27,0,0,4,1],[44,8,44,41,12,0,4,1],[44,8,44,41,14,0,4,1]],"parsefmt":[[387,7,387,15,0,0,4,0],[554,5,554,19,0,0,4,0]],"curl_global_init_mem":[[220,6,220,8,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_multi_wakeup","curl_multi_socket_all","curl_mprintf","curl_msnprintf","curl_easy_option_by_name","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_data","curl_ws_meta","curl_mime_free","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_easy_init","curl_multi_init","curl_multi_wakeup","curl_multi_socket_all","curl_mprintf","curl_msnprintf","curl_easy_option_by_name","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_data","curl_ws_meta","curl_mime_free","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to use curl_mvaprintf with variable arguments
static char*  id_000049_my_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // 1. Initialize curl with custom memory functions
    curl_global_init_mem(CURL_GLOBAL_DEFAULT, 
                        nullptr,  // malloc callback
                        nullptr,  // free callback  
                        nullptr,  // realloc callback
                        nullptr,  // strdup callback
                        nullptr); // calloc callback
    
    // 2. Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // 4. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = out_file ? fuzz_fileno(out_file) : -1;
    
    // 5. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Create multi handle
    CURLM *multi = curl_multi_init();
    if (multi) {
        // 7. Wake up multi handle
        curl_multi_wakeup(multi);
        
        // 8. Socket action on all sockets
        int running_handles = 0;
        curl_multi_socket_all(multi, &running_handles);
    }
    
    // 9. Use curl_mprintf - print to allocated buffer
    int printed = curl_mprintf("Fuzzing input size: %zu\n", size);
    (void)printed; // Suppress unused warning
    
    // 10. Use curl_msnprintf - safe formatted printing
    char buffer[256];
    int written = curl_msnprintf(buffer, sizeof(buffer), 
                                "Input data: %p, Size: %zu", data, size);
    (void)written; // Suppress unused warning
    
    // 11. Use curl_mvaprintf - variable argument printing
    char *dynamic_buffer =  id_000049_my_mvaprintf("Processing %zu bytes", size);
    
    // 12. Look up easy option by name
    const struct curl_easyoption *ws_option = 
        curl_easy_option_by_name(fuzz_str_1);
    
    // 13. Create mime structure for multipart data
    curl_mime *mime = nullptr;
    curl_mimepart *part = nullptr;
    if (easy) {
        mime = curl_mime_init(easy);
        if (mime) {
            part = curl_mime_addpart(mime);
            if (part && size > 0) {
                // 14. Set mime filename using first 100 bytes or less
                size_t filename_len = size > 100 ? 100 : size;
                char filename[101];
                if (in_file) {
                    fseek(in_file, 0, SEEK_SET);
                    fread(filename, 1, filename_len, in_file);
                    filename[filename_len] = '\0';
                    curl_mime_filename(part, filename);
                }
                
                // Set mime data from input
                fseek(in_file, 0, SEEK_SET);
                char *mime_data = (char*)malloc(size);
                if (mime_data) {
                    fread(mime_data, 1, size, in_file);
                    curl_mime_data(part, mime_data, size);
                    free(mime_data);
                }
            }
        }
    }
    
    // 15. Try to get WebSocket metadata (will return NULL since no WS connection)
    const struct curl_ws_frame *ws_meta = nullptr;
    if (easy) {
        ws_meta = curl_ws_meta(easy);
        // This will be NULL since we don't have an active WebSocket connection
    }
    
    // 16. Cleanup resources
    
    // Free mime structure
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Free dynamic buffer from mvaprintf
    if (dynamic_buffer) {
        curl_free(dynamic_buffer);
    }
    
    // Cleanup easy handle
    if (easy) {
        curl_easy_cleanup(easy);
    }
    
    // Cleanup multi handle
    if (multi) {
        curl_multi_cleanup(multi);
    }
    
    // Close files
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}