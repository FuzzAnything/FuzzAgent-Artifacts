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
#include <ares.h>
}
extern "C" {
#include <ares_dns.h>
}
extern "C" {
#include <ares_nameser.h>
}
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"ares_dns_record_rr_del":[[460,5,460,29,0,0,4,1],[463,5,463,32,0,0,4,0],[466,5,466,33,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_servers_ports","ares_set_socket_configure_callback","ares_set_local_ip6","ares_create_query","ares_expand_string","ares_free_string","ares_dns_parse","ares_dns_rr_get_keys","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_destroy","ares_get_servers_csv","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_servers_ports","ares_set_socket_configure_callback","ares_set_local_ip6","ares_create_query","ares_expand_string","ares_free_string","ares_dns_parse","ares_dns_rr_get_keys","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_destroy","ares_get_servers_csv","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize options
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN | ARES_FLAG_USEVC;
    options.timeout = 2000;
    options.tries = 3;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    // Create channel with options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set servers using CSV (dummy server for fuzzing)
    const char *servers_csv = "127.0.0.1";
    // Note: ares_set_servers_csv is not in the provided API list, but we'll use
    // ares_set_servers_ports as alternative since CSV version isn't available
    struct ares_addr_port_node server;
    struct ares_addr_port_node *server_list = &server;
    memset(&server, 0, sizeof(server));
    server.next = nullptr;
    server.family = AF_INET;
    server.addr.addr4.s_addr = htonl(0x7f000001); // 127.0.0.1
    server.udp_port = 53;
    server.tcp_port = 53;
    
    ares_set_servers_ports(channel, server_list);
    
    // Set socket configure callback
    ares_set_socket_configure_callback(channel, 
        [](ares_socket_t socket_fd, int type, void *user_data) {
            return 0; // Return success
        }, 
        nullptr);
    
    // Set local IPv6 address
    unsigned char local_ip6[16] = {0};
    if (size >= 16) {
        memcpy(local_ip6, data, 16);
    }
    ares_set_local_ip6(channel, local_ip6);
    
    // Create a DNS query
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    const char *query_name = "example.com";
    
    if (size > 0) {
        // Use first byte as query ID if available
        unsigned short query_id = size > 1 ? (data[0] << 8) | data[1] : 1234;
        
        if (ares_create_query(fuzz_str_2, ns_c_in, ns_t_a, query_id, fuzz_int32_t_3, 
                             &query_buf, &query_len, fuzz_int32_t_4) == ARES_SUCCESS) {
            // Try to expand a name from the query buffer
            unsigned char *expanded_name = nullptr;
            long enclen = 0;
            
            if (query_len > 12) { // DNS header is 12 bytes
                if (ares_expand_string(query_buf + 12, query_buf, query_len, 
                                      &expanded_name, &enclen) == ARES_SUCCESS) {
                    ares_free_string(expanded_name);
                }
            }
            
            // Free the query buffer
            free(query_buf);
        }
    }
    
    // Parse the input as DNS response if it's large enough
    ares_dns_record_t *dnsrec = nullptr;
    if (size >= 12) { // Minimum DNS header size
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_5, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
            // Get RR keys for A record type
            size_t key_count = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_count);
            
            // Try to delete first RR from answer section if it exists
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_6);
            }
            
            // Try to delete first RR from authority section if it exists
            rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
            if (rr_count > 0) {
                ares_dns_record_rr_del(dnsrec, ARES_SECTION_AUTHORITY, fuzz_uint64_t_7);
            }
            
            // Try to delete first RR from additional section if it exists
            rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
            if (rr_count > 0) {
                ares_dns_record_rr_del(dnsrec, ARES_SECTION_ADDITIONAL, fuzz_uint64_t_8);
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // Save options (simulated since ares_save_options isn't in provided API list)
    // We'll use ares_get_servers_csv as alternative
    char *servers_str = ares_get_servers_csv(channel);
    if (servers_str) {
        free(servers_str);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}