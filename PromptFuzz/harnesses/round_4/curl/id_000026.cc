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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 8
//<Quality> {"density":18,"unique_branches":{"Curl_uint32_tbl_next":[[195,6,195,57,0,0,4,0]],"curl_easy_option_next":[[70,8,70,18,0,0,4,1]],"curl_multi_get_handles":[[3804,6,3804,7,0,0,4,0],[3808,8,3808,58,0,0,4,0],[3812,12,3812,33,0,0,4,0],[3812,12,3812,33,0,0,4,1],[3814,15,3814,69,0,0,4,0],[3814,15,3814,69,0,0,4,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_multi_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_multi_add_handle","curl_easy_option_next","curl_easy_option_next","curl_multi_get_handles","curl_free","curl_slist_append","curl_slist_free_all","curl_multi_socket_action","curl_easy_nextheader","curl_unescape","curl_easy_send","curl_ws_meta","curl_multi_wakeup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup"],"critical_calls":["curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_easy_option_next","curl_easy_option_next","curl_multi_get_handles","curl_free","curl_slist_append","curl_slist_free_all","curl_multi_socket_action","curl_easy_nextheader","curl_unescape","curl_easy_send","curl_ws_meta","curl_multi_wakeup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for socket operations
    int fd = fuzz_fileno(in_file);

    // 1. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // 2. Use curl_easy_option_next to iterate through options
    const struct curl_easyoption *option = curl_easy_option_next(nullptr);
    while (option) {
        // Process options if needed
        option = curl_easy_option_next(option);
    }

    // 3. Get handles from multi handle
    CURL **handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        // Process handles if needed
        curl_free(handles);
    }

    // 4. Create and free curl_slist
    struct curl_slist *header_list = nullptr;
    header_list = curl_slist_append(header_list, fuzz_str_1);
    curl_slist_free_all(header_list);

    // 5. Use curl_multi_socket with file descriptor
    int running_handles = 0;
    curl_multi_socket(multi_handle, fd, &running_handles);

    // 6. Get next header (will likely return null since no request made)
    struct curl_header *header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_2, fuzz_int32_t_3, nullptr);

    // 7. Unescape data (use first part of input as escaped string)
    size_t str_len = size < 100 ? size : 100;
    char *escaped_str = (char *)malloc(str_len + 1);
    if (escaped_str) {
        memcpy(escaped_str, data, str_len);
        escaped_str[str_len] = 0;
        
        // Add some percent encoding to make it escapable
        if (str_len > 10) {
            escaped_str[5] = '%';
            escaped_str[6] = '2';
            escaped_str[7] = '0';
        }
        
        char *unescaped = curl_unescape(escaped_str, -1);
        if (unescaped) {
            // Use unescaped string for something
            free(unescaped);
        }
        free(escaped_str);
    }

    // 8. Send data using curl_easy_send
    size_t sent = 0;
    if (size > 0) {
        curl_easy_send(easy_handle, data, size > 1024 ? 1024 : size, &sent);
    }

    // 9. Get WebSocket metadata (will return null since not a WS connection)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(easy_handle);

    // 10. Wake up multi handle
    curl_multi_wakeup(multi_handle);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}