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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"setopt_func":[[2573,8,2573,26,0,0,4,1]]},"library_calls":["curl_global_init","curl_version","curl_strnequal","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_send","curl_multi_init","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_strnequal","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_send","curl_multi_init","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to achieve an event involving:
1. Getting curl version information
2. Comparing strings
3. Creating a mime part with headers
4. Sending data through an easy handle
5. Disabling notifications in a multi handle

*/


extern "C" {
}

// Custom callback for reading data
static size_t  id_000015_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    FILE *in_file = (FILE *)userdata;
    return fread(buffer, size, nitems, in_file);
}

// Custom callback for seeking in data
static int  id_000015_seek_callback(void *userdata, curl_off_t offset, int origin) {
    FILE *in_file = (FILE *)userdata;
    return fseek(in_file, (long)offset, origin);
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Need minimum data
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Get curl version
    char *version_str = curl_version();
    
    // 2. Compare strings using curl_strnequal
    // Use first 5 bytes from input as comparison string
    const char *compare_str = (const char *)data;
    size_t compare_len = size > 5 ? 5 : size;
    int str_equal = curl_strnequal(version_str, compare_str, compare_len);
    
    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create mime part with headers
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create headers list
    struct curl_slist *headers = NULL;
    // Use part of input data as header (up to 100 bytes or size)
    size_t header_len = size > 100 ? 100 : size;
    char *header_data = (char *)malloc(header_len + 1);
    if (header_data) {
        memcpy(header_data, data, header_len);
        header_data[header_len] = '\0';
        headers = curl_slist_append(headers, header_data);
        free(header_data);
    }
    
    // Add another header with version info
    if (version_str) {
        headers = curl_slist_append(headers, version_str);
    }
    
    // Set mime headers (taking ownership)
    CURLcode res = curl_mime_headers(part, headers, 1);
    
    // Set mime data using callback
    res = curl_mime_data_cb(part, size,  id_000015_read_callback,  id_000015_seek_callback, NULL, in_file);
    
    // 4. Use curl_easy_send to send data
    // First, set up easy handle for sending
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_READFUNCTION,  id_000015_read_callback);
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    
    // Rewind file
    fseek(in_file, 0, SEEK_SET);
    
    // Create a buffer for sending
    size_t send_size = size > 1024 ? 1024 : size;
    char *send_buffer = (char *)malloc(send_size);
    if (send_buffer) {
        // Read data from file
        size_t read_bytes = fread(send_buffer, 1, send_size, in_file);
        
        // Send data
        size_t sent = 0;
        if (read_bytes > 0) {
            curl_easy_send(easy, send_buffer, read_bytes, &sent);
        }
        free(send_buffer);
    }
    
    // 5. Create multi handle and disable notifications
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Add easy handle to multi
        curl_multi_add_handle(multi, easy);
        
        // Disable all notifications
        // Use input data to determine which notification to disable
        unsigned int notification = (unsigned int)(data[0] % 3); // 0, 1, or 2
        curl_multi_notify_disable(multi, notification);
        
        // Perform multi handle operations
        int running_handles = 0;
        curl_multi_perform(multi, &running_handles);
        
        // Remove and cleanup
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
    }
    
    // Cleanup
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}