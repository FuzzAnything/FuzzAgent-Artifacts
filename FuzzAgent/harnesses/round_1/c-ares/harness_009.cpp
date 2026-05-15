#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"
#include "ares_dns.h"
#include "ares_nameser.h"

namespace harness_009 {

// Callback function for ares_send_dnsrec - note: timeouts is size_t, not unsigned int
static void dnsrec_callback(void *arg, ares_status_t status,
                           size_t timeouts,
                           const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
    // Just consume the callback
}

// Simple socket functions for channel initialization
static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

static int dummy_aclose(ares_socket_t sock, void *user_data) {
    (void)sock;
    (void)user_data;
    return 0;
}

static int dummy_aconnect(ares_socket_t sock, const struct sockaddr *addr,
                         ares_socklen_t addrlen, void *user_data) {
    (void)sock;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sock, void *buf, size_t len,
                                   int flags, struct sockaddr *src_addr,
                                   ares_socklen_t *addrlen, void *user_data) {
    (void)sock;
    (void)buf;
    (void)len;
    (void)flags;
    (void)src_addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_asendv(ares_socket_t sock, const struct iovec *iov,
                                int iovcnt, void *user_data) {
    (void)sock;
    (void)iov;
    (void)iovcnt;
    (void)user_data;
    return -1;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }
    
    // 2. Create a channel (simpler initialization without custom socket functions in options)
    ares_channel_t *channel = NULL;
    
    // Use ares_init for simple channel creation
    int channel_result = ares_init(&channel);
    if (channel_result != ARES_SUCCESS || channel == NULL) {
        // Try ares_init_options with minimal options
        struct ares_options options;
        memset(&options, 0, sizeof(options));
        int optmask = 0;
        
        channel_result = ares_init_options(&channel, &options, optmask);
        if (channel_result != ARES_SUCCESS || channel == NULL) {
            ares_library_cleanup();
            return 0;
        }
    }
    
    // Set custom socket functions after channel creation (like harness_007.cpp does)
    struct ares_socket_functions socket_funcs;
    socket_funcs.asocket = dummy_asocket;
    socket_funcs.aclose = dummy_aclose;
    socket_funcs.aconnect = dummy_aconnect;
    socket_funcs.arecvfrom = dummy_arecvfrom;
    socket_funcs.asendv = dummy_asendv;
    
    void *user_data = (void*)(uintptr_t)fdp.ConsumeIntegral<uint32_t>();
    ares_set_socket_functions(channel, &socket_funcs, user_data);
    
    // 3. Create DNS record with fuzzed parameters
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record,
                                                  fdp.ConsumeIntegral<uint16_t>(),  // id
                                                  fdp.ConsumeIntegral<uint16_t>(),  // flags
                                                  static_cast<ares_dns_opcode_t>(
                                                      fdp.ConsumeIntegralInRange<uint8_t>(0, 5)),  // opcode
                                                  static_cast<ares_dns_rcode_t>(
                                                      fdp.ConsumeIntegralInRange<uint8_t>(0, 15))); // rcode
    
    if (status != ARES_SUCCESS || dns_record == NULL) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // 4. Add question section with fuzzed name and type
    if (fdp.remaining_bytes() > 16) {
        std::string domain_name = fdp.ConsumeRandomLengthString(64);
        ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
            fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
        ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<uint16_t>(1, 255));
        
