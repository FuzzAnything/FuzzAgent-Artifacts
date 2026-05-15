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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_ns_reply","ares_dns_record_create","ares_dns_record_get_flags","ares_dns_record_rr_add","ares_dns_rr_get_keys","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_get_ttl","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process","ares_dns_section_tostr","ares_dns_record_destroy","ares_free_hostent","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_ns_reply","ares_dns_record_create","ares_dns_record_get_flags","ares_dns_record_rr_add","ares_dns_rr_get_keys","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_get_ttl","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process","ares_dns_section_tostr","ares_dns_record_destroy","ares_free_hostent","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // 2. Create channel for DNS operations
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Parse the input as DNS NS reply
    struct hostent *host = nullptr;
    if (size > 0) {
        // Use the input data as DNS response buffer
        ares_parse_ns_reply(data, static_cast<int>(size), &host);
    }

    // 4. Create a new DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_record_create(&dnsrec, 
                           static_cast<unsigned short>(data[0] & 0xFF),  // ID from first byte
                           ARES_FLAG_RD,  // Standard query flag
                           ARES_OPCODE_QUERY,
                           ARES_RCODE_NOERROR);

    if (dnsrec) {
        // 5. Get current flags
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        
        // 6. Add a resource record to the DNS record
        ares_dns_rr_t *rr_out = nullptr;
        if (size > 10) {
            // Extract name from data if possible
            char name[256];
            size_t name_len = (size - 10) > 255 ? 255 : size - 10;
            memcpy(name, data + 10, name_len);
            name[name_len] = '\0';
            
            ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ANSWER,
                                   name, ARES_REC_TYPE_NS, 
                                   ARES_CLASS_IN, fuzz_uint32_t_2);
        }

        // 7. Get keys for NS record type
        size_t key_count = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_NS, &key_count);

        // 8. Access and modify RR if available
        if (rr_out) {
            // Get RR from different sections
            const ares_dns_rr_t *const_rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
            ares_dns_rr_t *mutable_rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
            
            if (mutable_rr) {
                // Get and set TTL
                unsigned int current_ttl = ares_dns_rr_get_ttl(mutable_rr);
                ares_dns_rr_set_ttl(mutable_rr, current_ttl + 100);
            }
        }

        // 9. Send DNS query using the created record
        unsigned short qid = 0;
        if (size > 20) {
            char query_name[256];
            size_t query_len = (size - 20) > 255 ? 255 : size - 20;
            memcpy(query_name, data + 20, query_len);
            query_name[query_len] = '\0';
            
            ares_query_dnsrec(channel, query_name, ARES_CLASS_IN, 
                             ARES_REC_TYPE_NS, nullptr, nullptr, &qid);
        }

        // 10. Process DNS queries
        fd_set read_fds, write_fds;
        struct timeval timeout;
        
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;  // 1ms timeout
        
        ares_process(channel, &read_fds, &write_fds);
        
        // 11. Get section string for logging/debugging
        const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);

        // 12. Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // 13. Free hostent if allocated
    if (host) {
        ares_free_hostent(host);
    }

    // 14. Cancel any pending operations
    ares_cancel(channel);

    // 15. Clean up channel
    ares_destroy(channel);

    // 16. Final library cleanup
    ares_library_cleanup();

    return 0;
}