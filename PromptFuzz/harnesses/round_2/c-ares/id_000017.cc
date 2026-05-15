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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 37
//<Quality> {"density":8,"unique_branches":{"ares_sysconfig_apply":[[520,29,520,67,0,0,4,1]],"ares_save_opt_servers":[[63,7,63,18,0,0,4,1],[67,57,67,70,0,0,4,0],[67,57,67,70,0,0,4,1],[71,9,71,39,0,0,4,1]],"ares_init_options_servers":[[249,7,249,29,0,0,4,1]],"ares_dns_record_get_rcode":[[127,7,127,21,0,0,4,1]],"ares_save_options":[[143,7,143,42,0,0,4,0],[145,9,145,33,0,0,4,1]],"ares_qcache_expire":[[145,7,145,20,0,0,4,0]],"ares_dup":[[519,7,519,33,0,0,4,0],[521,9,521,20,0,0,4,1],[532,9,532,27,0,0,4,1]],"ares_in_addr_to_sconfig_llist":[[953,7,953,16,0,0,4,1],[957,15,957,30,0,0,4,0],[957,34,957,46,0,0,4,0],[957,34,957,46,0,0,4,1],[961,9,961,24,0,0,4,1],[969,9,969,51,0,0,4,1]],"ares_init_by_options":[[470,7,470,33,0,0,4,0],[471,9,471,31,0,0,4,1],[477,11,477,33,0,0,4,1]],"ares_expand_name":[[91,7,91,22,0,0,4,1],[91,26,91,38,0,0,4,1],[91,42,91,51,0,0,4,1],[91,55,91,69,0,0,4,1]],"ares_expand_name_validated":[[45,7,45,22,0,0,4,1],[45,26,45,38,0,0,4,1],[45,42,45,51,0,0,4,1],[45,55,45,69,0,0,4,1],[49,7,49,21,0,0,4,1],[49,25,49,47,0,0,4,1],[56,7,56,8,0,0,4,0],[62,7,62,18,0,0,4,1],[67,7,67,29,0,0,4,1],[73,7,73,29,0,0,4,0],[73,7,73,29,0,0,4,1]]},"library_calls":["ares_library_init","ares_library_cleanup","ares_library_cleanup","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_expand_name","ares_free_string","ares_dns_record_destroy","ares_set_servers_ports","ares_dup","ares_save_options","ares_set_socket_configure_callback","ares_destroy","ares_destroy_options","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_expand_name","ares_free_string","ares_dns_record_destroy","ares_set_servers_ports","ares_dup","ares_save_options","ares_set_socket_configure_callback","ares_destroy","ares_destroy_options","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Custom callback for socket configuration
static int  id_000017_sock_config_callback(ares_socket_t sockfd, int type, void *user_data) {
    // Simple callback that just returns 0 (success)
    (void)sockfd;
    (void)type;
    (void)user_data;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Parse DNS record from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get DNS record rcode
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        (void)rcode; // Use rcode to avoid unused variable warning
        
        // Get query count
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // If there are queries, try to modify the first query's name
        if (query_cnt > 0) {
            // Create a test name
            const char *test_name = "test.example.com";
            ares_dns_record_query_set_name(dnsrec, 0, test_name);
        }
        
        // Try to expand a name from the DNS record
        if (size > 0) {
            char *expanded_name = nullptr;
            long enclen = 0;
            
            // Use a small portion of the data for expansion
            size_t expand_size = (size < 100) ? size : 100;
            int expand_result = ares_expand_name(
                data, 
                data, 
                (int)expand_size, 
                &expanded_name, 
                &enclen
            );
            
            if (expand_result == ARES_SUCCESS && expanded_name) {
                ares_free_string(expanded_name);
            }
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // Set up a dummy server list
    struct ares_addr_port_node *servers = nullptr;
    // Create one server entry
    servers = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
    if (servers) {
        memset(servers, 0, sizeof(struct ares_addr_port_node));
        servers->family = AF_INET;
        servers->addr.addr4.s_addr = htonl(0x7F000001); // 127.0.0.1
        servers->udp_port = 53;
        servers->tcp_port = 53;
        servers->next = nullptr;
        
        // Set servers on the channel
        ares_set_servers_ports(channel, servers);
        
        // Duplicate the channel
        ares_channel_t *dup_channel = nullptr;
        int dup_result = ares_dup(&dup_channel, channel);
        
        if (dup_result == ARES_SUCCESS && dup_channel) {
            // Save options from the original channel
            struct ares_options options;
            int optmask = 0;
            int save_result = ares_save_options(channel, &options, &optmask);
            
            // Set socket configure callback
            ares_set_socket_configure_callback(channel,  id_000017_sock_config_callback, nullptr);
            
            // Destroy the duplicated channel
            ares_destroy(dup_channel);
            
            // Free options if needed
            if (save_result == ARES_SUCCESS) {
                ares_destroy_options(&options);
            }
        }
        
        free(servers);
    }
    
    // Write some data to output file
    if (size > 0) {
        fwrite(data, 1, (size < 100) ? size : 100, out_file);
    }
    
    // Read from input file
    uint8_t read_buf[100];
    size_t read_size = (size < sizeof(read_buf)) ? size : sizeof(read_buf);
    if (read_size > 0) {
        fseek(in_file, 0, SEEK_SET);
        fread(read_buf, 1, read_size, in_file);
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Use to avoid unused variable warning
    (void)out_fd; // Use to avoid unused variable warning
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}