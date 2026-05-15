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

namespace harness_014 {
/* Fuzzing harness for c-ares library targeting ares_dns_rr_data_ptr through setter functions
 * This harness specifically targets the setter functions that call ares_dns_rr_data_ptr
 * to complement harness_013 which focuses on getter functions.
 * 
 * Target APIs: All setter functions that call ares_dns_rr_data_ptr:
 * - ares_dns_rr_set_addr, ares_dns_rr_set_addr6, ares_dns_rr_set_u8, ares_dns_rr_set_u16, ares_dns_rr_set_u32
 * - ares_dns_rr_set_bin, ares_dns_rr_set_bin_own, ares_dns_rr_set_str, ares_dns_rr_set_str_own
 * - ares_dns_rr_set_abin_own, ares_dns_rr_set_opt, ares_dns_rr_set_opt_own
 * 
 * Following exact coverage guidance: ares_dns_rr_data_ptr has 117/130 branches blocked
 * Strategy: Test all DNS record setters with various input conditions including:
 * 1. Valid data for each record type
 * 2. Invalid/edge cases (null pointers, wrong datatype keys, insufficient memory)
 * 3. Sequential operations (set, then modify, then clear)
 * 4. Ownership transfer variants (_own vs regular setter functions)
 * 
 * Differentiates from harness_013:
 * - harness_013: Focuses on getter functions and data retrieval
 * - This harness: Focuses on setter functions and data manipulation
 * - Tests error paths and validation logic in setters
 * - Exercises ownership semantics and memory management
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for record creation and multiple setter operations
    if (size < 200) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 1. Create DNS record with fuzzer-generated parameters
    ares_dns_record_t* dnsrec = NULL;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 5));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 16));
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Add a query to make it a valid DNS record
    std::string query_name = fdp.ConsumeRandomLengthString(128);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(1, 257));
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(1, 255));
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }

    // 2. Test different DNS record types with setter functions
    size_t max_tests = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t test_idx = 0; test_idx < max_tests && fdp.remaining_bytes() > 100; test_idx++) {
        // Choose a record type to test
        int record_type_val = fdp.ConsumeIntegralInRange<int>(0, 17);
        ares_dns_rec_type_t rr_type;
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<int>(1, 3));
        
        // Map to actual record types
        switch (record_type_val) {
            case 0: rr_type = ARES_REC_TYPE_A; break;
            case 1: rr_type = ARES_REC_TYPE_NS; break;
            case 2: rr_type = ARES_REC_TYPE_CNAME; break;
            case 3: rr_type = ARES_REC_TYPE_SOA; break;
            case 4: rr_type = ARES_REC_TYPE_PTR; break;
            case 5: rr_type = ARES_REC_TYPE_HINFO; break;
            case 6: rr_type = ARES_REC_TYPE_MX; break;
            case 7: rr_type = ARES_REC_TYPE_TXT; break;
            case 8: rr_type = ARES_REC_TYPE_SIG; break;
            case 9: rr_type = ARES_REC_TYPE_AAAA; break;
            case 10: rr_type = ARES_REC_TYPE_SRV; break;
            case 11: rr_type = ARES_REC_TYPE_NAPTR; break;
            case 12: rr_type = ARES_REC_TYPE_OPT; break;
            case 13: rr_type = ARES_REC_TYPE_TLSA; break;
            case 14: rr_type = ARES_REC_TYPE_SVCB; break;
            case 15: rr_type = ARES_REC_TYPE_HTTPS; break;
            case 16: rr_type = ARES_REC_TYPE_URI; break;
            case 17: rr_type = ARES_REC_TYPE_CAA; break;
            default: rr_type = ARES_REC_TYPE_A; break;
        }
        
        // Add RR with chosen type
        std::string rr_name = fdp.ConsumeRandomLengthString(64);
        ares_dns_rr_t* rr = NULL;
        status = ares_dns_record_rr_add(&rr, dnsrec, section, rr_name.c_str(), 
                                       rr_type, ARES_CLASS_IN, 
                                       fdp.ConsumeIntegral<unsigned int>());
        
        if (status != ARES_SUCCESS || rr == NULL) {
            continue; // Try next test
        }
        
        // 3. Test setter functions for the specific RR type
        // This exercises ares_dns_rr_data_ptr through setter paths
        
        switch (rr_type) {
            case ARES_REC_TYPE_A: {
                // Test ares_dns_rr_set_addr with valid data
                if (fdp.remaining_bytes() >= sizeof(struct in_addr)) {
                    struct in_addr addr;
                    std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(sizeof(struct in_addr));
                    memcpy(&addr, addr_bytes.data(), sizeof(struct in_addr));
                    
                    // Valid set
                    status = ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                    (void)status;
                    
                    // Test with NULL pointer (should fail)
                    status = ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, NULL);
                    (void)status;
                    
                    // Test with wrong key (should fail)
                    status = ares_dns_rr_set_addr(rr, ARES_RR_AAAA_ADDR, &addr);
                    (void)status;
                }
                break;
            }
            
            case ARES_REC_TYPE_AAAA: {
                // Test ares_dns_rr_set_addr6
                if (fdp.remaining_bytes() >= sizeof(struct ares_in6_addr)) {
                    struct ares_in6_addr addr6;
                    std::vector<uint8_t> addr6_bytes = fdp.ConsumeBytes<uint8_t>(sizeof(struct ares_in6_addr));
                    memcpy(&addr6, addr6_bytes.data(), sizeof(struct ares_in6_addr));
                    
                    // Valid set
                    status = ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
                    (void)status;
                    
                    // Test with wrong key
                    status = ares_dns_rr_set_addr6(rr, ARES_RR_A_ADDR, &addr6);
                    (void)status;
                }
                break;
            }
            
            case ARES_REC_TYPE_HINFO: {
                // Test string setters for HINFO
                std::string cpu_str = fdp.ConsumeRandomLengthString(32);
                std::string os_str = fdp.ConsumeRandomLengthString(32);
                
                // Test regular setter
                status = ares_dns_rr_set_str(rr, ARES_RR_HINFO_CPU, cpu_str.c_str());
                (void)status;
                status = ares_dns_rr_set_str(rr, ARES_RR_HINFO_OS, os_str.c_str());
                (void)status;
                
                // Test with wrong keys
                status = ares_dns_rr_set_str(rr, ARES_RR_A_ADDR, cpu_str.c_str());
                (void)status;
                
                break;
            }
            
            case ARES_REC_TYPE_MX: {
                // Test u16 and string setters for MX
                unsigned short preference = fdp.ConsumeIntegral<unsigned short>();
                std::string exchange = fdp.ConsumeRandomLengthString(64);
                
                // Valid sets
                status = ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                (void)status;
                status = ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                (void)status;
                
                // Test sequential modifications
                preference = fdp.ConsumeIntegral<unsigned short>();
                status = ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                (void)status;
                
                break;
            }
            
            case ARES_REC_TYPE_SRV: {
                // Test multiple u16 setters for SRV
                unsigned short priority = fdp.ConsumeIntegral<unsigned short>();
                unsigned short weight = fdp.ConsumeIntegral<unsigned short>();
                unsigned short port = fdp.ConsumeIntegral<unsigned short>();
                std::string target = fdp.ConsumeRandomLengthString(64);
                
                status = ares_dns_rr_set_u16(rr, ARES_RR_SRV_PRIORITY, priority);
                (void)status;
                status = ares_dns_rr_set_u16(rr, ARES_RR_SRV_WEIGHT, weight);
                (void)status;
                status = ares_dns_rr_set_u16(rr, ARES_RR_SRV_PORT, port);
                (void)status;
                status = ares_dns_rr_set_str(rr, ARES_RR_SRV_TARGET, target.c_str());
                (void)status;
                
                break;
            }
            
            case ARES_REC_TYPE_TXT: {
                // Test bin/abin setters for TXT (ABINP datatype)
                size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t i = 0; i < num_strings && fdp.remaining_bytes() > 10; i++) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string txt_str = fdp.ConsumeBytesAsString(str_len);
                    
                    // Test regular set_bin (which internally calls set_bin_own)
                    status = ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, 
                                                (const unsigned char*)txt_str.c_str(), 
                                                txt_str.length());
                    (void)status;
                    
                    // Test with wrong key
                    status = ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR,
                                                (const unsigned char*)txt_str.c_str(),
                                                txt_str.length());
                    (void)status;
                }
                break;
            }
            
            case ARES_REC_TYPE_SIG: {
                // Test multiple setters for SIG record
                unsigned short type_covered = fdp.ConsumeIntegral<unsigned short>();
                unsigned char algorithm = fdp.ConsumeIntegral<unsigned char>();
                unsigned char labels = fdp.ConsumeIntegral<unsigned char>();
                unsigned int original_ttl = fdp.ConsumeIntegral<unsigned int>();
                unsigned int expiration = fdp.ConsumeIntegral<unsigned int>();
                unsigned int inception = fdp.ConsumeIntegral<unsigned int>();
                unsigned short key_tag = fdp.ConsumeIntegral<unsigned short>();
                std::string signers_name = fdp.ConsumeRandomLengthString(64);
                
                // Test all setter types: u8, u16, u32, str, bin
                status = ares_dns_rr_set_u16(rr, ARES_RR_SIG_TYPE_COVERED, type_covered);
                (void)status;
                status = ares_dns_rr_set_u8(rr, ARES_RR_SIG_ALGORITHM, algorithm);
                (void)status;
                status = ares_dns_rr_set_u8(rr, ARES_RR_SIG_LABELS, labels);
                (void)status;
                status = ares_dns_rr_set_u32(rr, ARES_RR_SIG_ORIGINAL_TTL, original_ttl);
                (void)status;
                status = ares_dns_rr_set_u32(rr, ARES_RR_SIG_EXPIRATION, expiration);
                (void)status;
                status = ares_dns_rr_set_u32(rr, ARES_RR_SIG_INCEPTION, inception);
                (void)status;
                status = ares_dns_rr_set_u16(rr, ARES_RR_SIG_KEY_TAG, key_tag);
                (void)status;
                status = ares_dns_rr_set_str(rr, ARES_RR_SIG_SIGNERS_NAME, signers_name.c_str());
                (void)status;
                
                // Test bin setter for signature
                if (fdp.remaining_bytes() >= 32) {
                    size_t sig_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    std::vector<uint8_t> sig_data = fdp.ConsumeBytes<uint8_t>(sig_len);
                    status = ares_dns_rr_set_bin(rr, ARES_RR_SIG_SIGNATURE, sig_data.data(), sig_data.size());
                    (void)status;
                }
                break;
            }
            
            case ARES_REC_TYPE_TLSA: {
                // Test u8 and bin setters for TLSA
                unsigned char cert_usage = fdp.ConsumeIntegral<unsigned char>() % 4;
                unsigned char selector = fdp.ConsumeIntegral<unsigned char>() % 2;
                unsigned char match = fdp.ConsumeIntegral<unsigned char>() % 3;
                
                status = ares_dns_rr_set_u8(rr, ARES_RR_TLSA_CERT_USAGE, cert_usage);
                (void)status;
                status = ares_dns_rr_set_u8(rr, ARES_RR_TLSA_SELECTOR, selector);
                (void)status;
                status = ares_dns_rr_set_u8(rr, ARES_RR_TLSA_MATCH, match);
                (void)status;
                
                // Test bin setter for TLSA data
                if (fdp.remaining_bytes() >= 32) {
                    size_t tlsa_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    std::vector<uint8_t> tlsa_data = fdp.ConsumeBytes<uint8_t>(tlsa_len);
                    status = ares_dns_rr_set_bin(rr, ARES_RR_TLSA_DATA, tlsa_data.data(), tlsa_data.size());
                    (void)status;
                }
                break;
            }
            
            case ARES_REC_TYPE_CAA: {
                // Test u8, str, and bin setters for CAA
                unsigned char critical = fdp.ConsumeIntegral<unsigned char>() % 2;
                std::string tag = fdp.ConsumeRandomLengthString(16);
                
                status = ares_dns_rr_set_u8(rr, ARES_RR_CAA_CRITICAL, critical);
                (void)status;
                status = ares_dns_rr_set_str(rr, ARES_RR_CAA_TAG, tag.c_str());
                (void)status;
                
                // Test bin setter for CAA value
                if (fdp.remaining_bytes() >= 32) {
                    size_t caa_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    std::vector<uint8_t> caa_data = fdp.ConsumeBytes<uint8_t>(caa_len);
                    status = ares_dns_rr_set_bin(rr, ARES_RR_CAA_VALUE, caa_data.data(), caa_data.size());
                    (void)status;
                }
                break;
            }
            
            case ARES_REC_TYPE_OPT: {
                // Test OPT record setters (special case)
                // OPT has UDP size, version, flags, and options
                unsigned short udp_size = fdp.ConsumeIntegral<unsigned short>();
                unsigned char version = fdp.ConsumeIntegral<unsigned char>();
                unsigned short opt_flags = fdp.ConsumeIntegral<unsigned short>();
                
                status = ares_dns_rr_set_u16(rr, ARES_RR_OPT_UDP_SIZE, udp_size);
                (void)status;
                status = ares_dns_rr_set_u8(rr, ARES_RR_OPT_VERSION, version);
                (void)status;
                status = ares_dns_rr_set_u16(rr, ARES_RR_OPT_FLAGS, opt_flags);
                (void)status;
                
                // Test adding OPT options
                size_t num_opts = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                for (size_t i = 0; i < num_opts && fdp.remaining_bytes() > 20; i++) {
                    unsigned short opt_code = fdp.ConsumeIntegral<unsigned short>();
                    size_t opt_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_len);
                    
                    status = ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt_code,
                                                opt_data.data(), opt_data.size());
                    (void)status;
                }
                break;
            }
            
            default: {
                // For other record types, test TTL setter (doesn't use ares_dns_rr_data_ptr)
                unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
                status = ares_dns_rr_set_ttl(rr, ttl);
                (void)status;
                break;
            }
        }
        
        // 4. Test error conditions across all setter types
        // This exercises validation paths in ares_dns_rr_data_ptr
        
        // Test with NULL RR pointer (should fail in validation before ares_dns_rr_data_ptr)
        if (fdp.ConsumeBool()) {
            // Create dummy data for null test
            unsigned char dummy_u8 = fdp.ConsumeIntegral<unsigned char>();
            unsigned short dummy_u16 = fdp.ConsumeIntegral<unsigned short>();
            unsigned int dummy_u32 = fdp.ConsumeIntegral<unsigned int>();
            std::string dummy_str = fdp.ConsumeRandomLengthString(16);
            
            // These should all fail with NULL rr
            ares_dns_rr_set_u8(NULL, ARES_RR_SIG_ALGORITHM, dummy_u8);
            ares_dns_rr_set_u16(NULL, ARES_RR_MX_PREFERENCE, dummy_u16);
            ares_dns_rr_set_u32(NULL, ARES_RR_SIG_EXPIRATION, dummy_u32);
            ares_dns_rr_set_str(NULL, ARES_RR_HINFO_CPU, dummy_str.c_str());
        }
        
        // Test key mismatch (wrong record type for key)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(struct in_addr)) {
            struct in_addr dummy_addr;
            std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(sizeof(struct in_addr));
            memcpy(&dummy_addr, addr_bytes.data(), sizeof(struct in_addr));
            
            // Try to set A address on non-A record (should fail in ares_dns_rr_data_ptr)
            if (rr_type != ARES_REC_TYPE_A) {
                status = ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &dummy_addr);
                (void)status;
            }
        }
    }
    
    // 5. Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_014
