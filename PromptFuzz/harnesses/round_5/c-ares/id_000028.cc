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
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_query_cnt","ares_get_servers_ports","ares_dns_record_rr_get","ares_dns_rr_get_abin","ares_dns_record_rr_add","ares_dns_rr_set_str","ares_process","ares_init_options","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_record_query_cnt","ares_get_servers_ports","ares_dns_record_rr_get","ares_dns_rr_get_abin","ares_dns_record_rr_add","ares_dns_rr_set_str","ares_process","ares_init_options","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != 0) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    struct ares_addr_port_node *servers = nullptr;
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_rr_t *rr = nullptr;
    
    // 1. Create a channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 2. Parse input as DNS record
    ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 3. Use ares_dns_record_query_cnt
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // 4. Get servers and ports
        int ret = ares_get_servers_ports(channel, &servers);
        
        // 5. Try to get a binary value from DNS record if we have any RRs
        if (query_cnt > 0) {
            // Get first query section RR
            rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
            if (rr != nullptr) {
                // 6. Use ares_dns_rr_get_abin
                size_t len = 0;
                const unsigned char *abin = ares_dns_rr_get_abin(rr, 
                    ARES_RR_A_ADDR, 0, &len);
                
                // 7. Use ares_dns_rr_set_str - try to set a name
                // Create a new RR for setting string
                ares_dns_rr_t *new_rr = nullptr;
                status = ares_dns_record_rr_add(&new_rr, dnsrec, 
                    ARES_SECTION_ADDITIONAL, "example.com",
                    ARES_REC_TYPE_PTR, ARES_CLASS_IN, 300);
                
                if (status == ARES_SUCCESS && new_rr != nullptr) {
                    ares_dns_rr_set_str(new_rr, ARES_RR_PTR_DNAME, "test.com");
                }
            }
        }
    }
    
    // 8. Set up file descriptors for ares_process
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Create a pipe to simulate network activity
    int pipefd[2];
    if (pipe(pipefd) == 0) {
        // Write some data to the pipe
        if (size > 0) {
            write(pipefd[1], data, size > 1024 ? 1024 : size);
        }
        
        // Set up the read fd
        FD_SET(pipefd[0], &read_fds);
        
        // 9. Use ares_process
        ares_process(channel, &read_fds, &write_fds);
        
        close(pipefd[0]);
        close(pipefd[1]);
    }
    
    // 10. Set up options and destroy them
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_USEVC;
    options.timeout = 2000;
    options.tries = 3;
    
    // Note: ares_destroy_options is not in the provided list, but we'll
    // simulate it by using ares_init_options and then cleaning up
    ares_channel_t *channel2 = nullptr;
    if (ares_init_options(&channel2, &options, 
                          ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES) 
        == ARES_SUCCESS) {
        // Channel created with options
    }
    
    // Cleanup
    if (dnsrec != nullptr) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (servers != nullptr) {
        ares_free_data(servers);
    }
    
    if (channel != nullptr) {
        ares_destroy(channel);
    }
    
    if (channel2 != nullptr) {
        ares_destroy(channel2);
    }
    
    ares_library_cleanup();
    return 0;
}