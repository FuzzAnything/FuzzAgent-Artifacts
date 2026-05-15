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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 12.6, nr_unique_branch: 7
//<Quality> {"density":9,"unique_branches":{"ares_check_cleanup_conns":[[111,11,111,49,0,0,4,0]],"ares_slist_node_push":[[161,14,161,60,0,0,4,1]],"ares_htable_remove":[[402,7,402,55,0,0,4,0]],"ares_fds":[[67,11,67,27,0,0,4,1]],"ares_buf_ensure_space":[[170,7,170,36,0,0,4,0]],"ares_query_timeout_cmp_cb":[[77,7,77,40,0,0,4,0],[84,7,84,42,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_srv_reply","ares_free_data","ares_set_servers_ports","ares_expand_name","ares_dns_rec_type_tostr","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_mkquery","ares_send","ares_gethostbyaddr","ares_query_dnsrec","ares_dns_record_destroy","ares_dns_record_destroy","ares_free_string","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_srv_reply","ares_free_data","ares_set_servers_ports","ares_expand_name","ares_dns_rec_type_tostr","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_mkquery","ares_send","ares_gethostbyaddr","ares_query_dnsrec","ares_dns_record_destroy","ares_dns_record_destroy","ares_free_string","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":4}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Use the input data as DNS reply buffer for parsing
    struct ares_srv_reply *srv_out = nullptr;
    if (size > 0) {
        // Parse SRV reply from input data
        int parse_result = ares_parse_srv_reply(data, static_cast<int>(size), &srv_out);
        
        // Free SRV reply if parsing was successful
        if (parse_result == ARES_SUCCESS && srv_out) {
            ares_free_data(srv_out);
        }
    }

    // Set up a dummy server list using data
    struct ares_addr_port_node *servers = nullptr;
    if (size >= sizeof(struct in_addr)) {
        servers = static_cast<struct ares_addr_port_node*>(malloc(sizeof(struct ares_addr_port_node)));
        if (servers) {
            servers->next = nullptr;
            servers->family = AF_INET;
            memcpy(&servers->addr.addr4, data, sizeof(struct in_addr));
            servers->udp_port = 53;
            servers->tcp_port = 53;
            
            ares_set_servers_ports(channel, servers);
            free(servers);
        }
    }

    // Try to expand a name from the data
    if (size > 4) {
        const unsigned char *encoded = data;
        const unsigned char *abuf = data;
        char *expanded_name = nullptr;
        long enclen = 0;
        
        // Use a portion of data for expansion
        int alen = size > 1024 ? 1024 : static_cast<int>(size);
        ares_expand_name(encoded, abuf, alen, &expanded_name, &enclen);
        
        if (expanded_name) {
            // Convert a record type to string (using a sample type)
            const char *type_str = ares_dns_rec_type_tostr(ARES_REC_TYPE_A);
            (void)type_str; // Avoid unused warning
            
            // Use the expanded name for getaddrinfo
            struct ares_addrinfo_hints hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            
            ares_getaddrinfo(channel, expanded_name, fuzz_str_2, &hints, 
                            [](void *arg, int status, int timeouts, struct ares_addrinfo *result) {
                                if (result) {
                                    ares_freeaddrinfo(result);
                                }
                            }, nullptr);
            
            // Create a simple query using the expanded name
            unsigned char *qbuf = nullptr;
            int qlen = 0;
            if (ares_mkquery(expanded_name, ns_c_in, ns_t_a, fuzz_uint16_t_3, 1, &qbuf, &qlen) == ARES_SUCCESS) {
                if (qbuf && qlen > 0) {
                    // Send the query
                    ares_send(channel, qbuf, fuzz_int32_t_4, 
                             [](void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
                                 // Callback for send operation
                             }, nullptr);
                    free(qbuf);
                }
            }
            
            // Try gethostbyaddr using some data as address
            if (size >= sizeof(struct in_addr)) {
                struct in_addr addr;
                memcpy(&addr, data, sizeof(addr));
                
                ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_5,
                                  [](void *arg, int status, int timeouts, const struct hostent *host) {
                                      // Callback for gethostbyaddr
                                  }, nullptr);
            }
            
            // Try query_dnsrec
            unsigned short qid = 0;
            ares_query_dnsrec(channel, expanded_name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                             [](void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
                                 // Callback for query_dnsrec
                                 if (dnsrec) {
                                     ares_dns_record_destroy((ares_dns_record_t*)dnsrec);
                                 }
                             }, nullptr, &qid);
            
            ares_free_string(expanded_name);
        }
    }

    // Process any pending events
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        ares_process(channel, &read_fds, &write_fds);
    }

    // Clean up
    ares_cancel(channel);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}