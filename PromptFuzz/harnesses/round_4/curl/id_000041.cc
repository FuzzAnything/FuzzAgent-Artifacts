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
//<ID> 650
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 25
//<Quality> {"density":36,"unique_branches":{"tftp_setup_connection":[[1324,26,1324,69,0,0,4,0]],"file_do":[[506,18,506,38,0,0,4,1],[515,6,515,16,0,0,4,0],[539,10,539,20,0,0,4,0],[540,23,540,68,0,0,4,0],[548,10,548,19,0,0,4,0],[551,10,551,20,0,0,4,1],[551,25,551,35,0,0,4,0],[551,39,551,59,0,0,4,1],[554,10,554,20,0,0,4,0],[558,10,558,16,0,0,4,1],[562,10,562,16,0,0,4,1]],"multi_runsingle":[[2515,8,2515,58,0,0,4,0]],"multi_handle_timeout":[[1806,6,1806,20,0,0,4,0],[1809,8,1809,32,0,0,4,1],[1813,8,1813,41,0,0,4,1],[1819,10,1819,23,0,0,4,1],[1834,8,1834,18,0,0,4,0],[1836,10,1836,34,0,0,4,0]],"tftp_send_first":[[673,8,673,26,0,0,4,1],[712,22,712,40,0,0,4,1]],"tftp_set_timeouts":[[179,6,179,26,0,0,4,0],[182,6,182,27,0,0,4,1],[187,6,187,27,0,0,4,0]],"create_dnscache_id":[[85,6,85,24,0,0,4,0]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_data","curl_mime_name","curl_mime_type","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_pause","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_wait","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_pause","curl_multi_init","curl_multi_notify_enable","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_wait","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Helper function to create va_list for curl_mvfprintf
static void  id_000041_my_curl_mvfprintf(FILE* fd, const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Too small for meaningful fuzzing
    }

    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // 2. Create URL handle
    CURLU* url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Split input data: first part for URL, second for mime data
    size_t url_len = size / 2;
    size_t mime_data_len = size - url_len;
    const uint8_t* url_data = data;
    const uint8_t* mime_data = data + url_len;

    // 3. Set URL from input data (null-terminate the URL string)
    char* url_str = (char*)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, url_data, url_len);
    url_str[url_len] = '\0';

    CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    free(url_str);

    // 4. Create easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 5. Create mime structure
    curl_mime* mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 6. Add mime part
    curl_mimepart* mime_part = curl_mime_addpart(mime);
    if (!mime_part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 7. Set mime data from input
    CURLcode mime_data_res = curl_mime_data(mime_part, (const char*)mime_data, mime_data_len);

    // 8. Set mime name
    CURLcode mime_name_res = curl_mime_name(mime_part, fuzz_str_2);

    // 9. Create output file for curl_mvfprintf
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // 10. Use curl_mvfprintf to write to output file
         id_000041_my_curl_mvfprintf(out_file, "Fuzzing with size: %zu\nURL set result: %d\n", 
                         size, url_set_res);
        assert_file_closed(&out_file);;
    }

    // 11. Set mime type
    CURLcode mime_type_res = curl_mime_type(mime_part, fuzz_str_3);

    // 12. Get URL back from handle
    char* retrieved_url = nullptr;
    CURLUcode url_get_res = curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_4);

    // 13. Set easy handle options
    if (retrieved_url) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, retrieved_url);
        curl_free(retrieved_url);
    }
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L); // Short timeout
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 100L);

    // 14. Pause the easy handle
    curl_easy_pause(easy_handle, CURLPAUSE_ALL);

    // 15. Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // 16. Enable notifications
    curl_multi_notify_enable(multi_handle, fuzz_uint32_t_5);

    // 17. Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy_handle);

    // 18. Perform multi handle operations
    int running_handles = 0;
    CURLMcode multi_perform_res = curl_multi_perform(multi_handle, &running_handles);

    // 19. Use multi socket with timeout
    curl_socket_t timeout_sock = CURL_SOCKET_TIMEOUT;
    int running_handles2 = 0;
    CURLMcode multi_socket_res = curl_multi_socket(multi_handle, timeout_sock, &running_handles2);

    // 20. Wait for activity (short timeout)
    struct curl_waitfd extra_fds;
    extra_fds.fd = 0;
    extra_fds.events = CURL_WAIT_POLLIN;
    extra_fds.revents = 0;
    
    int numfds = 0;
    CURLMcode multi_wait_res = curl_multi_wait(multi_handle, &extra_fds, 1, fuzz_int32_t_6, &numfds);

    // 21. Remove easy handle from multi handle
    CURLMcode multi_remove_res = curl_multi_remove_handle(multi_handle, easy_handle);

    // Cleanup in reverse order
    // 22. Free mime
    curl_mime_free(mime);
    
    // 23. Cleanup easy handle
    curl_easy_cleanup(easy_handle);
    
    // 24. Cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // 25. Cleanup URL handle
    curl_url_cleanup(url_handle);
    
    // 26. Global cleanup
    curl_global_cleanup();

    assert_file_closed(&out_file);
	return 0;
}