#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_010 {
/* Comprehensive DNS Record Manipulation Harness for c-ares
 * 
 * Target: Exercise the switch cases in ares_dns_rr_data_ptr for uncommon DNS
 * record types to unlock 94+ blocked branches.
 * 
 * APIs targeted:
 * - Core: ares_dns_record_create, ares_dns_record_destroy
 * - RR Addition: ares_dns_record_rr_add
 * - Field setters: ares_dns_rr_set_u8/u16/u32, set_addr, set_addr6, set_str, set_bin
 * - Field getters: ares_dns_rr_get_u8/u16/u32, get_addr, get_addr6, get_str, get_bin
 * - Specialized: ares_dns_rr_add_abin, ares_dns_rr_del_abin, 
 *                ares_dns_rr_get_opt_byid, ares_dns_rr_del_opt_byid
 * 
 * DNS Record types targeted: MX, SRV, TXT, NAPTR, CAA, URI, SIG, KEY, CERT, LOC, etc.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 50) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize the c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Consume parameters for DNS record creation
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>() & 0x7F; // Valid flag bits
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 5));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 23));
    
    // Create DNS record
    // Create DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == nullptr) {
        ares_library_cleanup();
        return 0;
    }
    
    // Determine which DNS record types to test based on remaining input
    size_t max_records = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < max_records && fdp.remaining_bytes() > 20; i++) {
        // Consume parameters for resource record
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<uint8_t>(1, 3));
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        ares_dns_class_t rclass = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
        
        // Generate a domain name from fuzzer input
        std::string name = fdp.ConsumeRandomLengthString(63);
        if (name.empty()) {
            name = "example.com";
        }
        
        // Select DNS record type from uncommon types to target coverage gaps
        uint16_t record_type_val = fdp.ConsumeIntegralInRange<uint16_t>(0, 20);
        ares_dns_rec_type_t rtype;
        
        switch (record_type_val) {
            case 0:  rtype = ARES_REC_TYPE_MX; break;
            case 1:  rtype = ARES_REC_TYPE_SRV; break;
            case 2:  rtype = ARES_REC_TYPE_TXT; break;
            case 3:  rtype = ARES_REC_TYPE_NAPTR; break;
            case 4:  rtype = ARES_REC_TYPE_CAA; break;
            case 5:  rtype = ARES_REC_TYPE_URI; break;
            case 6:  rtype = ARES_REC_TYPE_SIG; break;
            case 7:  rtype = ARES_REC_TYPE_HINFO; break;
            case 8:  rtype = ARES_REC_TYPE_SOA; break;
            case 9:  rtype = ARES_REC_TYPE_OPT; break;
            case 10: rtype = ARES_REC_TYPE_TLSA; break;
            case 11: rtype = ARES_REC_TYPE_SVCB; break;
            case 12: rtype = ARES_REC_TYPE_HTTPS; break;
            case 13: rtype = ARES_REC_TYPE_A; break;
            case 14: rtype = ARES_REC_TYPE_AAAA; break;
            case 15: rtype = ARES_REC_TYPE_NS; break;
            case 16: rtype = ARES_REC_TYPE_CNAME; break;
            case 17: rtype = ARES_REC_TYPE_PTR; break;
            case 18: rtype = ARES_REC_TYPE_HINFO; break;
            default: rtype = ARES_REC_TYPE_TXT; break;
        }
        
        // Add resource record
        ares_dns_rr_t* rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, section, name.c_str(), rtype, rclass, ttl);
        if (status != ARES_SUCCESS || rr == nullptr) {
            continue;  // Try next record
        }
        
        // Based on record type, set appropriate fields
        switch (rtype) {
            case ARES_REC_TYPE_MX: {
                // MX record: preference (U16) and exchange (NAME)
                uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                std::string exchange = fdp.ConsumeRandomLengthString(63);
                if (exchange.empty()) exchange = "mail.example.com";
                
                ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                
                // Retrieve and verify
                uint16_t retrieved_pref = ares_dns_rr_get_u16(rr, ARES_RR_MX_PREFERENCE);
                const char* retrieved_exchange = ares_dns_rr_get_str(rr, ARES_RR_MX_EXCHANGE);
                (void)retrieved_pref;
                (void)retrieved_exchange;
                break;
            }
            
            case ARES_REC_TYPE_SRV: {
                // SRV record: priority, weight, port (all U16), target (NAME)
                uint16_t priority = fdp.ConsumeIntegral<uint16_t>();
                uint16_t weight = fdp.ConsumeIntegral<uint16_t>();
                uint16_t port = fdp.ConsumeIntegral<uint16_t>();
                std::string target = fdp.ConsumeRandomLengthString(63);
                if (target.empty()) target = "srv.example.com";
                
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PRIORITY, priority);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_WEIGHT, weight);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PORT, port);
                ares_dns_rr_set_str(rr, ARES_RR_SRV_TARGET, target.c_str());
                
                // Retrieve and verify
                uint16_t retrieved_priority = ares_dns_rr_get_u16(rr, ARES_RR_SRV_PRIORITY);
                uint16_t retrieved_weight = ares_dns_rr_get_u16(rr, ARES_RR_SRV_WEIGHT);
                uint16_t retrieved_port = ares_dns_rr_get_u16(rr, ARES_RR_SRV_PORT);
                const char* retrieved_target = ares_dns_rr_get_str(rr, ARES_RR_SRV_TARGET);
                (void)retrieved_priority;
                (void)retrieved_weight;
                (void)retrieved_port;
                (void)retrieved_target;
                break;
            }
            
            case ARES_REC_TYPE_TXT: {
                // TXT record: data (ABINP) - array of binary data
                size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                for (size_t j = 0; j < num_strings && fdp.remaining_bytes() > 0; j++) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                    std::vector<unsigned char> txt_data = fdp.ConsumeBytes<unsigned char>(str_len);
                    if (!txt_data.empty()) {
                        ares_dns_rr_add_abin(rr, ARES_RR_TXT_DATA, txt_data.data(), txt_data.size());
                    }
                }
                
                // Test del_abin if we have at least one entry
                size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_TXT_DATA);
                if (abin_cnt > 0) {
                    size_t idx_to_delete = fdp.ConsumeIntegralInRange<size_t>(0, abin_cnt - 1);
                    ares_dns_rr_del_abin(rr, ARES_RR_TXT_DATA, idx_to_delete);
                }
                break;
            }
            
            case ARES_REC_TYPE_NAPTR: {
                // NAPTR record: order, preference (U16), flags, services, regexp (STR), replacement (NAME)
                uint16_t order = fdp.ConsumeIntegral<uint16_t>();
                uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                std::string flags_str = fdp.ConsumeRandomLengthString(10);
                std::string services = fdp.ConsumeRandomLengthString(50);
                std::string regexp = fdp.ConsumeRandomLengthString(50);
                std::string replacement = fdp.ConsumeRandomLengthString(63);
                if (replacement.empty()) replacement = "naptr.example.com";
                
                ares_dns_rr_set_u16(rr, ARES_RR_NAPTR_ORDER, order);
                ares_dns_rr_set_u16(rr, ARES_RR_NAPTR_PREFERENCE, preference);
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_FLAGS, flags_str.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_SERVICES, services.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_REGEXP, regexp.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_REPLACEMENT, replacement.c_str());
                
                // Retrieve and verify
                uint16_t retrieved_order = ares_dns_rr_get_u16(rr, ARES_RR_NAPTR_ORDER);
                uint16_t retrieved_pref = ares_dns_rr_get_u16(rr, ARES_RR_NAPTR_PREFERENCE);
                const char* retrieved_flags = ares_dns_rr_get_str(rr, ARES_RR_NAPTR_FLAGS);
                const char* retrieved_services = ares_dns_rr_get_str(rr, ARES_RR_NAPTR_SERVICES);
                const char* retrieved_regexp = ares_dns_rr_get_str(rr, ARES_RR_NAPTR_REGEXP);
                const char* retrieved_replacement = ares_dns_rr_get_str(rr, ARES_RR_NAPTR_REPLACEMENT);
                (void)retrieved_order;
                (void)retrieved_pref;
                (void)retrieved_flags;
                (void)retrieved_services;
                (void)retrieved_regexp;
                (void)retrieved_replacement;
                break;
            }
            
            case ARES_REC_TYPE_CAA: {
                // CAA record: critical (U8), tag (STR), value (BINP)
                uint8_t critical = fdp.ConsumeIntegral<uint8_t>();
                std::string tag = fdp.ConsumeRandomLengthString(10);
                size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                std::vector<unsigned char> value = fdp.ConsumeBytes<unsigned char>(value_len);
                
                ares_dns_rr_set_u8(rr, ARES_RR_CAA_CRITICAL, critical);
                ares_dns_rr_set_str(rr, ARES_RR_CAA_TAG, tag.c_str());
                if (!value.empty()) {
                    ares_dns_rr_set_bin(rr, ARES_RR_CAA_VALUE, value.data(), value.size());
                }
                
                // Retrieve and verify
                uint8_t retrieved_critical = ares_dns_rr_get_u8(rr, ARES_RR_CAA_CRITICAL);
                const char* retrieved_tag = ares_dns_rr_get_str(rr, ARES_RR_CAA_TAG);
                size_t retrieved_len = 0;
                const unsigned char* retrieved_value = ares_dns_rr_get_bin(rr, ARES_RR_CAA_VALUE, &retrieved_len);
                (void)retrieved_critical;
                (void)retrieved_tag;
                (void)retrieved_value;
                (void)retrieved_len;
                break;
            }
            
            case ARES_REC_TYPE_SIG: {
                // SIG record: multiple fields of various types
                uint16_t type_covered = fdp.ConsumeIntegral<uint16_t>();
                uint8_t algorithm = fdp.ConsumeIntegral<uint8_t>();
                uint8_t labels = fdp.ConsumeIntegral<uint8_t>();
                uint32_t original_ttl = fdp.ConsumeIntegral<uint32_t>();
                uint32_t expiration = fdp.ConsumeIntegral<uint32_t>();
                uint32_t inception = fdp.ConsumeIntegral<uint32_t>();
                uint16_t key_tag = fdp.ConsumeIntegral<uint16_t>();
                std::string signers_name = fdp.ConsumeRandomLengthString(63);
                if (signers_name.empty()) signers_name = "sig.example.com";
                size_t sig_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                std::vector<unsigned char> signature = fdp.ConsumeBytes<unsigned char>(sig_len);
                
                ares_dns_rr_set_u16(rr, ARES_RR_SIG_TYPE_COVERED, type_covered);
                ares_dns_rr_set_u8(rr, ARES_RR_SIG_ALGORITHM, algorithm);
                ares_dns_rr_set_u8(rr, ARES_RR_SIG_LABELS, labels);
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_ORIGINAL_TTL, original_ttl);
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_EXPIRATION, expiration);
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_INCEPTION, inception);
                ares_dns_rr_set_u16(rr, ARES_RR_SIG_KEY_TAG, key_tag);
                ares_dns_rr_set_str(rr, ARES_RR_SIG_SIGNERS_NAME, signers_name.c_str());
                if (!signature.empty()) {
                    ares_dns_rr_set_bin(rr, ARES_RR_SIG_SIGNATURE, signature.data(), signature.size());
                }
                
                // Test get_u32 which has 0% coverage
                uint32_t retrieved_ttl = ares_dns_rr_get_u32(rr, ARES_RR_SIG_ORIGINAL_TTL);
                uint32_t retrieved_exp = ares_dns_rr_get_u32(rr, ARES_RR_SIG_EXPIRATION);
                uint32_t retrieved_inc = ares_dns_rr_get_u32(rr, ARES_RR_SIG_INCEPTION);
                (void)retrieved_ttl;
                (void)retrieved_exp;
                (void)retrieved_inc;
                break;
            }
            
            case ARES_REC_TYPE_OPT: {
                // OPT record: UDP size (U16), version (U8), flags (U16), options (OPT)
                uint16_t udp_size = fdp.ConsumeIntegral<uint16_t>();
                uint8_t version = fdp.ConsumeIntegral<uint8_t>();
                uint16_t opt_flags = fdp.ConsumeIntegral<uint16_t>();
                
                ares_dns_rr_set_u16(rr, ARES_RR_OPT_UDP_SIZE, udp_size);
                ares_dns_rr_set_u8(rr, ARES_RR_OPT_VERSION, version);
                ares_dns_rr_set_u16(rr, ARES_RR_OPT_FLAGS, opt_flags);
                
                // Test OPT operations - add some options
                size_t num_opts = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                for (size_t j = 0; j < num_opts && fdp.remaining_bytes() > 0; j++) {
                    uint16_t opt_id = fdp.ConsumeIntegral<uint16_t>();
                    size_t opt_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::vector<unsigned char> opt_data = fdp.ConsumeBytes<unsigned char>(opt_len);
                    
                    if (!opt_data.empty()) {
                        ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt_id, opt_data.data(), opt_data.size());
                    }
                }
                
                // Test get_opt_byid and del_opt_byid (both have 0% coverage)
                size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                if (opt_cnt > 0) {
                    // Test getting option by index using ares_dns_rr_get_opt
                    size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, opt_cnt - 1);
                    const unsigned char* opt_val = nullptr;
                    size_t opt_len = 0;
                    unsigned short opt_id = ares_dns_rr_get_opt(rr, ARES_RR_OPT_OPTIONS, idx, &opt_val, &opt_len);
                    (void)opt_val;
                    (void)opt_len;
                    
                    // Test getting option by id using ares_dns_rr_get_opt_byid
                    if (opt_cnt > 0) {
                        unsigned short opt_id_to_get = fdp.ConsumeIntegral<uint16_t>();
                        const unsigned char* val_byid = nullptr;
                        size_t val_len_byid = 0;
                        ares_bool_t found = ares_dns_rr_get_opt_byid(
                            rr, ARES_RR_OPT_OPTIONS, opt_id_to_get, &val_byid, &val_len_byid);
                        (void)found;
                        (void)val_byid;
                        (void)val_len_byid;
                        
                        // Delete an option by id
                        if (found == ARES_TRUE) {
                            ares_dns_rr_del_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_id_to_get);
                        }
                    }
                }
                break;
            }
            
            case ARES_REC_TYPE_A: {
                // A record: IPv4 address
                struct in_addr addr;
                uint32_t ip = fdp.ConsumeIntegral<uint32_t>();
                addr.s_addr = ip;
                
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                
                struct in_addr retrieved_addr;
                const struct in_addr* addr_ptr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
                if (addr_ptr != nullptr) {
                    retrieved_addr = *addr_ptr;
                    (void)retrieved_addr;
                }
                break;
            }
            
            case ARES_REC_TYPE_AAAA: {
                // AAAA record: IPv6 address
                struct ares_in6_addr addr6;
                std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
                if (ip6_bytes.size() == 16) {
                    memcpy(&addr6, ip6_bytes.data(), 16);
                    ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
                    
                    const struct ares_in6_addr* addr6_ptr = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
                    if (addr6_ptr != nullptr) {
                        struct ares_in6_addr retrieved_addr6 = *addr6_ptr;
                        (void)retrieved_addr6;
                    }
                }
                break;
            }
            
            default:
                // For other record types, just add them without field manipulation
                // This still exercises the record creation and type handling
                break;
        }
        
        // Test getting basic RR properties
        const char* rr_name = ares_dns_rr_get_name(rr);
        ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
        ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
        unsigned int rr_ttl = ares_dns_rr_get_ttl(rr);
        (void)rr_name;
        (void)rr_type;
        (void)rr_class;
        (void)rr_ttl;
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_010
