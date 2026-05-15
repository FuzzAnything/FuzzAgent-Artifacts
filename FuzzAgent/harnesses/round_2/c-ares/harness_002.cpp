#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_002 {
/*
 * Fuzzing harness for c-ares DNS library - DNS wire format parsing and low-level operations
 * Targets: ares_expand_name, ares_expand_string, DNS query/response functions, 
 *          ares_dns_record serialization/deserialization, and DNS parsing utilities
 */



// Simple callback for async operations
static void dns_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void dnsrec_callback(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Test DNS wire format expansion functions
    if (fdp.remaining_bytes() > 20) {
        // Create a buffer for expansion tests
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 512);
        std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
        
        if (buffer.size() >= 12) {  // Minimum for DNS header
            // Test ares_expand_name
            const unsigned char *encoded = buffer.data();
            const unsigned char *abuf = buffer.data();
            int alen = static_cast<int>(buffer.size());
            char *expanded_name = NULL;
            long enclen = 0;
            
            // Try expanding from different positions
            for (int offset = 0; offset < alen - 5; offset += 10) {
                expanded_name = NULL;
                enclen = 0;
                ares_expand_name(encoded + offset, abuf, alen, &expanded_name, &enclen);
                if (expanded_name) {
                    free(expanded_name);
                    expanded_name = NULL;
                }
            }
            
            // Test ares_expand_string
            unsigned char *expanded_string = NULL;
            enclen = 0;
            
            // Try from different positions
            for (int offset = 0; offset < alen - 2; offset += 5) {
                expanded_string = NULL;
                enclen = 0;
                ares_expand_string(encoded + offset, abuf, alen, &expanded_string, &enclen);
                if (expanded_string) {
                    free(expanded_string);
                    expanded_string = NULL;
                }
            }
        }
    }

    // Test DNS query creation with random parameters
    if (fdp.remaining_bytes() > 10) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string query_name = fdp.ConsumeBytesAsString(name_len);
        
        if (!query_name.empty() && fdp.remaining_bytes() > 8) {
            // Test ares_create_query with random parameters
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            int dnsclass = fdp.ConsumeIntegralInRange<int>(1, 65535);
            int type = fdp.ConsumeIntegralInRange<int>(1, 65535);
            unsigned short id = fdp.ConsumeIntegral<unsigned short>();
            int rd = fdp.ConsumeBool() ? 1 : 0;
            int max_udp_size = fdp.ConsumeIntegralInRange<int>(512, 4096);
            
            ares_create_query(query_name.c_str(), dnsclass, type, id, rd, 
                             &query_buf, &query_buflen, max_udp_size);
            
            if (query_buf) {
                free(query_buf);
                query_buf = NULL;
            }
            
            // Test ares_mkquery (legacy)
            query_buflen = 0;
            ares_mkquery(query_name.c_str(), dnsclass, type, id, rd, 
                        &query_buf, &query_buflen);
            
            if (query_buf) {
                free(query_buf);
                query_buf = NULL;
            }
        }
    }

    // Test DNS record creation and manipulation
    if (fdp.remaining_bytes() > 16) {
        // Create multiple DNS records with different parameters
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
            unsigned short id = fdp.ConsumeIntegral<unsigned short>();
            unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
            
            // Random opcode
            ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
                fdp.ConsumeIntegralInRange<int>(0, 5));
            
            // Random rcode  
            ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
                fdp.ConsumeIntegralInRange<int>(0, 16));
            
            ares_dns_record_t *dnsrec = NULL;
            ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
            
            if (status == ARES_SUCCESS && dnsrec) {
                // Add random queries
                if (fdp.remaining_bytes() > 10) {
                    size_t qname_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    std::string qname = fdp.ConsumeBytesAsString(qname_len);
                    
                    if (!qname.empty()) {
                        // Random query type and class
                        ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
                            fdp.ConsumeIntegralInRange<int>(1, 100));
                        ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
                            fdp.ConsumeIntegralInRange<int>(1, 255));
                        
                        ares_dns_record_query_add(dnsrec, qname.c_str(), qtype, qclass);
                        
                        // Test query getter
                        const char *ret_name;
                        ares_dns_rec_type_t ret_type;
                        ares_dns_class_t ret_class;
                        
                        if (ares_dns_record_query_get(dnsrec, 0, &ret_name, &ret_type, &ret_class) == ARES_SUCCESS) {
                            // Got query info
                        }
                    }
                }
                
                // Test record getters
                ares_dns_record_get_id(dnsrec);
                ares_dns_record_get_flags(dnsrec);
                ares_dns_record_get_opcode(dnsrec);
                ares_dns_record_get_rcode(dnsrec);
                ares_dns_record_query_cnt(dnsrec);
                
                // Test section counts
                ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
                ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
                ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
                
                // Clean up
                ares_dns_record_destroy(dnsrec);
            }
        }
    }

    // Test DNS parsing with random data
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> dns_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (dns_data.size() > 0 && dns_data.size() <= 65535) {
            // Test modern parser
            ares_dns_record_t *parsed_rec = NULL;
            ares_dns_parse(dns_data.data(), dns_data.size(), 0, &parsed_rec);
            
            if (parsed_rec) {
                // Exercise various getters on parsed record
                ares_dns_record_get_id(parsed_rec);
                ares_dns_record_get_opcode(parsed_rec);
                ares_dns_record_get_rcode(parsed_rec);
                ares_dns_record_get_flags(parsed_rec);
                
                size_t query_cnt = ares_dns_record_query_cnt(parsed_rec);
                for (size_t i = 0; i < query_cnt && i < 5; i++) {
                    const char *name;
                    ares_dns_rec_type_t qtype;
                    ares_dns_class_t qclass;
                    ares_dns_record_query_get(parsed_rec, i, &name, &qtype, &qclass);
                }
                
                // Test all section counts
                for (int section = ARES_SECTION_ANSWER; section <= ARES_SECTION_ADDITIONAL; section++) {
                    ares_dns_record_rr_cnt(parsed_rec, static_cast<ares_dns_section_t>(section));
                }
                
                ares_dns_record_destroy(parsed_rec);
            }
            
            // Test legacy parsers with the same data
            if (dns_data.size() >= 12) {
                struct hostent *host = NULL;
                struct ares_addrttl info[5];
                struct ares_addr6ttl info6[5];
                int count = 5;
                
                // Test various legacy reply parsers
                ares_parse_a_reply(dns_data.data(), dns_data.size(), &host, info, &count);
                if (host) {
                    ares_free_hostent(host);
                    host = NULL;
                }
                
                count = 5;
                ares_parse_aaaa_reply(dns_data.data(), dns_data.size(), &host, info6, &count);
                if (host) {
                    ares_free_hostent(host);
                    host = NULL;
                }
                
                unsigned char addrv4[4] = {0x10, 0x20, 0x30, 0x40};
                ares_parse_ptr_reply(dns_data.data(), dns_data.size(), addrv4, sizeof(addrv4), AF_INET, &host);
                if (host) {
                    ares_free_hostent(host);
                    host = NULL;
                }
                
                ares_parse_ns_reply(dns_data.data(), dns_data.size(), &host);
                if (host) {
                    ares_free_hostent(host);
                    host = NULL;
                }
                
                struct ares_srv_reply *srv = NULL;
                ares_parse_srv_reply(dns_data.data(), dns_data.size(), &srv);
                if (srv) {
                    ares_free_data(srv);
                }
                
                struct ares_mx_reply *mx = NULL;
                ares_parse_mx_reply(dns_data.data(), dns_data.size(), &mx);
                if (mx) {
                    ares_free_data(mx);
                }
                
                struct ares_txt_reply *txt = NULL;
                ares_parse_txt_reply(dns_data.data(), dns_data.size(), &txt);
                if (txt) {
                    ares_free_data(txt);
                }
                
                struct ares_soa_reply *soa = NULL;
                ares_parse_soa_reply(dns_data.data(), dns_data.size(), &soa);
                if (soa) {
                    ares_free_data(soa);
                }
                
                struct ares_caa_reply *caa = NULL;
                ares_parse_caa_reply(dns_data.data(), dns_data.size(), &caa);
                if (caa) {
                    ares_free_data(caa);
                }
                
                struct ares_uri_reply *uri = NULL;
                ares_parse_uri_reply(dns_data.data(), dns_data.size(), &uri);
                if (uri) {
                    ares_free_data(uri);
                }
            }
        }
    }

    // Test DNS query/response APIs with a channel
    if (fdp.remaining_bytes() > 20) {
        // Create a channel for testing query APIs
        ares_channel_t *channel = NULL;
        struct ares_options options;
        memset(&options, 0, sizeof(options));
        options.flags = ARES_FLAG_STAYOPEN;
        options.timeout = 100;
        
        int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS;
        if (ares_init_options(&channel, &options, optmask) == ARES_SUCCESS && channel) {
            // Test ares_query with random parameters
            if (fdp.remaining_bytes() > 10) {
                size_t qname_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                std::string qname = fdp.ConsumeBytesAsString(qname_len);
                
                if (!qname.empty()) {
                    int dnsclass = fdp.ConsumeIntegralInRange<int>(1, 4);
                    int type = fdp.ConsumeIntegralInRange<int>(1, 100);
                    
                    // Async query
                    ares_query(channel, qname.c_str(), dnsclass, type, dns_callback, NULL);
                }
            }
            
            // Test ares_search (deprecated)
            if (fdp.remaining_bytes() > 10) {
                size_t sname_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                std::string sname = fdp.ConsumeBytesAsString(sname_len);
                
                if (!sname.empty()) {
                    int dnsclass = fdp.ConsumeIntegralInRange<int>(1, 4);
                    int type = fdp.ConsumeIntegralInRange<int>(1, 100);
                    
                    ares_search(channel, sname.c_str(), dnsclass, type, dns_callback, NULL);
                }
            }
            
            // Test ares_send with random DNS data
            if (fdp.remaining_bytes() > 12) {
                size_t send_len = fdp.ConsumeIntegralInRange<size_t>(12, 512);
                std::vector<uint8_t> send_data = fdp.ConsumeBytes<uint8_t>(send_len);
                
                if (send_data.size() >= 12) {
                    // Create a simple DNS record to send
                    ares_dns_record_t *send_rec = NULL;
                    ares_status_t status = ares_dns_record_create(&send_rec, 
                        fdp.ConsumeIntegral<unsigned short>(),
                        ARES_FLAG_RD,
                        ARES_OPCODE_QUERY,
                        ARES_RCODE_NOERROR);
                    
                    if (status == ARES_SUCCESS && send_rec) {
                        // Add a query
                        std::string qname = "example.com";
                        ares_dns_record_query_add(send_rec, qname.c_str(), 
                                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
                        // Send the DNS record
                        unsigned short qid;
                        ares_send_dnsrec(channel, send_rec, dnsrec_callback, NULL, &qid);
                        
                        ares_dns_record_destroy(send_rec);
                    }
                }
            }
            
            // Test ares_query_dnsrec
            if (fdp.remaining_bytes() > 10) {
                size_t qname_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                std::string qname = fdp.ConsumeBytesAsString(qname_len);
                
                if (!qname.empty()) {
                    ares_dns_class_t dnsclass = static_cast<ares_dns_class_t>(
                        fdp.ConsumeIntegralInRange<int>(1, 4));
                    ares_dns_rec_type_t type = static_cast<ares_dns_rec_type_t>(
                        fdp.ConsumeIntegralInRange<int>(1, 100));
                    
                    unsigned short qid;
                    ares_query_dnsrec(channel, qname.c_str(), dnsclass, type, 
                                     dnsrec_callback, NULL, &qid);
                }
            }
            
            // Test ares_search_dnsrec - this one takes a DNS record, not name/class/type
            if (fdp.remaining_bytes() > 20) {
                // Create a DNS record for searching
                ares_dns_record_t *search_rec = NULL;
                ares_status_t status = ares_dns_record_create(&search_rec, 
                    fdp.ConsumeIntegral<unsigned short>(),
                    ARES_FLAG_RD,
                    ARES_OPCODE_QUERY,
                    ARES_RCODE_NOERROR);
                
                if (status == ARES_SUCCESS && search_rec) {
                    // Add a query
                    std::string qname = "test.example.com";
                    ares_dns_record_query_add(search_rec, qname.c_str(), 
                                             ARES_REC_TYPE_A, ARES_CLASS_IN);
                    
                    // Search with the DNS record
                    ares_search_dnsrec(channel, search_rec, dnsrec_callback, NULL);
                    
                    ares_dns_record_destroy(search_rec);
                }
            }

            
            // Clean up channel
            ares_destroy(channel);
        }
    }

    // Test utility functions
    if (fdp.remaining_bytes() > 4) {
        // Test ares_strerror with random error codes
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 1; i++) {
            int error_code = fdp.ConsumeIntegral<int>();
            const char *error_str = ares_strerror(error_code);
            (void)error_str;
        }
        
        // Test ares_version
        int version;
        const char *version_str = ares_version(&version);
        (void)version_str;
        (void)version;
        
        // Test ares_free_string, ares_free_data, ares_free_hostent (with NULL)
        ares_free_string(NULL);
        ares_free_data(NULL);
        ares_free_hostent(NULL);
        
        // Test ares_threadsafety
        ares_bool_t threadsafe = ares_threadsafety();
        (void)threadsafe;
        
        // Test ares_library_initialized
        int initialized = ares_library_initialized();
        (void)initialized;
    }

    // Cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_002
