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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_global_cleanup","curl_maprintf","curl_free","curl_easy_init","curl_multi_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_pushheader_byname","curl_multi_add_handle","curl_multi_get_handles","curl_free","curl_easy_perform","curl_slist_free_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_maprintf","curl_free","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_pushheader_byname","curl_multi_add_handle","curl_multi_get_handles","curl_free","curl_easy_perform","curl_slist_free_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate an HTTP/2 server push scenario:

*/


// Custom memory callbacks
static void * id_000002_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000002_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000002_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000002_custom_strdup(const char *str) {
    return strdup(str);
}

static void * id_000002_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // 1. Initialize curl with custom memory callbacks
    CURLcode global_init_result = curl_global_init_mem(
        CURL_GLOBAL_ALL,
         id_000002_custom_malloc,
         id_000002_custom_free,
         id_000002_custom_realloc,
         id_000002_custom_strdup,
         id_000002_custom_calloc
    );
    
    if (global_init_result != CURLE_OK) {
        return 0;
    }

    // Create a file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Use curl_maprintf to format a string with input data
    // Extract first 100 bytes for format string (or less if size is smaller)
    size_t format_len = size < 100 ? size : 100;
    char *format_buf = (char *)malloc(format_len + 1);
    if (!format_buf) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(format_buf, 1, format_len, in_file);
    format_buf[format_len] = '\0';
    
    // Create a format string using input data
    char *formatted_str = curl_maprintf("Received: %s", format_buf);
    free(format_buf);
    
    if (formatted_str) {
        curl_free(formatted_str);
    }

    // 3. Simulate push headers scenario
    CURL *easy = curl_easy_init();
    CURLM *multi = curl_multi_init();
    
    if (!easy || !multi) {
        if (easy) curl_easy_cleanup(easy);
        if (multi) curl_multi_cleanup(multi);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up easy handle with input data
    rewind(in_file);
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    
    // Create a mock push header structure
    struct curl_slist *headers = NULL;
    char header_buf[256];
    size_t header_len = size < 200 ? size : 200;
    fread(header_buf, 1, header_len, in_file);
    header_buf[header_len] = '\0';
    
    // Add header from input data
    char header_line[300];
    snprintf(header_line, sizeof(header_line), "X-Custom-Header: %s", header_buf);
    headers = curl_slist_append(headers, header_line);
    
    // Simulate push header lookup
    // Note: In real usage, curl_pushheader_byname would be called from a push callback
    // We'll simulate the structure and call
    struct curl_pushheaders *push_headers = NULL;
    // This is a simulation - actual push headers would come from server push
    char *push_header_value = curl_pushheader_byname(push_headers, fuzz_str_1);
    // push_header_value would be NULL in our simulation
    
    // 4. Get handles from multi handle
    curl_multi_add_handle(multi, easy);
    CURL **handles = curl_multi_get_handles(multi);
    
    if (handles) {
        // Count handles (just for demonstration)
        int handle_count = 0;
        while (handles[handle_count] != NULL) {
            handle_count++;
        }
        curl_free(handles);
    }

    // 5. Perform the easy transfer
    CURLcode perform_result = curl_easy_perform(easy);
    // We ignore the result as this is a fuzzing test

    // Cleanup
    curl_slist_free_all(headers);
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}