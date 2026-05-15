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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 703, nr_unique_branch: 64
//<Quality> {"density":37,"unique_branches":{"fetch_addr":[[236,6,236,10,0,0,4,1],[243,6,243,9,0,0,4,0],[243,13,243,51,0,0,4,0],[251,8,251,43,0,0,4,1],[258,6,258,9,0,0,4,0],[259,8,259,55,0,0,4,1],[263,13,263,48,0,0,4,1],[272,6,272,9,0,0,4,0],[272,13,272,23,0,0,4,1]],"curl_easy_option_next":[[70,8,70,18,0,0,4,1]],"curl_multi_waitfds":[[1343,6,1343,21,0,0,4,0],[1343,25,1343,29,0,0,4,0]],"Curl_splayinsert":[[110,6,110,7,0,0,4,0],[113,8,113,41,0,0,4,1],[128,6,128,8,0,0,4,1],[131,11,131,43,0,0,4,1]],"url_match_result":[[1285,11,1285,35,0,0,4,0],[1285,39,1285,64,0,0,4,1]],"url_match_conn":[[1213,6,1213,40,0,0,4,1],[1217,6,1217,37,0,0,4,1],[1220,6,1220,41,0,0,4,0]],"cwfds_add_sock":[[451,32,451,38,0,0,4,0],[452,10,452,35,0,0,4,1],[459,6,459,29,0,0,4,1]],"url_match_fully_connected":[[789,6,789,48,0,0,4,0],[793,8,793,24,0,0,4,0],[319,4,319,10,6,0,4,0],[319,14,319,33,6,0,4,1]],"hostip_resolv":[[664,6,664,11,0,0,4,0],[693,11,693,20,0,0,4,1],[319,4,319,10,18,0,4,0],[319,14,319,33,18,0,4,1]],"url_match_destination":[[1026,6,1026,46,0,0,4,1],[1028,9,1028,35,0,0,4,0],[1031,8,1031,67,0,0,4,1],[1048,9,1048,37,0,0,4,1],[1050,9,1050,37,0,0,4,1],[1055,8,1055,61,0,0,4,1],[1056,8,1056,51,0,0,4,1]],"Curl_dnscache_get":[[306,17,306,20,0,0,4,0]],"Curl_splay":[[54,10,54,21,0,0,4,1],[56,10,56,51,0,0,4,0],[61,12,61,23,0,0,4,0],[68,13,68,21,0,0,4,0],[69,10,69,20,0,0,4,0]],"Curl_waitfds_add_ps":[[477,8,477,37,0,0,4,1],[479,8,479,38,0,0,4,0]],"Curl_cpool_prune_dead":[[731,6,731,22,0,0,4,1]],"Curl_cpool_find":[[614,6,614,12,0,0,4,0],[616,11,616,15,0,0,4,0],[616,11,616,15,0,0,4,1],[621,10,621,33,0,0,4,1]],"Curl_cpool_add":[[480,6,480,13,0,0,4,1]],"url_match_connect_config":[[732,6,732,29,0,0,4,1],[732,33,732,49,0,0,4,1],[732,53,732,72,0,0,4,1],[736,6,736,51,0,0,4,1],[740,6,740,25,0,0,4,1],[740,29,740,49,0,0,4,1],[759,6,759,61,0,0,4,1],[764,6,764,61,0,0,4,1],[771,6,771,35,0,0,4,1],[779,11,779,35,0,0,4,1]],"cpool_remove_conn":[[172,10,172,43,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_data","curl_slist_append","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_socket_action","curl_ws_meta","curl_easy_option_by_id","curl_easy_option_next","curl_easy_ssls_export","curl_multi_remove_handle","curl_multi_remove_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_socket_action","curl_ws_meta","curl_easy_option_by_id","curl_easy_option_next","curl_easy_ssls_export","curl_multi_remove_handle","curl_multi_remove_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl global
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // Create duplicate easy handle
    CURL *easy_dup = curl_easy_duphandle(easy);
    if (!easy_dup) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy_dup);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy_dup);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Add mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy_dup);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Set mime data from fuzzer input
    if (size > 0) {
        curl_mime_data(part, reinterpret_cast<const char*>(data), size);
    }

    // Create header list
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    headers = curl_slist_append(headers, "X-Fuzzed: true");

    // Set mime headers (transfer ownership)
    curl_mime_headers(part, headers, 1);

    // Set various easy options
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/fuzz");
    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
    
    // Set options on duplicate handle too
    curl_easy_setopt(easy_dup, CURLOPT_URL, "http://localhost/fuzz2");
    curl_easy_setopt(easy_dup, CURLOPT_VERBOSE, 0L);

    // Add handles to multi
    curl_multi_add_handle(multi, easy);
    curl_multi_add_handle(multi, easy_dup);

    // Perform multi operations
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);

    // Create waitfd structure for multi_waitfds
    struct curl_waitfd waitfds[2];
    unsigned int fd_count = 0;
    
    // Open input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        waitfds[0].fd = fuzz_fileno(in_file);
        waitfds[0].events = CURL_WAIT_POLLIN;
        waitfds[0].revents = 0;
        
        // Use multi_waitfds
        curl_multi_waitfds(multi, waitfds, 1, &fd_count);
        
        assert_file_closed(&in_file);;
    }

    // Try socket operation (dummy socket)
    int dummy_socket = -1;
    curl_multi_socket(multi, dummy_socket, &running_handles);

    // Check WebSocket metadata (should be NULL for non-WebSocket)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(easy);

    // Iterate through easy options
    const struct curl_easyoption *opt = curl_easy_option_by_id(CURLOPT_URL);
    while (opt) {
        opt = curl_easy_option_next(opt);
    }

    // Try SSL export (will likely fail without SSL context)
    curl_easy_ssls_export(easy, nullptr, nullptr);

    // Remove handles from multi
    curl_multi_remove_handle(multi, easy);
    curl_multi_remove_handle(multi, easy_dup);

    // Clean up
    curl_mime_free(mime); // This also frees the headers due to take_ownership=1
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy_dup);
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}