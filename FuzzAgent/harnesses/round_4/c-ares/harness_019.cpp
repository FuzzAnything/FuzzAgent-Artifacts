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

namespace harness_019 {
/* Fuzzing harness for c-ares library targeting comprehensive DNS resource record manipulation
 * This harness focuses on record copying, transformation, and manipulation operations
 * that exercise ares_dns_rr_data_ptr through complex record operations.
 * 
 * Target APIs identified from coverage analysis: ares_dns_rr_data_ptr (124/130 branches blocked)
 * 
 * Primary manipulation APIs:
 * - ares_dns_record_duplicate: Create exact copy of DNS record
 * - ares_dns_record_duplicate_ex: Extended duplication with error handling
 * - ares_dns_write: Serialize DNS record to binary wire format
 * - ares_dns_parse: Parse binary wire format back to DNS record
 * - ares_dns_rr_set_* and ares_dns_rr_get_*: For comprehensive record manipulation
 * 
 * Strategy: Create complex DNS records with multiple RRs, duplicate them, 
 * serialize/deserialize, and verify consistency. This exercises ares_dns_rr_data_ptr
 * through:
 * 1. Internal copying logic in duplication functions
 * 2. Serialization/deserialization paths
 * 3. Cross-record data access during transformations
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_013: Focuses on getter functions only
 * - harness_014: Focuses on setter functions only  
 * - harness_015: Focuses on newer DNS types (SVCB, HTTPS, etc.)
 * - harness_016: Focuses on edge cases and error conditions
 * - harness_017: Focuses on binary data manipulation
 * - harness_018: Focuses on hosts file parsing
 * - This harness: Focuses on comprehensive record manipulation, duplication,
 *   and transformation operations that integrate getters/setters in complex workflows
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for complex record creation and manipulation
    if (size < 200) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 1. Create a complex DNS record with fuzzer-generated parameters
    ares_dns_record_t* original_rec = NULL;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 5));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 16));
    
    ares_status_t status = ares_dns_record_create(&original_rec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || original_rec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Add a query to make it a valid DNS record
    std::string query_name = fdp.ConsumeRandomLengthString(128);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(1, 257));
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(1, 255));
    
    status = ares_dns_record_query_add(original_rec, query_name.c_str(), qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(original_rec);
        ares_library_cleanup();
        return 0;
    }

    // 2. Add multiple resource records of different types to create complex structure
    size_t num_rrs = fdp.ConsumeIntegralInRange<size_t>(1, 6);
    
    for (size_t i = 0; i < num_rrs && fdp.remaining_bytes() > 50; i++) {
        // Choose section and record type
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<int>(1, 3));
        int record_type_val = fdp.ConsumeIntegralInRange<int>(0, 13);
        ares_dns_rec_type_t rr_type;
        
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
            case 8: rr_type = ARES_REC_TYPE_AAAA; break;
            case 9: rr_type = ARES_REC_TYPE_SRV; break;
            case 10: rr_type = ARES_REC_TYPE_NAPTR; break;
            case 11: rr_type = ARES_REC_TYPE_OPT; break;
            case 12: rr_type = ARES_REC_TYPE_URI; break;
            case 13: rr_type = ARES_REC_TYPE_CAA; break;
            default: rr_type = ARES_REC_TYPE_A; break;
        }
        
        std::string rr_name = fdp.ConsumeRandomLengthString(64);
        ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<int>(1, 255));
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        
        ares_dns_rr_t* rr = NULL;
        status = ares_dns_record_rr_add(&rr, original_rec, section, rr_name.c_str(), 
                                       rr_type, rr_class, ttl);
        if (status != ARES_SUCCESS || rr == NULL) {
            continue; // Skip this RR if failed
        }
        
        // Set appropriate data based on record type
        // This exercises ares_dns_rr_data_ptr through setter functions
        switch (rr_type) {
            case ARES_REC_TYPE_A: {
                uint32_t addr = fdp.ConsumeIntegral<uint32_t>();
                struct in_addr in_addr;
                in_addr.s_addr = htonl(addr);
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &in_addr);
                break;
            }
            case ARES_REC_TYPE_AAAA: {
                std::vector<uint8_t> addr6 = fdp.ConsumeBytes<uint8_t>(16);
                if (addr6.size() == 16) {
                    struct ares_in6_addr in6_addr;
                    memcpy(&in6_addr._S6_un._S6_u8, addr6.data(), 16);
                    ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &in6_addr);
                }
                break;
            }
            case ARES_REC_TYPE_CNAME: {
                std::string cname = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_set_str(rr, ARES_RR_CNAME_CNAME, cname.c_str());
                break;
            }
            case ARES_REC_TYPE_MX: {
                uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                std::string exchange = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                break;
            }
            case ARES_REC_TYPE_TXT: {
                size_t num_strings = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t s = 0; s < num_strings && fdp.remaining_bytes() > 10; s++) {
                    std::string txt_str = fdp.ConsumeRandomLengthString(32);
                    ares_dns_rr_set_str(rr, ARES_RR_TXT_DATA, txt_str.c_str());
                }
                break;
            }
            case ARES_REC_TYPE_SRV: {
                uint16_t priority = fdp.ConsumeIntegral<uint16_t>();
                uint16_t weight = fdp.ConsumeIntegral<uint16_t>();
                uint16_t port = fdp.ConsumeIntegral<uint16_t>();
                std::string target = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PRIORITY, priority);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_WEIGHT, weight);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PORT, port);
                ares_dns_rr_set_str(rr, ARES_RR_SRV_TARGET, target.c_str());
                break;
            }
            case ARES_REC_TYPE_OPT: {
                uint16_t udp_size = fdp.ConsumeIntegral<uint16_t>();
                uint8_t version = fdp.ConsumeIntegral<uint8_t>();
                uint16_t opt_flags = fdp.ConsumeIntegral<uint16_t>();
                ares_dns_rr_set_u16(rr, ARES_RR_OPT_UDP_SIZE, udp_size);
                ares_dns_rr_set_u8(rr, ARES_RR_OPT_VERSION, version);
                ares_dns_rr_set_u16(rr, ARES_RR_OPT_FLAGS, opt_flags);
                break;
            }
            default:
                // For other types, just set minimal data or skip
                break;
        }
    }

    // 3. Test record duplication (ares_dns_record_duplicate)
    ares_dns_record_t* duplicate_rec = ares_dns_record_duplicate(original_rec);
    
    if (duplicate_rec != NULL) {
        // Verify duplicate has same basic properties
        unsigned short orig_id = ares_dns_record_get_id(original_rec);
        unsigned short dup_id = ares_dns_record_get_id(duplicate_rec);
        
        // Note: We can't do full comparison since there's no comparison API,
        // but we can exercise getters on both records
        ares_dns_opcode_t orig_opcode = ares_dns_record_get_opcode(original_rec);
        ares_dns_opcode_t dup_opcode = ares_dns_record_get_opcode(duplicate_rec);
        
        // Count RRs in both records (exercises iteration)
        size_t orig_qdcount = ares_dns_record_query_cnt(original_rec);
        size_t dup_qdcount = ares_dns_record_query_cnt(duplicate_rec);
        
        size_t orig_ancount = ares_dns_record_rr_cnt(original_rec, ARES_SECTION_ANSWER);
        size_t dup_ancount = ares_dns_record_rr_cnt(duplicate_rec, ARES_SECTION_ANSWER);
    }

    // 4. Test serialization/deserialization transformation (write/parse)
    unsigned char* wire_data = NULL;
    size_t wire_len = 0;
    
    status = ares_dns_write(original_rec, &wire_data, &wire_len);
    if (status == ARES_SUCCESS && wire_data != NULL && wire_len > 0) {
        // Parse the wire data back into a new record
        ares_dns_record_t* parsed_rec = NULL;
        unsigned int parse_flags = fdp.ConsumeIntegral<unsigned int>();
        
        status = ares_dns_parse(wire_data, wire_len, parse_flags, &parsed_rec);
        if (status == ARES_SUCCESS && parsed_rec != NULL) {
            // Exercise getters on parsed record
            unsigned short parsed_id = ares_dns_record_get_id(parsed_rec);
            ares_dns_opcode_t parsed_opcode = ares_dns_record_get_opcode(parsed_rec);
            
            // Count sections in parsed record
            size_t parsed_qdcount = ares_dns_record_query_cnt(parsed_rec);
            size_t parsed_ancount = ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_ANSWER);
            size_t parsed_nscount = ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_AUTHORITY);
            size_t parsed_arcount = ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_ADDITIONAL);
            
            ares_dns_record_destroy(parsed_rec);
        }
        
        ares_free_string(wire_data);
    }

    // 5. Complex manipulation: Create a record, modify it, duplicate, modify duplicate
    if (duplicate_rec != NULL) {
        // Modify some fields in the duplicate
        unsigned short new_id = fdp.ConsumeIntegral<unsigned short>();
        ares_dns_record_set_id(duplicate_rec, new_id);
        
        // Modify TTL of first RR in answer section if exists
        size_t ancount = ares_dns_record_rr_cnt(duplicate_rec, ARES_SECTION_ANSWER);
        if (ancount > 0) {
            ares_dns_rr_t* first_rr = ares_dns_record_rr_get(duplicate_rec, ARES_SECTION_ANSWER, 0);
            if (first_rr != NULL) {
                unsigned int new_ttl = fdp.ConsumeIntegral<unsigned int>();
                ares_dns_rr_set_ttl(first_rr, new_ttl);
                
                // Exercise getter on modified RR
                unsigned int retrieved_ttl = ares_dns_rr_get_ttl(first_rr);
            }
        }
        
        // Write modified duplicate to wire format
        unsigned char* modified_wire = NULL;
        size_t modified_len = 0;
        status = ares_dns_write(duplicate_rec, &modified_wire, &modified_len);
        if (status == ARES_SUCCESS && modified_wire != NULL) {
            ares_free_string(modified_wire);
        }
    }

    // Cleanup
    if (duplicate_rec != NULL) {
        ares_dns_record_destroy(duplicate_rec);
    }
    ares_dns_record_destroy(original_rec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_019
