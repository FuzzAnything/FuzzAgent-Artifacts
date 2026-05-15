#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_015 {


// Fuzzer entry point for comprehensive DNS record type manipulation
// Specifically targeting ABIN (array of binary data) and OPT (option) data types
// Primary goal: Unblock 94/118 branches in ares_dns_rr_key_datatype (79.7% blocked)
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need substantial input for comprehensive testing of all record types
    if (size < 512) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // 2. Create DNS record with fuzzed parameters
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    // Consume parameters for record creation
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_OPCODE_QUERY, ARES_OPCODE_UPDATE));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_RCODE_NOERROR, ARES_RCODE_BADCOOKIE));
    
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Define all DNS record types to test
    ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_A,       // 1
        ARES_REC_TYPE_AAAA,    // 28  
        ARES_REC_TYPE_NS,      // 2
        ARES_REC_TYPE_CNAME,   // 5
        ARES_REC_TYPE_SOA,     // 6
        ARES_REC_TYPE_PTR,     // 12
        ARES_REC_TYPE_MX,      // 15
        ARES_REC_TYPE_TXT,     // 16
        ARES_REC_TYPE_SIG,     // 24
        ARES_REC_TYPE_SRV,     // 33
        ARES_REC_TYPE_NAPTR,   // 35
        ARES_REC_TYPE_OPT,     // 41
        ARES_REC_TYPE_TLSA,    // 52
        ARES_REC_TYPE_SVCB,    // 64
        ARES_REC_TYPE_HTTPS,   // 65
        ARES_REC_TYPE_URI,     // 256
        ARES_REC_TYPE_CAA,     // 257
        ARES_REC_TYPE_RAW_RR   // 65536
    };
    
    size_t num_record_types = sizeof(record_types) / sizeof(record_types[0]);
    
    // Choose a section for adding RRs
    ares_dns_section_t section = static_cast<ares_dns_section_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_SECTION_ANSWER, ARES_SECTION_ADDITIONAL));
    
    // 4. Test each record type systematically
    for (size_t type_idx = 0; type_idx < num_record_types && fdp.remaining_bytes() > 64; type_idx++) {
        ares_dns_rec_type_t rr_type = record_types[type_idx];
        
        // Consume RR parameters
        size_t rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        if (fdp.remaining_bytes() < rr_name_len) {
            continue;
        }
        std::string rr_name = fdp.ConsumeBytesAsString(rr_name_len);
        
        ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        
        // Add the RR
        ares_dns_rr_t *dns_rr = NULL;
        status = ares_dns_record_rr_add(&dns_rr, dnsrec, section, 
                                       rr_name.c_str(), rr_type, rr_class, ttl);
        if (status != ARES_SUCCESS || dns_rr == NULL) {
            continue; // Try next record type
        }
        
        // 5. Get available keys for this record type
        size_t key_count = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_count);
        
        if (keys == NULL || key_count == 0) {
            continue;
        }
        
        // 6. Test each key and its datatype
        // Limit to testing 3 keys per record type to avoid excessive consumption
        size_t keys_to_test = std::min(key_count, (size_t)3);
        
        for (size_t key_idx = 0; key_idx < keys_to_test && fdp.remaining_bytes() > 32; key_idx++) {
            ares_dns_rr_key_t key = keys[key_idx];
            
            // PRIMARY TARGET: Call ares_dns_rr_key_datatype to exercise switch cases
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
            
            // Based on datatype, test appropriate APIs
            switch (datatype) {
                case ARES_DATATYPE_ABINP: {
                    // Array of binary data (likely printable)
                    // Test ABIN manipulation APIs
                    
                    // First, add some binary data to the array
                    size_t num_abin_entries = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    
                    for (size_t i = 0; i < num_abin_entries && fdp.remaining_bytes() > 16; i++) {
                        size_t abin_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                        if (fdp.remaining_bytes() < abin_len) {
                            break;
                        }
                        std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_len);
                        
                        // TARGET API: ares_dns_rr_add_abin
                        status = ares_dns_rr_add_abin(dns_rr, key, abin_data.data(), abin_data.size());
                        (void)status;
                    }
                    
                    // TARGET API: ares_dns_rr_get_abin_cnt
                    size_t abin_count = ares_dns_rr_get_abin_cnt(dns_rr, key);
                    (void)abin_count;
                    
                    // Retrieve some ABIN entries if they exist
                    if (abin_count > 0) {
                        // Get a random index
                        size_t get_idx = fdp.ConsumeIntegralInRange<size_t>(0, abin_count - 1);
                        
                        // TARGET API: ares_dns_rr_get_abin
                        size_t retrieved_len = 0;
                        const uint8_t* retrieved_abin = ares_dns_rr_get_abin(dns_rr, key, get_idx, &retrieved_len);
                        (void)retrieved_abin;
                        (void)retrieved_len;
                        
                        // TARGET API: ares_dns_rr_del_abin - delete an entry if we have enough
                        if (abin_count > 1) {
                            size_t del_idx = fdp.ConsumeIntegralInRange<size_t>(0, abin_count - 1);
                            status = ares_dns_rr_del_abin(dns_rr, key, del_idx);
                            (void)status;
                        }
                    }
                    break;
                }
                
                case ARES_DATATYPE_OPT: {
                    // Option data type
                    // Test OPT manipulation
                    
                    // Add some options
                    size_t num_options = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                    
                    for (size_t i = 0; i < num_options && fdp.remaining_bytes() > 16; i++) {
                        unsigned short opt_id = fdp.ConsumeIntegral<unsigned short>();
                        size_t opt_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < opt_len) {
                            break;
                        }
                        std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_len);
                        // Set option
                        ares_dns_rr_set_opt(dns_rr, key, opt_id, opt_data.data(), opt_data.size());
                        
                        // TARGET API: ares_dns_rr_get_opt_byid - get option by ID
                        const unsigned char* retrieved_opt = NULL;
                        size_t retrieved_opt_len = 0;
                        ares_bool_t got_opt = ares_dns_rr_get_opt_byid(dns_rr, key, opt_id, &retrieved_opt, &retrieved_opt_len);
                        (void)got_opt;
                        (void)retrieved_opt;
                        (void)retrieved_opt_len;
                        
                        // Also test getting option name
                        const char* opt_name = ares_dns_opt_get_name(key, opt_id);
                        (void)opt_name;
                        
                        // Test deleting option by id
                        if (fdp.ConsumeBool()) {
                            ares_dns_rr_del_opt_byid(dns_rr, key, opt_id);
                        }
                    }
                    break;
                }
                
                case ARES_DATATYPE_INADDR: {
                    // IPv4 address
                    if (fdp.remaining_bytes() >= 4) {
                        struct in_addr addr;
                        memset(&addr, 0, sizeof(addr));
                        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
                        memcpy(&addr, addr_bytes.data(), 4);
                        
                        ares_dns_rr_set_addr(dns_rr, key, &addr);
                        
                        // TARGET API: ares_dns_rr_get_addr
                        const struct in_addr* retrieved_addr = ares_dns_rr_get_addr(dns_rr, key);
                        (void)retrieved_addr;
                    }
                    break;
                }
                
                case ARES_DATATYPE_INADDR6: {
                    // IPv6 address
                    if (fdp.remaining_bytes() >= 16) {
                        struct ares_in6_addr addr6;
                        memset(&addr6, 0, sizeof(addr6));
                        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(16);
                        memcpy(&addr6, addr_bytes.data(), 16);
                        
                        ares_dns_rr_set_addr6(dns_rr, key, &addr6);
                        
                        // TARGET API: ares_dns_rr_get_addr6
                        const struct ares_in6_addr* retrieved_addr6 = ares_dns_rr_get_addr6(dns_rr, key);
                        (void)retrieved_addr6;
                    }
                    break;
                }
                
                case ARES_DATATYPE_BIN: {
                    // Binary data
                    size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (fdp.remaining_bytes() >= bin_len) {
                        std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                        
                        ares_dns_rr_set_bin(dns_rr, key, bin_data.data(), bin_data.size());
                        
                        // TARGET API: ares_dns_rr_get_bin
                        size_t retrieved_len = 0;
                        const uint8_t* retrieved_bin = ares_dns_rr_get_bin(dns_rr, key, &retrieved_len);
                        (void)retrieved_bin;
                        (void)retrieved_len;
                    }
                    break;
                }
                
                case ARES_DATATYPE_U8: {
                    uint8_t value = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(dns_rr, key, value);
                    uint8_t retrieved_value = ares_dns_rr_get_u8(dns_rr, key);
                    (void)retrieved_value;
                    break;
                }
                
                case ARES_DATATYPE_U16: {
                    uint16_t value = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(dns_rr, key, value);
                    uint16_t retrieved_value = ares_dns_rr_get_u16(dns_rr, key);
                    (void)retrieved_value;
                    break;
                }
                
                case ARES_DATATYPE_U32: {
                    uint32_t value = fdp.ConsumeIntegral<uint32_t>();
                    ares_dns_rr_set_u32(dns_rr, key, value);
                    uint32_t retrieved_value = ares_dns_rr_get_u32(dns_rr, key);
                    (void)retrieved_value;
                    break;
                }
                
                case ARES_DATATYPE_STR:
                case ARES_DATATYPE_NAME: {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    if (fdp.remaining_bytes() >= str_len) {
                        std::string str_value = fdp.ConsumeBytesAsString(str_len);
                        ares_dns_rr_set_str(dns_rr, key, str_value.c_str());
                        const char* retrieved_str = ares_dns_rr_get_str(dns_rr, key);
                        (void)retrieved_str;
                    }
                    break;
                }
                
                case ARES_DATATYPE_BINP: {
                    // Binary data likely printable
                    size_t binp_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (fdp.remaining_bytes() >= binp_len) {
                        std::vector<uint8_t> binp_data = fdp.ConsumeBytes<uint8_t>(binp_len);
                        ares_dns_rr_set_bin(dns_rr, key, binp_data.data(), binp_data.size());
                        
                        size_t retrieved_len = 0;
                        const uint8_t* retrieved_binp = ares_dns_rr_get_bin(dns_rr, key, &retrieved_len);
                        (void)retrieved_binp;
                        (void)retrieved_len;
                    }
                    break;
                }
                
                default:
                    // Unknown datatype, just call the function for coverage
                    (void)datatype;
                    break;
            }
        }
        
        // Also test getting all keys for coverage completeness
        for (size_t i = 0; i < key_count; i++) {
            ares_dns_rr_key_t test_key = keys[i];
            // Just call datatype function for all keys to hit all switch cases
            ares_dns_datatype_t test_datatype = ares_dns_rr_key_datatype(test_key);
            (void)test_datatype;
        }
    }
    
    // 7. Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_015
