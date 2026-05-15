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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"ares_dns_write_rr_opt":[[673,9,673,12,0,0,4,1]],"ares_parse_a_reply":[[62,7,62,16,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_write","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_get_opt_byid","ares_dns_class_fromstr","ares_query_dnsrec","ares_process_pending_write","ares_parse_a_reply","ares_free_hostent","ares_inet_pton","ares_set_servers","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_write","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_get_opt_byid","ares_dns_class_fromstr","ares_query_dnsrec","ares_process_pending_write","ares_parse_a_reply","ares_free_hostent","ares_inet_pton","ares_set_servers","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse DNS response from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Test DNS record write functionality
        unsigned char *write_buf = nullptr;
        size_t write_buf_len = 0;
        if (ares_dns_write(dnsrec, &write_buf, &write_buf_len) == ARES_SUCCESS) {
            // Clean up the write buffer
            if (write_buf) {
                free(write_buf);
            }
        }

        // Test reading RR counts from various sections
        size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        size_t authority_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
        size_t additional_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);

        // Try to read RRs from answer section if available
        if (answer_cnt > 0) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
            if (rr) {
                // Get RR name
                const char *rr_name = ares_dns_rr_get_name(rr);
                
                // Try to get OPT count if this is an OPT record
                size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                if (opt_cnt > 0) {
                    const unsigned char *opt_val = nullptr;
                    size_t opt_val_len = 0;
                    
                    // Try both ways to get OPT values
                    for (size_t i = 0; i < opt_cnt; i++) {
                        unsigned short opt_code = ares_dns_rr_get_opt(rr, ARES_RR_OPT_OPTIONS, i, &opt_val, &opt_val_len);
                        
                        // Also try by ID
                        ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_code, &opt_val, &opt_val_len);
                    }
                }
            }
        }

        // Parse DNS class from string
        ares_dns_class_t dns_class;
        ares_dns_class_fromstr(&dns_class, "IN");

        // Test query with parsed record
        unsigned short query_id = 0;
        ares_query_dnsrec(channel, "example.com", dns_class, ARES_REC_TYPE_A, 
                         [](void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
                             // Callback - no action needed for fuzzing
                         }, nullptr, &query_id);

        // Process pending writes
        ares_process_pending_write(channel);

        // Try to parse input as a hostent (A record reply)
        struct hostent *host = nullptr;
        ares_parse_a_reply(data, static_cast<int>(size), &host, nullptr, nullptr);
        
        // Free hostent if allocated
        if (host) {
            ares_free_hostent(host);
        }

        // Test inet_pton with a sample IP
        struct in_addr addr;
        ares_inet_pton(AF_INET, "8.8.8.8", &addr);

        // Set up a server list
        struct ares_addr_node *servers = nullptr;
        struct ares_addr_node *server = static_cast<ares_addr_node*>(malloc(sizeof(ares_addr_node)));
        if (server) {
            server->family = AF_INET;
            server->addr.addr4.s_addr = htonl(0x08080808);  // 8.8.8.8
            server->next = nullptr;
            servers = server;
            
            // Set servers on channel
            ares_set_servers(channel, servers);
            
            // Free server list
            free(server);
        }

        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up resources
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}