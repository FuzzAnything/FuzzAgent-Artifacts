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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 506, nr_unique_branch: 206
//<Quality> {"density":23,"unique_branches":{"Curl_resolv_pollset":[[1010,9,1010,14,0,0,4,0],[1010,9,1010,14,0,0,4,1],[1010,18,1010,25,0,0,4,0],[1012,8,1012,18,0,0,4,1]],"file_upload":[[284,6,284,10,0,0,4,1],[287,6,287,13,0,0,4,0],[287,6,287,13,0,0,4,1],[291,6,291,29,0,0,4,1],[305,6,305,12,0,0,4,0]],"Curl_async_pollset":[[658,6,658,18,0,0,4,0]],"file_connect":[[241,6,241,25,0,0,4,1]],"Curl_cwriter_get_by_type":[[488,40,488,46,0,0,4,1]],"Curl_cw_out_done":[[515,6,515,12,0,0,4,1]],"match_time":[[309,6,309,15,0,0,4,0],[309,6,309,15,0,0,4,1],[309,19,309,30,0,0,4,0],[309,19,309,30,0,0,4,1],[311,8,311,15,0,0,4,0],[311,8,311,15,0,0,4,1],[312,10,312,21,0,0,4,0],[312,10,312,21,0,0,4,1],[314,12,314,20,0,0,4,0],[314,12,314,20,0,0,4,1],[44,22,44,34,1,0,4,0],[44,22,44,34,1,0,4,1],[44,38,44,50,1,0,4,0],[44,38,44,50,1,0,4,1],[44,22,44,34,2,0,4,0],[44,22,44,34,2,0,4,1],[44,38,44,50,2,0,4,0],[44,38,44,50,2,0,4,1]],"parsedate":[[363,9,363,14,0,0,4,0],[363,9,363,14,0,0,4,1],[363,18,363,28,0,0,4,0],[363,18,363,28,0,0,4,1],[372,28,372,44,0,0,4,0],[372,28,372,44,0,0,4,1],[377,10,377,25,0,0,4,0],[377,10,377,25,0,0,4,1],[378,12,378,25,0,0,4,0],[378,12,378,25,0,0,4,1],[380,14,380,27,0,0,4,0],[380,14,380,27,0,0,4,1],[383,12,383,18,0,0,4,0],[383,12,383,18,0,0,4,1],[383,22,383,36,0,0,4,0],[383,22,383,36,0,0,4,1],[385,14,385,26,0,0,4,0],[385,14,385,26,0,0,4,1],[389,12,389,18,0,0,4,0],[389,12,389,18,0,0,4,1],[389,22,389,35,0,0,4,0],[389,22,389,35,0,0,4,1],[392,14,392,25,0,0,4,0],[392,14,392,25,0,0,4,1],[396,10,396,16,0,0,4,0],[396,10,396,16,0,0,4,1],[405,10,405,24,0,0,4,0],[405,10,405,24,0,0,4,1],[406,10,406,60,0,0,4,0],[406,10,406,60,0,0,4,1],[414,12,414,49,0,0,4,0],[414,12,414,49,0,0,4,1],[421,12,421,25,0,0,4,0],[421,12,421,25,0,0,4,1],[422,12,422,29,0,0,4,0],[422,12,422,29,0,0,4,1],[423,12,423,25,0,0,4,0],[423,12,423,25,0,0,4,1],[424,12,424,27,0,0,4,0],[424,12,424,27,0,0,4,1],[425,13,425,28,0,0,4,1],[425,32,425,47,0,0,4,0],[425,32,425,47,0,0,4,1],[439,19,439,34,0,0,4,1],[442,17,442,34,0,0,4,0],[442,17,442,34,0,0,4,1],[443,17,443,32,0,0,4,0],[443,17,443,32,0,0,4,1],[444,17,444,31,0,0,4,0],[444,17,444,31,0,0,4,1],[445,17,445,32,0,0,4,0],[445,17,445,32,0,0,4,1],[453,12,453,18,0,0,4,0],[453,12,453,18,0,0,4,1],[453,22,453,44,0,0,4,0],[453,22,453,44,0,0,4,1],[453,48,453,63,0,0,4,0],[453,48,453,63,0,0,4,1],[454,14,454,23,0,0,4,0],[454,14,454,23,0,0,4,1],[454,27,454,37,0,0,4,0],[454,27,454,37,0,0,4,1],[461,12,461,18,0,0,4,0],[461,12,461,18,0,0,4,1],[461,22,461,44,0,0,4,0],[461,22,461,44,0,0,4,1],[461,48,461,63,0,0,4,0],[461,48,461,63,0,0,4,1],[464,14,464,27,0,0,4,0],[464,14,464,27,0,0,4,1],[465,16,465,28,0,0,4,0],[465,16,465,28,0,0,4,1],[470,14,470,27,0,0,4,0],[470,14,470,27,0,0,4,1],[474,12,474,18,0,0,4,0],[474,12,474,18,0,0,4,1],[484,6,484,18,0,0,4,0],[484,6,484,18,0,0,4,1],[487,6,487,21,0,0,4,0],[487,6,487,21,0,0,4,1],[488,6,488,20,0,0,4,0],[488,6,488,20,0,0,4,1],[489,6,489,21,0,0,4,0],[489,6,489,21,0,0,4,1],[523,6,523,20,0,0,4,0],[523,6,523,20,0,0,4,1],[527,6,527,20,0,0,4,0],[527,6,527,20,0,0,4,1],[527,24,527,37,0,0,4,0],[527,24,527,37,0,0,4,1],[528,6,528,20,0,0,4,1],[528,24,528,37,0,0,4,1],[528,41,528,54,0,0,4,1],[538,6,538,17,0,0,4,0],[538,6,538,17,0,0,4,1],[541,6,541,17,0,0,4,0],[541,6,541,17,0,0,4,1],[541,21,541,55,0,0,4,1],[44,22,44,34,10,0,4,0],[44,22,44,34,10,0,4,1],[44,38,44,50,10,0,4,0],[43,22,43,34,24,0,4,0],[43,22,43,34,24,0,4,1],[43,38,43,50,24,0,4,0],[42,22,42,34,25,0,4,0],[42,22,42,34,25,0,4,1],[42,38,42,50,25,0,4,0],[43,22,43,34,26,0,4,0],[43,22,43,34,26,0,4,1],[43,38,43,50,26,0,4,0],[43,38,43,50,26,0,4,1],[42,22,42,34,27,0,4,0],[42,22,42,34,27,0,4,1],[42,38,42,50,27,0,4,0],[42,38,42,50,27,0,4,1]],"multi_done":[[714,3,714,25,0,0,4,0],[721,3,721,10,0,0,4,1]],"Curl_wakeup_consume":[[346,8,346,11,0,0,4,1],[348,13,348,19,0,0,4,0],[348,13,348,19,0,0,4,1],[353,10,353,32,0,0,4,1],[355,10,355,40,0,0,4,0],[361,11,361,14,0,0,4,0]],"checktz":[[243,6,243,13,0,0,4,0],[243,6,243,13,0,0,4,1],[246,14,246,32,0,0,4,0],[246,14,246,32,0,0,4,1],[248,8,248,21,0,0,4,0],[248,8,248,21,0,0,4,1],[249,8,249,46,0,0,4,0],[249,8,249,46,0,0,4,1]],"Curl_async_timeleft_ms":[[61,6,61,23,0,0,4,0]],"checkday":[[205,6,205,13,0,0,4,0],[205,6,205,13,0,0,4,1],[207,11,207,19,0,0,4,0],[207,11,207,19,0,0,4,1],[211,14,211,19,0,0,4,0],[211,14,211,19,0,0,4,1],[213,8,213,21,0,0,4,0],[213,8,213,21,0,0,4,1],[214,8,214,43,0,0,4,0],[214,8,214,43,0,0,4,1]],"multi_wait":[[1643,10,1643,49,0,0,4,0]],"oneortwodigit":[[44,22,44,34,1,0,4,0],[44,22,44,34,1,0,4,1],[44,38,44,50,1,0,4,0],[44,38,44,50,1,0,4,1]],"skip":[[259,9,259,15,0,0,4,0],[259,9,259,15,0,0,4,1],[44,22,44,34,2,0,4,0],[44,22,44,34,2,0,4,1],[44,38,44,50,2,0,4,0],[44,38,44,50,2,0,4,1],[43,22,43,34,3,0,4,0],[43,22,43,34,3,0,4,1],[43,38,43,50,3,0,4,0],[43,38,43,50,3,0,4,1],[42,22,42,34,4,0,4,0],[42,22,42,34,4,0,4,1],[42,38,42,50,4,0,4,0],[42,38,42,50,4,0,4,1]],"state_do":[[2184,8,2184,15,0,0,4,1],[2220,13,2220,41,0,0,4,1],[2272,10,2272,20,0,0,4,0]],"Curl_multi_pollset":[[1162,24,1162,54,0,0,4,0]],"file_do":[[407,6,407,24,0,0,4,0]],"checkmonth":[[225,6,225,14,0,0,4,0],[225,6,225,14,0,0,4,1],[228,14,228,20,0,0,4,0],[228,14,228,20,0,0,4,1],[229,8,229,41,0,0,4,0],[229,8,229,41,0,0,4,1]],"curl_getdate":[[568,6,568,24,0,0,4,0],[568,6,568,24,0,0,4,1],[569,8,569,28,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_ssls_import","curl_getdate","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_easy_recv","curl_easy_pause","curl_easy_pause","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_ssls_import","curl_getdate","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_easy_recv","curl_easy_pause","curl_easy_pause","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data

    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Create multi and easy handles
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 3. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Set up easy handle options using input data
    // Use part of input as URL
    size_t url_len = size > 100 ? 100 : size;
    char *url = (char *)malloc(url_len + 1);
    if (!url) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    memcpy(url, data, url_len);
    url[url_len] = '\0';
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    free(url);

    // Set READDATA from input file
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);

    // 5. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_res != CURLM_OK) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. SSL session import (use part of input data)
    const char *session_key = "test_session";
    size_t shmac_len = size > 32 ? 32 : size;
    size_t sdata_len = size > 64 ? 64 : size;
    const unsigned char *shmac = data;
    const unsigned char *sdata = data + (size > 32 ? 32 : 0);
    
    curl_easy_ssls_import(easy_handle, session_key, shmac, shmac_len, sdata, sdata_len);

    // 7. Parse date from input (using middle part of data)
    if (size > 50) {
        char date_str[51];
        size_t date_len = size > 100 ? 50 : size/2;
        memcpy(date_str, data + size/4, date_len);
        date_str[date_len] = '\0';
        curl_getdate(date_str, NULL);
    }

    // 8. Multi perform loop
    int running_handles = 0;
    int max_fd = 0;
    fd_set read_fds, write_fds, exc_fds;
    
    // Initial perform
    curl_multi_perform(multi_handle, &running_handles);

    // Poll with timeout
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = fuzz_fileno(in_file);
    extra_fds[0].events = CURL_WAIT_POLLIN;
    int ret = 0;
    
    curl_multi_poll(multi_handle, extra_fds, 1, 100, &ret);

    // Socket action using file descriptor
    curl_socket_t sock_fd = fuzz_fileno(in_file);
    int ev_bitmask = CURL_CSELECT_IN;
    curl_multi_socket_action(multi_handle, sock_fd, ev_bitmask, &running_handles);

    // 9. Try to receive data (non-blocking)
    char recv_buffer[1024];
    size_t bytes_recv = 0;
    curl_easy_recv(easy_handle, recv_buffer, sizeof(recv_buffer), &bytes_recv);

    // 10. Pause and resume transfer
    curl_easy_pause(easy_handle, CURLPAUSE_RECV);
    curl_easy_pause(easy_handle, CURLPAUSE_CONT);

    // 11. Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);

    // 12. Cleanup
    assert_file_closed(&in_file);;
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(sock_fd);
	return 0;
}