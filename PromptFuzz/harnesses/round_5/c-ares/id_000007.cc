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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 33
//<Quality> {"density":10,"unique_branches":{"ares_parse_sortlist":[[233,7,233,18,0,0,4,0],[261,9,261,31,0,0,4,0],[261,35,261,59,0,0,4,0],[261,35,261,59,0,0,4,1],[265,9,265,31,0,0,4,0],[281,7,281,29,0,0,4,0]],"ares_set_sortlist":[[593,7,593,29,0,0,4,1],[593,33,593,41,0,0,4,1]],"ip_natural_mask":[[78,7,78,31,0,0,4,0],[78,7,78,31,0,0,4,1],[83,7,83,17,0,0,4,0],[83,7,83,17,0,0,4,1],[87,7,87,17,0,0,4,0],[87,7,87,17,0,0,4,1]],"ares_parse_a_reply":[[87,27,87,40,0,0,4,1]],"parse_sort":[[125,7,125,29,0,0,4,0],[132,7,132,77,0,0,4,0],[138,7,138,29,0,0,4,0],[144,7,144,58,0,0,4,0],[149,7,149,63,0,0,4,1],[160,9,161,10,0,0,4,0],[167,9,167,31,0,0,4,0],[171,9,171,32,0,0,4,0],[174,11,174,19,0,0,4,0],[174,11,174,19,0,0,4,1],[174,23,174,33,0,0,4,0],[174,23,174,33,0,0,4,1],[177,11,177,38,0,0,4,0],[177,11,177,38,0,0,4,1],[177,42,177,51,0,0,4,0],[177,42,177,51,0,0,4,1],[188,11,188,62,0,0,4,0],[205,7,205,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_a_reply","ares_set_sortlist","ares_set_servers","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_get_opt_byid","ares_dns_parse","ares_dns_record_destroy","ares_inet_pton","ares_dns_class_fromstr","ares_process_pending_write","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_a_reply","ares_set_sortlist","ares_set_servers","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_get_opt_byid","ares_dns_parse","ares_dns_record_destroy","ares_inet_pton","ares_dns_class_fromstr","ares_process_pending_write","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
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
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Parse A record reply from input data
    struct hostent *host = nullptr;
    struct ares_addrttl addrttls[10];
    int naddrttls = 0;
    
    // Use the input data as DNS reply buffer
    int parse_result = ares_parse_a_reply(data, size, &host, addrttls, &naddrttls);
    
    // 2. Set sortlist using a string from input data
    if (size > 0) {
        // Create null-terminated string from input data for sortlist
        char *sortstr = (char *)malloc(size + 1);
        if (sortstr) {
            memcpy(sortstr, data, size);
            sortstr[size] = '\0';
            ares_set_sortlist(channel, sortstr);
            free(sortstr);
        }
    }

    // 3. Set servers using input data
    if (size >= sizeof(struct in_addr)) {
        struct ares_addr_node *servers = (struct ares_addr_node *)malloc(sizeof(struct ares_addr_node));
        if (servers) {
            servers->next = nullptr;
            servers->family = AF_INET;
            
            // Use first 4 bytes of input as IPv4 address
            memcpy(&servers->addr.addr4, data, sizeof(struct in_addr));
            
            ares_set_servers(channel, servers);
            free(servers);
        }
    }

    // 4. Create a DNS record and use ares_dns_rr_get_opt_byid
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        // Try to parse input as DNS record
        ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
        
        if (status == ARES_SUCCESS && dnsrec) {
            // Get first RR from answer section if available
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (rr_cnt > 0) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
                if (rr) {
                    // Try to get OPT record data if present
                    const unsigned char *opt_val = nullptr;
                    size_t opt_len = 0;
                    ares_dns_rr_key_t keys[] = {ARES_RR_OPT_OPTIONS};
                    size_t keys_cnt = 1;
                    
                    for (size_t i = 0; i < keys_cnt; i++) {
                        size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, keys[i]);
                        for (size_t j = 0; j < opt_cnt; j++) {
                            unsigned short opt_id;
                            const unsigned char *val;
                            size_t val_len;
                            opt_id = ares_dns_rr_get_opt(rr, keys[i], j, &val, &val_len);
                            
                            // Use ares_dns_rr_get_opt_byid
                            const unsigned char *byid_val;
                            size_t byid_len;
                            ares_bool_t found = ares_dns_rr_get_opt_byid(rr, keys[i], opt_id, &byid_val, &byid_len);
                            
                            // Try to delete an RR (simulate deletion)
                            // Note: We need mutable record for deletion, so create a copy
                            ares_dns_record_t *mutable_dnsrec = nullptr;
                            if (ares_dns_parse(data, size, 0, &mutable_dnsrec) == ARES_SUCCESS) {
                                if (rr_cnt > 0) {
                                    // Delete first RR from answer section
                                    // Note: ares_dns_record_rr_del is not in provided list, using alternative approach
                                    // We'll skip actual deletion since function not available
                                }
                                ares_dns_record_destroy(mutable_dnsrec);
                            }
                        }
                    }
                }
            }
        }
    }

    // 5. Use ares_inet_pton with input data
    if (size >= 4) {
        struct in_addr addr;
        char ip_str[INET_ADDRSTRLEN];
        
        // Create IP string from first 4 bytes
        snprintf(ip_str, sizeof(ip_str), "%u.%u.%u.%u", 
                 data[0], data[1], data[2], data[3]);
        
        int pton_result = ares_inet_pton(AF_INET, ip_str, &addr);
    }

    // 6. Use ares_dns_class_fromstr with input data
    if (size > 0) {
        ares_dns_class_t qclass;
        char class_str[16];
        
        // Create class string from input (limited to 15 chars)
        size_t str_len = size < 15 ? size : 15;
        memcpy(class_str, data, str_len);
        class_str[str_len] = '\0';
        
        ares_bool_t class_result = ares_dns_class_fromstr(&qclass, class_str);
    }

    // 7. Process pending write operations
    ares_process_pending_write(channel);

    // Cleanup
    if (host) {
        ares_free_hostent(host);
    }
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}