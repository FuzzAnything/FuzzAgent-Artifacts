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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 43
//<Quality> {"density":20,"unique_branches":{"Curl_resolver_error":[[1066,6,1066,22,0,0,4,1],[1075,9,1075,15,0,0,4,1],[1075,29,1075,35,0,0,4,1],[1075,51,1075,57,0,0,4,1]],"create_dnscache_id":[[83,16,83,20,0,0,4,0]],"Curl_resolv_take_result":[[990,11,990,49,0,0,4,0]],"Curl_expire_done":[[319,4,319,10,4,0,4,0],[319,14,319,33,4,0,4,1]],"dnscache_add_addr":[[550,6,550,10,0,0,4,1],[560,6,560,11,0,0,4,1]],"multi_deltimeout":[[3553,8,3553,21,0,0,4,0]],"Curl_async_get_ai":[[786,8,786,20,0,0,4,0],[790,8,790,23,0,0,4,0]],"dnscache_entry_is_stale":[[113,8,113,18,0,0,4,0]],"cf_ip_happy_connect":[[794,3,794,18,0,0,4,1],[802,3,802,21,0,0,4,0]],"cf_dns_connect":[[208,6,208,19,0,0,4,1],[216,21,216,40,0,0,4,1],[221,6,221,24,0,0,4,0],[319,4,319,10,17,0,4,0],[319,14,319,33,17,0,4,1]],"hostip_resolv_take_result":[[424,6,424,27,0,0,4,1],[426,11,426,17,0,0,4,0]],"dnscache_entry_create":[[442,6,442,12,0,0,4,1],[480,6,480,12,0,0,4,1]],"Curl_async_take_result":[[692,6,692,29,0,0,4,0],[692,33,692,45,0,0,4,1],[703,6,703,18,0,0,4,1],[707,6,707,19,0,0,4,1],[710,7,710,19,0,0,4,0],[710,23,710,40,0,0,4,1],[711,7,711,22,0,0,4,0],[711,26,711,46,0,0,4,1],[738,6,738,9,0,0,4,1],[750,6,750,13,0,0,4,0],[750,17,750,23,0,0,4,0],[752,6,752,12,0,0,4,0],[753,6,753,44,0,0,4,1]],"async_thrdd_get_ai":[[766,14,766,16,0,0,4,1]],"Curl_dnscache_add_negative":[[601,6,601,15,0,0,4,1],[609,6,609,9,0,0,4,0],[319,4,319,10,6,0,4,0],[319,14,319,33,6,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_ssls_import","curl_getdate","curl_multi_add_handle","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_easy_recv","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_multi_poll","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_ssls_import","curl_getdate","curl_multi_add_handle","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_easy_recv","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_multi_poll","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses all specified curl APIs to simulate an HTTP request with SSL session resumption and multi-handle management:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create input/output files using provided data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 2. Initialize easy and multi handles
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Set easy handle options using input data
    // Parse data for various options
    size_t data_offset = 0;
    
    // First 50 bytes for URL (ensure null-termination)
    size_t url_len = size < 50 ? size : 50;
    char *url = (char*)malloc(url_len + 1);
    if (url) {
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        free(url);
        data_offset = url_len;
    }
    
    // Set input/output file descriptors
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    
    // Enable uploading for recv/send operations
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // 4. SSL session import (use next portion of data)
    if (size - data_offset > 50) {
        const char *session_key = "fuzz_session";
        const unsigned char *shmac = data + data_offset;
        size_t shmac_len = 16;
        const unsigned char *sdata = data + data_offset + 16;
        size_t sdata_len = size - data_offset - 16;
        
        if (sdata_len > 0) {
            CURLcode ssl_res = curl_easy_ssls_import(easy_handle, session_key, 
                                                     shmac, shmac_len, sdata, sdata_len);
            // Continue regardless of SSL import result
        }
        data_offset += 16 + sdata_len;
    }
    
    // 5. Parse date string from remaining data
    if (size - data_offset > 20) {
        char date_str[21];
        size_t date_len = size - data_offset < 20 ? size - data_offset : 20;
        memcpy(date_str, data + data_offset, date_len);
        date_str[date_len] = '\0';
        
        time_t parsed_date = curl_getdate(date_str, NULL);
        // Use parsed_date if needed
        data_offset += date_len;
    }
    
    // 6. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    int running_handles = 0;
    int ret = 0;
    
    // 7. Perform initial multi operations
    curl_multi_perform(multi_handle, &running_handles);
    
    // 8. Set up polling structure
    struct curl_waitfd poll_fd;
    poll_fd.fd = in_fd;
    poll_fd.events = CURL_WAIT_POLLIN;
    poll_fd.revents = 0;
    
    // 9. Poll for activity
    curl_multi_poll(multi_handle, &poll_fd, 1, 1000, &ret);
    
    // 10. Process socket actions
    curl_socket_t sock = (curl_socket_t)in_fd;
    curl_multi_socket_action(multi_handle, sock, CURL_POLL_IN, &running_handles);
    
    // 11. Try to receive data from easy handle
    uint8_t recv_buffer[1024];
    size_t bytes_received = 0;
    if (running_handles > 0) {
        CURLcode recv_res = curl_easy_recv(easy_handle, recv_buffer, 
                                          sizeof(recv_buffer), &bytes_received);
        
        // 12. Pause the transfer
        if (recv_res == CURLE_OK) {
            curl_easy_pause(easy_handle, CURLPAUSE_RECV);
            
            // Try to send some data back
            size_t bytes_sent = 0;
            uint8_t send_data[] = "HTTP/1.1 200 OK\r\n\r\n";
            // Note: curl_easy_send requires different setup, skip for this fuzzer
            
            // Resume transfer
            curl_easy_pause(easy_handle, CURLPAUSE_CONT);
        }
    }
    
    // 13. Continue performing operations
    while (running_handles > 0) {
        curl_multi_perform(multi_handle, &running_handles);
        curl_multi_poll(multi_handle, NULL, 0, 100, &ret);
    }
    
    // 14. Remove handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // 15. Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}