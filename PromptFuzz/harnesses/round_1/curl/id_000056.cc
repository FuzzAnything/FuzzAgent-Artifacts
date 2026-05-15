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
//<ID> 1118
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 1
//<Quality> {"density":34,"unique_branches":{"curl_mime_addpart":[[1228,8,1228,22,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_data","curl_msprintf","curl_slist_append","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_option_next","curl_multi_add_handle","curl_multi_perform","curl_multi_strerror","curl_multi_waitfds","curl_multi_socket_all","curl_ws_send","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_mime_data_cb","curl_mime_data","curl_msprintf","curl_slist_append","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_option_next","curl_multi_add_handle","curl_multi_perform","curl_multi_strerror","curl_multi_waitfds","curl_multi_socket_all","curl_ws_send","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom memory functions that track allocations
static size_t  id_000056_total_allocated = 0;
static size_t  id_000056_allocation_count = 0;

static void*  id_000056_custom_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
         id_000056_total_allocated += size;
         id_000056_allocation_count++;
    }
    return ptr;
}

static void  id_000056_custom_free(void* ptr) {
    free(ptr);
}

static void*  id_000056_custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

static char*  id_000056_custom_strdup(const char* str) {
    return strdup(str);
}

static void*  id_000056_custom_calloc(size_t nmemb, size_t size) {
    void* ptr = calloc(nmemb, size);
    if (ptr) {
         id_000056_total_allocated += nmemb * size;
         id_000056_allocation_count++;
    }
    return ptr;
}

// Callbacks for curl_mime_data_cb
static size_t  id_000056_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    FILE* file = (FILE*)arg;
    if (!file) return CURL_READFUNC_ABORT;
    return fread(buffer, size, nitems, file);
}

static int  id_000056_seek_callback(void* arg, curl_off_t offset, int origin) {
    FILE* file = (FILE*)arg;
    if (!file) return CURL_SEEKFUNC_FAIL;
    return fseek(file, (long)offset, origin) == 0 ? CURL_SEEKFUNC_OK : CURL_SEEKFUNC_FAIL;
}

static void  id_000056_free_callback(void* arg) {
    // File will be closed elsewhere
}

// Helper function to use curl_mvsnprintf
static int  id_000056_format_string(char* buffer, size_t maxlen, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = curl_mvsnprintf(buffer, maxlen, fmt, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data
    
    // 1. Initialize curl with custom memory functions
    CURLcode global_init_result = curl_global_init_mem(CURL_GLOBAL_ALL,
         id_000056_custom_malloc,  id_000056_custom_free,  id_000056_custom_realloc,  id_000056_custom_strdup,  id_000056_custom_calloc);
    if (global_init_result != CURLE_OK) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);  // File descriptor for possible use
    
    // 2. Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Create easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Create mime structure
    curl_mime* mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Add mime parts
    curl_mimepart* part1 = curl_mime_addpart(mime);
    curl_mimepart* part2 = curl_mime_addpart(mime);
    
    if (!part1 || !part2) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Set up mime data with callback (using fuzzer input)
    CURLcode mime_cb_result = curl_mime_data_cb(part1, size, 
         id_000056_read_callback,  id_000056_seek_callback,  id_000056_free_callback, in_file);
    
    // 7. Set static mime data (using first 100 bytes or less)
    size_t data_size = size > 100 ? 100 : size;
    CURLcode mime_data_result = curl_mime_data(part2, (const char*)data, data_size);
    
    // 8. Create headers list
    struct curl_slist* headers = nullptr;
    char header_buf[256];
    
    // Use curl_msprintf
    curl_msprintf(header_buf, "X-Fuzz-Data: %zu bytes", size);
    headers = curl_slist_append(headers, header_buf);
    
    // Use  id_000056_format_string (which uses curl_mvsnprintf)
     id_000056_format_string(header_buf, sizeof(header_buf), "X-Allocations: %zu",  id_000056_allocation_count);
    headers = curl_slist_append(headers, header_buf);
    
    // Set headers on mime part
    curl_mime_headers(part1, headers, 1);  // Takes ownership
    
    // 9. Set easy options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");  // Safe URL
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    
    // 10. Iterate through easy options (just a few)
    const struct curl_easyoption* option = nullptr;
    int option_count = 0;
    while ((option = curl_easy_option_next(option)) != nullptr && option_count < 5) {
        option_count++;
    }
    
    // 11. Add easy handle to multi handle
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    
    // 12. Set up wait structures
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    // 13. Perform multi operations with timeout
    int running_handles = 0;
    int max_iterations = 10;  // Limit iterations to avoid hanging
    
    for (int i = 0; i < max_iterations && running_handles > 0; i++) {
        CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
        
        if (perform_result != CURLM_OK) {
            // Get error string
            const char* error_str = curl_multi_strerror(perform_result);
            break;
        }
        
        // Wait for activity
        CURLMcode wait_result = curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
        
        if (wait_result != CURLM_OK || fd_count == 0) {
            // Small delay
            struct timeval timeout = {0, 10000};  // 10ms
            select(0, nullptr, nullptr, nullptr, &timeout);
        }
    }
    
    // 14. Try socket_all method
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // 15. Attempt WebSocket send (will likely fail but we call the API)
    size_t sent = 0;
    char ws_buf[10] = "test";
    curl_ws_send(easy_handle, fuzz_str_1, static_cast<size_t>(fuzz_str_sz_1), &sent, fuzz_int64_t_2, fuzz_uint32_t_3);
    
    // 16. Clean up multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // 17. Clean up mime
    curl_mime_free(mime);
    
    // 18. Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // 19. Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 20. Clean up global state
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}