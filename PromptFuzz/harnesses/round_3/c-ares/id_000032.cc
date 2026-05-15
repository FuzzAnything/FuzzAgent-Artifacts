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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"ares_process_fds_nolock":[[209,9,209,40,0,0,4,0],[223,9,223,40,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_dup","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_ttl","ares_set_servers","ares_process_fds","ares_save_options","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_destroy_options","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dup","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_ttl","ares_set_servers","ares_process_fds","ares_save_options","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_destroy_options","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    ares_channel_t *channel_dup = nullptr;
    ares_dns_record_t *dnsrec = nullptr;
    struct ares_options options;
    int optmask = 0;
    struct ares_addr_node *servers = nullptr;
    ares_fd_events_t events[1];
    
    // Step 1: Create initial channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        goto cleanup;
    }

    // Step 2: Duplicate the channel
    if (ares_dup(&channel_dup, channel) != ARES_SUCCESS) {
        goto cleanup;
    }

    // Step 3: Parse DNS data from input
    if (size > 0) {
        ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
        if (status == ARES_SUCCESS && dnsrec != nullptr) {
            // Step 4: Get and modify RRs in the parsed DNS record
            size_t answer_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            
            for (size_t i = 0; i < answer_count; i++) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
                if (rr) {
                    // Get current class and TTL
                    ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                    unsigned int current_ttl = ares_dns_rr_get_ttl(rr);
                    
                    // Modify TTL (increase by 1)
                    // Note: We need a non-const RR to modify, but we only have const
                    // In real usage, we would need to extract and recreate RRs
                    // For fuzzing, we'll just demonstrate the API calls
                }
            }
        }
    }

    // Step 5: Configure servers (dummy configuration)
    servers = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
    if (servers) {
        servers->next = nullptr;
        servers->family = AF_INET;
        servers->addr.addr4.s_addr = htonl(0x08080808); // 8.8.8.8
        
        if (ares_set_servers(channel, servers) != ARES_SUCCESS) {
            // Continue anyway for fuzzing
        }
    }

    // Step 6: Process file descriptors (dummy processing)
    events[0].fd = -1;  // Invalid fd for fuzzing
    events[0].events = 0;
    ares_process_fds(channel, events, 1, fuzz_uint32_t_3);

    // Step 7: Save channel options
    if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
        // Options saved successfully
    }

cleanup:
    // Clean up DNS record
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up servers list
    if (servers) {
        free(servers);
    }

    // Destroy channels
    if (channel_dup) {
        ares_destroy(channel_dup);
    }
    if (channel) {
        ares_destroy(channel);
    }

    // Clean up options if they were allocated
    if (optmask != 0) {
        ares_destroy_options(&options);
    }

    // Library cleanup
    ares_library_cleanup();

    return 0;
}