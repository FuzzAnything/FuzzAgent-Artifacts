#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_000 {
/*
 * Fuzzing harness for c-ares DNS library
 * Targets core initialization, DNS query creation, and DNS parsing APIs
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Create a channel with random options
    ares_channel_t *channel = NULL;
    int options_mask = fdp.ConsumeIntegral<int>() & (ARES_OPT_FLAGS | ARES_OPT_SOCK_STATE_CB | ARES_OPT_TIMEOUTMS);
    
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    if (options_mask & ARES_OPT_FLAGS) {
        options.flags = fdp.ConsumeIntegral<int>();
    }
    if (options_mask & ARES_OPT_TIMEOUTMS) {
        options.timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
    }
    
    int optmask = options_mask;
    ares_init_options(&channel, &options, optmask);

    if (channel == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Test DNS query creation with random parameters
    if (fdp.remaining_bytes() > 10) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string query_name = fdp.ConsumeBytesAsString(name_len);
        
        // Ensure null termination
        if (!query_name.empty()) {
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            // Use random DNS class and type
            int dns_class = fdp.ConsumeIntegralInRange<int>(1, 4);  // C_IN, C_CHAOS, etc
            int dns_type = fdp.ConsumeIntegralInRange<int>(1, 16);  // Common DNS types
            int query_id = fdp.ConsumeIntegralInRange<int>(1, 65535);
            
            // Legacy query creation
            ares_create_query(query_name.c_str(), dns_class, dns_type, query_id, 0, 
                             &query_buf, &query_buflen, 1024);
            
            if (query_buf) {
                free(query_buf);
            }
            
            // Test modern DNS record creation
            ares_dns_record_t *dnsrec = NULL;
            ares_status_t status = ares_dns_record_create(&dnsrec, query_id, 
                                                         ARES_FLAG_RD,
                                                         ARES_OPCODE_QUERY,
                                                         ARES_RCODE_NOERROR);
            
            if (status == ARES_SUCCESS && dnsrec) {
                // Add a question to the DNS record
                ares_dns_record_query_add(dnsrec, query_name.c_str(), 
                                         ARES_REC_TYPE_A, ARES_CLASS_IN);
                
                // Free the DNS record
                ares_dns_record_destroy(dnsrec);
            }
        }
    }

    // Test DNS parsing with remaining input
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> dns_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (dns_data.size() > 0 && dns_data.size() <= 65535) {
            ares_dns_record_t *parsed_rec = NULL;
            
            // Try parsing as DNS response
            ares_dns_parse(dns_data.data(), dns_data.size(), 0, &parsed_rec);
            
            if (parsed_rec) {
                // Exercise some getter functions
                ares_dns_record_get_id(parsed_rec);
                ares_dns_record_get_opcode(parsed_rec);
                ares_dns_record_get_rcode(parsed_rec);
                ares_dns_record_get_flags(parsed_rec);
                
                // Get query count
                size_t query_cnt = ares_dns_record_query_cnt(parsed_rec);
                
                if (query_cnt > 0) {
                    const char *name;
                    ares_dns_rec_type_t qtype;
                    ares_dns_class_t qclass;
                    
                    // Get first query
                    ares_dns_record_query_get(parsed_rec, 0, &name, &qtype, &qclass);
                }
                
                // Get RR counts for different sections
                ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_ANSWER);
                ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_AUTHORITY);
                ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_ADDITIONAL);
                
                ares_dns_record_destroy(parsed_rec);
            }
            
            // Also test legacy parsers with a subset of data
            if (dns_data.size() >= 12) {  // Minimum DNS header size
                struct hostent *host = NULL;
                struct ares_addrttl info[5];
                struct ares_addr6ttl info6[5];
                int count = 5;
                unsigned char addrv4[4] = {0x10, 0x20, 0x30, 0x40};
                
                // Test a few legacy parsers
                ares_parse_a_reply(dns_data.data(), dns_data.size(), &host, info, &count);
                if (host) {
                    ares_free_hostent(host);
                    host = NULL;
                }
                
                count = 5;
                ares_parse_aaaa_reply(dns_data.data(), dns_data.size(), &host, info6, &count);
                if (host) {
                    ares_free_hostent(host);
                }
            }
        }
    }

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_000
