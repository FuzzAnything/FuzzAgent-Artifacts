#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_nameser.h"
#include "ares_dns_record.h"

namespace harness_003 {


// Callback function for generating DNS responses
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Helper function to generate a DNS response based on fuzzer input
static unsigned char* generate_dns_response(FuzzedDataProvider& fdp, size_t* out_len) {
    // Generate various DNS response components from fuzzer input
    uint16_t id = fdp.ConsumeIntegral<uint16_t>();
    uint8_t opcode = fdp.ConsumeIntegral<uint8_t>() % 16;  // 4 bits max
    uint8_t rcode = fdp.ConsumeIntegral<uint8_t>() % 16;   // 4 bits max
    uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume number of questions, answers, authority, additional
    uint16_t qdcount = fdp.ConsumeIntegralInRange<uint16_t>(0, 10);
    uint16_t ancount = fdp.ConsumeIntegralInRange<uint16_t>(0, 20);
    uint16_t nscount = fdp.ConsumeIntegralInRange<uint16_t>(0, 10);
    uint16_t arcount = fdp.ConsumeIntegralInRange<uint16_t>(0, 10);
    
    // Calculate approximate buffer size needed
    size_t buffer_size = 12;  // DNS header size
    buffer_size += qdcount * 64;   // Approximate question size
    buffer_size += ancount * 128;  // Approximate answer size  
    buffer_size += nscount * 128;  // Approximate authority size
    buffer_size += arcount * 128;  // Approximate additional size
    
    // Limit buffer size to prevent excessive memory usage
    buffer_size = fdp.ConsumeIntegralInRange<size_t>(12, 4096);
    
    // Consume remaining bytes for DNS response data
    auto response_data = fdp.ConsumeBytes<unsigned char>(buffer_size);
    
    // Create a simple DNS response header if we have enough data
    if (response_data.size() >= 12) {
        // Set DNS header fields
        response_data[0] = id >> 8;
        response_data[1] = id & 0xFF;
        response_data[2] = (opcode << 3) | ((flags & 0x01) ? 0x04 : 0x00); // QR=1 (response)
        response_data[3] = rcode;
        response_data[4] = qdcount >> 8;
        response_data[5] = qdcount & 0xFF;
        response_data[6] = ancount >> 8;
        response_data[7] = ancount & 0xFF;
        response_data[8] = nscount >> 8;
        response_data[9] = nscount & 0xFF;
        response_data[10] = arcount >> 8;
        response_data[11] = arcount & 0xFF;
    }
    
    *out_len = response_data.size();
    unsigned char* result = (unsigned char*)malloc(*out_len);
    if (result) {
        memcpy(result, response_data.data(), *out_len);
    }
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic DNS response generation
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Initialize channel for DNS operations
    ares_channel_t* channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Generate multiple DNS responses with different structures
    for (int response_num = 0; response_num < 3 && fdp.remaining_bytes() > 16; response_num++) {
        size_t dns_response_len = 0;
        unsigned char* dns_response = generate_dns_response(fdp, &dns_response_len);
        
        if (!dns_response || dns_response_len < 12) {
            free(dns_response);
            continue;
        }

        // Test 1: New unified parser - ares_dns_parse
        ares_dns_record_t* dnsrec = NULL;
        ares_status_t parse_status = ares_dns_parse(dns_response, dns_response_len, 0, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec != NULL) {
            // Exercise various DNS record accessors
            unsigned short record_id = ares_dns_record_get_id(dnsrec);
            unsigned short flags = ares_dns_record_get_flags(dnsrec);
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
            ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
            
            size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
            size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            size_t authority_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
            size_t additional_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
            
            // Try to get some query details if available
            for (size_t i = 0; i < query_cnt && i < 5; i++) {
                const char* name = NULL;
                ares_dns_rec_type_t qtype;
                ares_dns_class_t qclass;
                if (ares_dns_record_query_get(dnsrec, i, &name, &qtype, &qclass) == ARES_SUCCESS) {
                    // Just access the values, don't use them
                    (void)name;
                    (void)qtype;
                    (void)qclass;
                }
            }
            
            ares_dns_record_destroy(dnsrec);
        }

        // Test 2: Legacy parsers with the generated DNS response
        // Use different variations of the same response data
        
        // Declare variables for each parser type
        struct hostent* host = NULL;
        struct ares_addrttl addrttls[5];
        struct ares_addr6ttl addr6ttls[5];
        int addr_count = 5;
        
        // For non-hostent parsers
        struct ares_srv_reply* srv = NULL;
        struct ares_mx_reply* mx = NULL;
        struct ares_txt_reply* txt = NULL;
        struct ares_txt_ext* txt_ext = NULL;
        struct ares_soa_reply* soa = NULL;
        struct ares_naptr_reply* naptr = NULL;
        struct ares_caa_reply* caa = NULL;
        struct ares_uri_reply* uri = NULL;
        
        // Consume some parameters from fuzzer input for legacy parsers
        unsigned char ipv4_addr[4];
        if (fdp.remaining_bytes() >= 4) {
            auto addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
            memcpy(ipv4_addr, addr_bytes.data(), 4);
        } else {
            ipv4_addr[0] = 192; ipv4_addr[1] = 168; ipv4_addr[2] = 1; ipv4_addr[3] = 1;
        }
        
        // Test each legacy parser with possible variations
        // Instead of just one parser per iteration, test multiple based on fuzzer input
        uint8_t parsers_to_test = fdp.ConsumeIntegral<uint8_t>();
        
        // Test A record parser (if bit 0 is set or random choice)
        if ((parsers_to_test & 0x01) || fdp.ConsumeBool()) {
            ares_parse_a_reply(dns_response, dns_response_len, &host, addrttls, &addr_count);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
        }
        
        // Test AAAA record parser (if bit 1 is set or random choice)
        if ((parsers_to_test & 0x02) || fdp.ConsumeBool()) {
            addr_count = 5;
            ares_parse_aaaa_reply(dns_response, dns_response_len, &host, addr6ttls, &addr_count);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
        }
        
        // Test CAA record parser (if bit 2 is set or random choice)
        if ((parsers_to_test & 0x04) || fdp.ConsumeBool()) {
            ares_parse_caa_reply(dns_response, dns_response_len, &caa);
            if (caa) {
                ares_free_data(caa);
                caa = NULL;
            }
        }
        
        // Test PTR record parser (if bit 3 is set or random choice)
        if ((parsers_to_test & 0x08) || fdp.ConsumeBool()) {
            ares_parse_ptr_reply(dns_response, dns_response_len, ipv4_addr, 4, AF_INET, &host);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
        }
        
        // Test NS record parser (if bit 4 is set or random choice)
        if ((parsers_to_test & 0x10) || fdp.ConsumeBool()) {
            ares_parse_ns_reply(dns_response, dns_response_len, &host);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
        }
        
        // Test SRV record parser (if bit 5 is set or random choice)
        if ((parsers_to_test & 0x20) || fdp.ConsumeBool()) {
            ares_parse_srv_reply(dns_response, dns_response_len, &srv);
            if (srv) {
                ares_free_data(srv);
                srv = NULL;
            }
        }
        
        // Test MX record parser (if bit 6 is set or random choice)
        if ((parsers_to_test & 0x40) || fdp.ConsumeBool()) {
            ares_parse_mx_reply(dns_response, dns_response_len, &mx);
            if (mx) {
                ares_free_data(mx);
                mx = NULL;
            }
        }
        
        // Test TXT record parser (if bit 7 is set or random choice)
        if ((parsers_to_test & 0x80) || fdp.ConsumeBool()) {
            ares_parse_txt_reply(dns_response, dns_response_len, &txt);
            if (txt) {
                ares_free_data(txt);
                txt = NULL;
            }
        }
        
        // Test TXT reply ext parser (random choice if enough bytes)
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            ares_parse_txt_reply_ext(dns_response, dns_response_len, &txt_ext);
            if (txt_ext) {
                ares_free_data(txt_ext);
                txt_ext = NULL;
            }
        }
        
        // Test NAPTR record parser (random choice if enough bytes)
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            ares_parse_naptr_reply(dns_response, dns_response_len, &naptr);
            if (naptr) {
                ares_free_data(naptr);
                naptr = NULL;
            }
        }
        
        // Test SOA record parser (random choice if enough bytes)
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            ares_parse_soa_reply(dns_response, dns_response_len, &soa);
            if (soa) {
                ares_free_data(soa);
                soa = NULL;
            }
        }
        
        // Test URI record parser (random choice if enough bytes)
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            ares_parse_uri_reply(dns_response, dns_response_len, &uri);
            if (uri) {
                ares_free_data(uri);
                uri = NULL;
            }
        }
        
        // Test 3: Generate a query and send it through the channel
        // to exercise the full request-response cycle
        if (fdp.remaining_bytes() > 10) {
            std::string query_name = fdp.ConsumeRandomLengthString(253);
            uint16_t query_type = fdp.ConsumeIntegral<uint16_t>() % 65535;
            uint16_t query_class = fdp.ConsumeIntegral<uint16_t>() % 4;
            
            ares_query(channel, query_name.c_str(), query_class, query_type, query_callback, NULL);
        }
        
        free(dns_response);
        
        // Process any pending events (non-blocking)
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_003
