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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 28.333334, nr_unique_branch: 9
//<Quality> {"density":17,"unique_branches":{"create_dnscache_id":[[85,6,85,24,0,0,4,0]],"set_url_scheme":[[1642,6,1642,29,0,0,4,0]],"urlget_url":[[1454,8,1454,17,0,0,4,1],[1456,13,1456,41,0,0,4,1]],"curl_url_set":[[1857,3,1857,22,0,0,4,0],[1884,8,1884,20,0,0,4,0],[1884,24,1884,40,0,0,4,0],[1884,24,1884,40,0,0,4,1],[1886,10,1886,16,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_url","curl_multi_cleanup","curl_global_cleanup","curl_url_set","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_strequal","curl_unescape","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_info_read","curl_multi_remove_handle","curl_free","curl_free","curl_free","curl_easy_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_url_get","curl_strequal","curl_unescape","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_info_read","curl_multi_remove_handle","curl_free","curl_free","curl_free","curl_easy_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" {
}

// Helper function to consume input data
struct DataConsumer {
    const uint8_t* data;
    size_t size;
    size_t offset;
    
    DataConsumer(const uint8_t* d, size_t s) : data(d), size(s), offset(0) {}
    
    bool hasMore(size_t needed = 1) const {
        return offset + needed <= size;
    }
    
    const char* getString(size_t len = 0) {
        if (len == 0) {
            // Find null terminator or end of data
            size_t str_len = 0;
            while (offset + str_len < size && data[offset + str_len] != 0) {
                str_len++;
            }
            len = str_len + 1;
        }
        
        if (!hasMore(len)) {
            return nullptr;
        }
        
        const char* result = reinterpret_cast<const char*>(data + offset);
        offset += len;
        return result;
    }
    
    uint8_t getByte() {
        if (!hasMore()) {
            return 0;
        }
        return data[offset++];
    }
};

// Custom variadic function to use curl_mvfprintf
static int  id_000022_custom_vfprintf(FILE* fd, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

// Custom variadic function to use curl_mvaprintf
static char*  id_000022_custom_vaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<23) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size < 2) {
        return 0;
    }
    
    DataConsumer consumer(data, size);
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a multi handle for asynchronous operations
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create a URL handle
    CURLU* url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Consume some data for URL parts
    const char* scheme = consumer.getString();
    const char* host = consumer.getString();
    const char* path = consumer.getString();
    
    // Build a URL using consumed data
    if (scheme && host && path) {
        curl_url_set(url_handle, CURLUPART_SCHEME, scheme, fuzz_uint32_t_1);
        curl_url_set(url_handle, CURLUPART_HOST, host, fuzz_uint32_t_2);
        curl_url_set(url_handle, CURLUPART_PATH, path, fuzz_uint32_t_3);
    }
    
    // Get URL components using curl_url_get
    char* url_part = nullptr;
    CURLUcode url_result = curl_url_get(url_handle, CURLUPART_URL, &url_part, fuzz_uint32_t_4);
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        if (url_part) curl_free(url_part);
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    
    // Use curl_strequal to compare strings
    const char* test_str1 = "test";
    const char* test_str2 = consumer.getString();
    int equal_result = 0;
    if (test_str2) {
        equal_result = curl_strequal(fuzz_str_5, test_str2);
    }
    
    // Unescape a string if we have data for it
    const char* escaped_str = consumer.getString();
    char* unescaped_str = nullptr;
    if (escaped_str) {
        unescaped_str = curl_unescape(escaped_str, strlen(escaped_str));
    }
    
    // Create a simple easy handle
    CURL* easy = curl_easy_init();
    if (easy && url_part) {
        curl_easy_setopt(easy, CURLOPT_URL, url_part);
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy);
        
        // Use curl_multi_socket_all
        int running_handles = 0;
        CURLMcode multi_result = curl_multi_socket_all(multi_handle, &running_handles);
        
        // Check for messages using curl_multi_info_read
        int msgs_in_queue = 0;
        CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        
        // Remove easy handle
        curl_multi_remove_handle(multi_handle, easy);
    }
    
    // Use curl_mvfprintf with variadic arguments
    if (out_file) {
        if (url_part) {
             id_000022_custom_vfprintf(out_file, "URL: %s\n", url_part);
        }
        if (unescaped_str) {
             id_000022_custom_vfprintf(out_file, "Unescaped: %s\n", unescaped_str);
        }
         id_000022_custom_vfprintf(out_file, "String comparison result: %d\n", equal_result);
    }
    
    // Use curl_mvaprintf with variadic arguments
    char* formatted_str = nullptr;
    if (url_part && unescaped_str) {
        formatted_str =  id_000022_custom_vaprintf("URL: %s, Unescaped: %s", url_part, unescaped_str);
    }
    
    // Clean up
    if (formatted_str) {
        curl_free(formatted_str);
    }
    
    if (unescaped_str) {
        curl_free(unescaped_str);
    }
    
    if (url_part) {
        curl_free(url_part);
    }
    
    if (easy) {
        curl_easy_cleanup(easy);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    curl_url_cleanup(url_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}