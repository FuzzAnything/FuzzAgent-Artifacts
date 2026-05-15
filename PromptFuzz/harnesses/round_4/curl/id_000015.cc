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
#include <mprintf.h>
}
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":27,"unique_branches":{"dollarstring":[[141,6,141,48,0,0,4,1],[142,6,142,31,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_multi_assign","curl_easy_setopt","curl_ws_meta","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_multi_init","curl_multi_add_handle","curl_multi_assign","curl_easy_setopt","curl_ws_meta","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":2}
/*Here's a C++ fuzz driver that uses all the specified curl APIs to create a WebSocket server scenario that processes input data:

*/


extern "C" {
}

// Custom memory callbacks
static void * id_000015_custom_malloc(size_t size) { return malloc(size); }
static void  id_000015_custom_free(void *ptr) { free(ptr); }
static void * id_000015_custom_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
static char * id_000015_custom_strdup(const char *str) { return strdup(str); }
static void * id_000015_custom_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

// Custom va_list wrapper for curl_mvsnprintf
static int  id_000015_call_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize curl with custom memory callbacks
    curl_global_init_mem(CURL_GLOBAL_ALL,  id_000015_custom_malloc,  id_000015_custom_free, 
                         id_000015_custom_realloc,  id_000015_custom_strdup,  id_000015_custom_calloc);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Create a mime structure and subparts for multipart data
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Create main mime structure
    curl_mime *mime = curl_mime_init(easy);
    curl_mimepart *main_part = curl_mime_addpart(mime);
    
    // Create subparts
    curl_mime *sub_mime = curl_mime_init(easy);
    curl_mimepart *sub_part = curl_mime_addpart(sub_mime);
    
    // Add fuzzed data to subpart
    curl_mime_data(sub_part, reinterpret_cast<const char*>(data), size);
    
    // Attach subparts to main part (API 1)
    curl_mime_subparts(main_part, sub_mime);
    
    // Create multi handle for WebSocket simulation
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Add easy handle to multi
    curl_multi_add_handle(multi, easy);
    
    // Open input data as file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    // Get file descriptor for socket assignment
    int sock_fd = fuzz_fileno(in_file);
    
    // Assign socket to multi handle (API 5)
    curl_multi_assign(multi, static_cast<curl_socket_t>(sock_fd), in_file);
    
    // Set up WebSocket options
    curl_easy_setopt(easy, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    
    // Simulate WebSocket metadata retrieval (API 4)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(easy);
    
    // Format and write WebSocket info using curl_mfprintf (API 6)
    if (ws_meta) {
        curl_mfprintf(out_file, "WS Frame: age=%d, flags=%d, offset=%lld, bytesleft=%lld, len=%zu\n",
                     ws_meta->age, ws_meta->flags, 
                     static_cast<long long>(ws_meta->offset),
                     static_cast<long long>(ws_meta->bytesleft),
                     ws_meta->len);
    }
    
    // Format buffer using curl_mvsnprintf (API 3)
    char buffer[256];
     id_000015_call_curl_mvsnprintf(buffer, sizeof(buffer), 
                        "Input processed: size=%zu, first_byte=0x%02x, last_byte=0x%02x",
                        size, data[0], data[size-1]);
    
    // Write formatted string to output
    curl_mfprintf(out_file, "%s\n", buffer);
    
    // Write some input data to output file
    size_t write_size = size > 100 ? 100 : size;
    curl_mfprintf(out_file, "Data preview (first %zu bytes): ", write_size);
    for (size_t i = 0; i < write_size; i++) {
        curl_mfprintf(out_file, "%02x ", data[i]);
    }
    curl_mfprintf(out_file, "\n");
    
    // Clean up in reverse order
    assert_file_closed(&in_file);;
    
    // Remove handles from multi
    curl_multi_remove_handle(multi, easy);
    
    // Perform multi cleanup
    curl_multi_cleanup(multi);
    
    // Clean up mime structures
    curl_mime_free(mime);
    
    // Clean up easy handle
    curl_easy_cleanup(easy);
    
    // Close output file
    assert_file_closed(&out_file);;
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(sock_fd);
	return 0;
}