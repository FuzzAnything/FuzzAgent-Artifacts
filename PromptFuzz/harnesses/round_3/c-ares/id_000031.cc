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
//<ID> 543
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"ares_dns_record_query_set_name":[[289,25,289,58,0,0,4,0]]},"library_calls":["ares_library_init","ares_version","ares_init_options","ares_library_cleanup","ares_set_socket_callback","ares_dns_record_create","ares_destroy","ares_library_cleanup","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_get_bin","ares_dns_record_get_rcode","ares_parse_naptr_reply","ares_free_data","ares_search_dnsrec","ares_fds","ares_process","ares_cancel","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init_options","ares_set_socket_callback","ares_dns_record_create","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_get_bin","ares_dns_record_get_rcode","ares_parse_naptr_reply","ares_free_data","ares_search_dnsrec","ares_fds","ares_process","ares_cancel","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Dummy callback for socket creation
static ares_socket_t  id_000031_dummy_socket_create_callback(ares_socket_t sockfd, int type, void *data) {
    return sockfd;
}

// Dummy callback for DNS record search
static void  id_000031_dummy_dnsrec_callback(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Get c-ares version
    int version;
    const char *ver_str = ares_version(&version);

    // Create channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Set some basic options
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    
    options.timeout = 2000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 2;
    optmask |= ARES_OPT_TRIES;

    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set socket callback
    ares_set_socket_callback(channel,  id_000031_dummy_socket_create_callback, nullptr);

    // Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, fuzz_uint16_t_3, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // Set query name using data if possible
    if (size > 0) {
        // Use first part of data as query name (null-terminated if possible)
        size_t name_len = size < 100 ? size : 100;
        char *name = (char*)malloc(name_len + 1);
        if (name) {
            memcpy(name, data, name_len);
            name[name_len] = '\0';
            // Try to set query name (index 0)
            ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_4, name);
            free(name);
        }
    }

    // Add a resource record
    ares_dns_rr_t *rr = nullptr;
    if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, fuzz_str_5, 
                               ARES_REC_TYPE_A, ARES_CLASS_IN, fuzz_uint32_t_6) == ARES_SUCCESS) {
        // Try to get binary data from the RR
        size_t len = 0;
        const unsigned char *bin_data = ares_dns_rr_get_bin(rr, ARES_RR_A_ADDR, &len);
        // We could use bin_data and len here if needed
    }

    // Get RCODE from DNS record
    ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);

    // Parse input as NAPTR reply if we have enough data
    if (size >= 12) {  // Minimum size for a DNS header
        struct ares_naptr_reply *naptr_out = nullptr;
        int parse_result = ares_parse_naptr_reply(data, size, &naptr_out);
        
        if (parse_result == ARES_SUCCESS && naptr_out) {
            // Free the parsed NAPTR data
            ares_free_data(naptr_out);
        }
    }

    // Perform DNS search with the created record
    ares_search_dnsrec(channel, dnsrec,  id_000031_dummy_dnsrec_callback, nullptr);

    // Process any pending operations
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        ares_process(channel, &read_fds, &write_fds);
    }

    // Cancel any pending operations
    ares_cancel(channel);

    // Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}