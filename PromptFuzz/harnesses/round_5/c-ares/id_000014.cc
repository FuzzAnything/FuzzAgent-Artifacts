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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"ares_socket_deref_error":[[93,5,93,12,0,0,4,0]],"ares_servers_update":[[792,11,792,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_mkquery","ares_set_servers_ports","ares_send","ares_fds","ares_dns_record_create","ares_dns_record_get_id","ares_dns_section_tostr","ares_dns_record_query_add","ares_reinit","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_mkquery","ares_set_servers_ports","ares_send","ares_fds","ares_dns_record_create","ares_dns_record_get_id","ares_dns_section_tostr","ares_dns_record_query_add","ares_reinit","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Use the input data as a DNS query/response buffer
    if (size > 0) {
        // Create a DNS query using the input data
        unsigned char *query_buf = nullptr;
        int query_len = 0;
        
        // Try to create a query with the first part of input as domain name
        size_t name_len = (size > 255) ? 255 : size;
        char *domain_name = static_cast<char*>(malloc(name_len + 1));
        if (!domain_name) {
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
        
        // Convert input to a null-terminated string for domain name
        for (size_t i = 0; i < name_len; i++) {
            // Convert to valid DNS label characters
            char c = static_cast<char>(data[i]);
            domain_name[i] = (c >= 'a' && c <= 'z') ? c :
                            (c >= 'A' && c <= 'Z') ? c :
                            (c >= '0' && c <= '9') ? c :
                            (c == '.' || c == '-') ? c : 'a';
        }
        domain_name[name_len] = '\0';
        
        // Create DNS query
        int ret = ares_mkquery(domain_name, 
                               C_IN,  // DNS class IN
                               T_A,   // Type A record
                               0x1234, // Query ID
                               1,     // Recursion desired
                               &query_buf,
                               &query_len);
        
        if (ret == ARES_SUCCESS && query_buf && query_len > 0) {
            // Set up servers using input data
            struct ares_addr_port_node servers[2];
            memset(&servers, 0, sizeof(servers));
            
            // Use first 4 bytes for IPv4 address if available
            if (size >= 4) {
                servers[0].family = AF_INET;
                memcpy(&servers[0].addr.addr4, data, 4);
                servers[0].tcp_port = 53;
                servers[0].udp_port = 53;
                servers[0].next = &servers[1];
                
                // Use next 4 bytes for second server if available
                if (size >= 8) {
                    servers[1].family = AF_INET;
                    memcpy(&servers[1].addr.addr4, data + 4, 4);
                    servers[1].tcp_port = 53;
                    servers[1].udp_port = 53;
                    servers[1].next = nullptr;
                } else {
                    servers[0].next = nullptr;
                }
                
                ares_set_servers_ports(channel, servers);
            }
            
            // Send the query
            ares_send(channel, query_buf, query_len, 
                     [](void* /*arg*/, int /*status*/, int /*timeouts*/, 
                        const unsigned char* /*abuf*/, int /*alen*/) {
                         // Callback - do nothing for fuzzing
                     }, nullptr);
            
            // Get file descriptors for the channel
            fd_set read_fds, write_fds;
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            int nfds = ares_fds(channel, &read_fds, &write_fds);
            
            // Process file descriptors
            if (nfds > 0) {
                // Create a DNS record to test other APIs
                ares_dns_record_t *dnsrec = nullptr;
                ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                              0x1234,  // ID
                                                              0x0100,  // Flags
                                                              ARES_OPCODE_QUERY,
                                                              ARES_RCODE_NOERROR);
                
                if (status == ARES_SUCCESS && dnsrec) {
                    // Get DNS record ID
                    unsigned short record_id = ares_dns_record_get_id(dnsrec);
                    
                    // Test ares_dns_section_tostr with different sections
                    const char* section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
                    
                    // Add a query to the DNS record
                    ares_dns_record_query_add(dnsrec, domain_name, 
                                             ARES_REC_TYPE_A, ARES_CLASS_IN);
                    
                    // Reinitialize the channel
                    ares_reinit(channel);
                    
                    ares_dns_record_destroy(dnsrec);
                }
            }
            
            free(query_buf);
        }
        
        free(domain_name);
    }
    
    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}