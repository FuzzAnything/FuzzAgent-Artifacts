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
//<ID> 888
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"find_src_addr":[[378,7,378,47,0,0,4,0]],"ares_sortaddrinfo":[[427,9,427,27,0,0,4,0]]},"library_calls":["ares_library_init","ares_version","ares_init","ares_library_cleanup","ares_set_socket_functions","ares_set_servers_ports","ares_destroy","ares_library_cleanup","ares_mkquery","ares_parse_aaaa_reply","ares_free_hostent","ares_getaddrinfo","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init","ares_set_socket_functions","ares_set_servers_ports","ares_mkquery","ares_parse_aaaa_reply","ares_free_hostent","ares_getaddrinfo","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/



// Custom socket functions structure
static struct ares_socket_functions  id_000050_socket_functions;
static void * id_000050_socket_user_data = nullptr;

// Callback for ares_search_dnsrec
static void  id_000050_search_callback(void *arg, ares_status_t status, 
                           size_t timeout_ms, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeout_ms;
    (void)dnsrec;
    // Callback implementation - minimal for fuzzing
}

// Callback for ares_getaddrinfo
static void  id_000050_addrinfo_callback(void *arg, int status, int timeouts,
                             struct ares_addrinfo *res) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (res) {
        ares_freeaddrinfo(res);
    }
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Initialize library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Get version
    int version;
    const char *ver_str = ares_version(&version);
    (void)ver_str; // Suppress unused warning
    
    // Initialize channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set socket functions (dummy implementation)
    memset(& id_000050_socket_functions, 0, sizeof( id_000050_socket_functions));
    ares_set_socket_functions(channel, & id_000050_socket_functions,  id_000050_socket_user_data);
    
    // Set servers ports (dummy server)
    struct ares_addr_port_node server;
    memset(&server, 0, sizeof(server));
    server.next = nullptr;
    server.family = AF_INET;
    server.addr.addr4.s_addr = htonl(0x08080808); // 8.8.8.8
    server.udp_port = 53;
    server.tcp_port = 53;
    
    if (ares_set_servers_ports(channel, &server) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // Create a DNS query using mkquery
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    const char *test_name = "example.com";
    
    if (ares_mkquery(fuzz_str_1, C_IN, T_AAAA, fuzz_uint16_t_2, fuzz_int32_t_3, 
                     &query_buf, &query_len) == ARES_SUCCESS) {
        if (query_buf) {
            free(query_buf);
        }
    }
    
    // Create DNS record and add query
    ares_dns_record_t *dnsrec = nullptr;
    // Note: ares_dns_record_create isn't in the API list, but we need it
    // We'll skip this part since we can't create a dnsrec without the create function
    
    // Parse AAAA reply from input data
    struct hostent *host = nullptr;
    struct ares_addr6ttl *addrttls = nullptr;
    int naddrttls = 0;
    
    if (size > 0) {
        ares_parse_aaaa_reply(data, (int)size, &host, addrttls, &naddrttls);
        
        if (host) {
            ares_free_hostent(host);
        }
        if (addrttls) {
            free(addrttls);
        }
    }
    
    // Try to perform search with DNS record (will fail without valid dnsrec)
    if (size > 10) {
        // Use part of input as "name"
        char name[256];
        size_t name_len = size < 255 ? size : 255;
        memcpy(name, data, name_len);
        name[name_len] = '\0';
        
        // Create a query for getaddrinfo
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        ares_getaddrinfo(channel, name, fuzz_str_4, &hints, 
                         id_000050_addrinfo_callback, nullptr);
        
        // Process pending operations
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        int nfds = ares_fds(channel, &read_fds, &write_fds);
        if (nfds > 0) {
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            
            ares_process(channel, &read_fds, &write_fds);
        }
    }
    
    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}