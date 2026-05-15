#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_004 {
/*
 * Fuzzing harness for c-ares DNS library - DNS Record Manipulation APIs
 * Targets: Low-level DNS record manipulation APIs identified as coverage gaps
 * Specifically targets: ares_dns_rr_add_abin, ares_dns_rr_set_bin, ares_dns_rr_set_str,
 *                      ares_dns_rr_del_abin, ares_dns_rr_set_addr, ares_dns_rr_set_addr6,
 *                      ares_dns_rr_set_u32, ares_dns_rr_get_addr, ares_dns_rr_get_addr6,
 *                      ares_dns_rr_key_tostr
 * Focuses on diverse DNS record types (A, AAAA, TXT, etc.) and data formats (IPv4, IPv6, strings, binary, integers)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for various data types
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Create a DNS record with random parameters
    ares_dns_record_t *dnsrec = NULL;
    uint16_t query_id = fdp.ConsumeIntegral<uint16_t>();
    uint16_t flags = fdp.ConsumeIntegral<uint16_t>() & 0xFF; // Valid flag range
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(fdp.ConsumeIntegral<uint8_t>() % 6); // Valid opcodes 0-5
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(fdp.ConsumeIntegral<uint8_t>() % 24); // Valid rcode range 0-23
    
    ares_status_t status = ares_dns_record_create(&dnsrec, query_id, flags, opcode, rcode);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Add a query section with random domain name
    if (fdp.remaining_bytes() > 10) {
        size_t domain_len = fdp.ConsumeIntegralInRange<size_t>(1, 255);
        std::string domain_name = fdp.ConsumeBytesAsString(domain_len);
        
        // Ensure domain name is null-terminated and valid for DNS
        if (!domain_name.empty() && domain_name.find('\0') == std::string::npos) {
            // Add query with different record types
            uint8_t record_type_choice = fdp.ConsumeIntegral<uint8_t>() % 10;
            ares_dns_rec_type_t rec_type = ARES_REC_TYPE_A; // Default
            
            switch (record_type_choice) {
                case 0: rec_type = ARES_REC_TYPE_A; break;      // IPv4 address
                case 1: rec_type = ARES_REC_TYPE_AAAA; break;   // IPv6 address
                case 2: rec_type = ARES_REC_TYPE_TXT; break;    // Text records
                case 3: rec_type = ARES_REC_TYPE_MX; break;     // Mail exchange
                case 4: rec_type = ARES_REC_TYPE_NS; break;     // Name server
                case 5: rec_type = ARES_REC_TYPE_CNAME; break;  // Canonical name
                case 6: rec_type = ARES_REC_TYPE_SOA; break;    // Start of authority
                case 7: rec_type = ARES_REC_TYPE_PTR; break;    // Pointer
                case 8: rec_type = ARES_REC_TYPE_SRV; break;    // Service record
                case 9: rec_type = ARES_REC_TYPE_CAA; break;    // CAA record
            }
            
            ares_dns_record_query_add(dnsrec, domain_name.c_str(), rec_type, ARES_CLASS_IN);
        }
    }

    // Now test DNS Resource Record (RR) manipulation APIs
    // Add RRs to different sections (answer, authority, additional)
    for (int section_idx = 0; section_idx < 3 && fdp.remaining_bytes() > 20; section_idx++) {
        ares_dns_section_t section = static_cast<ares_dns_section_t>(section_idx + 1); // 1=ANSWER, 2=AUTHORITY, 3=ADDITIONAL
        
        // Add a resource record
        size_t rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::string rr_name = fdp.ConsumeBytesAsString(rr_name_len);
        
        if (rr_name.empty() || rr_name.find('\0') != std::string::npos) {
            continue;
        }
        
        // Choose record type for this RR
        uint8_t rr_type_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
        ares_dns_rec_type_t rr_type = ARES_REC_TYPE_A;
        
        switch (rr_type_choice) {
            case 0: rr_type = ARES_REC_TYPE_A; break;
            case 1: rr_type = ARES_REC_TYPE_AAAA; break;
            case 2: rr_type = ARES_REC_TYPE_TXT; break;
            case 3: rr_type = ARES_REC_TYPE_MX; break;
            case 4: rr_type = ARES_REC_TYPE_CAA; break;
            case 5: rr_type = ARES_REC_TYPE_SRV; break;
            case 6: rr_type = ARES_REC_TYPE_OPT; break;
            case 7: rr_type = ARES_REC_TYPE_RAW_RR; break;
        }
        
        // Add the RR to the record
        ares_dns_rr_t *rr_out = NULL;
        ares_status_t rr_status = ares_dns_record_rr_add(&rr_out, dnsrec, section, rr_name.c_str(), rr_type, ARES_CLASS_IN, 3600);
        
        if (rr_status != ARES_SUCCESS || rr_out == NULL) {
            continue;
        }
        
        // Get the RR we just added (should be the last one in the section)
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, section);
        if (rr_count == 0) {
            continue;
        }
        
        ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, section, rr_count - 1);
        if (rr == NULL) {
            continue;
        }

        
        // Now test various RR manipulation APIs based on record type
        
        // 1. Test ares_dns_rr_set_addr() and ares_dns_rr_get_addr() for A records
        if (rr_type == ARES_REC_TYPE_A && fdp.remaining_bytes() >= 4) {
            struct in_addr ipv4_addr;
            uint32_t ipv4_raw = fdp.ConsumeIntegral<uint32_t>();
            ipv4_addr.s_addr = ipv4_raw;
            
            // Set IPv4 address
            ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &ipv4_addr);
            
            // Get IPv4 address back
            const struct in_addr *retrieved_addr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
            // Could compare retrieved_addr->s_addr with ipv4_addr.s_addr
        }
        
        // 2. Test ares_dns_rr_set_addr6() and ares_dns_rr_get_addr6() for AAAA records
        if (rr_type == ARES_REC_TYPE_AAAA && fdp.remaining_bytes() >= 16) {
            struct ares_in6_addr ipv6_addr;
            std::vector<uint8_t> ipv6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            memcpy(ipv6_addr._S6_un._S6_u8, ipv6_bytes.data(), 16);
            
            // Set IPv6 address
            ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &ipv6_addr);
            
            // Get IPv6 address back
            const struct ares_in6_addr *retrieved_addr6 = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
            // Could compare retrieved address
        }
        
        // 3. Test ares_dns_rr_set_str() for TXT and other string-based records
        if ((rr_type == ARES_REC_TYPE_TXT || rr_type == ARES_REC_TYPE_CNAME || 
             rr_type == ARES_REC_TYPE_NS || rr_type == ARES_REC_TYPE_PTR) && 
            fdp.remaining_bytes() > 5) {
            
            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 200);
            std::string str_data = fdp.ConsumeBytesAsString(str_len);
            
            // Determine appropriate key based on record type
            ares_dns_rr_key_t str_key;
            switch (rr_type) {
                case ARES_REC_TYPE_TXT: str_key = ARES_RR_TXT_DATA; break;
                case ARES_REC_TYPE_CNAME: str_key = ARES_RR_CNAME_CNAME; break;
                case ARES_REC_TYPE_NS: str_key = ARES_RR_NS_NSDNAME; break;
                case ARES_REC_TYPE_PTR: str_key = ARES_RR_PTR_DNAME; break;
                default: str_key = ARES_RR_TXT_DATA; break;
            }
            
            if (!str_data.empty() && str_data.find('\0') == std::string::npos) {
                ares_dns_rr_set_str(rr, str_key, str_data.c_str());
                
                // Get string back
                const char *retrieved_str = ares_dns_rr_get_str(rr, str_key);
                // Could compare strings
            }
        }
        
        // 4. Test ares_dns_rr_set_bin() for binary data (e.g., RAW_RR records)
        if (rr_type == ARES_REC_TYPE_RAW_RR && fdp.remaining_bytes() > 10) {
            size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
            
            ares_dns_rr_set_bin(rr, ARES_RR_RAW_RR_DATA, bin_data.data(), bin_data.size());
            
            // Get binary data back
            size_t retrieved_len = 0;
            const unsigned char *retrieved_bin = ares_dns_rr_get_bin(rr, ARES_RR_RAW_RR_DATA, &retrieved_len);
            // Could compare binary data
        }
        
        // 5. Test ares_dns_rr_set_u32() for 32-bit unsigned integers
        // Some record types use U32 datatype (e.g., SOA serial, refresh, etc.)
        if (fdp.remaining_bytes() >= 4) {
            uint32_t u32_val = fdp.ConsumeIntegral<uint32_t>();
            
            // Try setting on various keys that might accept U32
            // Note: Need to check which keys actually accept U32 datatype
            // For now, we'll try with SOA record if that's what we have
            if (rr_type == ARES_REC_TYPE_SOA) {
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_SERIAL, u32_val);
                // Get it back
                unsigned int retrieved_u32 = ares_dns_rr_get_u32(rr, ARES_RR_SOA_SERIAL);
                // Could compare values
            }
        }
        
        // 6. Test ares_dns_rr_add_abin() and ares_dns_rr_del_abin() for binary arrays
        // CAA records use ABINP datatype for their value field
        if (rr_type == ARES_REC_TYPE_CAA && fdp.remaining_bytes() > 10) {
            // Add multiple binary array entries
            int num_entries = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            for (int i = 0; i < num_entries && fdp.remaining_bytes() > 5; i++) {
                size_t abin_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_len);
                
                ares_dns_rr_add_abin(rr, ARES_RR_CAA_VALUE, abin_data.data(), abin_data.size());
            }
            
            // Delete some entries if we added enough
            size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_CAA_VALUE);
            if (abin_cnt > 0) {
                // Delete first entry
                ares_dns_rr_del_abin(rr, ARES_RR_CAA_VALUE, 0);
            }
        }
        
        // 7. Test ares_dns_rr_key_tostr() for key-to-string conversion
        // Test a few different RR keys
        const ares_dns_rr_key_t test_keys[] = {
            ARES_RR_A_ADDR,
            ARES_RR_AAAA_ADDR,
            ARES_RR_TXT_DATA,
            ARES_RR_CAA_TAG,
            ARES_RR_SOA_SERIAL
        };
        
        for (size_t i = 0; i < sizeof(test_keys)/sizeof(test_keys[0]); i++) {
            const char *key_str = ares_dns_rr_key_tostr(test_keys[i]);
            // key_str could be NULL for invalid keys, that's okay
        }
        
        // 8. Test edge cases and error conditions
        // Try calling functions with invalid parameters
        
        // Try getting/setting with potentially invalid keys
        if (fdp.remaining_bytes() > 0) {
            uint16_t random_key = fdp.ConsumeIntegral<uint16_t>();
            const char *random_key_str = ares_dns_rr_key_tostr(static_cast<ares_dns_rr_key_t>(random_key));
            // This may return NULL for invalid keys, which is expected
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_004
