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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 6
//<Quality> {"density":8,"unique_branches":{"ares_parse_aaaa_reply":[[65,7,65,16,0,0,4,1]],"ares_parse_a_reply":[[62,7,62,16,0,0,4,1]],"ares_addrpnode_to_sconfig_llist":[[906,9,906,32,0,0,4,0],[906,36,906,60,0,0,4,1],[916,9,916,32,0,0,4,1],[919,16,919,48,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_dns_opt_get_datatype","ares_dns_opt_get_datatype","ares_set_servers_ports","ares_set_local_ip4","ares_dns_class_fromstr","ares_dns_class_fromstr","ares_query_dnsrec","ares_queue_wait_empty","ares_parse_a_reply","ares_free_hostent","ares_parse_aaaa_reply","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_dns_opt_get_datatype","ares_dns_opt_get_datatype","ares_set_servers_ports","ares_set_local_ip4","ares_dns_class_fromstr","ares_query_dnsrec","ares_queue_wait_empty","ares_parse_a_reply","ares_free_hostent","ares_parse_aaaa_reply","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use ares_dns_opt_get_datatype - check datatype for some DNS keys
    ares_dns_opt_datatype_t dt1 = ares_dns_opt_get_datatype(ARES_RR_A_ADDR, fuzz_uint16_t_2);
    ares_dns_opt_datatype_t dt2 = ares_dns_opt_get_datatype(ARES_RR_TXT_DATA, fuzz_uint16_t_3);
    (void)dt1; (void)dt2; // Prevent unused variable warnings

    // 2. Use ares_set_servers_ports - create server list from input data
    if (size >= sizeof(struct in6_addr) + sizeof(uint16_t)) {
        struct ares_addr_port_node *servers = nullptr;
        struct ares_addr_port_node *server = 
            (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
        if (server) {
            server->next = nullptr;
            server->family = AF_INET6;
            memcpy(&server->addr.addr6, data, sizeof(struct ares_in6_addr));
            server->udp_port = ntohs(*(const uint16_t*)(data + sizeof(struct ares_in6_addr)));
            server->tcp_port = server->udp_port;
            servers = server;
            
            // Use ares_set_servers_ports
            ares_set_servers_ports(channel, servers);
            
            // Clean up
            free(server);
        }
    }

    // 3. Use ares_set_local_ip4 - set local IPv4 address
    if (size >= sizeof(uint32_t)) {
        uint32_t ip4;
        memcpy(&ip4, data, sizeof(uint32_t));
        ares_set_local_ip4(channel, ip4);
    }

    // 4. Use ares_dns_class_fromstr - convert string to DNS class
    ares_dns_class_t qclass;
    const char *class_str = "IN"; // Default class
    if (size > 0) {
        // Create null-terminated string from input data (max 10 chars for safety)
        char class_buf[11] = {0};
        size_t len = size < 10 ? size : 10;
        memcpy(class_buf, data, len);
        class_buf[len] = '\0';
        ares_dns_class_fromstr(&qclass, class_buf);
    } else {
        ares_dns_class_fromstr(&qclass, fuzz_str_4);
    }

    // 5. Create and send DNS query using ares_send_dnsrec
    // Note: ares_send_dnsrec requires ares_dns_record_t which we need to create first
    ares_dns_record_t *dnsrec = nullptr;
    // We'll use ares_dns_record_create if available, otherwise use query APIs
    // Since ares_dns_record_create isn't in the provided list, we'll use ares_query_dnsrec directly
    
    // Use ares_query_dnsrec (similar to ares_send_dnsrec functionality)
    unsigned short qid = 0;
    ares_query_dnsrec(channel, fuzz_str_5, qclass, ARES_REC_TYPE_A, 
                      nullptr, nullptr, &qid);

    // 6. Use ares_queue_wait_empty - wait for pending queries
    ares_queue_wait_empty(channel, fuzz_int32_t_6); // 100ms timeout

    // 7. & 8. Use ares_dns_rr_get_addr and ares_dns_rr_get_addr6
    // Create a dummy DNS record to test address extraction
    // We'll parse the input data as if it were DNS response data
    
    // Try to parse as A record reply
    struct hostent *host_a = nullptr;
    if (size > 0) {
        ares_parse_a_reply(data, size, &host_a, nullptr, 0);
        if (host_a) {
            ares_free_hostent(host_a);
        }
    }
    
    // Try to parse as AAAA record reply  
    struct hostent *host_aaaa = nullptr;
    if (size > 0) {
        ares_parse_aaaa_reply(data, size, &host_aaaa, nullptr, 0);
        if (host_aaaa) {
            ares_free_hostent(host_aaaa);
        }
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}