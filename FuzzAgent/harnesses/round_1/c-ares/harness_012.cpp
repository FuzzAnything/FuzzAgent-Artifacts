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

namespace harness_012 {


// Fuzzer entry point for DNS record binary data manipulation operations
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    // Need enough for: record creation, query name, RR name, binary data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Global library initialization
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
        fdp.ConsumeIntegralInRange<int>(0, 15)); // Standard RCODE range
    
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Add a query to the record (required helper API)
    size_t query_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string query_name = fdp.ConsumeBytesAsString(query_name_len);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_REC_TYPE_A, ARES_REC_TYPE_RAW_RR));
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    // Don't fail if query add fails - could be invalid parameters
    
    // 4. Add TXT Resource Record to test binary array operations
    // TXT records use ARES_RR_TXT_DATA key which has ABINP datatype
    ares_dns_section_t section = static_cast<ares_dns_section_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_SECTION_ANSWER, ARES_SECTION_ADDITIONAL));
    
    // Consume RR parameters for TXT record
    size_t rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string rr_name = fdp.ConsumeBytesAsString(rr_name_len);
    unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
    
    // Add TXT RR (ARES_REC_TYPE_TXT = 16)
    ares_dns_rr_t *txt_rr = NULL;
    status = ares_dns_record_rr_add(&txt_rr, dnsrec, section, 
                                   rr_name.c_str(), ARES_REC_TYPE_TXT, ARES_CLASS_IN, ttl);
    if (status != ARES_SUCCESS || txt_rr == NULL) {
        // Clean up and exit if we can't add RR
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }
    
    // 5. Test ares_dns_rr_add_abin - add binary array values to TXT record
    // Determine how many binary chunks to add (1-5)
    size_t num_abin_chunks = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    for (size_t i = 0; i < num_abin_chunks && fdp.remaining_bytes() > 4; i++) {
        // Consume binary data for this chunk
        size_t chunk_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (fdp.remaining_bytes() < chunk_len) {
            break; // Not enough data
        }
        std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_len);
        
        // Add binary data to TXT record using ARES_RR_TXT_DATA key
        status = ares_dns_rr_add_abin(txt_rr, ARES_RR_TXT_DATA, 
                                     chunk_data.data(), chunk_data.size());
        // Continue even if addition fails (should succeed for TXT records)
    }
    
    // 6. Test ares_dns_rr_get_abin_cnt - get count of binary array values
    size_t abin_count = ares_dns_rr_get_abin_cnt(txt_rr, ARES_RR_TXT_DATA);
    (void)abin_count; // Use the result
    
    // 7. Test ares_dns_rr_get_abin - retrieve binary array values by index
    if (abin_count > 0) {
        // Test retrieving each element
        for (size_t i = 0; i < abin_count; i++) {
            size_t retrieved_len = 0;
            const unsigned char *retrieved_data = 
                ares_dns_rr_get_abin(txt_rr, ARES_RR_TXT_DATA, i, &retrieved_len);
            (void)retrieved_data;
            (void)retrieved_len;
        }
        
        // Also test invalid indices
        size_t invalid_len = 0;
        const unsigned char *invalid_data = 
            ares_dns_rr_get_abin(txt_rr, ARES_RR_TXT_DATA, abin_count + 100, &invalid_len);
        (void)invalid_data;
    }
    
    // 8. Test ares_dns_rr_get_bin - retrieve all binary data concatenated
    size_t total_bin_len = 0;
    const unsigned char *total_bin_data = 
        ares_dns_rr_get_bin(txt_rr, ARES_RR_TXT_DATA, &total_bin_len);
    (void)total_bin_data;
    (void)total_bin_len;
    
    // 9. Add another RR type to test address getters (A record for IPv4)
    if (fdp.remaining_bytes() > 32) {
        ares_dns_rr_t *a_rr = NULL;
        size_t a_rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string a_rr_name = fdp.ConsumeBytesAsString(a_rr_name_len);
        
        status = ares_dns_record_rr_add(&a_rr, dnsrec, section,
                                       a_rr_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN, ttl);
        if (status == ARES_SUCCESS && a_rr != NULL) {
            // Set IPv4 address for A record
            struct in_addr ipv4_addr;
            if (fdp.remaining_bytes() >= 4) {
                std::vector<uint8_t> ipv4_bytes = fdp.ConsumeBytes<uint8_t>(4);
                memcpy(&ipv4_addr, ipv4_bytes.data(), 4);
                status = ares_dns_rr_set_addr(a_rr, ARES_RR_A_ADDR, &ipv4_addr);
                
                // Test ares_dns_rr_get_addr - retrieve IPv4 address
                const struct in_addr *retrieved_addr = 
                    ares_dns_rr_get_addr(a_rr, ARES_RR_A_ADDR);
                (void)retrieved_addr;
            }
        }
    }
    
    // 10. Add AAAA record to test IPv6 address getter
    if (fdp.remaining_bytes() > 48) {
        ares_dns_rr_t *aaaa_rr = NULL;
        size_t aaaa_rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string aaaa_rr_name = fdp.ConsumeBytesAsString(aaaa_rr_name_len);
        
        status = ares_dns_record_rr_add(&aaaa_rr, dnsrec, section,
                                       aaaa_rr_name.c_str(), ARES_REC_TYPE_AAAA, ARES_CLASS_IN, ttl);
        if (status == ARES_SUCCESS && aaaa_rr != NULL) {
            // Set IPv6 address for AAAA record
            struct ares_in6_addr ipv6_addr;
            if (fdp.remaining_bytes() >= 16) {
                std::vector<uint8_t> ipv6_bytes = fdp.ConsumeBytes<uint8_t>(16);
                memcpy(&ipv6_addr, ipv6_bytes.data(), 16);
                status = ares_dns_rr_set_addr6(aaaa_rr, ARES_RR_AAAA_ADDR, &ipv6_addr);
                
                // Test ares_dns_rr_get_addr6 - retrieve IPv6 address
                const struct ares_in6_addr *retrieved_addr6 = 
                    ares_dns_rr_get_addr6(aaaa_rr, ARES_RR_AAAA_ADDR);
                (void)retrieved_addr6;
            }
        }
    }
    
    // 11. Test ares_dns_rr_get_opt - get options (for OPT records)
    // Add an OPT record to test options
    if (fdp.remaining_bytes() > 32) {
        ares_dns_rr_t *opt_rr = NULL;
        status = ares_dns_record_rr_add(&opt_rr, dnsrec, ARES_SECTION_ADDITIONAL,
                                       "", ARES_REC_TYPE_OPT, ARES_CLASS_IN, 0);
        if (status == ARES_SUCCESS && opt_rr != NULL) {
            // Set some OPT options
            status = ares_dns_rr_set_u16(opt_rr, ARES_RR_OPT_UDP_SIZE, 1280);
            status = ares_dns_rr_set_u8(opt_rr, ARES_RR_OPT_VERSION, 0);
            
            // Add an OPT option with binary data
            if (fdp.remaining_bytes() > 16) {
                size_t opt_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_data_len);
                unsigned short opt_id = fdp.ConsumeIntegral<unsigned short>();
                
                status = ares_dns_rr_set_opt(opt_rr, ARES_RR_OPT_OPTIONS, 
                                           opt_id, opt_data.data(), opt_data.size());
                
                // Test ares_dns_rr_get_opt_cnt - get option count
                size_t opt_count = ares_dns_rr_get_opt_cnt(opt_rr, ARES_RR_OPT_OPTIONS);
                (void)opt_count;
                
                // Test ares_dns_rr_get_opt - get option by index
                if (opt_count > 0) {
                    unsigned short retrieved_opt_id = 0;
                    size_t retrieved_opt_len = 0;
                    const unsigned char *retrieved_opt_val = NULL;
                    
                    retrieved_opt_id = ares_dns_rr_get_opt(opt_rr, ARES_RR_OPT_OPTIONS, 0,
                                                          &retrieved_opt_val,
                                                          &retrieved_opt_len);
                    (void)retrieved_opt_id;
                    (void)retrieved_opt_val;
                    (void)retrieved_opt_len;
                }
            }
        }
    }
    
    // 12. Test ares_dns_rr_del_abin - delete binary array values
    if (abin_count > 0) {
        // Try to delete some elements (valid and invalid indices)
        size_t delete_idx = fdp.ConsumeIntegral<size_t>() % (abin_count * 2);
        status = ares_dns_rr_del_abin(txt_rr, ARES_RR_TXT_DATA, delete_idx);
        // Continue even if deletion fails (invalid index)
        
        // After deletion, get new count
        size_t new_abin_count = ares_dns_rr_get_abin_cnt(txt_rr, ARES_RR_TXT_DATA);
        (void)new_abin_count;
    }
    
    // 13. Test ares_dns_record_rr_get - retrieve RR by index (required helper API)
    size_t rr_count = ares_dns_record_rr_cnt(dnsrec, section);
    if (rr_count > 0) {
        // Get first RR in section
        const ares_dns_rr_t *retrieved_rr = ares_dns_record_rr_get(dnsrec, section, 0);
        (void)retrieved_rr;
        
        // Test with invalid index
        const ares_dns_rr_t *invalid_rr = ares_dns_record_rr_get(dnsrec, section, rr_count + 100);
        (void)invalid_rr;
    }
    
    // 14. Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_012
