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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"ares_version":[[31,7,31,14,0,0,4,1]],"ares_save_options":[[115,7,115,44,0,0,4,0],[119,7,119,40,0,0,4,0]]},"library_calls":["ares_library_init","ares_version","ares_init_options","ares_library_cleanup","ares_parse_srv_reply","ares_free_data","ares_save_options","ares_set_servers_ports","ares_expand_name","ares_free_string","ares_mkquery","ares_send","ares_gethostbyaddr","ares_query_dnsrec","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_fds","ares_process","ares_cancel","ares_expand_string","ares_free_string","ares_dns_rec_type_tostr","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init_options","ares_parse_srv_reply","ares_free_data","ares_save_options","ares_set_servers_ports","ares_expand_name","ares_free_string","ares_mkquery","ares_send","ares_gethostbyaddr","ares_query_dnsrec","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_fds","ares_process","ares_cancel","ares_expand_string","ares_free_string","ares_dns_rec_type_tostr","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include c-ares headers

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }
    
    // Get version info
    const char* version_str = ares_version(NULL);
    
    // 2. Initialize channel with options
    ares_channel_t* channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 2;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    int channel_result = ares_init_options(&channel, &options, fuzz_int32_t_1);
    if (channel_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Try to parse input as SRV reply
    struct ares_srv_reply* srv_reply = NULL;
    int parse_srv_result = ares_parse_srv_reply(data, (int)size, &srv_reply);
    if (parse_srv_result == ARES_SUCCESS && srv_reply != NULL) {
        ares_free_data(srv_reply);
    }
    
    // 4. Save channel options
    struct ares_options saved_options;
    int saved_optmask = 0;
    int save_result = ares_save_options(channel, &saved_options, &saved_optmask);
    
    // 5. Set custom servers (simulated - not using real input data for safety)
    struct ares_addr_port_node servers[1];
    memset(&servers[0], 0, sizeof(servers[0]));
    servers[0].family = AF_INET;
    servers[0].addr.addr4.s_addr = htonl(0x08080808); // 8.8.8.8
    servers[0].udp_port = 53;
    servers[0].tcp_port = 53;
    
    int set_servers_result = ares_set_servers_ports(channel, servers);
    
    // 6. Try to expand a name from the input
    char* expanded_name = NULL;
    long enclen = 0;
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file != NULL) {
        // Read a small portion for name expansion
        unsigned char encoded[64];
        size_t read_size = fread(encoded, 1, sizeof(encoded) < size ? sizeof(encoded) : size, in_file);
        assert_file_closed(&in_file);;
        
        if (read_size > 0) {
            int expand_result = ares_expand_name(encoded, encoded, (int)read_size, 
                                                &expanded_name, &enclen);
            if (expanded_name != NULL) {
                ares_free_string(expanded_name);
            }
        }
    }
    
    // 7. Create DNS query using input data as name (limited to safe size)
    unsigned char* query_buf = NULL;
    int query_buflen = 0;
    char query_name[256];
    if (size > 0 && size < sizeof(query_name)) {
        memcpy(query_name, data, size);
        query_name[size < sizeof(query_name)-1 ? size : sizeof(query_name)-1] = '\0';
        
        // Replace non-printable characters
        for (size_t i = 0; i < strlen(query_name); i++) {
            if (query_name[i] < 32 || query_name[i] > 126) {
                query_name[i] = 'a';
            }
        }
        
        int mkquery_result = ares_mkquery(query_name, C_IN, T_A, 
                                         (unsigned short)(size & 0xFFFF), 1,
                                         &query_buf, &query_buflen);
        
        if (mkquery_result == ARES_SUCCESS && query_buf != NULL) {
            // 8. Send the query
            ares_send(channel, query_buf, fuzz_int32_t_2, 
                     [](void* arg, int status, int timeouts, const unsigned char* abuf, int alen) {
                         // Callback for query response
                     }, NULL);
            
            free(query_buf);
        }
    }
    
    // 9. Try gethostbyaddr with input data as address
    if (size >= 4) {
        struct in_addr addr;
        memcpy(&addr, data, 4);
        
        ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_3,
                          [](void* arg, int status, int timeouts, const struct hostent* hostent) {
                              // Callback for reverse lookup
                          }, NULL);
    }
    
    // 10. Try query_dnsrec
    if (size > 0) {
        char dns_name[256];
        size_t name_len = size < sizeof(dns_name)-1 ? size : sizeof(dns_name)-1;
        memcpy(dns_name, data, name_len);
        dns_name[name_len] = '\0';
        
        // Make safe for DNS
        for (size_t i = 0; i < name_len; i++) {
            if (dns_name[i] < 33 || dns_name[i] > 126) {
                dns_name[i] = 'a';
            }
        }
        
        unsigned short qid = 0;
        ares_query_dnsrec(channel, dns_name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                         [](void* arg, ares_status_t status, size_t timeouts, 
                            const ares_dns_record_t* dnsrec) {
                             // Callback for DNS record query
                             if (dnsrec != NULL) {
                                 // Note: We would normally destroy this, but it's const
                                 // ares_dns_record_destroy((ares_dns_record_t*)dnsrec);
                             }
                         }, NULL, &qid);
    }
    
    // 11. Try getaddrinfo
    if (size > 0) {
        char node_name[256];
        size_t node_len = size < sizeof(node_name)-1 ? size : sizeof(node_name)-1;
        memcpy(node_name, data, node_len);
        node_name[node_len] = '\0';
        
        // Make safe for hostname
        for (size_t i = 0; i < node_len; i++) {
            if (node_name[i] < 33 || node_name[i] > 126) {
                node_name[i] = 'a';
            }
        }
        
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        ares_getaddrinfo(channel, node_name, fuzz_str_4, &hints,
                        [](void* arg, int status, int timeouts, struct ares_addrinfo* ai) {
                            // Callback for getaddrinfo
                            if (ai != NULL) {
                                ares_freeaddrinfo(ai);
                            }
                        }, NULL);
    }
    
    // 12. Process pending operations
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        tv.tv_sec = 0;
        tv.tv_usec = 1000;
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // 13. Cancel all pending operations
    ares_cancel(channel);
    
    // 14. Try to expand string from input
    if (size > 0) {
        unsigned char* expanded_str = NULL;
        long str_enclen = 0;
        
        FILE* in_file2 = fmemopen((void*)data, size, "rb");
        if (in_file2 != NULL) {
            unsigned char encoded_str[64];
            size_t read_size = fread(encoded_str, 1, sizeof(encoded_str) < size ? sizeof(encoded_str) : size, in_file2);
            assert_file_closed(&in_file2);;
            
            if (read_size > 0) {
                int expand_str_result = ares_expand_string(encoded_str, encoded_str, 
                                                          (int)read_size, &expanded_str, &str_enclen);
                if (expanded_str != NULL) {
                    ares_free_string(expanded_str);
                }
            }
        }
    }
    
    // 15. Test DNS record type to string conversion
    const char* type_str = ares_dns_rec_type_tostr(ARES_REC_TYPE_A);
    
    // 16. Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}