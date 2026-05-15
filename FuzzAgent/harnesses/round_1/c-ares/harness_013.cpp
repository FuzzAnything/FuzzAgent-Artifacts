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

namespace harness_013 {


// Fuzzer entry point for comprehensive DNS record manipulation
// Targeting all DNS record types to unblock ares_dns_rr_key_datatype function
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for comprehensive testing
    // Need enough for: record creation, multiple record types, various data types
    if (size < 128) {
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
    
    // 3. Add a query to the record (required for some operations)
    size_t query_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string query_name = fdp.ConsumeBytesAsString(query_name_len);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_REC_TYPE_A, ARES_REC_TYPE_RAW_RR));
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    // Continue even if query add fails
    
    // 4. Define all DNS record types to test
    ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_A,      // 1 - IPv4 address
        ARES_REC_TYPE_AAAA,   // 28 - IPv6 address
        ARES_REC_TYPE_NS,     // 2 - Name server
        ARES_REC_TYPE_CNAME,  // 5 - Canonical name
        ARES_REC_TYPE_SOA,    // 6 - Start of authority
        ARES_REC_TYPE_PTR,    // 12 - Pointer
        ARES_REC_TYPE_MX,     // 15 - Mail exchange
        ARES_REC_TYPE_SIG,    // 24 - SIG record
        ARES_REC_TYPE_SRV,    // 33 - Service record
        ARES_REC_TYPE_NAPTR,  // 35 - Naming authority pointer
        ARES_REC_TYPE_OPT,    // 41 - OPT record (EDNS0)
        ARES_REC_TYPE_TLSA,   // 52 - TLSA record
        ARES_REC_TYPE_SVCB,   // 64 - Service binding
        ARES_REC_TYPE_HTTPS,  // 65 - HTTPS service binding
        ARES_REC_TYPE_URI,    // 256 - URI record
        ARES_REC_TYPE_CAA,    // 257 - Certification authority authorization
        ARES_REC_TYPE_TXT,    // 16 - Text record
    };
    
    // 5. Test each record type with appropriate operations
    for (size_t type_idx = 0; type_idx < sizeof(record_types)/sizeof(record_types[0]); type_idx++) {
        ares_dns_rec_type_t rec_type = record_types[type_idx];
        
        // Skip if we don't have enough data
        if (fdp.remaining_bytes() < 32) {
            break;
        }
        
        // Choose a section for this record
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<int>(ARES_SECTION_ANSWER, ARES_SECTION_ADDITIONAL));
        
        // Consume RR parameters
        size_t rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        if (fdp.remaining_bytes() < rr_name_len) {
            break;
        }
        std::string rr_name = fdp.ConsumeBytesAsString(rr_name_len);
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        
        // Add the resource record
        ares_dns_rr_t *rr = NULL;
        status = ares_dns_record_rr_add(&rr, dnsrec, section, 
                                       rr_name.c_str(), rec_type, ARES_CLASS_IN, ttl);
        if (status != ARES_SUCCESS || rr == NULL) {
            continue; // Try next record type
        }
        
        // Get keys for this record type
        size_t key_count = 0;
        const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(rec_type, &key_count);
        
        if (keys != NULL && key_count > 0) {
            // Test a subset of keys based on available data
            size_t keys_to_test = std::min(key_count, (size_t)3);
            
            for (size_t key_idx = 0; key_idx < keys_to_test; key_idx++) {
                ares_dns_rr_key_t key = keys[key_idx];
                
                // Get datatype for this key (exercises ares_dns_rr_key_datatype)
                ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
                (void)datatype; // Use the result
                
                // Test appropriate setter/getter based on datatype
                switch (datatype) {
                    case ARES_DATATYPE_INADDR: {
                        // IPv4 address
                        struct in_addr addr;
                        memset(&addr, 0, sizeof(addr));
                        if (fdp.remaining_bytes() >= 4) {
                            std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
                            memcpy(&addr, addr_bytes.data(), 4);
                            ares_dns_rr_set_addr(rr, key, &addr);
                            
                            const struct in_addr* retrieved_addr = ares_dns_rr_get_addr(rr, key);
                            (void)retrieved_addr;
                        }
                        break;
                    }
                    
                    case ARES_DATATYPE_INADDR6: {
                        // IPv6 address
                        struct ares_in6_addr addr6;
                        memset(&addr6, 0, sizeof(addr6));
                        if (fdp.remaining_bytes() >= 16) {
                            std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(16);
                            memcpy(&addr6, addr_bytes.data(), 16);
                            ares_dns_rr_set_addr6(rr, key, &addr6);
                            
                            const struct ares_in6_addr* retrieved_addr6 = ares_dns_rr_get_addr6(rr, key);
                            (void)retrieved_addr6;
                        }
                        break;
                    }
                    
                    case ARES_DATATYPE_U8: {
                        // 8-bit unsigned integer
                        uint8_t value = fdp.ConsumeIntegral<uint8_t>();
                        ares_dns_rr_set_u8(rr, key, value);
                        
                        uint8_t retrieved_value = ares_dns_rr_get_u8(rr, key);
                        (void)retrieved_value;
                        break;
                    }
                    
                    case ARES_DATATYPE_U16: {
                        // 16-bit unsigned integer
                        uint16_t value = fdp.ConsumeIntegral<uint16_t>();
                        ares_dns_rr_set_u16(rr, key, value);
                        
                        uint16_t retrieved_value = ares_dns_rr_get_u16(rr, key);
                        (void)retrieved_value;
                        break;
                    }
                    
                    case ARES_DATATYPE_U32: {
                        // 32-bit unsigned integer
                        uint32_t value = fdp.ConsumeIntegral<uint32_t>();
                        ares_dns_rr_set_u32(rr, key, value);
                        
                        uint32_t retrieved_value = ares_dns_rr_get_u32(rr, key);
                        (void)retrieved_value;
                        break;
                    }
                    
                    case ARES_DATATYPE_STR:
                    case ARES_DATATYPE_NAME: {
                        // String or domain name
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                        if (fdp.remaining_bytes() >= str_len) {
                            std::string str_value = fdp.ConsumeBytesAsString(str_len);
                            ares_dns_rr_set_str(rr, key, str_value.c_str());
                            
                            const char* retrieved_str = ares_dns_rr_get_str(rr, key);
                            (void)retrieved_str;
                        }
                        break;
                    }
                    
                    case ARES_DATATYPE_BIN:
                    case ARES_DATATYPE_BINP: {
                        // Binary data
                        size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                        if (fdp.remaining_bytes() >= bin_len) {
                            std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                            ares_dns_rr_set_bin(rr, key, bin_data.data(), bin_data.size());
                            
                            size_t retrieved_len = 0;
                            const uint8_t* retrieved_bin = ares_dns_rr_get_bin(rr, key, &retrieved_len);
                            (void)retrieved_bin;
                            (void)retrieved_len;
                        }
                        break;
                    }
                    
                    case ARES_DATATYPE_ABINP: {
                        // Array of binary data (e.g., TXT records)
                        // First test getting count
                        size_t abin_count = ares_dns_rr_get_abin_cnt(rr, key);
                        (void)abin_count;
                        
                        // Add some binary chunks
                        size_t num_chunks = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                        for (size_t chunk_idx = 0; chunk_idx < num_chunks; chunk_idx++) {
                            size_t chunk_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                            if (fdp.remaining_bytes() < chunk_len) {
                                break;
                            }
                            std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_len);
                            ares_dns_rr_add_abin(rr, key, chunk_data.data(), chunk_data.size());
                        }
                        
                        // Test retrieval if we added chunks
                        if (num_chunks > 0) {
                            size_t new_count = ares_dns_rr_get_abin_cnt(rr, key);
                            if (new_count > 0) {
                                // Get first chunk
                                size_t chunk_len = 0;
                                const uint8_t* chunk_data = ares_dns_rr_get_abin(rr, key, 0, &chunk_len);
                                (void)chunk_data;
                                (void)chunk_len;
                                
                                // Delete first chunk if we have multiple
                                if (new_count > 1) {
                                    ares_dns_rr_del_abin(rr, key, 0);
                                }
                            }
                        }
                        break;
                    }
                    
                    case ARES_DATATYPE_OPT: {
                        // OPT records (EDNS0 options)
                        // First test getting option count
                        size_t opt_count = ares_dns_rr_get_opt_cnt(rr, key);
                        (void)opt_count;
                        
                        // Try to get first option if available
                        if (opt_count > 0) {
                            const unsigned char* opt_val = NULL;
                            size_t opt_val_len = 0;
                            unsigned short opt_id = ares_dns_rr_get_opt(rr, key, 0, &opt_val, &opt_val_len);
                            (void)opt_id;
                            (void)opt_val;
                            (void)opt_val_len;
                        }
                        
                        // For OPT records, we can also test deleting options by ID
                        if (rec_type == ARES_REC_TYPE_OPT) {
                            // Try deleting an option with a random ID
                            unsigned short opt_id = fdp.ConsumeIntegral<unsigned short>();
                            ares_dns_rr_del_opt_byid(rr, key, opt_id);
                        }
                        break;
                    }
                    
                    default:
                        // Unknown datatype, skip
                        break;
                }
            }
        }
    }
    
    // 6. Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_013
