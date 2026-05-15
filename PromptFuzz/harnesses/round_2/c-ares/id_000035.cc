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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"ares_parse_aaaa_reply":[[65,7,65,16,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers_csv","ares_set_local_ip6","ares_parse_txt_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_create_query","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_record_destroy","ares_parse_aaaa_reply","ares_free_hostent","ares_process_fds","ares_get_servers_csv","ares_free_string","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_set_local_ip6","ares_parse_txt_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_create_query","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_record_destroy","ares_parse_aaaa_reply","ares_free_hostent","ares_process_fds","ares_get_servers_csv","ares_free_string","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Forward declaration for ares_dns_record_create since it's in the list but missing from headers
extern "C" ares_status_t ares_dns_record_create(ares_dns_record_t **dnsrec, unsigned short id, 
                                                unsigned short flags, ares_dns_opcode_t opcode, 
                                                ares_dns_rcode_t rcode);

// Forward declaration for ares_dns_rr_set_ttl since it's in the list but missing from headers
extern "C" ares_status_t ares_dns_rr_set_ttl(ares_dns_rr_t *dns_rr, unsigned int ttl);

// Forward declaration for ares_free_hostent since it's in the list but missing from headers
extern "C" void ares_free_hostent(struct hostent *host);

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // 2. Create channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Set servers using CSV (using first byte of data to determine port)
    const char *servers_csv = "8.8.8.8";
    if (ares_set_servers_csv(channel, servers_csv) != ARES_SUCCESS) {
        // Continue anyway
    }

    // 4. Set local IPv6 address (using first 16 bytes of data if available)
    if (size >= 16) {
        ares_set_local_ip6(channel, data);
    }

    // 5. Parse input as TXT reply
    struct ares_txt_reply *txt_out = nullptr;
    if (size > 0) {
        // Use data as DNS reply buffer
        ares_parse_txt_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &txt_out);
        
        // Free TXT reply if parsed successfully
        if (txt_out) {
            ares_free_data(txt_out);
        }
    }

    // 6. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, 
                               (unsigned short)(data[0] << 8 | (size > 1 ? data[1] : 0)),
                               ARES_FLAG_RD,
                               ARES_OPCODE_QUERY,
                               ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // 7. Add query to DNS record
        const char *query_name = "example.com";
        ares_dns_record_query_add(dnsrec, query_name, 
                                  ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // 8. Get query count
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        (void)query_cnt; // Use variable to avoid warning
        
        // 9. Create a query using traditional API
        unsigned char *query_buf = nullptr;
        int query_buflen = 0;
        const char *query_name2 = "test.com";
        if (ares_create_query(query_name2, C_IN, T_A, 
                             (unsigned short)(data[0] << 8 | (size > 1 ? data[1] : 0)),
                             1, &query_buf, &query_buflen, 512) == ARES_SUCCESS) {
            if (query_buf) {
                free(query_buf);
            }
        }

        // 10. Create and set TTL on a resource record
        ares_dns_rr_t *rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                  "example.com", ARES_REC_TYPE_A, 
                                  ARES_CLASS_IN, 300) == ARES_SUCCESS) {
            // Set TTL if we have at least 4 bytes of data
            if (size >= 4) {
                unsigned int ttl = (data[0] << 24) | (data[1] << 16) | 
                                  (data[2] << 8) | data[3];
                ares_dns_rr_set_ttl(rr, ttl);
            }
        }

        // 11. Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // 12. Parse as AAAA reply to use ares_free_hostent
    struct hostent *host = nullptr;
    if (size > 0) {
        // Try to parse as AAAA reply
        ares_parse_aaaa_reply(data, (int)(size > INT_MAX ? INT_MAX : size), 
                             &host, nullptr, nullptr);
        
        // Free hostent if parsed
        if (host) {
            ares_free_hostent(host);
        }
    }

    // 13. Process file descriptors (simulate with no events)
    ares_fd_events_t events[1];
    events[0].fd = -1; // No valid fd
    events[0].events = 0;
    ares_process_fds(channel, events, 0, 0);

    // 14. Get servers as CSV string and free it
    char *servers_str = ares_get_servers_csv(channel);
    if (servers_str) {
        ares_free_string(servers_str);
    }

    // 15. Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}