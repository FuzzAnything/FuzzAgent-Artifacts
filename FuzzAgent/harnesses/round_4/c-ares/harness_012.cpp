#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_012 {
/* Fuzzing harness for c-ares library targeting DNS record query type setting and RR deletion operations
 * This harness specifically targets uncovered APIs identified in coverage analysis:
 * - ares_dns_record_query_set_type (completely uncovered, 6 branches)
 * - ares_dns_record_rr_del (reached but 0% coverage, 12 branches)
 * - ares_dns_record_get_id (reached but 0% coverage, 2 branches)
 * - ares_dns_record_get_rcode (reached but 0% coverage, 2 branches)
 * 
 * Helper APIs: ares_dns_record_create, ares_dns_record_destroy, 
 *              ares_dns_record_query_add, ares_dns_record_rr_add
 * 
 * Following exact invocation sequence from coverage guidance:
 * 1. ares_dns_record_create to create DNS record object
 * 2. ares_dns_record_query_add to add query section
 * 3. ares_dns_record_query_set_type to set query type (primary target)
 * 4. ares_dns_record_get_id to retrieve record ID (secondary target)
 * 5. ares_dns_record_get_rcode to retrieve response code (secondary target)
 * 6. ares_dns_record_rr_add to add resource record
 * 7. ares_dns_record_rr_del to delete resource record (primary target)
 * 8. ares_dns_record_destroy to clean up
 * 
 * Differentiates from previous harnesses:
 * - harness_004: DNS record manipulation focusing on binary/string/address data
 * - harness_010: Comprehensive DNS record type testing including HINFO and SIG
 * - harness_011: SIG and NAPTR record manipulation
 * - This harness: Focuses on DNS query type setting and RR deletion lifecycle
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough data for: record creation + query addition + RR addition + parameters
    if (size < 50) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 1. Call ares_dns_record_create to create a new DNS record object
    ares_dns_record_t* dnsrec = NULL;
    
    // Consume parameters for record creation
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    
    // Use appropriate ranges for opcode and rcode enums
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 5));  // 0-5 valid opcodes
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 16));  // 0-16 valid rcodes
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // 2. Call ares_dns_record_query_add to add query section to the record
    std::string query_name = fdp.ConsumeRandomLengthString(128);
    
    // Choose a valid record type from the enum (1-257, plus 65536 for RAW_RR)
    // Use modulo to get a reasonable range
    uint32_t rec_type_val = fdp.ConsumeIntegral<uint32_t>() % 258;
    if (rec_type_val == 0) rec_type_val = 1;  // Avoid 0, use 1 (A record)
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(rec_type_val);
    
    // Choose a valid class (1-255, with some specific values)
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }

    // 3. Call ares_dns_record_query_set_type to set query type (primary target)
    // Determine which query index to modify (typically 0 since we added one query)
    size_t query_idx = 0;  // First and only query
    
    // Choose a new query type to set
    uint32_t new_rec_type_val = fdp.ConsumeIntegral<uint32_t>() % 258;
    if (new_rec_type_val == 0) new_rec_type_val = 1;
    ares_dns_rec_type_t new_qtype = static_cast<ares_dns_rec_type_t>(new_rec_type_val);
    
    status = ares_dns_record_query_set_type(dnsrec, query_idx, new_qtype);
    // Don't fail on error - continue testing other APIs even if set_type fails

    // 4. Call ares_dns_record_get_id to retrieve record ID (secondary target)
    unsigned short retrieved_id = ares_dns_record_get_id(dnsrec);
    (void)retrieved_id;  // Mark as used to avoid compiler warnings

    // 5. Call ares_dns_record_get_rcode to retrieve response code (secondary target)
    ares_dns_rcode_t retrieved_rcode = ares_dns_record_get_rcode(dnsrec);
    (void)retrieved_rcode;  // Mark as used

    // 6. Call ares_dns_record_rr_add to add a resource record
    // We need to add at least one RR to be able to delete it
    ares_dns_rr_t* rr = NULL;
    
    if (fdp.remaining_bytes() > 20) {
        std::string rr_name = fdp.ConsumeRandomLengthString(128);
        
        // Choose RR type
        uint32_t rr_type_val = fdp.ConsumeIntegral<uint32_t>() % 258;
        if (rr_type_val == 0) rr_type_val = 1;
        ares_dns_rec_type_t rr_type = static_cast<ares_dns_rec_type_t>(rr_type_val);
        
        // Choose RR class
        ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
        
        // Choose TTL
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        
        // Choose section (Answer, Authority, Additional)
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<uint8_t>(1, 3));
        
        status = ares_dns_record_rr_add(&rr, dnsrec, section, 
                                       rr_name.c_str(), rr_type, rr_class, ttl);
        
        // If RR addition succeeded, we can attempt deletion
        if (status == ARES_SUCCESS && rr != NULL) {
            // 7. Call ares_dns_record_rr_del to delete the resource record (primary target)
            // Delete the RR we just added (index 0 in the chosen section)
            size_t rr_idx = 0;
            status = ares_dns_record_rr_del(dnsrec, section, rr_idx);
            // Don't fail on error - continue to cleanup
        }
    }

    // 8. Call ares_dns_record_destroy to clean up the record
    ares_dns_record_destroy(dnsrec);

    // Cleanup library
    ares_library_cleanup();

    return 0;
}

} // namespace harness_012
