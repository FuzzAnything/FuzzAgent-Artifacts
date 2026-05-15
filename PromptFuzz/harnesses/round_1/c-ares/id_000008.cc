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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"ares_parse_aaaa_reply":[[90,7,90,23,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_parse_uri_reply","ares_parse_aaaa_reply","ares_dns_record_create","ares_dns_record_get_flags","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_get_type","ares_dns_rr_set_bin","ares_queue_wait_empty","ares_dns_record_destroy","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_uri_reply","ares_parse_aaaa_reply","ares_dns_record_create","ares_dns_record_get_flags","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_get_type","ares_dns_rr_set_bin","ares_queue_wait_empty","ares_dns_record_destroy","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to process DNS response data:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create ares channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse the input as URI reply
    struct ares_uri_reply *uri_out = nullptr;
    if (size > 0) {
        ares_parse_uri_reply(data, static_cast<int>(size), &uri_out);
    }

    // Parse the input as AAAA reply
    struct hostent *host = nullptr;
    struct ares_addr6ttl *addrttls = nullptr;
    int naddrttls = 0;
    if (size > 0) {
        ares_parse_aaaa_reply(data, static_cast<int>(size), &host, addrttls, &naddrttls);
    }

    // Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, fuzz_uint16_t_2, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Get flags from the DNS record
        unsigned short flags = ares_dns_record_get_flags(dnsrec);

        // Add a query to the DNS record
        const char *query_name = "example.com";
        ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_4, fuzz_str_3);

        // Add a resource record to the DNS record (ANSWER section)
        ares_dns_rr_t *rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                  fuzz_str_5, ARES_REC_TYPE_AAAA,
                                  ARES_CLASS_IN, fuzz_uint32_t_6) == ARES_SUCCESS) {
            // Get the type of the resource record
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);

            // Set binary data in the resource record
            // We'll use the input data as binary value for AAAA record
            if (size > 0) {
                // For AAAA record, we need 16 bytes for IPv6 address
                // Use as much data as we have, up to 16 bytes
                size_t bin_len = (size < 16) ? size : 16;
                ares_dns_rr_set_bin(rr, ARES_RR_AAAA_ADDR, data, bin_len);
            }
        }

        // Wait for any pending operations in the channel to complete
        ares_queue_wait_empty(channel, fuzz_int32_t_7); // 1 second timeout

        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up parsed replies
    if (uri_out) {
        // Free URI reply chain
        struct ares_uri_reply *current = uri_out;
        while (current) {
            struct ares_uri_reply *next = current->next;
            if (current->uri) {
                free(current->uri);
            }
            free(current);
            current = next;
        }
    }

    if (host) {
        ares_free_hostent(host);
    }

    // Clean up channel
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}