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
//<ID> 1412
//<Prompt> []
/*<Combination>: [
*/
//<score> 624, nr_unique_branch: 172
//<Quality> {"density":24,"unique_branches":{"cf_udp_connect":[[1825,6,1825,19,0,0,4,1],[1831,6,1831,34,0,0,4,0],[1833,8,1833,14,0,0,4,1],[1838,8,1838,38,0,0,4,1]],"cf_ip_happy_shutdown":[[740,6,740,19,0,0,4,0]],"cf_socket_update_data":[[1542,6,1542,19,0,0,4,0],[1542,23,1542,53,0,0,4,0]],"tftp_do":[[1290,6,1290,12,0,0,4,1],[1304,6,1304,13,0,0,4,1]],"curl_easy_strerror":[[177,3,177,24,0,0,4,0],[228,3,228,26,0,0,4,0]],"set_local_ip":[[990,6,990,54,0,0,4,1]],"tftp_send_first":[[658,6,658,30,0,0,4,1],[663,3,663,23,0,0,4,0],[664,3,664,26,0,0,4,1],[667,8,667,41,0,0,4,1],[673,8,673,26,0,0,4,1],[686,8,686,38,0,0,4,0],[686,8,686,38,0,0,4,1],[692,8,692,14,0,0,4,1],[695,8,695,60,0,0,4,1],[708,8,708,34,0,0,4,0],[712,22,712,40,0,0,4,1],[716,10,716,28,0,0,4,0],[721,10,721,28,0,0,4,0],[723,10,723,28,0,0,4,0],[728,10,728,28,0,0,4,0],[730,10,730,28,0,0,4,0],[733,10,733,28,0,0,4,1],[747,8,747,20,0,0,4,1],[754,8,754,35,0,0,4,0],[761,3,761,23,0,0,4,1],[770,3,770,22,0,0,4,1],[774,3,774,23,0,0,4,1],[778,3,778,24,0,0,4,1],[782,3,782,10,0,0,4,1]],"cshutdn_run_once":[[97,34,97,79,0,0,4,1]],"cf_socket_shutdown":[[953,6,953,19,0,0,4,0],[960,8,960,36,0,0,4,0],[961,8,961,37,0,0,4,1],[319,4,319,10,8,0,4,0],[319,14,319,33,8,0,4,1]],"cf_get_remote_addr":[[961,6,961,8,0,0,4,0],[962,6,963,49,0,0,4,0]],"tftp_done":[[1010,6,1010,25,0,0,4,1],[1014,6,1014,11,0,0,4,0]],"tftp_setup_connection":[[1320,6,1320,17,0,0,4,0],[1320,6,1320,17,0,0,4,1],[1320,21,1320,67,0,0,4,1],[1324,11,1324,22,0,0,4,0],[1324,11,1324,22,0,0,4,1],[1324,26,1324,69,0,0,4,1]],"multi_runsingle":[[2560,12,2560,21,0,0,4,0],[2560,25,2560,32,0,0,4,0],[2561,14,2561,37,0,0,4,0],[2562,14,2562,61,0,0,4,1]],"Curl_conn_is_connected":[[618,11,618,50,0,0,4,1]],"Curl_shutdown_started":[[205,12,205,28,0,0,4,0]],"tftp_connect":[[910,6,910,12,0,0,4,1],[911,6,911,74,0,0,4,1],[915,6,915,28,0,0,4,1],[921,6,921,41,0,0,4,1],[924,6,924,26,0,0,4,0],[927,8,927,28,0,0,4,1],[931,6,931,26,0,0,4,0],[934,8,934,28,0,0,4,1],[951,6,951,18,0,0,4,1],[958,6,958,12,0,0,4,1],[961,6,961,23,0,0,4,0],[977,8,977,10,0,0,4,1]],"cf_dns_connect":[[238,18,238,24,0,0,4,1]],"Curl_conn_connect":[[539,19,539,24,0,0,4,0]],"cf_ip_happy_query":[[877,6,877,20,0,0,4,1],[901,10,901,18,0,0,4,0]],"cf_ip_attempt_connect":[[228,10,228,20,0,0,4,0]],"Curl_socktype_for_transport":[[715,3,715,10,0,0,4,0]],"cf_socket_active":[[1558,6,1558,34,0,0,4,0]],"Curl_cf_udp_create":[[1885,6,1885,10,0,0,4,1],[1891,6,1891,12,0,0,4,1],[1897,10,1897,19,0,0,4,0],[1898,6,1898,12,0,0,4,1]],"trspeed":[[422,6,422,12,0,0,4,0]],"tftp_translate_code":[[801,6,801,28,0,0,4,1]],"Curl_conn_get_remote_addr":[[1000,6,1000,16,0,0,4,0],[1002,10,1002,12,0,0,4,0],[406,36,406,46,1,0,4,0],[406,50,406,59,1,0,4,0]],"Curl_conn_shutdown":[[173,9,173,11,0,0,4,0],[173,16,173,30,0,0,4,1],[173,34,173,46,0,0,4,1],[176,6,176,9,0,0,4,1],[182,6,182,45,0,0,4,1],[187,8,187,22,0,0,4,1],[194,9,194,11,0,0,4,0],[194,9,194,11,0,0,4,1],[195,8,195,21,0,0,4,0],[198,10,198,16,0,0,4,1],[202,15,202,22,0,0,4,1],[319,4,319,10,20,0,4,0],[319,14,319,33,20,0,4,1]],"cf_ip_ballers_run":[[362,10,362,20,0,0,4,0],[369,15,369,26,0,0,4,1],[319,4,319,10,41,0,4,0],[319,14,319,33,41,0,4,1]],"get_cf_create":[[88,8,88,53,0,0,4,1]],"Curl_shutdown_timeleft":[[169,6,169,45,0,0,4,1],[170,6,170,38,0,0,4,1],[176,10,176,17,0,0,4,0]],"cf_verboseconnect":[[319,4,319,10,1,0,4,0],[319,14,319,33,1,0,4,1]],"multi_conn_should_close":[[602,6,602,22,0,0,4,0]],"cf_ip_attempt_free":[[170,8,170,13,0,0,4,1]],"Curl_conncontrol":[[319,11,319,48,0,0,4,1]],"protocol_connect":[[1916,6,1916,36,0,0,4,1]],"cf_socket_query":[[1639,3,1639,28,0,0,4,0],[1641,50,1641,63,0,0,4,0],[1656,5,1656,21,0,0,4,0],[1660,10,1660,29,0,0,4,1],[1665,5,1665,12,0,0,4,1]],"cf_ip_happy_close":[[854,6,854,14,0,0,4,0]],"tftp_option_add":[[333,6,333,31,0,0,4,1],[334,6,334,52,0,0,4,1]],"cf_socket_open":[[1102,5,1102,51,0,0,4,1],[1107,6,1107,12,0,0,4,1],[1110,6,1110,12,0,0,4,1]],"Curl_protocol_for_transport":[[727,3,727,10,0,0,4,0]],"tftp_perform":[[1256,6,1256,12,0,0,4,1],[1261,6,1261,38,0,0,4,1],[1261,42,1261,48,0,0,4,0]],"state_do":[[2220,13,2220,41,0,0,4,0],[2221,13,2221,35,0,0,4,1]],"Curl_resolv_destroy":[[1024,9,1024,17,0,0,4,1]],"cf_socket_cntrl":[[1572,10,1572,15,0,0,4,0],[1572,10,1572,15,0,0,4,1],[1573,3,1573,32,0,0,4,0],[1573,3,1573,32,0,0,4,1],[1577,3,1577,26,0,0,4,1],[1580,3,1580,29,0,0,4,1]],"cf_setup_connect":[[358,18,358,24,0,0,4,1],[362,6,362,42,0,0,4,1],[373,6,373,39,0,0,4,0],[373,43,373,68,0,0,4,1],[382,6,382,44,0,0,4,0],[382,48,382,72,0,0,4,1],[405,6,405,41,0,0,4,0],[407,8,407,33,0,0,4,1],[419,8,419,17,0,0,4,1],[419,21,419,41,0,0,4,1],[423,6,423,37,0,0,4,0],[435,8,435,17,0,0,4,1],[435,21,435,41,0,0,4,1]],"cf_socket_close":[[937,8,937,50,0,0,4,0]],"Curl_conn_is_multiplex":[[695,8,695,59,0,0,4,0]],"tftp_set_timeouts":[[163,6,163,20,0,0,4,1],[170,6,170,20,0,0,4,0],[179,6,179,26,0,0,4,1],[182,6,182,27,0,0,4,0],[187,6,187,27,0,0,4,1],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,1]],"parseurlandfillconn":[[1664,31,1664,70,0,0,4,1]],"tftp_state_machine":[[856,3,856,24,0,0,4,0],[860,3,860,21,0,0,4,1],[864,3,864,21,0,0,4,1],[868,3,868,22,0,0,4,1],[871,3,871,10,0,0,4,1],[1070,19,1070,20,1,0,4,0],[1070,19,1070,20,1,0,4,1]],"cf_ip_happy_connect":[[804,19,804,24,0,0,4,0],[819,10,819,55,0,0,4,1],[319,4,319,10,18,0,4,0],[319,14,319,33,18,0,4,1]]},"library_calls":["curl_global_init","curl_getenv","curl_url","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_formadd","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_waitfds","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_perform","curl_easy_strerror","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_perform","curl_easy_strerror","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_url","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_formadd","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_waitfds","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_perform","curl_easy_strerror","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_perform","curl_easy_strerror","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_free","curl_global_cleanup"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Get environment variable
    char *env_var = curl_getenv(fuzz_str_1);
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set URL parts using input data
        std::string url_str(reinterpret_cast<const char*>(data), 
                           size > 100 ? 100 : size);
        curl_url_set(url_handle, CURLUPART_URL, url_str.c_str(), fuzz_uint32_t_2);
        
        // Get URL part back
        char *url_part = nullptr;
        curl_url_get(url_handle, CURLUPART_URL, &url_part, fuzz_uint32_t_3);
        if (url_part) {
            curl_free(url_part);
        }
    }
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set some easy handle options
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        
        // Set URL from URL handle
        if (url_handle) {
            char *url = nullptr;
            curl_url_get(url_handle, CURLUPART_URL, &url, fuzz_uint32_t_4);
            if (url) {
                curl_easy_setopt(easy, CURLOPT_URL, url);
                curl_free(url);
            }
        }
        
        // Create MIME structure
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            // Add MIME part
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Set MIME data using input data
                if (size > 0) {
                    curl_mime_data(part, reinterpret_cast<const char*>(data), size);
                }
                curl_mime_name(part, fuzz_str_5);
            }
            
            // Set MIME as POST data
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
        }
        
        // Create form data
        struct curl_httppost *formpost = nullptr;
        struct curl_httppost *lastptr = nullptr;
        if (size > 0) {
            std::string form_data(reinterpret_cast<const char*>(data), 
                                 size > 50 ? 50 : size);
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "fuzzfield",
                        CURLFORM_COPYCONTENTS, form_data.c_str(),
                        CURLFORM_END);
        }
        
        // Create multi handle
        CURLM *multi = curl_multi_init();
        if (multi) {
            // Add easy handle to multi handle
            curl_multi_add_handle(multi, easy);
            
            // Get timeout
            long timeout = 0;
            curl_multi_timeout(multi, &timeout);
            
            // Create waitfd array
            struct curl_waitfd ufds[10];
            unsigned int fd_count = 0;
            curl_multi_waitfds(multi, ufds, 10, &fd_count);
            
            // Try socket operations
            int running_handles = 0;
            curl_multi_socket_all(multi, &running_handles);
            
            // Try send and receive operations
            if (size > 0) {
                char buffer[1024];
                size_t sent = 0, recvd = 0;
                
                // Try to send some data
                curl_easy_send(easy, data, size > 1024 ? 1024 : size, &sent);
                
                // Try to receive some data
                curl_easy_recv(easy, buffer, sizeof(buffer), &recvd);
            }
            
            // Perform multi operations
            curl_multi_perform(multi, &running_handles);
            
            // Get error string (if any)
            const char *err_str = curl_easy_strerror(CURLE_OK);
            
            // Remove handle from multi
            curl_multi_remove_handle(multi, easy);
            
            // Clean up multi
            curl_multi_cleanup(multi);
        }
        
        // Perform easy operation
        CURLcode res = curl_easy_perform(easy);
        
        // Get error info
        if (res != CURLE_OK) {
            const char *err = curl_easy_strerror(res);
        }
        
        // Clean up form
        if (formpost) {
            curl_formfree(formpost);
        }
        
        // Clean up mime
        if (mime) {
            curl_mime_free(mime);
        }
        
        // Clean up easy handle
        curl_easy_cleanup(easy);
    }
    
    // Clean up URL handle
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    // Free environment variable if obtained
    if (env_var) {
        curl_free(env_var);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}