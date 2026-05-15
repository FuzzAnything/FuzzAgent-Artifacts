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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"ares_expand_string_ex":[[54,7,54,21,0,0,4,0]],"ares_dns_rr_set_opt":[[1421,7,1421,29,0,0,4,0]],"ares_dns_rr_set_opt_own":[[1362,7,1362,22,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_version","ares_set_socket_functions","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_dns_rr_del_abin","ares_dns_record_destroy","ares_parse_aaaa_reply","ares_free_hostent","ares_expand_string","ares_free_string","ares_freeaddrinfo","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_version","ares_set_socket_functions","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_dns_rr_del_abin","ares_dns_record_destroy","ares_parse_aaaa_reply","ares_free_hostent","ares_expand_string","ares_free_string","ares_freeaddrinfo","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Call ares_version
    int version;
    const char *version_str = ares_version(&version);

    // 2. Set up socket functions
    struct ares_socket_functions socket_funcs = {
        .asocket = nullptr,
        .aclose = nullptr,
        .aconnect = nullptr,
        .arecvfrom = nullptr,
        .asendv = nullptr
    };
    ares_set_socket_functions(channel, &socket_funcs, nullptr);

    // 3. Set servers ports using input data
    if (size >= sizeof(struct in_addr) + sizeof(uint16_t)) {
        struct ares_addr_port_node *servers = 
            (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
        if (servers) {
            servers->next = nullptr;
            servers->family = AF_INET;
            memcpy(&servers->addr.addr4, data, sizeof(struct in_addr));
            servers->udp_port = ntohs(*reinterpret_cast<const uint16_t*>(data + sizeof(struct in_addr)));
            servers->tcp_port = servers->udp_port;
            
            ares_set_servers_ports(channel, servers);
            free(servers);
        }
    }

    // 4. Create DNS record and manipulate RR
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, fuzz_uint16_t_2, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Create a RR to manipulate
        ares_dns_rr_t *rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                        fuzz_str_3, ARES_REC_TYPE_A, 
                                        ARES_CLASS_IN, fuzz_uint32_t_4);
        
        if (status == ARES_SUCCESS && rr) {
            // 5. Set address in RR
            struct in_addr addr;
            if (size >= sizeof(struct in_addr)) {
                memcpy(&addr, data, sizeof(struct in_addr));
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
            }

            // 6. Set option in RR (for OPT type)
            if (size >= 4) {
                unsigned short opt_val = *reinterpret_cast<const uint16_t*>(data);
                ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, 
                                    opt_val, data + 2, size - 2);
            }

            // 7. Delete binary data from RR
            ares_dns_rr_del_abin(rr, ARES_RR_OPT_OPTIONS, fuzz_uint64_t_5);
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // 8. Parse AAAA reply using input data
    struct hostent *host = nullptr;
    struct ares_addr6ttl *addrttls = nullptr;
    int naddrttls = 0;
    
    if (size > 0) {
        ares_parse_aaaa_reply(data, size, &host, addrttls, &naddrttls);
        
        if (host) {
            ares_free_hostent(host);
        }
        if (addrttls) {
            free(addrttls);
        }
    }

    // 9. Test expand_string
    if (size > 4) {
        unsigned char *expanded_str = nullptr;
        long enclen = 0;
        
        // Use part of input as encoded string
        size_t encoded_len = size / 2;
        ares_expand_string(data, data + encoded_len, size - encoded_len, 
                          &expanded_str, &enclen);
        
        if (expanded_str) {
            ares_free_string(expanded_str);
        }
    }

    // 10. Create addrinfo structure to free
    struct ares_addrinfo *ai = 
        (struct ares_addrinfo*)malloc(sizeof(struct ares_addrinfo));
    if (ai) {
        memset(ai, 0, sizeof(struct ares_addrinfo));
        ares_freeaddrinfo(ai);
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}