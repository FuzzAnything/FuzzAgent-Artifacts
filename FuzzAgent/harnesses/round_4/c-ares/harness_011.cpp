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

namespace harness_011 {
/* Fuzzing harness for c-ares library targeting DNS record manipulation 
 * focusing on SIG and NAPTR record types as identified in coverage analysis.
 * This harness specifically targets ares_dns_rr_data_ptr which has 96 blocked branches (73.8% blocked).
 * 
 * Target APIs: ares_dns_rr_set_ttl (0% covered, 2 undiscovered branches), 
 *              ares_dns_rr_add_abin, ares_dns_rr_del_abin, ares_dns_rr_set_addr, 
 *              ares_dns_rr_set_addr6, ares_dns_rr_set_u16, ares_dns_rr_set_u32, 
 *              ares_dns_rr_set_u8, ares_dns_rr_get_abin, ares_dns_rr_get_abin_cnt
 * Helper APIs: ares_dns_record_create, ares_dns_record_rr_add, ares_dns_record_destroy
 * 
 * Follows exact invocation sequence from coverage guidance:
 * 1. ares_dns_record_create to create DNS record structure
 * 2. ares_dns_record_rr_add for SIG and NAPTR record types (focus on uncovered types)
 * 3. ares_dns_rr_set_ttl to set TTL values (currently 0% covered)
 * 4. Various setter APIs (ares_dns_rr_set_addr, addr6, u16, u32, u8)
 * 5. Getter APIs (ares_dns_rr_get_abin, ares_dns_rr_get_abin_cnt)
 * 6. ares_dns_rr_add_abin and ares_dns_rr_del_abin for binary data manipulation
 * 7. ares_dns_record_destroy to clean up memory
 * 
 * Differentiates from previous harnesses:
 * - harness_004 focused on TLSA/TXT records with binary/string/address data
 * - harness_010 tested multiple record types including HINFO and SIG
 * - This harness specifically targets SIG and NAPTR record manipulation
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough data for: record creation + RR addition + TTL + various data types
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 1. Call ares_dns_record_create to create a new DNS record
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  fdp.ConsumeIntegral<unsigned short>(),  // id
                                                  fdp.ConsumeIntegral<unsigned short>(),  // flags
                                                  (ares_dns_opcode_t)(fdp.ConsumeIntegral<uint8_t>() % 6),  // opcode
                                                  (ares_dns_rcode_t)(fdp.ConsumeIntegral<uint8_t>() % 16)); // rcode
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

    // 2. Call ares_dns_record_rr_add for SIG and NAPTR record types
    // Choose which record type to create based on fuzzer input
    bool create_sig_record = fdp.ConsumeBool();
    bool create_naptr_record = fdp.ConsumeBool();
    
    ares_dns_rr_t* sig_rr = NULL;
    ares_dns_rr_t* naptr_rr = NULL;
    
    // Create SIG record (type 24) if requested and we have enough data
    if (create_sig_record && fdp.remaining_bytes() > 50) {
        std::string sig_name = fdp.ConsumeRandomLengthString(64);
        unsigned int sig_ttl = fdp.ConsumeIntegral<unsigned int>();
        
        status = ares_dns_record_rr_add(&sig_rr, dnsrec,
                                        ARES_SECTION_ANSWER,
                                        sig_name.c_str(),
                                        ARES_REC_TYPE_SIG,  // SIG record type
                                        ARES_CLASS_IN,
                                        sig_ttl);
        
        if (status == ARES_SUCCESS && sig_rr != NULL) {
            // 3. Call ares_dns_rr_set_ttl to set TTL (currently 0% covered)
            unsigned int new_ttl = fdp.ConsumeIntegral<unsigned int>();
            status = ares_dns_rr_set_ttl(sig_rr, new_ttl);
            // Don't fail on error - continue testing other APIs
            
            // 4. Call various setter APIs for SIG record fields
            // SIG record has: TYPE_COVERED (U16), ALGORITHM (U8), LABELS (U8), 
            // ORIGINAL_TTL (U32), EXPIRATION (U32), INCEPTION (U32), KEY_TAG (U16)
            
            // Set TYPE_COVERED (U16)
            if (fdp.remaining_bytes() >= 2) {
                unsigned short type_covered = fdp.ConsumeIntegral<unsigned short>();
                status = ares_dns_rr_set_u16(sig_rr, ARES_RR_SIG_TYPE_COVERED, type_covered);
            }
            
            // Set ALGORITHM (U8)
            if (fdp.remaining_bytes() >= 1) {
                unsigned char algorithm = fdp.ConsumeIntegral<uint8_t>();
                status = ares_dns_rr_set_u8(sig_rr, ARES_RR_SIG_ALGORITHM, algorithm);
            }
            
            // Set LABELS (U8)
            if (fdp.remaining_bytes() >= 1) {
                unsigned char labels = fdp.ConsumeIntegral<uint8_t>();
                status = ares_dns_rr_set_u8(sig_rr, ARES_RR_SIG_LABELS, labels);
            }
            
            // Set ORIGINAL_TTL (U32)
            if (fdp.remaining_bytes() >= 4) {
                unsigned int original_ttl = fdp.ConsumeIntegral<unsigned int>();
                status = ares_dns_rr_set_u32(sig_rr, ARES_RR_SIG_ORIGINAL_TTL, original_ttl);
            }
            
            // Set EXPIRATION (U32)
            if (fdp.remaining_bytes() >= 4) {
                unsigned int expiration = fdp.ConsumeIntegral<unsigned int>();
                status = ares_dns_rr_set_u32(sig_rr, ARES_RR_SIG_EXPIRATION, expiration);
            }
            
            // Set INCEPTION (U32)
            if (fdp.remaining_bytes() >= 4) {
                unsigned int inception = fdp.ConsumeIntegral<unsigned int>();
                status = ares_dns_rr_set_u32(sig_rr, ARES_RR_SIG_INCEPTION, inception);
            }
            
            // Set KEY_TAG (U16)
            if (fdp.remaining_bytes() >= 2) {
                unsigned short key_tag = fdp.ConsumeIntegral<unsigned short>();
                status = ares_dns_rr_set_u16(sig_rr, ARES_RR_SIG_KEY_TAG, key_tag);
            }
            
            // Set SIGNERS_NAME (NAME datatype)
            if (fdp.remaining_bytes() >= 10) {
                std::string signers_name = fdp.ConsumeRandomLengthString(64);
                status = ares_dns_rr_set_str(sig_rr, ARES_RR_SIG_SIGNERS_NAME, signers_name.c_str());
            }
            
            // Set SIGNATURE (BIN datatype) - binary signature data
            if (fdp.remaining_bytes() >= 20) {
                size_t sig_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                if (fdp.remaining_bytes() >= sig_len) {
                    std::vector<uint8_t> signature = fdp.ConsumeBytes<uint8_t>(sig_len);
                    status = ares_dns_rr_set_bin(sig_rr, ARES_RR_SIG_SIGNATURE, 
                                                signature.data(), signature.size());
                }
            }
        }
    }
    
    // Create NAPTR record (type 35) if requested and we have enough data
    if (create_naptr_record && fdp.remaining_bytes() > 50) {
        std::string naptr_name = fdp.ConsumeRandomLengthString(64);
        unsigned int naptr_ttl = fdp.ConsumeIntegral<unsigned int>();
        
        status = ares_dns_record_rr_add(&naptr_rr, dnsrec,
                                        ARES_SECTION_ANSWER,
                                        naptr_name.c_str(),
                                        ARES_REC_TYPE_NAPTR,  // NAPTR record type
                                        ARES_CLASS_IN,
                                        naptr_ttl);
        
        if (status == ARES_SUCCESS && naptr_rr != NULL) {
            // 3. Call ares_dns_rr_set_ttl to set TTL (currently 0% covered)
            unsigned int new_ttl = fdp.ConsumeIntegral<unsigned int>();
            status = ares_dns_rr_set_ttl(naptr_rr, new_ttl);
            // Don't fail on error - continue testing other APIs
            
            // 4. Call various setter APIs for NAPTR record fields
            // NAPTR record has: ORDER (U16), PREFERENCE (U16), FLAGS (STR),
            // SERVICES (STR), REGEXP (STR), REPLACEMENT (NAME)
            
            // Set ORDER (U16)
            if (fdp.remaining_bytes() >= 2) {
                unsigned short order = fdp.ConsumeIntegral<unsigned short>();
                status = ares_dns_rr_set_u16(naptr_rr, ARES_RR_NAPTR_ORDER, order);
            }
            
            // Set PREFERENCE (U16)
            if (fdp.remaining_bytes() >= 2) {
                unsigned short preference = fdp.ConsumeIntegral<unsigned short>();
                status = ares_dns_rr_set_u16(naptr_rr, ARES_RR_NAPTR_PREFERENCE, preference);
            }
            
            // Set FLAGS (STR)
            if (fdp.remaining_bytes() >= 10) {
                std::string flags = fdp.ConsumeRandomLengthString(32);
                status = ares_dns_rr_set_str(naptr_rr, ARES_RR_NAPTR_FLAGS, flags.c_str());
            }
            
            // Set SERVICES (STR)
            if (fdp.remaining_bytes() >= 10) {
                std::string services = fdp.ConsumeRandomLengthString(64);
                status = ares_dns_rr_set_str(naptr_rr, ARES_RR_NAPTR_SERVICES, services.c_str());
            }
            
            // Set REGEXP (STR)
            if (fdp.remaining_bytes() >= 10) {
                std::string regexp = fdp.ConsumeRandomLengthString(64);
                status = ares_dns_rr_set_str(naptr_rr, ARES_RR_NAPTR_REGEXP, regexp.c_str());
            }
            
            // Set REPLACEMENT (NAME)
            if (fdp.remaining_bytes() >= 10) {
                std::string replacement = fdp.ConsumeRandomLengthString(64);
                status = ares_dns_rr_set_str(naptr_rr, ARES_RR_NAPTR_REPLACEMENT, replacement.c_str());
            }
            
            // 5. Test ares_dns_rr_set_addr and ares_dns_rr_set_addr6
            // These don't apply to NAPTR records directly, but we can test them
            // by creating additional A/AAAA records
            
            // Create an A record to test addr setter
            if (fdp.remaining_bytes() > 30) {
                ares_dns_rr_t* a_rr = NULL;
                std::string a_name = fdp.ConsumeRandomLengthString(32);
                status = ares_dns_record_rr_add(&a_rr, dnsrec,
                                                ARES_SECTION_ADDITIONAL,
                                                a_name.c_str(),
                                                ARES_REC_TYPE_A,
                                                ARES_CLASS_IN,
                                                fdp.ConsumeIntegral<unsigned int>());
                
                if (status == ARES_SUCCESS && a_rr != NULL) {
                    struct in_addr ipv4_addr;
                    if (fdp.remaining_bytes() >= 4) {
                        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
                        memcpy(&ipv4_addr, addr_bytes.data(), 4);
                        status = ares_dns_rr_set_addr(a_rr, ARES_RR_A_ADDR, &ipv4_addr);
                    }
                }
            }
            
            // Create an AAAA record to test addr6 setter
            if (fdp.remaining_bytes() > 50) {
                ares_dns_rr_t* aaaa_rr = NULL;
                std::string aaaa_name = fdp.ConsumeRandomLengthString(32);
                status = ares_dns_record_rr_add(&aaaa_rr, dnsrec,
                                                ARES_SECTION_ADDITIONAL,
                                                aaaa_name.c_str(),
                                                ARES_REC_TYPE_AAAA,
                                                ARES_CLASS_IN,
                                                fdp.ConsumeIntegral<unsigned int>());
                
                if (status == ARES_SUCCESS && aaaa_rr != NULL) {
                    struct ares_in6_addr ipv6_addr;
                    if (fdp.remaining_bytes() >= 16) {
                        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(16);
                        memcpy(&ipv6_addr, addr_bytes.data(), 16);
                        status = ares_dns_rr_set_addr6(aaaa_rr, ARES_RR_AAAA_ADDR, &ipv6_addr);
                    }
                }
            }
            
            // 6. Call getter APIs (ares_dns_rr_get_abin, ares_dns_rr_get_abin_cnt)
            // NAPTR doesn't have ABINP fields, but we can test with TXT record
            
            if (fdp.remaining_bytes() > 40) {
                ares_dns_rr_t* txt_rr = NULL;
                std::string txt_name = fdp.ConsumeRandomLengthString(32);
                status = ares_dns_record_rr_add(&txt_rr, dnsrec,
                                                ARES_SECTION_ADDITIONAL,
                                                txt_name.c_str(),
                                                ARES_REC_TYPE_TXT,
                                                ARES_CLASS_IN,
                                                fdp.ConsumeIntegral<unsigned int>());
                
                if (status == ARES_SUCCESS && txt_rr != NULL) {
                    // 7. Call ares_dns_rr_add_abin for binary data manipulation
                    // TXT records use ARES_RR_TXT_DATA which is ABINP datatype
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                        size_t abin_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                        if (fdp.remaining_bytes() >= abin_len) {
                            std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_len);
                            status = ares_dns_rr_add_abin(txt_rr, ARES_RR_TXT_DATA, 
                                                         abin_data.data(), abin_data.size());
                        }
                    }
                    
                    // 5. Call getter APIs after adding ABIN data
                    if (txt_rr != NULL) {
                        // Get ABIN count
                        size_t abin_cnt = ares_dns_rr_get_abin_cnt(txt_rr, ARES_RR_TXT_DATA);
                        
                        // Get individual ABIN entries if count > 0
                        if (abin_cnt > 0) {
                            size_t get_len = 0;
                            for (size_t idx = 0; idx < abin_cnt && idx < 3; idx++) {
                                const unsigned char* abin_data = ares_dns_rr_get_abin(txt_rr, ARES_RR_TXT_DATA, idx, &get_len);
                                (void)abin_data; // Just retrieve, no need to use
                                (void)get_len;
                            }
                            
                            // 7. Call ares_dns_rr_del_abin for binary data manipulation
                            // Delete an ABIN entry if we have at least one
                            if (abin_cnt > 0) {
                                size_t del_idx = fdp.ConsumeIntegralInRange<size_t>(0, abin_cnt - 1);
                                status = ares_dns_rr_del_abin(txt_rr, ARES_RR_TXT_DATA, del_idx);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Clean up
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
    
    ares_library_cleanup();
    return 0;
}

} // namespace harness_011
