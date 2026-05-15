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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 23
//<Quality> {"density":6,"unique_branches":{"ares_conn_write":[[205,7,205,35,0,0,4,0],[218,7,218,35,0,0,4,1],[225,14,225,45,0,0,4,1]],"ares_requeue_query":[[980,7,980,26,0,0,4,0]],"ares_socket_deref_error":[[79,5,79,22,0,0,4,0]],"default_asendto":[[390,7,390,22,0,0,4,1]],"ares_send_query":[[1331,5,1331,22,0,0,4,1],[1341,5,1341,27,0,0,4,0],[1345,11,1345,34,0,0,4,1],[1391,7,1391,26,0,0,4,1]],"ares_conn_flush":[[280,9,280,37,0,0,4,0],[281,11,281,42,0,0,4,0],[303,7,303,29,0,0,4,1]],"timeadd":[[187,7,187,27,0,0,4,0]],"ares_fetch_connection":[[1215,7,1215,40,0,0,4,0]],"ares_socket_write":[[145,7,145,14,0,0,4,0]],"legacycb_asendto":[[551,7,551,49,0,0,4,1]],"ares_query_timeout_cmp_cb":[[77,7,77,40,0,0,4,0]],"ares_requeue_queries":[[39,10,39,71,0,0,4,0]],"handle_conn_error":[[958,7,958,23,0,0,4,0],[960,15,960,49,0,0,4,1]],"ares_calc_query_timeout":[[1155,7,1155,17,0,0,4,0],[1172,7,1172,17,0,0,4,0]]},"library_calls":["ares_library_init","ares_version","ares_init","ares_library_cleanup","ares_set_socket_functions","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_dns_rr_del_abin","ares_parse_aaaa_reply","ares_free_hostent","ares_search_dnsrec","ares_dns_record_destroy","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init","ares_set_socket_functions","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_dns_rr_del_abin","ares_parse_aaaa_reply","ares_free_hostent","ares_search_dnsrec","ares_dns_record_destroy","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// c-ares headers

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Get library version
    int version;
    const char *ver_str = ares_version(&version);
    (void)ver_str; // Mark as used to avoid warning

    // Create channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set up dummy socket functions (won't actually be used for fuzzing)
    struct ares_socket_functions socket_funcs = {};
    ares_set_socket_functions(channel, &socket_funcs, nullptr);

    // Set up servers from input data (limited to first 256 bytes for addresses)
    if (size >= sizeof(struct in_addr)) {
        struct ares_addr_port_node server;
        server.next = nullptr;
        server.family = AF_INET;
        server.addr.addr4.s_addr = *reinterpret_cast<const uint32_t*>(data);
        server.udp_port = 53;
        server.tcp_port = 53;
        
        if (ares_set_servers_ports(channel, &server) != ARES_SUCCESS) {
            // Continue anyway - this is just for fuzzing
        }
    }

    // Create a DNS record from input data
    ares_dns_record_t *dnsrec = nullptr;
    if (size >= 4) {
        unsigned short id = *reinterpret_cast<const uint16_t*>(data);
        unsigned short flags = *reinterpret_cast<const uint16_t*>(data + 2);
        
        if (ares_dns_record_create(&dnsrec, id, flags, 
                                   ARES_OPCODE_QUERY, 
                                   ARES_RCODE_NOERROR) == ARES_SUCCESS) {
            
            // Add a resource record to the DNS record
            ares_dns_rr_t *rr = nullptr;
            if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                      fuzz_str_1, ARES_REC_TYPE_A,
                                      ARES_CLASS_IN, fuzz_uint32_t_2) == ARES_SUCCESS) {
                
                // Set address from input data
                if (size >= 8) {
                    struct in_addr addr;
                    addr.s_addr = *reinterpret_cast<const uint32_t*>(data + 4);
                    ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                }
                
                // Set OPT record from input data
                if (size >= 12) {
                    unsigned short opt = *reinterpret_cast<const uint16_t*>(data + 8);
                    size_t opt_len = size - 10;
                    if (opt_len > 0 && opt_len <= 65535) {
                        ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt, 
                                          data + 10, opt_len);
                    }
                }
                
                // Delete binary data from the record (simulate modification)
                ares_dns_rr_del_abin(rr, ARES_RR_OPT_OPTIONS, fuzz_uint64_t_3);
            }
            
            // Create a FILE* to read input data
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                // Read some data from the file to simulate usage
                char buffer[256];
                size_t read_size = size > sizeof(buffer) ? sizeof(buffer) : size;
                fread(buffer, 1, read_size, in_file);
                assert_file_closed(&in_file);;
            }
            
            // Create a FILE* to write output
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                // Write some data to the file
                fwrite(data, 1, size > 100 ? 100 : size, out_file);
                assert_file_closed(&out_file);;
            }
            
            // Parse the input as AAAA reply
            if (size >= 4) {
                struct hostent *host = nullptr;
                struct ares_addr6ttl addrttls[1];
                int naddrttls = 0;
                
                ares_parse_aaaa_reply(data, size, &host, addrttls, &naddrttls);
                
                if (host) {
                    ares_free_hostent(host);
                }
            }
            
            // Search DNS record (simulate query)
            ares_search_dnsrec(channel, dnsrec, 
                              [](void *arg, ares_status_t status, 
                                 size_t timeout, const ares_dns_record_t *dnsrec) {
                                  // Callback - do nothing for fuzzing
                              }, 
                              nullptr);
            
            // Destroy DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Perform getaddrinfo lookup (will use our channel configuration)
    struct ares_addrinfo_hints hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, fuzz_str_4, fuzz_str_5, &hints,
                    [](void *arg, int status, int timeouts, 
                       struct ares_addrinfo *result) {
                        // Callback - do nothing for fuzzing
                        if (result) {
                            ares_freeaddrinfo(result);
                        }
                    }, 
                    nullptr);

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}