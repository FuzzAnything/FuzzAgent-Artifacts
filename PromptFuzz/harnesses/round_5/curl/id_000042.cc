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
//<ID> 809
//<Prompt> []
/*<Combination>: [
*/
//<score> 840, nr_unique_branch: 229
//<Quality> {"density":28,"unique_branches":{"cf_socket_cntrl":[[1572,10,1572,15,0,0,4,0],[1572,10,1572,15,0,0,4,1],[1573,3,1573,32,0,0,4,0],[1573,3,1573,32,0,0,4,1],[1577,3,1577,26,0,0,4,1],[1580,3,1580,29,0,0,4,1]],"cf_ip_attempt_connect":[[228,10,228,20,0,0,4,0]],"tftp_do":[[1290,6,1290,12,0,0,4,1],[1304,6,1304,13,0,0,4,0],[1304,6,1304,13,0,0,4,1]],"tftp_send_first":[[658,6,658,30,0,0,4,1],[663,3,663,23,0,0,4,0],[664,3,664,26,0,0,4,1],[667,8,667,41,0,0,4,1],[673,8,673,26,0,0,4,0],[676,10,676,38,0,0,4,0],[686,8,686,38,0,0,4,0],[686,8,686,38,0,0,4,1],[692,8,692,14,0,0,4,0],[692,8,692,14,0,0,4,1],[695,8,695,60,0,0,4,1],[708,8,708,34,0,0,4,0],[712,22,712,40,0,0,4,0],[712,44,712,74,0,0,4,0],[716,10,716,28,0,0,4,0],[721,10,721,28,0,0,4,0],[723,10,723,28,0,0,4,0],[728,10,728,28,0,0,4,0],[730,10,730,28,0,0,4,0],[733,10,733,28,0,0,4,1],[747,8,747,20,0,0,4,1],[754,8,754,35,0,0,4,0],[754,8,754,35,0,0,4,1],[761,3,761,23,0,0,4,1],[770,3,770,22,0,0,4,1],[774,3,774,23,0,0,4,1],[778,3,778,24,0,0,4,1],[782,3,782,10,0,0,4,1]],"cf_dns_connect":[[238,18,238,24,0,0,4,1]],"protocol_connect":[[1916,6,1916,36,0,0,4,1]],"Curl_conn_is_connected":[[618,11,618,50,0,0,4,1]],"cf_socket_shutdown":[[953,6,953,19,0,0,4,0],[960,8,960,36,0,0,4,0],[961,8,961,37,0,0,4,1],[319,4,319,10,8,0,4,0],[319,14,319,33,8,0,4,1]],"set_url_scheme":[[1642,6,1642,29,0,0,4,1],[1642,33,1642,43,0,0,4,1],[1647,6,1647,41,0,0,4,0],[1647,46,1647,48,0,0,4,1],[1647,52,1647,59,0,0,4,1],[1649,6,1649,8,0,0,4,1]],"set_local_ip":[[990,6,990,54,0,0,4,1]],"tftp_state_machine":[[856,3,856,24,0,0,4,0],[860,3,860,21,0,0,4,1],[864,3,864,21,0,0,4,1],[868,3,868,22,0,0,4,1],[871,3,871,10,0,0,4,1],[1070,19,1070,20,1,0,4,0],[1070,19,1070,20,1,0,4,1]],"multi_conn_should_close":[[602,6,602,22,0,0,4,0]],"Curl_conn_is_multiplex":[[695,8,695,59,0,0,4,0]],"curl_url_set":[[1831,3,1831,24,0,0,4,0],[1833,8,1833,14,0,0,4,1],[1889,8,1889,17,0,0,4,1],[1921,10,1921,16,0,0,4,1],[1924,13,1924,15,0,0,4,0],[1924,13,1924,15,0,0,4,1],[1926,12,1926,23,0,0,4,1]],"cf_ip_ballers_run":[[362,10,362,20,0,0,4,0],[369,15,369,26,0,0,4,1],[319,4,319,10,41,0,4,0],[319,14,319,33,41,0,4,1]],"Curl_socket_check":[[129,6,129,34,0,0,4,1],[141,6,141,32,0,0,4,0],[147,6,147,32,0,0,4,1],[153,6,153,32,0,0,4,1],[161,6,161,12,0,0,4,0]],"tftp_doing":[[1230,6,1230,19,0,0,4,1],[1233,11,1233,18,0,0,4,0]],"cf_ip_attempt_free":[[170,8,170,13,0,0,4,1]],"tftp_translate_code":[[801,6,801,28,0,0,4,1]],"trspeed":[[422,6,422,12,0,0,4,0]],"tftp_multi_statemach":[[1172,6,1172,12,0,0,4,1],[1176,6,1176,20,0,0,4,1],[1180,6,1180,30,0,0,4,1],[1193,8,1193,16,0,0,4,1],[1200,13,1200,15,0,0,4,1]],"multi_runsingle":[[2560,12,2560,21,0,0,4,0],[2560,25,2560,32,0,0,4,0],[2561,14,2561,37,0,0,4,0],[2562,14,2562,61,0,0,4,1],[2630,5,2630,22,0,0,4,0],[2634,10,2634,17,0,0,4,0],[2635,12,2635,24,0,0,4,1]],"curlx_strerror":[[256,6,256,13,0,0,4,1],[294,8,294,11,0,0,4,0],[294,15,294,25,0,0,4,0],[314,6,314,7,0,0,4,1],[317,6,317,7,0,0,4,1],[320,6,320,24,0,0,4,1]],"Curl_shutdown_started":[[205,12,205,28,0,0,4,0]],"cf_verboseconnect":[[319,4,319,10,1,0,4,0],[319,14,319,33,1,0,4,1]],"tftp_state_timeout":[[1137,6,1137,11,0,0,4,0],[1141,6,1141,20,0,0,4,1],[1147,6,1147,50,0,0,4,1]],"tftp_connect":[[910,6,910,12,0,0,4,1],[911,6,911,74,0,0,4,1],[915,6,915,28,0,0,4,1],[921,6,921,41,0,0,4,1],[924,6,924,26,0,0,4,0],[927,8,927,28,0,0,4,1],[931,6,931,26,0,0,4,0],[934,8,934,28,0,0,4,1],[951,6,951,18,0,0,4,1],[958,6,958,12,0,0,4,1],[961,6,961,23,0,0,4,0],[977,8,977,10,0,0,4,1]],"Curl_shutdown_timeleft":[[169,6,169,45,0,0,4,1],[170,6,170,38,0,0,4,1],[176,10,176,17,0,0,4,0]],"cf_socket_active":[[1558,6,1558,34,0,0,4,0]],"cf_socket_query":[[1639,3,1639,28,0,0,4,0],[1641,50,1641,63,0,0,4,0],[1656,5,1656,21,0,0,4,0],[1660,10,1660,29,0,0,4,1],[1665,5,1665,12,0,0,4,1]],"cshutdn_run_once":[[97,34,97,79,0,0,4,1]],"protocol_doing":[[1879,6,1879,10,0,0,4,0],[1879,14,1879,38,0,0,4,0]],"Curl_resolv_destroy":[[1024,9,1024,17,0,0,4,1]],"Curl_conn_connect":[[539,19,539,24,0,0,4,0]],"tftp_set_timeouts":[[163,6,163,20,0,0,4,1],[170,6,170,20,0,0,4,0],[179,6,179,26,0,0,4,1],[182,6,182,27,0,0,4,0],[187,6,187,27,0,0,4,1],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,1]],"type_url_check":[[4388,20,4388,61,0,0,4,0],[4395,5,4395,13,0,0,4,1],[4399,5,4399,13,0,0,4,1],[4403,5,4403,13,0,0,4,1],[4404,5,4404,12,0,0,4,0]],"out_number":[[720,6,720,24,0,0,4,0],[722,8,722,29,0,0,4,0],[723,13,723,24,0,0,4,1],[726,8,726,26,0,0,4,1],[44,8,44,41,4,0,4,1]],"parsefmt":[[435,7,435,15,0,0,4,0]],"cf_socket_update_data":[[1542,6,1542,19,0,0,4,0],[1542,23,1542,53,0,0,4,0]],"cf_get_remote_addr":[[952,6,952,8,0,0,4,0],[953,6,954,49,0,0,4,0]],"cf_ip_happy_close":[[854,6,854,14,0,0,4,0]],"tftp_option_add":[[333,6,333,31,0,0,4,1],[334,6,334,52,0,0,4,1]],"Curl_conncontrol":[[315,13,315,42,0,0,4,1],[316,14,316,39,0,0,4,0],[316,43,316,56,0,0,4,0],[317,6,317,31,0,0,4,0],[317,35,317,47,0,0,4,1],[319,11,319,48,0,0,4,1]],"curl_multi_remove_handle":[[821,6,821,30,0,0,4,0],[822,6,822,37,0,0,4,0]],"cf_socket_close":[[937,8,937,50,0,0,4,0]],"Curl_expire_done":[[319,4,319,10,4,0,4,0],[319,14,319,33,4,0,4,1]],"state_do":[[2185,10,2185,23,0,0,4,0],[2188,12,2188,37,0,0,4,1],[2220,13,2220,41,0,0,4,0],[2221,13,2221,35,0,0,4,1]],"cf_ip_happy_shutdown":[[740,6,740,19,0,0,4,0]],"cf_ip_happy_connect":[[804,19,804,24,0,0,4,0],[819,10,819,55,0,0,4,1],[319,4,319,10,18,0,4,0],[319,14,319,33,18,0,4,1]],"Curl_conn_shutdown":[[173,9,173,11,0,0,4,0],[173,16,173,30,0,0,4,1],[173,34,173,46,0,0,4,1],[176,6,176,9,0,0,4,1],[182,6,182,45,0,0,4,1],[187,8,187,22,0,0,4,1],[194,9,194,11,0,0,4,0],[194,9,194,11,0,0,4,1],[195,8,195,21,0,0,4,0],[198,10,198,16,0,0,4,1],[202,15,202,22,0,0,4,1],[319,4,319,10,20,0,4,0],[319,14,319,33,20,0,4,1]],"tftp_done":[[1010,6,1010,25,0,0,4,1],[1014,6,1014,11,0,0,4,0]],"cf_ip_happy_query":[[877,6,877,20,0,0,4,1],[901,10,901,18,0,0,4,0]],"cf_udp_connect":[[1825,6,1825,19,0,0,4,1],[1831,6,1831,34,0,0,4,0],[1833,8,1833,14,0,0,4,1],[1838,8,1838,38,0,0,4,1]],"tftp_perform":[[1256,6,1256,12,0,0,4,1],[1261,6,1261,38,0,0,4,1],[1261,42,1261,48,0,0,4,0],[1261,42,1261,48,0,0,4,1],[1266,6,1266,19,0,0,4,1]],"tftp_setup_connection":[[1324,26,1324,69,0,0,4,0]],"Curl_conn_get_remote_addr":[[991,6,991,16,0,0,4,0],[993,10,993,12,0,0,4,0],[406,36,406,46,1,0,4,0],[406,50,406,59,1,0,4,0]],"cf_setup_connect":[[358,18,358,24,0,0,4,1],[362,6,362,42,0,0,4,1],[373,6,373,39,0,0,4,0],[373,43,373,68,0,0,4,1],[382,6,382,44,0,0,4,0],[382,48,382,72,0,0,4,1],[405,6,405,41,0,0,4,0],[407,8,407,33,0,0,4,1],[419,8,419,17,0,0,4,1],[419,21,419,41,0,0,4,1],[423,6,423,37,0,0,4,0],[435,8,435,17,0,0,4,1],[435,21,435,41,0,0,4,1]],"cf_socket_open":[[1102,5,1102,51,0,0,4,1],[1107,6,1107,12,0,0,4,1],[1110,6,1110,12,0,0,4,1]],"Curl_cf_udp_create":[[1885,6,1885,10,0,0,4,1],[1891,6,1891,12,0,0,4,1],[1897,10,1897,19,0,0,4,0],[1898,6,1898,12,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_global_cleanup","curl_url_set","curl_url_set","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_maprintf","curl_free","curl_multi_poll","curl_multi_strerror","curl_multi_socket_action","curl_easy_pause","curl_easy_perform","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_multi_remove_handle","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_set","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_maprintf","curl_free","curl_multi_poll","curl_multi_strerror","curl_multi_socket_action","curl_easy_pause","curl_easy_perform","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_multi_remove_handle","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback function to write data to FILE*
static size_t  id_000042_write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

