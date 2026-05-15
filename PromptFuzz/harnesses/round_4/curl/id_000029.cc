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
//<ID> 427
//<Prompt> []
/*<Combination>: [
*/
//<score> 306.25, nr_unique_branch: 151
//<Quality> {"density":25,"unique_branches":{"curl_url_get":[[1579,3,1579,24,0,0,4,0]],"Curl_pgrs_upload_inc":[[375,6,375,11,0,0,4,0]],"Curl_creader_set_fread":[[1131,6,1131,12,0,0,4,1],[1131,16,1131,18,0,0,4,1],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,1]],"Curl_creader_read":[[521,6,521,13,0,0,4,1]],"Curl_is_ipaddr":[[438,8,438,52,0,0,4,0]],"tftp_doing":[[1230,6,1230,19,0,0,4,1],[1233,11,1233,18,0,0,4,0]],"file_connect":[[241,6,241,25,0,0,4,1]],"Curl_socket_check":[[129,6,129,34,0,0,4,1],[141,6,141,32,0,0,4,0],[153,6,153,32,0,0,4,1],[161,6,161,12,0,0,4,0]],"multi_done":[[714,3,714,25,0,0,4,0],[721,3,721,10,0,0,4,1]],"tftp_perform":[[1261,42,1261,48,0,0,4,1],[1266,6,1266,19,0,0,4,1]],"Curl_cf_def_data_pending":[[72,10,72,18,0,0,4,0],[72,10,72,18,0,0,4,1]],"state_do":[[2185,10,2185,23,0,0,4,0],[2188,12,2188,37,0,0,4,1]],"cr_in_read":[[652,6,652,18,0,0,4,1],[657,6,657,19,0,0,4,1],[663,6,663,25,0,0,4,1],[667,6,667,18,0,0,4,0],[667,22,667,26,0,0,4,0],[675,3,675,9,0,0,4,0],[675,3,675,9,0,0,4,1],[676,8,676,29,0,0,4,1],[688,3,688,27,0,0,4,1],[697,3,697,27,0,0,4,1],[714,3,714,10,0,0,4,0],[714,3,714,10,0,0,4,1],[715,8,715,20,0,0,4,1],[726,8,726,27,0,0,4,1],[319,4,319,10,29,0,4,0],[319,14,319,33,29,0,4,1]],"cf_dns_adjust_pollset":[[273,6,273,20,0,0,4,1]],"tftp_multi_statemach":[[1172,6,1172,12,0,0,4,1],[1176,6,1176,20,0,0,4,1],[1180,6,1180,30,0,0,4,1],[1193,8,1193,16,0,0,4,1],[1200,13,1200,15,0,0,4,1]],"file_do":[[407,6,407,24,0,0,4,0]],"cf_ip_happy_data_pending":[[865,6,865,20,0,0,4,1]],"Curl_client_read":[[1191,6,1191,29,0,0,4,0],[1191,6,1191,29,0,0,4,1],[1193,8,1193,14,0,0,4,1],[1197,6,1197,31,0,0,4,0],[1197,6,1197,31,0,0,4,1],[1202,6,1202,51,0,0,4,1],[319,4,319,10,10,0,4,0],[319,14,319,33,10,0,4,1]],"curlx_sztouz":[[299,6,299,15,0,0,4,1]],"cf_ip_happy_adjust_pollset":[[757,6,757,20,0,0,4,1]],"curl_multi_setopt":[[3255,3,3255,27,0,0,4,0],[3256,27,3256,67,0,0,4,0]],"protocol_doing":[[1879,6,1879,10,0,0,4,0],[1879,14,1879,38,0,0,4,0]],"Curl_creader_create":[[933,6,933,8,0,0,4,1],[943,14,943,20,0,0,4,1],[944,6,944,12,0,0,4,1]],"zonefrom_url":[[1498,6,1498,9,0,0,4,1]],"cf_socket_adjust_pollset":[[1350,13,1350,27,0,0,4,1],[1355,13,1355,25,0,0,4,1]],"mstate_do_pollset":[[1076,6,1076,38,0,0,4,0],[1082,6,1082,13,0,0,4,0],[1084,6,1084,13,0,0,4,0]],"Curl_ipv6_scope":[[76,5,76,16,0,0,4,1],[79,10,79,11,0,0,4,0],[79,15,79,28,0,0,4,0],[82,5,82,12,0,0,4,0]],"file_upload":[[284,6,284,10,0,0,4,1],[287,6,287,13,0,0,4,0],[287,6,287,13,0,0,4,1],[291,6,291,29,0,0,4,1],[305,6,305,12,0,0,4,0],[305,6,305,12,0,0,4,1],[310,6,310,34,0,0,4,1],[315,6,315,33,0,0,4,1],[325,6,325,12,0,0,4,1],[328,9,328,16,0,0,4,0],[328,20,328,24,0,0,4,0],[334,8,334,14,0,0,4,1],[337,8,337,18,0,0,4,0],[337,8,337,18,0,0,4,1],[343,8,343,31,0,0,4,1],[360,8,360,36,0,0,4,1],[360,40,360,59,0,0,4,1],[368,6,368,13,0,0,4,0]],"tftp_set_timeouts":[[179,6,179,26,0,0,4,1],[182,6,182,27,0,0,4,0],[187,6,187,27,0,0,4,1]],"Curl_conn_cf_adjust_pollset":[[806,15,806,29,0,0,4,1]],"Curl_multi_xfer_ulbuf_borrow":[[3921,6,3921,18,0,0,4,1],[3925,6,3925,35,0,0,4,1],[3929,6,3929,38,0,0,4,1],[3934,6,3934,29,0,0,4,1],[3942,6,3942,30,0,0,4,0],[3945,8,3945,32,0,0,4,1]],"Curl_str2addr":[[411,6,411,47,0,0,4,1],[417,8,417,51,0,0,4,0]],"cf_ip_ballers_run":[[414,8,414,50,0,0,4,0]],"Curl_pollset_want_recv":[[709,14,709,23,0,0,4,0],[709,14,709,23,0,0,4,1],[710,8,710,32,0,0,4,0],[710,8,710,32,0,0,4,1],[710,36,710,67,0,0,4,0]],"curl_multi_remove_handle":[[821,6,821,30,0,0,4,0],[822,6,822,37,0,0,4,0]],"ip2addr":[[357,6,357,19,0,0,4,1],[360,11,360,25,0,0,4,0],[381,3,381,15,0,0,4,1],[390,3,390,16,0,0,4,0]],"parseurlandfillconn":[[1601,18,1601,36,0,0,4,0]],"cl_reset_reader":[[62,9,62,15,0,0,4,0]],"tftp_do":[[1304,6,1304,13,0,0,4,0]],"tftp_state_timeout":[[1137,6,1137,11,0,0,4,0],[1141,6,1141,20,0,0,4,1],[1147,6,1147,50,0,0,4,1]],"Curl_rlimit_active":[[190,10,190,32,0,0,4,1],[190,36,190,46,0,0,4,1]],"multi_adjust_pollset":[[985,6,985,11,0,0,4,0],[991,8,991,20,0,0,4,1],[991,24,991,36,0,0,4,1],[1012,8,1012,21,0,0,4,0],[1013,10,1013,73,0,0,4,0],[1014,10,1014,51,0,0,4,1],[1015,10,1015,77,0,0,4,1]],"multi_runsingle":[[2630,5,2630,22,0,0,4,0],[2634,10,2634,17,0,0,4,0],[2635,12,2635,24,0,0,4,1]],"Curl_conn_data_pending":[[774,9,774,11,0,0,4,0],[774,15,774,29,0,0,4,1],[777,6,777,8,0,0,4,0],[406,36,406,46,3,0,4,0],[406,50,406,59,3,0,4,0]],"Curl_multi_pollset":[[1161,5,1161,27,0,0,4,1],[1174,5,1174,22,0,0,4,0]],"tftp_send_first":[[673,8,673,26,0,0,4,0],[676,10,676,38,0,0,4,1],[712,22,712,40,0,0,4,0],[712,44,712,74,0,0,4,1],[754,8,754,35,0,0,4,1]],"Curl_rlimit_avail":[[201,6,201,16,0,0,4,1],[203,11,203,27,0,0,4,1]],"do_init_reader_stack":[[1111,6,1111,10,0,0,4,0],[1111,15,1111,29,0,0,4,1],[1113,9,1113,33,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_easy_init","curl_multi_init","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_wait","curl_easy_send","curl_ws_send","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_wait","curl_easy_send","curl_ws_send","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":3}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // 2. Create URL handle and parse URL from input data
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Use first part of data as URL string (null-terminated)
    size_t url_len = size > 256 ? 256 : size;
    char *url_str = (char *)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';

    CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    
    // Get the parsed URL back
    char *retrieved_url = nullptr;
    CURLUcode url_get_res = curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_2);

    // 3. Create easy handle and multi handle
    CURL *easy_handle = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();
    
    if (!easy_handle || !multi_handle) {
        if (retrieved_url) curl_free(retrieved_url);
        free(url_str);
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 4. Set options on easy handle
    if (retrieved_url) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, retrieved_url);
    }
    
    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor for socket action
        int fd = fuzz_fileno(in_file);
        
        // Set up easy handle for reading from file
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
        }

        // 5. Set multi handle option
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);

        // 6. Add easy handle to multi handle
        CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);

        // 7. Perform multi handle operations
        int running_handles = 0;
        CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);

        // 8. Socket action with file descriptor
        int ev_bitmask = CURL_POLL_IN | CURL_POLL_OUT;
        curl_multi_socket_action(multi_handle, fd, ev_bitmask, &running_handles);

        // 9. Wait for activity
        struct curl_waitfd extra_fds;
        extra_fds.fd = fd;
        extra_fds.events = CURL_WAIT_POLLIN | CURL_WAIT_POLLOUT;
        extra_fds.revents = 0;
        
        int ret = 0;
        curl_multi_wait(multi_handle, &extra_fds, 1, fuzz_int32_t_3, &ret);

        // 10. Try to send data using curl_easy_send
        size_t sent = 0;
        if (size > 0) {
            curl_easy_send(easy_handle, data, size > 1024 ? 1024 : size, &sent);
        }

        // 11. Try to send data using curl_ws_send (WebSocket)
        // Note: This requires WebSocket setup which we don't have in this simple test
        // We still call it for coverage
        curl_off_t fragsize = 0;
        unsigned int flags = 0;
        curl_ws_send(easy_handle, data, size > 512 ? 512 : size, &sent, fuzz_int64_t_4, fuzz_uint32_t_5);

        // 12. Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);

        // Close files
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
    }

    // 13. Cleanup
    if (retrieved_url) {
        curl_free(retrieved_url);
    }
    free(url_str);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}