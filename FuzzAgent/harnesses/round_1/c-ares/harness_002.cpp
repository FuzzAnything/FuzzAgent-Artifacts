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


// Fuzzer entry point for DNS Resource Record manipulation
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Global library initialization
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // 2. Create DNS record
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
    
    // 3. Add a query to the record
    size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string query_name = fdp.ConsumeBytesAsString(name_len);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_REC_TYPE_A, ARES_REC_TYPE_RAW_RR));
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    // Don't fail if query add fails - could be invalid parameters
    
    // 4. Add Resource Record to the record
    // Choose a section: ANSWER, AUTHORITY, or ADDITIONAL
    ares_dns_section_t section = static_cast<ares_dns_section_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_SECTION_ANSWER, ARES_SECTION_ADDITIONAL));
    
    // Consume RR parameters
    size_t rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string rr_name = fdp.ConsumeBytesAsString(rr_name_len);
    ares_dns_rec_type_t rr_type = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_REC_TYPE_A, ARES_REC_TYPE_RAW_RR));
    ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
    unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
    
    // Add the RR
    ares_dns_rr_t *dns_rr = NULL;
    status = ares_dns_record_rr_add(&dns_rr, dnsrec, section, 
                                   rr_name.c_str(), rr_type, rr_class, ttl);
    if (status != ARES_SUCCESS || dns_rr == NULL) {
        // Clean up and exit if we can't add RR
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }
    
    // 5. Test ares_dns_rr_key_tostr - convert RR key to string
    // Test a few different keys
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        ares_dns_rr_key_t test_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(0, 50)); // Arbitrary range
        const char *key_str = ares_dns_rr_key_tostr(test_key);
        // Just call it - result may be NULL for invalid keys
        (void)key_str;
    }
    
    // 6. Manipulate RR data
    
    // Test ares_dns_rr_set_str - set string data
    if (fdp.remaining_bytes() > 16) {
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string str_data = fdp.ConsumeBytesAsString(str_len);
        // Try with different keys that accept string data
        ares_dns_rr_key_t str_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(0, 50));
        status = ares_dns_rr_set_str(dns_rr, str_key, str_data.c_str());
        // Don't fail if it doesn't work - key might not accept strings
    }
    
    // Test ares_dns_rr_set_bin - set binary data
    if (fdp.remaining_bytes() > 16) {
        size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
        ares_dns_rr_key_t bin_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(0, 50));
        status = ares_dns_rr_set_bin(dns_rr, bin_key, bin_data.data(), bin_data.size());
        // Don't fail if it doesn't work
    }
    
    // Test ares_dns_rr_add_abin - add binary array value
    if (fdp.remaining_bytes() > 16) {
        size_t abin_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_len);
        ares_dns_rr_key_t abin_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(0, 50));
        status = ares_dns_rr_add_abin(dns_rr, abin_key, abin_data.data(), abin_data.size());
        // Don't fail if it doesn't work
    }
    
    // 7. Test ares_dns_rr_get_opt_byid - get option by ID
    if (fdp.remaining_bytes() > 4) {
        ares_dns_rr_key_t opt_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(0, 50));
        unsigned short opt_id = fdp.ConsumeIntegral<unsigned short>();
        const unsigned char *opt_val = NULL;
        size_t opt_val_len = 0;
        
        ares_bool_t found = ares_dns_rr_get_opt_byid(dns_rr, opt_key, opt_id, 
                                                   &opt_val, &opt_val_len);
        (void)found; // Result may be ARES_FALSE
    }
    
    // 8. Test ares_dns_rr_del_abin - delete binary array value
    if (fdp.remaining_bytes() > 4) {
        ares_dns_rr_key_t del_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(0, 50));
        size_t del_idx = fdp.ConsumeIntegral<size_t>() % 10; // Small index
        status = ares_dns_rr_del_abin(dns_rr, del_key, del_idx);
        // Don't fail if it doesn't work
    }
    
    // 9. Test ares_dns_record_duplicate - duplicate the record
    ares_dns_record_t *duplicate = ares_dns_record_duplicate(dnsrec);
    if (duplicate != NULL) {
        // 10. Test ares_dns_record_rr_del - delete RR from duplicate
        // Try to delete the first RR if it exists
        size_t rr_count = ares_dns_record_rr_cnt(duplicate, section);
        if (rr_count > 0) {
            status = ares_dns_record_rr_del(duplicate, section, 0);
            // Don't fail if it doesn't work
        }
        
        // Clean up duplicate
        ares_dns_record_destroy(duplicate);
    }
    
    // 11. Optional: Test ares_dns_parse and ares_dns_write
    if (fdp.remaining_bytes() > 16) {
        // Try to write the current record to buffer
        unsigned char *write_buf = NULL;
        size_t write_len = 0;
        status = ares_dns_write(dnsrec, &write_buf, &write_len);
        if (status == ARES_SUCCESS && write_buf != NULL && write_len > 0) {
            // Try to parse it back
            ares_dns_record_t *parsed = NULL;
            unsigned int parse_flags = fdp.ConsumeIntegral<unsigned int>();
            status = ares_dns_parse(write_buf, write_len, parse_flags, &parsed);
            if (status == ARES_SUCCESS && parsed != NULL) {
                ares_dns_record_destroy(parsed);
            }
            ares_free_string(write_buf);
        }
    }
    
    // 12. Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_002
