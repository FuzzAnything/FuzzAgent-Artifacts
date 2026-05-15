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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 5
//<Quality> {"density":10,"unique_branches":{"ares_dns_section_tostr":[[739,11,739,18,0,0,4,1],[740,5,740,29,0,0,4,0],[742,5,742,32,0,0,4,1],[744,5,744,33,0,0,4,1]],"ares_sysconfig_apply":[[520,29,520,67,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_reinit","ares_destroy","ares_library_cleanup","ares_set_servers_ports","ares_mkquery","ares_send","ares_free_string","ares_fds","ares_dns_record_create","ares_dns_record_get_id","ares_dns_section_tostr","ares_dns_record_destroy","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_reinit","ares_set_servers_ports","ares_mkquery","ares_send","ares_free_string","ares_fds","ares_dns_record_create","ares_dns_record_get_id","ares_dns_section_tostr","ares_dns_record_destroy","ares_process","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// c-ares headers

// Callback function for ares_send
static void  id_000015_query_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    (void)arg;  // Unused parameter
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
    // Callback implementation - can be empty for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use ares_reinit to reinitialize the channel
    if (ares_reinit(channel) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // 2. Use ares_set_servers_ports to set servers
    // Parse input data to create server list (simplified example)
    struct ares_addr_port_node *servers = nullptr;
    if (size >= 6) {  // Need at least 6 bytes for IP+port
        struct ares_addr_port_node *node = 
            (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
        if (node) {
            node->next = nullptr;
            node->family = AF_INET;
            // Use first 4 bytes for IPv4 address
            memcpy(&node->addr.addr4, data, 4);
            // Use next 2 bytes for port (big-endian)
            uint16_t port;
            memcpy(&port, data + 4, 2);
            port = ntohs(port);
            if (port == 0) port = 53;  // Default DNS port
            node->udp_port = port;
            node->tcp_port = port;
            servers = node;
        }
    }
    
    if (servers) {
        ares_set_servers_ports(channel, servers);
        // Free the server node
        free(servers);
    }

    // 3. Use ares_mkquery to create a DNS query
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    unsigned short query_id = 12345;  // Example query ID
    
    // Use part of input data for domain name if available
    char domain[256] = "example.com";
    if (size > 0) {
        size_t domain_len = size < 255 ? size : 255;
        memcpy(domain, data, domain_len);
        domain[domain_len] = '\0';
        
        // Ensure domain is null-terminated and valid for DNS
        for (size_t i = 0; i < domain_len; i++) {
            if (domain[i] < 32 || domain[i] > 126) {
                domain[i] = 'a';
            }
        }
    }
    
    if (ares_mkquery(domain, C_IN, T_A, query_id, 1, &query_buf, &query_len) == ARES_SUCCESS) {
        // 4. Use ares_send to send the query
        ares_send(channel, query_buf, query_len,  id_000015_query_callback, nullptr);
        ares_free_string(query_buf);
    }

    // 5. Use ares_fds to get file descriptors
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_fds(channel, &read_fds, &write_fds);

    // 6. Use ares_dns_record_get_id (requires a DNS record)
    // Create a simple DNS record for testing
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, 12345, 
                               ARES_FLAG_QR | ARES_FLAG_AA,
                               ARES_OPCODE_QUERY,
                               ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Get the record ID
        unsigned short record_id = ares_dns_record_get_id(dnsrec);
        (void)record_id;  // Use the ID to avoid unused variable warning
        
        // 7. Use ares_dns_section_tostr
        const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
        (void)section_str;  // Use the string to avoid unused variable warning
        
        ares_dns_record_destroy(dnsrec);
    }

    // Process any pending operations
    struct timeval tv = {0, 0};
    ares_process(channel, &read_fds, &write_fds);

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}