// Callback function to read data from FILE*
static size_t  id_000042_read_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fread(ptr, size, nmemb, stream);
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Extract a URL from input data (first 256 bytes or less)
    char url_buf[257];
    size_t url_len = size > 256 ? 256 : size;
    memcpy(url_buf, data, url_len);
    url_buf[url_len] = '\0';

    // 3. Set URL parts
    CURLUcode url_res = curl_url_set(url_handle, CURLUPART_URL, url_buf, 0);
    
    // Also try setting scheme separately
    curl_url_set(url_handle, CURLUPART_SCHEME, "http", 0);

    // 4. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_url_cleanup(url_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_url_cleanup(url_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Set easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, url_buf);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION,  id_000042_write_data);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy, CURLOPT_READFUNCTION,  id_000042_read_data);
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);

    // 7. Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi, easy);

    // 8. Use curl_maprintf for formatting
    char *formatted_str = curl_maprintf("Fuzzing with size: %zu, URL: %s", size, url_buf);
    if (formatted_str) {
        // Do something with formatted string
        curl_free(formatted_str);
    }

    // Get file descriptor for polling
    int in_fd = fuzz_fileno(in_file);
    
    // 9. Setup waitfd structure for polling
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    int ret = 0;
    // 10. Poll with multi handle
    CURLMcode poll_res = curl_multi_poll(multi, extra_fds, 1, 1000, &ret);

    // 11. Get error string for multi operations
    const char *multi_error_str = curl_multi_strerror(multi_add_res);

    // 12. Perform socket action (simulate activity)
    int running_handles = 0;
    CURLMcode socket_res = curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &running_handles);

    // 13. Pause the easy handle
    CURLcode pause_res = curl_easy_pause(easy, CURLPAUSE_RECV);

    // 14. First perform attempt (will likely fail but we call it)
    CURLcode perform_res1 = curl_easy_perform(easy);

    // 15. Second perform attempt
    CURLcode perform_res2 = curl_easy_perform(easy);

    // 16. Get easy error string
    const char *easy_error_str = curl_easy_strerror(perform_res1);

    // 17. Compare strings
    int str_equal = curl_strnequal("http", url_buf, 4);

    // Clean up in proper order
    // 18. Remove handle from multi
    if (multi) {
        curl_multi_remove_handle(multi, easy);
    }

    // 19. Cleanup URL handle
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }

    // 20. Cleanup multi handle
    if (multi) {
        curl_multi_cleanup(multi);
    }

    // 21. Cleanup easy handle
    if (easy) {
        curl_easy_cleanup(easy);
    }

    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // 22. Free memory if any was allocated
    // (curl_free was already called for formatted_str if it existed)

    // 23. Global cleanup
    curl_global_cleanup();

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}