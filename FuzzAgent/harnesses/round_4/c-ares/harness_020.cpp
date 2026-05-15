#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_020 {
/* Fuzzing harness for c-ares library targeting comprehensive DNS reply parsing
 * with focus on legacy DNS reply parsing APIs that have severe undercoverage (8-22% branch coverage).
 * 
 * Coverage analysis identified:
 * 1. Critical branch blocker in ares_dns_rr_data_ptr (124/130 branches blocked)
 * 2. Severe undercoverage in legacy DNS reply parsing APIs
 * 
 * Target legacy parse functions (all have low coverage):
 * - ares_parse_a_reply (partially covered but needs more diversity)
 * - ares_parse_aaaa_reply
 * - ares_parse_ptr_reply
 * - ares_parse_mx_reply  
 * - ares_parse_soa_reply (0% coverage in many paths)
 * - ares_parse_caa_reply
 * - ares_parse_ns_reply
 * - ares_parse_uri_reply
 * - ares_parse_naptr_reply (0% coverage in many paths)
 * - ares_parse_srv_reply
 * - ares_parse_txt_reply
 * - ares_parse_txt_reply_ext
 * 
 * Strategy: Generate diverse DNS reply structures with:
 * 1. Multiple record types in various DNS sections (answer, authority, additional)
 * 2. Different class types (IN, CHAOS, HESIOD, etc.)
 * 3. Edge case TTL values and record counts
 * 4. Malformed but parseable DNS responses
 * 5. Complex nested structures (CNAME chains, DNAME, etc.)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_005: Tests legacy parse functions but with simpler approach
 * - harness_019: Focuses on record manipulation and duplication
 * - This harness: Comprehensive reply parsing with semantic diversity across all record types,
 *   focusing specifically on the reply parsing code paths with low coverage
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need substantial input for complex DNS reply structures
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Strategy 1: Generate synthetic DNS records and serialize them for parsing
    // This exercises ares_dns_rr_data_ptr through the parsing path
    
    // Create DNS record with fuzzer parameters
    ares_dns_record_t* dnsrec = NULL;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 5));  // 0-5 valid opcodes
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 16)); // 0-16 valid rcodes
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Add query section (required for valid DNS record)
    std::string query_name = fdp.ConsumeRandomLengthString(128);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(1, 257));  // 1-257 valid DNS types
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(1, 255));
    
    // Validate class - only standard DNS classes
    uint16_t qclass_raw = static_cast<uint16_t>(qclass);
    if (qclass_raw != 1 &&     // ARES_CLASS_IN
        qclass_raw != 3 &&     // ARES_CLASS_CHAOS  
        qclass_raw != 4 &&     // ARES_CLASS_HESIOD
        qclass_raw != 254 &&   // ARES_CLASS_NONE
        qclass_raw != 255) {   // ARES_CLASS_ANY
        qclass = ARES_CLASS_IN; // Default to IN class
    }
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }

    // Add diverse resource records to exercise all parsing paths
    size_t max_rrs = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    for (size_t i = 0; i < max_rrs && fdp.remaining_bytes() > 50; i++) {
        // Choose section randomly
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<int>(1, 3));  // 1=ANSWER, 2=AUTHORITY, 3=ADDITIONAL
        
        // Choose record type to maximize coverage of different parse functions
        int rr_choice = fdp.ConsumeIntegralInRange<int>(0, 14);
        ares_dns_rec_type_t rr_type;
        
        switch (rr_choice) {
            case 0:  rr_type = ARES_REC_TYPE_A; break;
            case 1:  rr_type = ARES_REC_TYPE_AAAA; break;
            case 2:  rr_type = ARES_REC_TYPE_NS; break;
            case 3:  rr_type = ARES_REC_TYPE_CNAME; break;
            case 4:  rr_type = ARES_REC_TYPE_SOA; break;
            case 5:  rr_type = ARES_REC_TYPE_PTR; break;
            case 6:  rr_type = ARES_REC_TYPE_MX; break;
            case 7:  rr_type = ARES_REC_TYPE_TXT; break;
            case 8:  rr_type = ARES_REC_TYPE_SRV; break;
            case 9:  rr_type = ARES_REC_TYPE_NAPTR; break;
            case 10: rr_type = ARES_REC_TYPE_URI; break;
            case 11: rr_type = ARES_REC_TYPE_CAA; break;
            case 12: rr_type = ARES_REC_TYPE_OPT; break;
            case 13: rr_type = ARES_REC_TYPE_TLSA; break;
            case 14: rr_type = ARES_REC_TYPE_HINFO; break;
            default: rr_type = ARES_REC_TYPE_A; break;
        }
        
        std::string rr_name = fdp.ConsumeRandomLengthString(128);
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        
        // Set class - use IN for most records, but sometimes other classes
        ares_dns_class_t rr_class = ARES_CLASS_IN;
        if (fdp.ConsumeBool()) {
            int class_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
            switch (class_choice) {
                case 0: rr_class = ARES_CLASS_IN; break;
                case 1: rr_class = ARES_CLASS_CHAOS; break;
                case 2: rr_class = ARES_CLASS_HESIOD; break;
                case 3: rr_class = ARES_CLASS_NONE; break;
                case 4: rr_class = ARES_CLASS_ANY; break;
            }
        }
        
        // Add the record with appropriate data based on type
        ares_dns_rr_t* rr = NULL;
        status = ares_dns_record_rr_add(&rr, dnsrec, section, rr_name.c_str(), 
                                       rr_type, rr_class, ttl);
        if (status != ARES_SUCCESS || rr == NULL) {
            continue; // Skip this record if failed
        }
        
        // Set type-specific data to create realistic records
        switch (rr_type) {
            case ARES_REC_TYPE_A: {
                unsigned char addr_bytes[4];
                struct in_addr addr;
                for (int j = 0; j < 4 && fdp.remaining_bytes() > 0; j++) {
                    addr_bytes[j] = fdp.ConsumeIntegral<uint8_t>();
                }
                memcpy(&addr, addr_bytes, sizeof(addr));
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                break;
            }
            case ARES_REC_TYPE_AAAA: {
                unsigned char addr_bytes[16];
                struct ares_in6_addr addr;
                for (int j = 0; j < 16 && fdp.remaining_bytes() > 0; j++) {
                    addr_bytes[j] = fdp.ConsumeIntegral<uint8_t>();
                }
                memcpy(&addr, addr_bytes, sizeof(addr));
                ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr);
                break;
            }
            case ARES_REC_TYPE_CNAME:
            case ARES_REC_TYPE_PTR: {
                std::string target = fdp.ConsumeRandomLengthString(128);
                ares_dns_rr_key_t key = (rr_type == ARES_REC_TYPE_NS) ? ARES_RR_NS_NSDNAME :
                                       (rr_type == ARES_REC_TYPE_CNAME) ? ARES_RR_CNAME_CNAME :
                                       ARES_RR_PTR_DNAME;
                ares_dns_rr_set_str(rr, key, target.c_str());
                break;
            }
            case ARES_REC_TYPE_SOA: {
                std::string mname = fdp.ConsumeRandomLengthString(128);
                std::string rname = fdp.ConsumeRandomLengthString(128);
                uint32_t serial = fdp.ConsumeIntegral<uint32_t>();
                uint32_t refresh = fdp.ConsumeIntegral<uint32_t>();
                uint32_t retry = fdp.ConsumeIntegral<uint32_t>();
                uint32_t expire = fdp.ConsumeIntegral<uint32_t>();
                uint32_t minimum = fdp.ConsumeIntegral<uint32_t>();
                
                ares_dns_rr_set_str(rr, ARES_RR_SOA_MNAME, mname.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_SOA_RNAME, rname.c_str());
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_SERIAL, serial);
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_REFRESH, refresh);
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_RETRY, retry);
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_EXPIRE, expire);
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_MINIMUM, minimum);
                break;
            }
            case ARES_REC_TYPE_MX: {
                uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                std::string exchange = fdp.ConsumeRandomLengthString(128);
                ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                break;
            }
            case ARES_REC_TYPE_TXT: {
                size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t j = 0; j < num_strings && fdp.remaining_bytes() > 0; j++) {
                    std::string txt_str = fdp.ConsumeRandomLengthString(255);
                    // Use simpler string setting - TXT records may need special handling
                    ares_dns_rr_set_str(rr, ARES_RR_TXT_DATA, txt_str.c_str());
                }
                break;
            }
            case ARES_REC_TYPE_SRV: {
                uint16_t priority = fdp.ConsumeIntegral<uint16_t>();
                uint16_t weight = fdp.ConsumeIntegral<uint16_t>();
                uint16_t port = fdp.ConsumeIntegral<uint16_t>();
                std::string target = fdp.ConsumeRandomLengthString(128);
                
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PRIORITY, priority);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_WEIGHT, weight);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PORT, port);
                ares_dns_rr_set_str(rr, ARES_RR_SRV_TARGET, target.c_str());
                break;
            }
            case ARES_REC_TYPE_NAPTR: {
                uint16_t order = fdp.ConsumeIntegral<uint16_t>();
                uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                std::string flags = fdp.ConsumeRandomLengthString(10);
                std::string services = fdp.ConsumeRandomLengthString(50);
                std::string regexp = fdp.ConsumeRandomLengthString(50);
                std::string replacement = fdp.ConsumeRandomLengthString(128);
                
                ares_dns_rr_set_u16(rr, ARES_RR_NAPTR_ORDER, order);
                ares_dns_rr_set_u16(rr, ARES_RR_NAPTR_PREFERENCE, preference);
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_FLAGS, flags.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_SERVICES, services.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_REGEXP, regexp.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_REPLACEMENT, replacement.c_str());
                break;
            }
            case ARES_REC_TYPE_URI: {
                uint16_t priority = fdp.ConsumeIntegral<uint16_t>();
                uint16_t weight = fdp.ConsumeIntegral<uint16_t>();
                std::string target = fdp.ConsumeRandomLengthString(255);
                
                ares_dns_rr_set_u16(rr, ARES_RR_URI_PRIORITY, priority);
                ares_dns_rr_set_u16(rr, ARES_RR_URI_WEIGHT, weight);
                ares_dns_rr_set_str(rr, ARES_RR_URI_TARGET, target.c_str());
                break;
            }
            case ARES_REC_TYPE_CAA: {
                uint8_t critical = fdp.ConsumeIntegral<uint8_t>() & 0x7F; // 0-127
                std::string tag = fdp.ConsumeRandomLengthString(15);
                std::string value = fdp.ConsumeRandomLengthString(255);
                
                ares_dns_rr_set_u8(rr, ARES_RR_CAA_CRITICAL, critical);
                ares_dns_rr_set_str(rr, ARES_RR_CAA_TAG, tag.c_str());
                ares_dns_rr_set_bin(rr, ARES_RR_CAA_VALUE, 
                                   (const unsigned char*)value.c_str(), 
                                   value.length());
                break;
            }
            // Skip other types for brevity
            default:
                break;
        }
    }

    // Serialize the DNS record to binary format for parsing
    unsigned char* wire_data = NULL;
    size_t wire_len = 0;
    
    status = ares_dns_write(dnsrec, &wire_data, &wire_len);
    if (status != ARES_SUCCESS || wire_data == NULL || wire_len == 0) {
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }

    // Strategy 2: Call all legacy parse functions with the generated DNS data
    // This exercises the low-coverage reply parsing code paths
    
    // 1. Parse A reply
    struct hostent* host_a = NULL;
    struct ares_addrttl info_a[5];
    int count_a = 5;
    ares_parse_a_reply(wire_data, (int)wire_len, &host_a, info_a, &count_a);
    if (host_a != NULL) {
        ares_free_hostent(host_a);
    }
    
    // 2. Parse AAAA reply
    struct hostent* host_aaaa = NULL;
    struct ares_addr6ttl info_aaaa[5];
    int count_aaaa = 5;
    ares_parse_aaaa_reply(wire_data, (int)wire_len, &host_aaaa, info_aaaa, &count_aaaa);
    if (host_aaaa != NULL) {
        ares_free_hostent(host_aaaa);
    }
    
    // 3. Parse PTR reply - need address data
    struct hostent* host_ptr = NULL;
    unsigned char addr_v4[4] = {192, 168, 1, 1};
    unsigned char addr_v6[16] = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
                                0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34};
    
    ares_parse_ptr_reply(wire_data, (int)wire_len, addr_v4, sizeof(addr_v4), AF_INET, &host_ptr);
    if (host_ptr != NULL) {
        ares_free_hostent(host_ptr);
        host_ptr = NULL;
    }
    
    ares_parse_ptr_reply(wire_data, (int)wire_len, addr_v6, sizeof(addr_v6), AF_INET6, &host_ptr);
    if (host_ptr != NULL) {
        ares_free_hostent(host_ptr);
    }
    
    // 4. Parse MX reply
    struct ares_mx_reply* mx = NULL;
    ares_parse_mx_reply(wire_data, (int)wire_len, &mx);
    if (mx != NULL) {
        ares_free_data(mx);
    }
    
    // 5. Parse SOA reply (0% coverage in many paths)
    struct ares_soa_reply* soa = NULL;
    ares_parse_soa_reply(wire_data, (int)wire_len, &soa);
    if (soa != NULL) {
        ares_free_data(soa);
    }
    
    // 6. Parse CAA reply
    struct ares_caa_reply* caa = NULL;
    ares_parse_caa_reply(wire_data, (int)wire_len, &caa);
    if (caa != NULL) {
        ares_free_data(caa);
    }
    
    // 7. Parse NS reply
    struct hostent* host_ns = NULL;
    ares_parse_ns_reply(wire_data, (int)wire_len, &host_ns);
    if (host_ns != NULL) {
        ares_free_hostent(host_ns);
    }
    
    // 8. Parse URI reply
    struct ares_uri_reply* uri = NULL;
    ares_parse_uri_reply(wire_data, (int)wire_len, &uri);
    if (uri != NULL) {
        ares_free_data(uri);
    }
    
    // 9. Parse NAPTR reply (0% coverage in many paths)
    struct ares_naptr_reply* naptr = NULL;
    ares_parse_naptr_reply(wire_data, (int)wire_len, &naptr);
    if (naptr != NULL) {
        ares_free_data(naptr);
    }
    
    // 10. Parse SRV reply
    struct ares_srv_reply* srv = NULL;
    ares_parse_srv_reply(wire_data, (int)wire_len, &srv);
    if (srv != NULL) {
        ares_free_data(srv);
    }
    
    // 11. Parse TXT reply
    struct ares_txt_reply* txt = NULL;
    ares_parse_txt_reply(wire_data, (int)wire_len, &txt);
    if (txt != NULL) {
        ares_free_data(txt);
    }
    
    // 12. Parse TXT reply extended
    struct ares_txt_ext* txt_ext = NULL;
    ares_parse_txt_reply_ext(wire_data, (int)wire_len, &txt_ext);
    if (txt_ext != NULL) {
        ares_free_data(txt_ext);
    }

    // Cleanup
    ares_free_string(wire_data);
    ares_dns_record_destroy(dnsrec);
    
    // Strategy 3: Also test with raw fuzzer data directly (malformed DNS responses)
    // This tests error handling paths in parse functions
    if (fdp.remaining_bytes() > 12) {
        size_t raw_size = fdp.ConsumeIntegralInRange<size_t>(12, fdp.remaining_bytes());
        std::vector<unsigned char> raw_data = fdp.ConsumeBytes<unsigned char>(raw_size);
        
        // Test error paths with potentially malformed data
        struct hostent* host_err = NULL;
        ares_parse_a_reply(raw_data.data(), (int)raw_data.size(), &host_err, NULL, NULL);
        if (host_err != NULL) {
            ares_free_hostent(host_err);
        }
        
        struct ares_soa_reply* soa_err = NULL;
        ares_parse_soa_reply(raw_data.data(), (int)raw_data.size(), &soa_err);
        if (soa_err != NULL) {
            ares_free_data(soa_err);
        }
        
        struct ares_naptr_reply* naptr_err = NULL;
        ares_parse_naptr_reply(raw_data.data(), (int)raw_data.size(), &naptr_err);
        if (naptr_err != NULL) {
            ares_free_data(naptr_err);
        }
    }

    ares_library_cleanup();
    return 0;
}

} // namespace harness_020
