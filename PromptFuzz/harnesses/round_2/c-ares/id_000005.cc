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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 15
//<Quality> {"density":12,"unique_branches":{"ares_qcache_calc_key":[[120,9,120,17,0,0,4,1]],"ares_send_nolock":[[158,9,158,32,0,0,4,0]],"ares_dns_rcode_tostr":[[912,11,912,16,0,0,4,0],[913,5,913,28,0,0,4,1],[915,5,915,28,0,0,4,0],[917,5,917,29,0,0,4,0],[919,5,919,29,0,0,4,0],[921,5,921,27,0,0,4,0],[923,5,923,28,0,0,4,0],[925,5,925,29,0,0,4,0],[927,5,927,28,0,0,4,0],[929,5,929,28,0,0,4,0],[931,5,931,28,0,0,4,0],[933,5,933,28,0,0,4,0],[935,5,935,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers","ares_get_servers_csv","ares_free_string","ares_queue_active_queries","ares_query","ares_dns_record_create","ares_dns_record_query_add","ares_send_dnsrec","ares_dns_record_destroy","ares_process","ares_parse_ptr_reply","ares_free_hostent","ares_dns_rcode_tostr","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers","ares_get_servers_csv","ares_free_string","ares_queue_active_queries","ares_query","ares_dns_record_create","ares_dns_record_query_add","ares_send_dnsrec","ares_dns_record_destroy","ares_process","ares_parse_ptr_reply","ares_free_hostent","ares_dns_rcode_tostr","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open memory buffer as FILE for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    int fd = fuzz_fileno(in_file);
    assert_file_closed(&in_file);;
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Create and initialize ares channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Set up server configuration using dummy loopback address
    struct ares_addr_node servers[1];
    servers[0].next = nullptr;
    servers[0].family = AF_INET;
    servers[0].addr.addr4.s_addr = htonl(INADDR_LOOPBACK);
    
    // Use ares_set_servers
    ares_set_servers(channel, servers);
    
    // 3. Get servers as CSV string
    char *csv = ares_get_servers_csv(channel);
    if (csv) {
        ares_free_string(csv);
    }
    
    // 4. Check active queries (should be 0 initially)
    size_t active = ares_queue_active_queries(channel);
    
    // 5. Create a callback for queries
    auto query_callback = [](void* arg, int status, int timeouts, 
                           const unsigned char* abuf, int alen) {
        // Empty callback for fuzzing
    };
    
    // 6. Perform DNS query using input data as query name (if possible)
    if (size > 0) {
        // Copy data as null-terminated string for query name
        char *name = (char*)malloc(size + 1);
        if (name) {
            memcpy(name, data, size);
            name[size] = '\0';
            
            // Use ares_query
            ares_query(channel, name, ns_c_in, ns_t_a, query_callback, nullptr);
            free(name);
        }
    }
    
    // 7. Create and send a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 12345, 
                                                  ARES_FLAG_RD, 
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a query to the record
        if (size > 0) {
            char *name = (char*)malloc(size + 1);
            if (name) {
                memcpy(name, data, size);
                name[size] = '\0';
                if (strlen(name) > 0) {
                    ares_dns_record_query_add(dnsrec, name, 
                                             ARES_REC_TYPE_A, 
                                             ARES_CLASS_IN);
                }
                free(name);
            }
        }
        
        // Send the DNS record
        unsigned short qid = 0;
        auto dnsrec_callback = [](void* arg, ares_status_t status,
                                 size_t timeouts, const ares_dns_record_t* dnsrec) {
            // Empty callback
        };
        
        ares_send_dnsrec(channel, dnsrec, dnsrec_callback, nullptr, &qid);
        ares_dns_record_destroy(dnsrec);
    }
    
    // 8. Process file descriptors using dummy fd_sets
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    if (fd >= 0) {
        FD_SET(fd, &read_fds);
    }
    
    // Use ares_process to handle I/O
    ares_process(channel, &read_fds, &write_fds);
    
    // 9. Parse input as PTR reply
    if (size >= sizeof(struct in_addr)) {
        struct hostent *host = nullptr;
        struct in_addr addr;
        memcpy(&addr, data, sizeof(struct in_addr));
        
        ares_parse_ptr_reply(data, size, &addr, sizeof(addr), 
                           AF_INET, &host);
        
        if (host) {
            ares_free_hostent(host);
        }
    }
    
    // 10. Convert rcode to string for different rcode values
    const char *rcode_str = nullptr;
    for (int rcode = 0; rcode <= 15; rcode++) {
        rcode_str = ares_dns_rcode_tostr((ares_dns_rcode_t)rcode);
        // Just reference to avoid unused variable warning
        (void)rcode_str;
    }
    
    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}