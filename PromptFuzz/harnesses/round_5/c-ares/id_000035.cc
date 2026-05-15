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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 24
//<Quality> {"density":18,"unique_branches":{"ares_parse_a_reply":[[87,27,87,40,0,0,4,0]],"ares_send_nolock":[[221,33,221,36,0,0,4,0]],"ares_addrinfo2addrttl":[[240,7,240,24,0,0,4,1],[244,7,244,17,0,0,4,1],[244,21,244,38,0,0,4,1],[248,7,248,24,0,0,4,0],[248,28,248,44,0,0,4,1],[252,7,252,25,0,0,4,1],[256,7,256,25,0,0,4,1],[263,10,263,20,0,0,4,0],[263,10,263,20,0,0,4,1],[264,9,264,36,0,0,4,0],[270,26,270,38,0,0,4,0],[270,26,270,38,0,0,4,1],[271,9,271,34,0,0,4,0],[271,9,271,34,0,0,4,1],[275,9,275,36,0,0,4,1],[279,9,279,27,0,0,4,1],[291,11,291,35,0,0,4,0],[291,11,291,35,0,0,4,1]],"ares_addr_node_to_sconfig_llist":[[854,9,854,32,0,0,4,0],[854,36,854,60,0,0,4,1],[864,9,864,32,0,0,4,1],[867,16,867,48,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_inet_pton","ares_inet_pton","ares_set_servers","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_get_opt_byid","ares_dns_class_fromstr","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_query_dnsrec","ares_dns_write","ares_dns_record_destroy","ares_process_pending_write","ares_parse_a_reply","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_inet_pton","ares_inet_pton","ares_set_servers","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_get_opt_byid","ares_dns_class_fromstr","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_query_dnsrec","ares_dns_write","ares_dns_record_destroy","ares_process_pending_write","ares_parse_a_reply","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
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

    // Set up DNS servers (using loopback addresses)
    struct ares_addr_node servers[2];
    struct in_addr addr4;
    struct ares_in6_addr addr6;
    
    // IPv4 server: 127.0.0.1
    if (ares_inet_pton(AF_INET, "127.0.0.1", &addr4) == 1) {
        servers[0].family = AF_INET;
        servers[0].addr.addr4 = addr4;
        servers[0].next = &servers[1];
    }
    
    // IPv6 server: ::1
    if (ares_inet_pton(AF_INET6, "::1", &addr6) == 1) {
        servers[1].family = AF_INET6;
        servers[1].addr.addr6 = addr6;
        servers[1].next = nullptr;
    }
    
    ares_set_servers(channel, servers);

    // Parse input data as DNS response
    ares_dns_record_t *dnsrec = nullptr;
    unsigned int flags = 0;
    ares_status_t parse_status = ares_dns_parse(data, size, flags, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Count resource records in each section
        for (int sect = ARES_SECTION_ANSWER; sect <= ARES_SECTION_ADDITIONAL; sect++) {
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)sect);
            
            // Examine each resource record
            for (size_t i = 0; i < rr_count; i++) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, (ares_dns_section_t)sect, i);
                if (rr) {
                    // Check for OPT options
                    size_t opt_count = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                    for (size_t opt_idx = 0; opt_idx < opt_count; opt_idx++) {
                        const unsigned char *opt_val = nullptr;
                        size_t opt_len = 0;
                        unsigned short opt_code = ares_dns_rr_get_opt(rr, ARES_RR_OPT_OPTIONS, opt_idx, &opt_val, &opt_len);
                        
                        // Also try to get by specific option ID
                        const unsigned char *byid_val = nullptr;
                        size_t byid_len = 0;
                        ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_code, &byid_val, &byid_len);
                    }
                }
            }
        }

        // Create a new query based on the parsed response
        unsigned short qid = 0;
        ares_dns_class_t dnsclass;
        const char *class_str = "IN";
        if (ares_dns_class_fromstr(&dnsclass, class_str)) {
            // Use a callback that does nothing
            auto dummy_callback = [](void* arg, ares_status_t status, 
                                    size_t timeouts, const ares_dns_record_t* dnsrec) {};
            
            // Try to query for "example.com" using the parsed record's type if available
            size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
            if (query_cnt > 0) {
                const char *name = nullptr;
                ares_dns_rec_type_t qtype;
                ares_dns_class_t qclass;
                if (ares_dns_record_query_get(dnsrec, 0, &name, &qtype, &qclass) == ARES_SUCCESS && name) {
                    ares_query_dnsrec(channel, name, dnsclass, qtype, dummy_callback, nullptr, &qid);
                }
            }
        }

        // Write DNS record to buffer
        unsigned char *out_buf = nullptr;
        size_t out_len = 0;
        ares_dns_write(dnsrec, &out_buf, &out_len);
        if (out_buf) {
            free(out_buf);
        }

        // Clean up parsed DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Process pending writes
    ares_process_pending_write(channel);

    // Try to parse input as A record reply and free hostent if successful
    struct hostent *host = nullptr;
    struct ares_addrttl addrttls[10];
    int naddrttls = 10;
    int parse_a_status = ares_parse_a_reply(data, size, &host, addrttls, &naddrttls);
    if (parse_a_status == ARES_SUCCESS && host) {
        ares_free_hostent(host);
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}