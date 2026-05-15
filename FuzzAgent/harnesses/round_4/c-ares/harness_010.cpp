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

namespace harness_010 {
/* Fuzzing harness for c-ares library targeting comprehensive DNS record type testing
 * This harness tests all DNS record types, especially HINFO and SIG records which have 0% coverage
 * Target APIs: ares_dns_record_create, ares_dns_record_rr_add, ares_dns_rr_set_str (for HINFO CPU/OS fields),
 *              ares_dns_rr_set_u16/u32 (for SIG record fields), ares_dns_rr_set_bin (for SIG signature data),
 *              ares_dns_write, ares_dns_parse, ares_dns_record_destroy
 * Helper APIs: ares_library_init, ares_library_cleanup
 * Following invocation sequence from coverage guidance:
 * 1. ares_library_init() to initialize the library.
 * 2. ares_dns_record_create() to create a new DNS record.
 * 3. For each DNS record type (especially HINFO and SIG):
 *    a. Call ares_dns_record_rr_add with appropriate record type.
 *    b. Use type-specific setters:
 *       - For HINFO: ares_dns_rr_set_str for CPU and OS fields.
 *       - For SIG: ares_dns_rr_set_u16/u32 for type_covered, algorithm, labels, original_ttl, expiration, inception, key_tag.
 *       - For SIG: ares_dns_rr_set_bin for signature data.
 * 4. Call ares_dns_write to serialize the record.
 * 5. Call ares_dns_parse to parse it back.
 * 6. Call ares_dns_record_destroy to clean up the record.
 * 7. Call ares_library_cleanup.
 * Differentiates from previous harnesses by focusing on comprehensive DNS record type creation and manipulation,
 * specifically targeting the blocked branches in ares_dns_rr_data_ptr (94/130) and ares_dns_rr_key_datatype (89/118).
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Step 2: Create DNS record with fuzzer-generated parameters
    ares_dns_record_t* dnsrec = NULL;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(fdp.ConsumeIntegralInRange<int>(0, 5));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(fdp.ConsumeIntegralInRange<int>(0, 16));
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Add a query to make it a valid DNS record
    std::string query_name = fdp.ConsumeRandomLengthString(128);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(fdp.ConsumeIntegralInRange<int>(1, 257));
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(fdp.ConsumeIntegralInRange<int>(1, 255));
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }

    // Step 3: Test various DNS record types, especially HINFO (13) and SIG (24)
    // Determine how many RRs to add based on remaining input
    size_t max_rrs = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < max_rrs && fdp.remaining_bytes() > 20; i++) {
        // Choose a record type, with emphasis on HINFO and SIG
        int record_type_choice = fdp.ConsumeIntegralInRange<int>(0, 100);
        ares_dns_rec_type_t rr_type;
        
        if (record_type_choice < 20) {
            rr_type = ARES_REC_TYPE_HINFO;  // 20% chance for HINFO
        } else if (record_type_choice < 40) {
            rr_type = ARES_REC_TYPE_SIG;    // 20% chance for SIG
        } else if (record_type_choice < 50) {
            rr_type = ARES_REC_TYPE_A;      // 10% chance for A
        } else if (record_type_choice < 60) {
            rr_type = ARES_REC_TYPE_AAAA;   // 10% chance for AAAA
        } else if (record_type_choice < 70) {
            rr_type = ARES_REC_TYPE_CNAME;  // 10% chance for CNAME
        } else if (record_type_choice < 80) {
            rr_type = ARES_REC_TYPE_MX;     // 10% chance for MX
        } else if (record_type_choice < 90) {
            rr_type = ARES_REC_TYPE_TXT;    // 10% chance for TXT
        } else {
            rr_type = ARES_REC_TYPE_SRV;    // 10% chance for SRV
        }
        
        // Choose a section (Answer, Authority, Additional)
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<int>(ARES_SECTION_ANSWER, ARES_SECTION_ADDITIONAL));
        
        // Add RR with a name
        std::string rr_name = fdp.ConsumeRandomLengthString(64);
        ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
        
        // Add the RR
        ares_dns_rr_t* rr = NULL;
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        status = ares_dns_record_rr_add(&rr, dnsrec, section, rr_name.c_str(), rr_type, rr_class, ttl);
        if (status != ARES_SUCCESS || rr == NULL) {
            continue;  // Skip to next RR if this one fails
        }

        // Set type-specific fields based on record type
        switch (rr_type) {
            case ARES_REC_TYPE_HINFO: {
                // HINFO has CPU and OS string fields
                std::string cpu = fdp.ConsumeRandomLengthString(32);
                std::string os = fdp.ConsumeRandomLengthString(32);
                
                ares_dns_rr_set_str(rr, ARES_RR_HINFO_CPU, cpu.c_str());
                ares_dns_rr_set_str(rr, ARES_RR_HINFO_OS, os.c_str());
                break;
            }
            
            case ARES_REC_TYPE_SIG: {
                // SIG has multiple numeric fields and a binary signature
                unsigned short type_covered = fdp.ConsumeIntegral<unsigned short>();
                unsigned char algorithm = fdp.ConsumeIntegral<unsigned char>();
                unsigned char labels = fdp.ConsumeIntegral<unsigned char>();
                unsigned int original_ttl = fdp.ConsumeIntegral<unsigned int>();
                unsigned int expiration = fdp.ConsumeIntegral<unsigned int>();
                unsigned int inception = fdp.ConsumeIntegral<unsigned int>();
                unsigned short key_tag = fdp.ConsumeIntegral<unsigned short>();
                
                ares_dns_rr_set_u16(rr, ARES_RR_SIG_TYPE_COVERED, type_covered);
                ares_dns_rr_set_u8(rr, ARES_RR_SIG_ALGORITHM, algorithm);
                ares_dns_rr_set_u8(rr, ARES_RR_SIG_LABELS, labels);
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_ORIGINAL_TTL, original_ttl);
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_EXPIRATION, expiration);
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_INCEPTION, inception);
                ares_dns_rr_set_u16(rr, ARES_RR_SIG_KEY_TAG, key_tag);
                
                // Signer's name
                std::string signer_name = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_set_str(rr, ARES_RR_SIG_SIGNERS_NAME, signer_name.c_str());
                
                // Signature data (binary)
                size_t sig_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<unsigned char> signature = fdp.ConsumeBytes<unsigned char>(sig_size);
                // Check if vector is empty to avoid NULL pointer
                if (signature.empty()) {
                    // Use a dummy buffer with a single zero byte
                    unsigned char dummy = 0;
                    ares_dns_rr_set_bin(rr, ARES_RR_SIG_SIGNATURE, &dummy, 1);
                } else {
                    ares_dns_rr_set_bin(rr, ARES_RR_SIG_SIGNATURE, signature.data(), signature.size());
                }
                break;
            }
            
            case ARES_REC_TYPE_A: {
                // A record has an IPv4 address
                struct in_addr addr;
                unsigned char addr_bytes[4];
                for (int j = 0; j < 4; j++) {
                    addr_bytes[j] = fdp.ConsumeIntegral<unsigned char>();
                }
                memcpy(&addr, addr_bytes, 4);
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                break;
            }
            
            case ARES_REC_TYPE_AAAA: {
                // AAAA record has an IPv6 address
                struct ares_in6_addr addr6;
                unsigned char addr6_bytes[16];
                for (int j = 0; j < 16; j++) {
                    addr6_bytes[j] = fdp.ConsumeIntegral<unsigned char>();
                }
                memcpy(&addr6._S6_un._S6_u8, addr6_bytes, 16);
                ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
                break;
            }
            
            case ARES_REC_TYPE_CNAME: {
                // CNAME record has a target name
                std::string cname = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_set_str(rr, ARES_RR_CNAME_CNAME, cname.c_str());
                break;
            }
            
            case ARES_REC_TYPE_MX: {
                // MX record has preference and exchange
                unsigned short preference = fdp.ConsumeIntegral<unsigned short>();
                std::string exchange = fdp.ConsumeRandomLengthString(64);
                
                ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                break;
            }
            
            case ARES_REC_TYPE_TXT: {
                // TXT record has binary data (array of binary strings)
                // For simplicity, create a single TXT record
                size_t txt_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<unsigned char> txt_data = fdp.ConsumeBytes<unsigned char>(txt_size);
                // Check if vector is empty to avoid NULL pointer
                if (txt_data.empty()) {
                    // Use a dummy buffer with a single zero byte
                    unsigned char dummy = 0;
                    ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, &dummy, 1);
                } else {
                    ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, txt_data.data(), txt_data.size());
                }
                break;
            }
            case ARES_REC_TYPE_SRV: {
                // SRV record has priority, weight, port, and target
                unsigned short priority = fdp.ConsumeIntegral<unsigned short>();
                unsigned short weight = fdp.ConsumeIntegral<unsigned short>();
                unsigned short port = fdp.ConsumeIntegral<unsigned short>();
                std::string target = fdp.ConsumeRandomLengthString(64);
                
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PRIORITY, priority);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_WEIGHT, weight);
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PORT, port);
                ares_dns_rr_set_str(rr, ARES_RR_SRV_TARGET, target.c_str());
                break;
            }
            
            default:
                // For other record types, we'll just add them without setting fields
                // This still exercises the creation and addition code paths
                break;
        }
    }

    // Step 4: Serialize the DNS record
    unsigned char* out_buf = NULL;
    size_t out_len = 0;
    status = ares_dns_write(dnsrec, &out_buf, &out_len);
    
    if (status == ARES_SUCCESS && out_buf != NULL && out_len > 0) {
        // Step 5: Parse it back to test round-trip
        ares_dns_record_t* parsed_rec = NULL;
        unsigned int parse_flags = fdp.ConsumeIntegral<unsigned int>();
        
        ares_dns_parse(out_buf, out_len, parse_flags, &parsed_rec);
        
        if (parsed_rec != NULL) {
            // Clean up parsed record
            ares_dns_record_destroy(parsed_rec);
        }
        
        // Free the output buffer
        ares_free_string(out_buf);
    }

    // Step 6: Clean up original record
    ares_dns_record_destroy(dnsrec);

    // Step 7: Clean up library
    ares_library_cleanup();

    return 0;
}

} // namespace harness_010
