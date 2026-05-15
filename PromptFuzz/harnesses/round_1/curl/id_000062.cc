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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 448, nr_unique_branch: 255
//<Quality> {"density":8,"unique_branches":{"hostip_resolv":[[327,4,327,40,20,0,4,0],[319,14,319,33,24,0,4,0],[320,5,320,24,24,0,4,0]],"Curl_trc_write":[[327,4,327,40,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]],"mstate":[[327,4,327,40,8,0,4,0]],"cf_dns_connect":[[324,4,324,8,12,0,4,0],[324,12,324,53,12,0,4,0],[324,4,324,8,13,0,4,0],[324,12,324,53,13,0,4,0],[324,4,324,8,14,0,4,0],[324,12,324,53,14,0,4,0],[319,14,319,33,15,0,4,0],[320,5,320,24,15,0,4,0],[319,14,319,33,17,0,4,0],[320,5,320,24,17,0,4,0],[319,14,319,33,19,0,4,0],[320,5,320,24,19,0,4,0]],"cw_raw_write":[[310,33,310,50,0,0,4,0],[310,54,310,75,0,0,4,0]],"Curl_multi_will_close":[[327,4,327,40,3,0,4,0],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"Curl_trc_read":[[327,4,327,40,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]],"Curl_dnscache_add_negative":[[327,4,327,40,5,0,4,0],[319,14,319,33,6,0,4,0],[320,5,320,24,6,0,4,0]],"curl_multi_add_handle":[[327,4,327,40,16,0,4,0]],"Curl_async_shutdown":[[327,4,327,40,4,0,4,0],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"Curl_trc_mstate_name":[[375,6,375,18,0,0,4,0],[375,22,375,77,0,0,4,0]],"Curl_conn_terminate":[[145,24,145,35,10,0,4,0],[145,24,145,35,10,0,4,1],[319,14,319,33,16,0,4,0],[320,5,320,24,16,0,4,0]],"Curl_client_write":[[327,4,327,40,6,0,4,0],[319,14,319,33,7,0,4,0],[320,5,320,24,7,0,4,0]],"Curl_trc_timer":[[327,4,327,40,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]],"multi_run_dirty":[[327,4,327,40,5,0,4,0],[319,14,319,33,7,0,4,0],[320,5,320,24,7,0,4,0]],"multi_warn_debug":[[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"Curl_conn_connect":[[324,4,324,8,18,0,4,0],[324,12,324,53,18,0,4,0],[324,4,324,8,19,0,4,0],[324,12,324,53,19,0,4,0],[319,14,319,33,24,0,4,0],[320,5,320,24,24,0,4,0],[319,14,319,33,26,0,4,0],[320,5,320,24,26,0,4,0]],"trc_print_ids":[[94,20,94,30,0,0,4,0],[94,20,94,30,0,0,4,1],[96,6,96,19,0,0,4,0],[97,8,97,16,0,0,4,0],[97,8,97,16,0,0,4,1]],"Curl_thrdq_trace":[[392,8,392,41,0,0,4,0],[392,8,392,41,0,0,4,1],[393,8,393,41,0,0,4,0],[396,46,396,47,0,0,4,0],[396,46,396,47,0,0,4,1],[401,46,401,47,0,0,4,1],[327,4,327,40,1,0,4,0],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"socket_connect_result":[[319,14,319,33,6,0,4,0],[320,5,320,24,6,0,4,0]],"async_thrdd_report_item":[[487,9,487,11,0,0,4,1],[319,14,319,33,2,0,4,0],[320,5,320,24,2,0,4,0],[145,24,145,35,4,0,4,0],[145,24,145,35,4,0,4,1],[145,24,145,35,4,0,4,1],[319,4,319,10,7,0,4,0],[319,14,319,33,7,0,4,0],[320,5,320,24,7,0,4,0]],"Curl_trc_multi":[[384,25,384,40,0,0,4,0],[327,4,327,40,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]],"cf_dns_start":[[324,4,324,8,10,0,4,0],[324,12,324,53,10,0,4,0],[319,14,319,33,13,0,4,0],[320,5,320,24,13,0,4,0]],"trc_end_buf":[[113,6,113,39,0,0,4,1],[113,24,113,29,0,0,4,0],[120,11,120,16,0,0,4,0]],"xfer_setup":[[327,4,327,40,14,0,4,0],[319,14,319,33,15,0,4,0],[320,5,320,24,15,0,4,0]],"Curl_async_destroy":[[327,4,327,40,6,0,4,0],[319,14,319,33,7,0,4,0],[320,5,320,24,7,0,4,0]],"Curl_debug":[[129,6,129,23,0,0,4,0],[134,8,134,24,0,0,4,1],[155,7,155,25,0,0,4,1],[156,7,156,31,0,0,4,1],[157,7,157,30,0,0,4,1],[167,7,167,14,0,0,4,0]],"Curl_host_is_ipnum":[[314,6,314,49,0,0,4,0]],"multi_runsingle":[[327,4,327,40,56,0,4,0],[319,14,319,33,59,0,4,0],[320,5,320,24,59,0,4,0]],"Curl_cw_out_done":[[327,4,327,40,3,0,4,0],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"Curl_expire_done":[[327,4,327,40,3,0,4,0],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"Curl_client_reset":[[327,4,327,40,4,0,4,0]],"multi_done_locked":[[327,4,327,40,18,0,4,0],[327,4,327,40,20,0,4,0],[319,14,319,33,23,0,4,0],[320,5,320,24,23,0,4,0],[319,14,319,33,27,0,4,0],[320,5,320,24,27,0,4,0]],"trc_infof":[[251,6,251,10,0,0,4,0],[251,6,251,10,0,0,4,1],[253,6,253,12,0,0,4,0],[253,6,253,12,0,0,4,1],[254,8,254,22,0,0,4,1],[90,3,90,51,1,0,4,0],[319,4,319,10,9,0,4,0],[319,14,319,33,9,0,4,0],[320,5,320,24,9,0,4,0]],"cf_dns_report":[[121,6,121,23,0,0,4,1],[121,27,121,60,0,0,4,0],[319,14,319,33,1,0,4,0],[320,5,320,24,1,0,4,0]],"Curl_trc_feat_infof":[[327,4,327,40,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]],"Curl_conn_cf_add":[[324,4,324,8,5,0,4,0],[324,12,324,53,5,0,4,0],[319,14,319,33,6,0,4,0],[320,5,320,24,6,0,4,0]],"trc_timer_name":[[321,6,321,16,0,0,4,0],[321,20,321,72,0,0,4,0]],"cf_ip_ballers_run":[[155,35,155,46,9,0,4,1],[155,35,155,46,10,0,4,0],[155,35,155,46,10,0,4,1],[155,35,155,46,11,0,4,1],[155,35,155,46,11,0,4,0],[324,4,324,8,28,0,4,0],[324,12,324,53,28,0,4,0],[324,4,324,8,30,0,4,0],[324,12,324,53,30,0,4,0],[324,4,324,8,31,0,4,0],[324,12,324,53,31,0,4,0],[324,4,324,8,32,0,4,0],[324,12,324,53,32,0,4,0],[324,4,324,8,33,0,4,0],[324,12,324,53,33,0,4,0],[324,4,324,8,37,0,4,0],[324,12,324,53,37,0,4,0],[324,4,324,8,38,0,4,0],[324,12,324,53,38,0,4,0],[319,14,319,33,43,0,4,0],[320,5,320,24,43,0,4,0],[319,14,319,33,47,0,4,0],[320,5,320,24,47,0,4,0],[319,14,319,33,49,0,4,0],[320,5,320,24,49,0,4,0],[319,14,319,33,51,0,4,0],[320,5,320,24,51,0,4,0],[319,14,319,33,53,0,4,0],[320,5,320,24,53,0,4,0],[319,14,319,33,60,0,4,0],[320,5,320,24,60,0,4,0],[319,14,319,33,62,0,4,0],[320,5,320,24,62,0,4,0]],"async_thrdd_query":[[327,4,327,40,4,0,4,0],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"cf_tcp_connect":[[324,4,324,8,21,0,4,0],[324,12,324,53,21,0,4,0],[319,14,319,33,26,0,4,0],[320,5,320,24,26,0,4,0],[319,14,319,33,27,0,4,0],[320,5,320,24,27,0,4,0]],"Curl_cpool_add":[[327,4,327,40,13,0,4,0],[319,14,319,33,14,0,4,0],[320,5,320,24,14,0,4,0]],"cw_pause_write":[[327,4,327,40,8,0,4,0],[319,14,319,33,10,0,4,0],[320,5,320,24,10,0,4,0]],"Curl_trc_dns":[[327,4,327,40,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]],"Curl_resolv_query_str":[[144,3,144,36,0,0,4,0]],"multi_done":[[327,4,327,40,9,0,4,0],[319,14,319,33,10,0,4,0],[320,5,320,24,10,0,4,0]],"cw_out_cb_write":[[160,28,160,39,6,0,4,1],[327,4,327,40,17,0,4,0],[319,14,319,33,19,0,4,0],[320,5,320,24,19,0,4,0]],"curl_multi_remove_handle":[[327,4,327,40,21,0,4,0]],"cf_socket_open":[[319,14,319,33,18,0,4,0],[320,5,320,24,18,0,4,0],[324,4,324,8,19,0,4,0],[324,12,324,53,19,0,4,0],[319,14,319,33,22,0,4,0],[320,5,320,24,22,0,4,0]],"cf_ip_happy_init":[[324,4,324,8,3,0,4,0],[324,12,324,53,3,0,4,0],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"Curl_thrdpool_trace":[[446,8,446,40,0,0,4,1],[450,45,450,46,0,0,4,0],[450,45,450,46,0,0,4,1],[453,10,453,25,0,0,4,1],[457,15,457,39,0,0,4,0],[457,15,457,39,0,0,4,1],[457,43,457,68,0,0,4,0],[461,15,461,26,0,0,4,0],[461,15,461,26,0,0,4,1],[467,32,467,54,0,0,4,0],[327,4,327,40,1,0,4,0],[319,4,319,10,4,0,4,0],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"Curl_connect":[[327,4,327,40,12,0,4,0],[319,14,319,33,13,0,4,0],[320,5,320,24,13,0,4,0]],"Curl_infof":[[319,4,319,10,2,0,4,0],[319,14,319,33,2,0,4,0],[320,5,320,24,2,0,4,0]],"cw_download_write":[[327,4,327,40,21,0,4,0],[327,4,327,40,24,0,4,0],[319,14,319,33,27,0,4,0],[320,5,320,24,27,0,4,0],[319,14,319,33,31,0,4,0],[320,5,320,24,31,0,4,0]],"Curl_trc_cf_infof":[[324,4,324,8,2,0,4,0],[324,12,324,53,2,0,4,0],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,0],[320,5,320,24,3,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_multi_perform","curl_multi_timeout","curl_url","curl_url_set","curl_url_set","curl_url_dup","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_multi_perform","curl_multi_timeout","curl_url","curl_url_set","curl_url_set","curl_url_dup","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom types for clarity
using namespace std;

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	

    // Step 1: Global initialization
    curl_global_init(CURL_GLOBAL_ALL);
    curl_global_trace(fuzz_str_1);  // Enable all tracing

    // Step 2: Version info (string used later)
    char *version_str = curl_version();
    
    // Step 3: Parse date from input (first 100 bytes max)
    size_t date_len = size > 100 ? 100 : size;
    char *date_buf = (char*)malloc(date_len + 1);
    memcpy(date_buf, data, date_len);
    date_buf[date_len] = '\0';
    time_t parsed_time = curl_getdate(date_buf, nullptr);
    free(date_buf);
    
    // Step 4: Create and use easy handles
    CURL *easy1 = curl_easy_init();
    CURL *easy2 = nullptr;
    
    if (easy1) {
        // Unescape some input data (next 200 bytes max)
        size_t unescape_len = (size - date_len) > 200 ? 200 : (size - date_len);
        if (unescape_len > 0) {
            int outlength = 0;
            const char *to_unescape = (const char*)(data + date_len);
            char *unescaped = curl_easy_unescape(easy1, to_unescape, unescape_len, &outlength);
            if (unescaped) {
                curl_free(unescaped);
            }
        }
        
        // Set various options using input data
        // Use portion of data for URL
        size_t url_len = size > 300 ? 100 : (size > 200 ? size - 200 : 0);
        if (url_len > 0) {
            char *url_buf = (char*)malloc(url_len + 1);
            memcpy(url_buf, data + 200, url_len);
            url_buf[url_len] = '\0';
            curl_easy_setopt(easy1, CURLOPT_URL, url_buf);
            free(url_buf);
        }
        
        // Set more options
        curl_easy_setopt(easy1, CURLOPT_VERBOSE, 1L);
        
        // Perform upkeep
        curl_easy_upkeep(easy1);
        
        // Duplicate handle
        easy2 = curl_easy_duphandle(easy1);
    }
    
    // Step 5: Create and configure share handle
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // Step 6: Create and configure multi handle
    CURLM *multi = curl_multi_init();
    if (multi && easy1) {
        curl_multi_add_handle(multi, easy1);
        
        // Create a FILE from input data for socket simulation
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            int sockfd = fuzz_fileno(in_file);
            
            // Assign socket data
            void *sockdata = (void*)0x12345678;  // Dummy data
            curl_multi_assign(multi, (curl_socket_t)sockfd, sockdata);
            
            // Perform socket action
            int running = 0;
            curl_multi_socket_action(multi, (curl_socket_t)sockfd, fuzz_int32_t_2, &running);
            
            // Perform multi handle processing
            curl_multi_perform(multi, &running);
            
            // Get timeout
            long timeout = 0;
            curl_multi_timeout(multi, &timeout);
            
            assert_file_closed(&in_file);;
        }
    }
    
    // Step 7: URL handle operations
    CURLU *urlp1 = curl_url();
    CURLU *urlp2 = nullptr;
    
    if (urlp1) {
        // Use portion of data for URL parts
        size_t url_part_len = size > 400 ? 50 : (size > 350 ? size - 350 : 0);
        if (url_part_len > 0) {
            char *part_buf = (char*)malloc(url_part_len + 1);
            memcpy(part_buf, data + 350, url_part_len);
            part_buf[url_part_len] = '\0';
            
            // Try setting various URL parts
            curl_url_set(urlp1, CURLUPART_HOST, part_buf, fuzz_uint32_t_3);
            curl_url_set(urlp1, CURLUPART_PATH, fuzz_str_4, fuzz_uint32_t_5);
            
            free(part_buf);
        }
        
        // Duplicate URL handle
        urlp2 = curl_url_dup(urlp1);
    }
    
    // Step 8: Cleanup in reverse order
    
    // Remove easy handle from multi
    if (multi && easy1) {
        curl_multi_remove_handle(multi, easy1);
    }
    
    // Cleanup multi handle
    if (multi) {
        curl_multi_cleanup(multi);
    }
    
    // Cleanup share handle
    if (share) {
        curl_share_cleanup(share);
    }
    
    // Cleanup easy handles
    if (easy2) {
        curl_easy_cleanup(easy2);
    }
    if (easy1) {
        curl_easy_cleanup(easy1);
    }
    
    // Cleanup URL handles
    if (urlp2) {
        curl_url_cleanup(urlp2);
    }
    if (urlp1) {
        curl_url_cleanup(urlp1);
    }
    
    // Final global cleanup
    curl_global_cleanup();
    
    return 0;
}