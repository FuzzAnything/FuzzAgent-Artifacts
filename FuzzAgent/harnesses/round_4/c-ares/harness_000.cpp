#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_000 {
/* Fuzzing harness for c-ares library targeting main API entry points
 * This harness tests fundamental c-ares APIs with fuzzer-generated input
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Test 1: Channel initialization with options
    ares_channel_t* channel = NULL;
    ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Consume fuzzer data for initialization parameters
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    int ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    bool tcp = fdp.ConsumeBool();
    
    options.timeout = timeout;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = tries;
    optmask |= ARES_OPT_TRIES;
    
    options.ndots = ndots;
    optmask |= ARES_OPT_NDOTS;
    
    if (tcp) {
        options.flags |= ARES_FLAG_USEVC;
        optmask |= ARES_OPT_FLAGS;
    }
    
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Test 2: DNS parsing with fuzzer data
    if (fdp.remaining_bytes() > 12) {
        size_t parse_size = fdp.ConsumeIntegralInRange<size_t>(12, fdp.remaining_bytes());
        std::vector<uint8_t> parse_data = fdp.ConsumeBytes<uint8_t>(parse_size);
        
        ares_dns_record_t* dnsrec = NULL;
        unsigned int parse_flags = fdp.ConsumeIntegral<unsigned int>();
        
        // Try parsing the DNS data
        ares_dns_parse(parse_data.data(), parse_data.size(), parse_flags, &dnsrec);
        
        if (dnsrec != NULL) {
            // If parsing succeeded, test writing it back
            unsigned char* out_buf = NULL;
            size_t out_len = 0;
            ares_dns_write(dnsrec, &out_buf, &out_len);
            if (out_buf != NULL) {
                ares_free_string(out_buf);
            }
            
            // Test getting various properties
            unsigned short id = ares_dns_record_get_id(dnsrec);
            unsigned short flags = ares_dns_record_get_flags(dnsrec);
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
            ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
            
            // Test getting query count
            size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
            if (query_cnt > 0 && query_cnt < 100) {
                for (size_t i = 0; i < query_cnt && i < 5; i++) {
                    const char* qname = NULL;
                    ares_dns_rec_type_t qtype;
                    ares_dns_class_t qclass;
                    ares_dns_record_query_get(dnsrec, i, &qname, &qtype, &qclass);
                }
            }
            
            // Test getting RR counts for different sections
            for (int section = ARES_SECTION_ANSWER; section <= ARES_SECTION_ADDITIONAL; section++) {
                size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)section);
                if (rr_cnt > 0 && rr_cnt < 100) {
                    for (size_t i = 0; i < rr_cnt && i < 3; i++) {
                        const ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, (ares_dns_section_t)section, i);
                        if (rr != NULL) {
                            // Test getting RR properties
                            const char* rr_name = ares_dns_rr_get_name(rr);
                            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                            unsigned int rr_ttl = ares_dns_rr_get_ttl(rr);
                        }
                    }
                }
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Test 3: Create query with fuzzer-generated name
    if (fdp.remaining_bytes() > 1) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::string query_name = fdp.ConsumeBytesAsString(name_len);
        
        // Ensure null termination
        query_name.push_back('\0');
        
        unsigned char* query_buf = NULL;
        int query_buflen = 0;
        
        // Try different query types based on fuzzer data
        unsigned short qtype = fdp.ConsumeIntegralInRange<unsigned short>(1, 100);
        unsigned short qclass = fdp.ConsumeIntegralInRange<unsigned short>(1, 4);
        unsigned short id = fdp.ConsumeIntegral<unsigned short>();
        
        // Use deprecated API for backward compatibility testing
        ares_create_query(query_name.c_str(), qclass, qtype, id, 0, &query_buf, &query_buflen, 1024);
        
        if (query_buf != NULL) {
            free(query_buf);
        }
    }

    // Test 4: Legacy parsing functions (deprecated but still useful for coverage)
    if (fdp.remaining_bytes() > 20) {
        size_t legacy_size = fdp.ConsumeIntegralInRange<size_t>(20, fdp.remaining_bytes());
        std::vector<uint8_t> legacy_data = fdp.ConsumeBytes<uint8_t>(legacy_size);
        
        // Test a subset of legacy parsers
        struct hostent* host = NULL;
        struct ares_addrttl info[3];
        int count = 3;
        
        ares_parse_a_reply(legacy_data.data(), legacy_data.size(), &host, info, &count);
        if (host != NULL) {
            ares_free_hostent(host);
        }
        
        // Try another parser
        struct ares_txt_reply* txt = NULL;
        ares_parse_txt_reply(legacy_data.data(), legacy_data.size(), &txt);
        if (txt != NULL) {
            ares_free_data(txt);
        }
    }

    // Test 5: Server configuration with CSV
    if (fdp.remaining_bytes() > 1) {
        size_t csv_len = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::string csv_data = fdp.ConsumeBytesAsString(csv_len);
        csv_data.push_back('\0');
        
        ares_set_servers_csv(channel, csv_data.c_str());
    }

    // Test 6: Get host by name (async API setup - we can't complete async ops in fuzzer)
    if (fdp.remaining_bytes() > 1 && channel != NULL) {
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::string hostname = fdp.ConsumeBytesAsString(hostname_len);
        hostname.push_back('\0');
        
        // We can't properly test async APIs in fuzzer context, but we can
        // at least call the setup function
        ares_gethostbyname(channel, hostname.c_str(), AF_INET, NULL, NULL);
    }

    // Cleanup
    if (channel != NULL) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_000