        status = ares_dns_record_query_add(dns_record, domain_name.c_str(),
                                          qtype, qclass);
        // Don't fail if query add fails - just continue
    }
    
    // 5. Add answer section with various record types
    if (fdp.remaining_bytes() > 32) {
        // Try adding different record types based on fuzzed input
        uint8_t record_type_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (record_type_choice) {
            case 0: {
                // A record
                std::string name = fdp.ConsumeRandomLengthString(32);
                uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                uint8_t addr[4];
                for (int i = 0; i < 4 && fdp.remaining_bytes() > 0; i++) {
                    addr[i] = fdp.ConsumeIntegral<uint8_t>();
                }
                ares_dns_rr_t *rr_out = NULL;
                status = ares_dns_record_rr_add(&rr_out, dns_record, ARES_SECTION_ANSWER,
                                               name.c_str(), ARES_REC_TYPE_A,
                                               ARES_CLASS_IN, ttl);
                if (status == ARES_SUCCESS && rr_out != NULL) {
                    struct in_addr addr_struct;
                    memcpy(&addr_struct, addr, 4);
                    ares_dns_rr_set_addr(rr_out, ARES_RR_A_ADDR, &addr_struct);
                }
                break;
            }
            case 1: {
                // TXT record
                std::string name = fdp.ConsumeRandomLengthString(32);
                uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                std::string txt_data = fdp.ConsumeRandomLengthString(128);
                ares_dns_rr_t *rr_out = NULL;
                status = ares_dns_record_rr_add(&rr_out, dns_record, ARES_SECTION_ANSWER,
                                               name.c_str(), ARES_REC_TYPE_TXT,
                                               ARES_CLASS_IN, ttl);
                if (status == ARES_SUCCESS && rr_out != NULL) {
                    ares_dns_rr_set_str(rr_out, ARES_RR_TXT_DATA, txt_data.c_str());
                }
                break;
            }
            case 2: {
                // CNAME record
                std::string name = fdp.ConsumeRandomLengthString(32);
                uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                std::string cname = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_t *rr_out = NULL;
                status = ares_dns_record_rr_add(&rr_out, dns_record, ARES_SECTION_ANSWER,
                                               name.c_str(), ARES_REC_TYPE_CNAME,
                                               ARES_CLASS_IN, ttl);
                if (status == ARES_SUCCESS && rr_out != NULL) {
                    ares_dns_rr_set_str(rr_out, ARES_RR_CNAME_CNAME, cname.c_str());
                }
                break;
            }
            case 3: {
                // NS record
                std::string name = fdp.ConsumeRandomLengthString(32);
                uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                std::string ns = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_t *rr_out = NULL;
                status = ares_dns_record_rr_add(&rr_out, dns_record, ARES_SECTION_ANSWER,
                                               name.c_str(), ARES_REC_TYPE_NS,
                                               ARES_CLASS_IN, ttl);
                if (status == ARES_SUCCESS && rr_out != NULL) {
                    ares_dns_rr_set_str(rr_out, ARES_RR_NS_NSDNAME, ns.c_str());
                }
                break;
            }
            case 4: {
                // AAAA record
                std::string name = fdp.ConsumeRandomLengthString(32);
                uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                uint8_t addr6[16];
                for (int i = 0; i < 16 && fdp.remaining_bytes() > 0; i++) {
                    addr6[i] = fdp.ConsumeIntegral<uint8_t>();
                }
                ares_dns_rr_t *rr_out = NULL;
                status = ares_dns_record_rr_add(&rr_out, dns_record, ARES_SECTION_ANSWER,
                                               name.c_str(), ARES_REC_TYPE_AAAA,
                                               ARES_CLASS_IN, ttl);
                if (status == ARES_SUCCESS && rr_out != NULL) {
                    struct ares_in6_addr addr6_struct;
                    memcpy(&addr6_struct, addr6, 16);
                    ares_dns_rr_set_addr6(rr_out, ARES_RR_AAAA_ADDR, &addr6_struct);
                }
                break;
            }
        }
    }
    
    // 6. Test ares_dns_write - serialize DNS record to wire format
    unsigned char *wire_buf = NULL;
    size_t wire_len = 0;
    status = ares_dns_write(dns_record, &wire_buf, &wire_len);
    
    if (status == ARES_SUCCESS && wire_buf != NULL && wire_len > 0) {
        // 7. Test ares_dns_parse - parse wire format back to DNS record
        ares_dns_record_t *parsed_record = NULL;
        ares_status_t parse_status = ares_dns_parse(wire_buf, wire_len, 0, &parsed_record);
        
        if (parse_status == ARES_SUCCESS && parsed_record != NULL) {
            // Test round-trip: write the parsed record again
            unsigned char *wire_buf2 = NULL;
            size_t wire_len2 = 0;
            ares_dns_write(parsed_record, &wire_buf2, &wire_len2);
            
            if (wire_buf2 != NULL) {
                ares_free_string(wire_buf2);
            }
            
            ares_dns_record_destroy(parsed_record);
        }
        // 8. Test ares_expand_name with the wire format
        // Note: ares_expand_name is a legacy function that works with DNS wire format
        if (wire_len > 12) {  // Need at least DNS header
            char *expanded_name = NULL;
            long encode_len = 0;
            // Use offset 12 (past header) for name expansion
            int expand_result = ares_expand_name(wire_buf + 12, wire_buf, (int)(wire_len - 12),
                                                &expanded_name, &encode_len);
            if (expand_result == ARES_SUCCESS && expanded_name != NULL) {
                ares_free_string(expanded_name);
            }
        }
        
        // 9. Test ares_expand_string with the wire format
        if (wire_len > 20) {  // Need enough data for string expansion
            unsigned char *expanded_string = NULL;
            long str_encode_len = 0;
            // Try expanding a string from the wire format
            int expand_str_result = ares_expand_string(wire_buf + 20, wire_buf, (int)(wire_len - 20),
                                                      &expanded_string, &str_encode_len);
            if (expand_str_result == ARES_SUCCESS && expanded_string != NULL) {
                ares_free_string((char *)expanded_string);
            }
        }
        
        // Clean up wire buffer
        ares_free_string(wire_buf);
    }
    
    // 10. Test ares_send_dnsrec with the DNS record
    // This will fail with our dummy socket functions, but will still exercise the API
    unsigned short qid = 0;
    status = ares_send_dnsrec(channel, dns_record, dnsrec_callback, NULL, &qid);
    // Don't check status - it's expected to fail with dummy sockets
    
    // 11. Cleanup
    ares_dns_record_destroy(dns_record);
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_009
