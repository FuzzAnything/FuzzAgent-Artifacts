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
//<ID> 772
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"ares_dns_rec_type_tostr":[[216,5,216,27,0,0,4,0]],"ares_dns_rr_key_datatype":[[453,11,453,14,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_ip6","ares_set_pending_write_cb","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_dns_record_rr_get_const","ares_dns_rr_add_abin","ares_dns_opt_get_name","ares_search_dnsrec","ares_getsock","ares_process_fd","ares_process_pending_write","ares_queue_wait_empty","ares_dns_record_destroy","ares_parse_mx_reply","ares_free_data","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip6","ares_set_pending_write_cb","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_dns_record_rr_get_const","ares_dns_rr_add_abin","ares_dns_opt_get_name","ares_search_dnsrec","ares_getsock","ares_process_fd","ares_process_pending_write","ares_queue_wait_empty","ares_dns_record_destroy","ares_parse_mx_reply","ares_free_data","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize the c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));

    // Initialize channel with options
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IPv6 address
    unsigned char local_ip6[16] = {0};
    ares_set_local_ip6(channel, local_ip6);

    // Set pending write callback
    ares_set_pending_write_cb(channel, nullptr, nullptr);

    // Parse the input data as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, 0, &dnsrec);

    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get query count from DNS record
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // Get first query if available
        if (query_cnt > 0) {
            const char *name = nullptr;
            ares_dns_rec_type_t qtype;
            ares_dns_class_t qclass;
            ares_dns_record_query_get(dnsrec, 0, &name, &qtype, &qclass);
        }

        // Try to add binary data to first RR if possible
        const ares_dns_rr_t *rr_const = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
        if (rr_const != nullptr) {
            // Cast to non-const for modification (this is safe in fuzzing context)
            ares_dns_rr_t *rr = (ares_dns_rr_t *)rr_const;
            unsigned char dummy_data[] = {0x01, 0x02, 0x03};
            ares_dns_rr_add_abin(rr, (ares_dns_rr_key_t)0, dummy_data, sizeof(dummy_data));
        }

        // Get option name for demonstration
        const char *opt_name = ares_dns_opt_get_name((ares_dns_rr_key_t)0, 0);

        // Define callback function for search
        auto search_callback = [](void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
            // Callback implementation - do nothing in fuzzing context
        };

        // Perform DNS search with the parsed record
        ares_search_dnsrec(channel, dnsrec, search_callback, nullptr);

        // Process the channel
        ares_socket_t socks[ARES_GETSOCK_MAXNUM];
        int num_socks = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);
        
        for (int i = 0; i < num_socks; i++) {
            ares_process_fd(channel, socks[i], ARES_SOCKET_BAD);
        }

        // Process pending writes
        ares_process_pending_write(channel);

        // Wait for queue to empty
        ares_queue_wait_empty(channel, 100);

        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Also try to parse the input as an MX reply
    struct ares_mx_reply *mx_out = nullptr;
    int mx_parsed = ares_parse_mx_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &mx_out);
    
    if (mx_parsed == ARES_SUCCESS && mx_out != nullptr) {
        // Free the MX reply data
        ares_free_data(mx_out);
    }

    // Cancel any ongoing operations
    ares_cancel(channel);

    // Clean up resources
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}