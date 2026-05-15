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
//<ID> 823
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 13
//<Quality> {"density":9,"unique_branches":{"ares_get_server_addr":[[1062,7,1062,38,0,0,4,0],[1064,9,1064,31,0,0,4,1],[1076,7,1076,38,0,0,4,0],[1078,9,1078,31,0,0,4,1]],"ares_get_servers_csv":[[1333,9,1333,26,0,0,4,0],[1335,11,1335,33,0,0,4,1]],"ares_slist_insert":[[228,7,228,34,0,0,4,0],[232,9,232,20,0,0,4,1]],"ares_servers_update":[[792,11,792,29,0,0,4,0]],"ares_addr_node_to_sconfig_llist":[[854,9,854,32,0,0,4,0],[854,36,854,60,0,0,4,1],[864,9,864,32,0,0,4,1],[867,16,867,48,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_functions_ex","ares_set_servers","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_get_flags","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_set_addr6","ares_dns_record_destroy","ares_process_fd","ares_get_servers_csv","ares_free_string","ares_timeout","ares_getsock","ares_parse_txt_reply_ext","ares_free_data","ares_parse_soa_reply","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_functions_ex","ares_set_servers","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_get_flags","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_set_addr6","ares_dns_record_destroy","ares_process_fd","ares_get_servers_csv","ares_free_string","ares_timeout","ares_getsock","ares_parse_txt_reply_ext","ares_free_data","ares_parse_soa_reply","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom socket functions structure to satisfy ares_set_socket_functions_ex
static struct ares_socket_functions_ex  id_000039_custom_socket_funcs = {
    0,  // version - set to 0 for basic compatibility
    0,  // flags
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < sizeof(uint16_t)) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create ares channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set custom socket functions (using default null functions for fuzzing)
    ares_set_socket_functions_ex(channel, & id_000039_custom_socket_funcs, nullptr);

    // Create DNS server list from input data
    struct ares_addr_node *servers = nullptr;
    struct ares_addr_node *server_node = nullptr;
    
    // Parse input to create server addresses
    size_t offset = 0;
    std::vector<struct in_addr> ipv4_addrs;
    std::vector<struct ares_in6_addr> ipv6_addrs;
    
    // Read IPv4 addresses (minimum 4 bytes each)
    while (offset + sizeof(struct in_addr) <= size) {
        struct in_addr addr;
        memcpy(&addr, data + offset, sizeof(struct in_addr));
        ipv4_addrs.push_back(addr);
        offset += sizeof(struct in_addr);
    }
    
    // Reset offset and read IPv6 addresses (minimum 16 bytes each)
    offset = 0;
    while (offset + sizeof(struct ares_in6_addr) <= size) {
        struct ares_in6_addr addr6;
        memcpy(&addr6, data + offset, sizeof(struct ares_in6_addr));
        ipv6_addrs.push_back(addr6);
        offset += sizeof(struct ares_in6_addr);
    }
    
    // Create server list
    for (const auto& addr : ipv4_addrs) {
        struct ares_addr_node *new_node = 
            (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
        if (!new_node) break;
        
        new_node->family = AF_INET;
        new_node->addr.addr4 = addr;
        new_node->next = nullptr;
        
        if (!servers) {
            servers = new_node;
            server_node = servers;
        } else {
            server_node->next = new_node;
            server_node = new_node;
        }
    }
    
    for (const auto& addr6 : ipv6_addrs) {
        struct ares_addr_node *new_node = 
            (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
        if (!new_node) break;
        
        new_node->family = AF_INET6;
        new_node->addr.addr6 = addr6;
        new_node->next = nullptr;
        
        if (!servers) {
            servers = new_node;
            server_node = servers;
        } else {
            server_node->next = new_node;
            server_node = new_node;
        }
    }
    
    // Set DNS servers
    if (servers) {
        ares_set_servers(channel, servers);
    }
    
    // Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    uint16_t id = size > 0 ? data[0] + (data[1] << 8) : 1234;
    ares_dns_record_create(&dnsrec, id, fuzz_uint16_t_1, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (dnsrec) {
        // Add query to DNS record
        const char *query_name = "example.com";
        ares_dns_record_query_add(dnsrec, fuzz_str_2, ARES_REC_TYPE_AAAA, ARES_CLASS_IN);
        
        // Get flags from DNS record
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        
        // Get RR count from answer section
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        
        // Try to get a const RR if available
        if (rr_count > 0) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
            if (rr && size >= sizeof(struct ares_in6_addr)) {
                // Create IPv6 address from input data
                struct ares_in6_addr ipv6_addr;
                memcpy(&ipv6_addr, data, std::min(size, sizeof(ipv6_addr)));
                
                // Create a mutable RR to set address
                ares_dns_rr_t *mutable_rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
                if (mutable_rr) {
                    ares_dns_rr_set_addr6(mutable_rr, ARES_RR_AAAA_ADDR, &ipv6_addr);
                }
            }
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Process with file descriptors
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        ares_process_fd(channel, fd, ARES_SOCKET_BAD);
        assert_file_closed(&in_file);;
    }
    
    // Get servers in CSV format
    char *servers_csv = ares_get_servers_csv(channel);
    if (servers_csv) {
        ares_free_string(servers_csv);
    }
    
    // Get timeout
    struct timeval maxtv, tv;
    ares_timeout(channel, &maxtv, &tv);
    
    // Get sockets
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    ares_getsock(channel, socks, fuzz_int32_t_4);
    
    // Parse TXT reply from input data
    struct ares_txt_ext *txt_out = nullptr;
    if (size > 0) {
        ares_parse_txt_reply_ext(data, std::min((int)size, 65535), &txt_out);
        if (txt_out) {
            ares_free_data(txt_out);
        }
    }
    
    // Parse SOA reply from input data
    struct ares_soa_reply *soa_out = nullptr;
    if (size > 0) {
        ares_parse_soa_reply(data, std::min((int)size, 65535), &soa_out);
        if (soa_out) {
            ares_free_data(soa_out);
        }
    }
    
    // Clean up server list
    struct ares_addr_node *current = servers;
    while (current) {
        struct ares_addr_node *next = current->next;
        free(current);
        current = next;
    }
    
    // Destroy channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}