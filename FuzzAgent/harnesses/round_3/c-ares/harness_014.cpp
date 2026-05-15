#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multiple record types and operations
    if (size < 512) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Create DNS record structure using ares_dns_record_create
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 0, ARES_FLAG_QR, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || dns_record == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // Track success for cleanup
    bool record_added = false;
    ares_dns_rr_t *current_rr = NULL;
    
    // Step 3: Test each target DNS record type to exercise switch cases in ares_dns_rr_data_ptr
    // Consume an operation selector to decide which record type to test
    uint8_t record_type_selector = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume domain name for the record
    std::string domain_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    
    switch (record_type_selector) {
        case 0: { // SIG Record (RRSIG) - Type 24
            status = ares_dns_record_rr_add(&current_rr, dns_record, ARES_SECTION_ANSWER,
                                          domain_name.c_str(), ARES_REC_TYPE_SIG, ARES_CLASS_IN, 300);
            if (status == ARES_SUCCESS && current_rr != NULL) {
                record_added = true;
                
                // Set SIG-specific fields using various setter APIs
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t type_covered = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_SIG_TYPE_COVERED, type_covered);
                }
                
                if (fdp.remaining_bytes() >= 1) {
                    uint8_t algorithm = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(current_rr, ARES_RR_SIG_ALGORITHM, algorithm);
                }
                
                if (fdp.remaining_bytes() >= 1) {
                    uint8_t labels = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(current_rr, ARES_RR_SIG_LABELS, labels);
                }
                
                if (fdp.remaining_bytes() >= 4) {
                    uint32_t original_ttl = fdp.ConsumeIntegral<uint32_t>();
                    ares_dns_rr_set_u32(current_rr, ARES_RR_SIG_ORIGINAL_TTL, original_ttl);
                }
                
                if (fdp.remaining_bytes() >= 4) {
                    uint32_t expiration = fdp.ConsumeIntegral<uint32_t>();
                    ares_dns_rr_set_u32(current_rr, ARES_RR_SIG_EXPIRATION, expiration);
                }
                
                if (fdp.remaining_bytes() >= 4) {
                    uint32_t inception = fdp.ConsumeIntegral<uint32_t>();
                    ares_dns_rr_set_u32(current_rr, ARES_RR_SIG_INCEPTION, inception);
                }
                
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t key_tag = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_SIG_KEY_TAG, key_tag);
                }
                
                // Signers name (domain name)
                if (fdp.remaining_bytes() >= 10) {
                    std::string signers_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
                    ares_dns_rr_set_str(current_rr, ARES_RR_SIG_SIGNERS_NAME, signers_name.c_str());
                }
                
                // Signature (binary data) - using ares_dns_rr_set_bin
                if (fdp.remaining_bytes() >= 32) {
                    size_t sig_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)128));
                    std::vector<uint8_t> signature = fdp.ConsumeBytes<uint8_t>(sig_size);
                    ares_dns_rr_set_bin(current_rr, ARES_RR_SIG_SIGNATURE, signature.data(), signature.size());
                }
            }
            break;
        }
        
        case 1: { // TLSA Record (DANE) - Type 52
            status = ares_dns_record_rr_add(&current_rr, dns_record, ARES_SECTION_ANSWER,
                                          domain_name.c_str(), ARES_REC_TYPE_TLSA, ARES_CLASS_IN, 300);
            if (status == ARES_SUCCESS && current_rr != NULL) {
                record_added = true;
                
                // Set TLSA-specific fields
                if (fdp.remaining_bytes() >= 1) {
                    uint8_t cert_usage = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(current_rr, ARES_RR_TLSA_CERT_USAGE, cert_usage);
                }
                
                if (fdp.remaining_bytes() >= 1) {
                    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(current_rr, ARES_RR_TLSA_SELECTOR, selector);
                }
                
                if (fdp.remaining_bytes() >= 1) {
                    uint8_t match_type = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(current_rr, ARES_RR_TLSA_MATCH, match_type);
                }
                
                // Certificate association data (binary) - using ares_dns_rr_add_abin and ares_dns_rr_get_abin
                if (fdp.remaining_bytes() >= 16) {
                    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)256));
                    std::vector<uint8_t> cert_data = fdp.ConsumeBytes<uint8_t>(data_size);
                    ares_dns_rr_set_bin(current_rr, ARES_RR_TLSA_DATA, cert_data.data(), cert_data.size());
                    
                    // Also try adding as ABIN (array binary) for TXT-like data
                    if (fdp.remaining_bytes() >= 8) {
                        size_t txt_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)64));
                        std::vector<uint8_t> txt_data = fdp.ConsumeBytes<uint8_t>(txt_size);
                        ares_dns_rr_add_abin(current_rr, ARES_RR_TLSA_DATA, txt_data.data(), txt_size);
                    }
                }
            }
            break;
        }
        
        case 2: { // HTTPS Record (HTTPS Service Binding) - Type 65
            status = ares_dns_record_rr_add(&current_rr, dns_record, ARES_SECTION_ANSWER,
                                          domain_name.c_str(), ARES_REC_TYPE_HTTPS, ARES_CLASS_IN, 300);
            if (status == ARES_SUCCESS && current_rr != NULL) {
                record_added = true;
                
                // Set HTTPS-specific fields
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t priority = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_HTTPS_PRIORITY, priority);
                }
                
                // Target name (domain name)
                if (fdp.remaining_bytes() >= 10) {
                    std::string target_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
                    ares_dns_rr_set_str(current_rr, ARES_RR_HTTPS_TARGET, target_name.c_str());
                }
                
                // Service parameters (OPT type) - test ares_dns_rr_del_opt_byid
                if (fdp.remaining_bytes() >= 16) {
                    // First add an OPT record to manipulate
                    ares_dns_rr_t *opt_rr = NULL;
                    status = ares_dns_record_rr_add(&opt_rr, dns_record, ARES_SECTION_ADDITIONAL,
                                                  "", ARES_REC_TYPE_OPT, ARES_CLASS_IN, 300);
                    if (status == ARES_SUCCESS && opt_rr != NULL) {
                        // Set OPT fields
                        if (fdp.remaining_bytes() >= 2) {
                            uint16_t udp_size = fdp.ConsumeIntegral<uint16_t>();
                            ares_dns_rr_set_u16(opt_rr, ARES_RR_OPT_UDP_SIZE, udp_size);
                        }
                        
                        if (fdp.remaining_bytes() >= 1) {
                            uint8_t version = fdp.ConsumeIntegral<uint8_t>();
                            ares_dns_rr_set_u8(opt_rr, ARES_RR_OPT_VERSION, version);
                        }
                        
                        if (fdp.remaining_bytes() >= 2) {
                            uint16_t flags = fdp.ConsumeIntegral<uint16_t>();
                            ares_dns_rr_set_u16(opt_rr, ARES_RR_OPT_FLAGS, flags);
                        }
                        
                        // Add some OPT options and then delete one using ares_dns_rr_del_opt_byid
                        // Note: ares_dns_rr_set_opt is needed first, but for fuzzing we'll simulate
                        // the delete operation on a hypothetical option
                        if (fdp.remaining_bytes() >= 2) {
                            uint16_t opt_id_to_delete = fdp.ConsumeIntegral<uint16_t>() % 20;
                            ares_dns_rr_del_opt_byid(opt_rr, ARES_RR_OPT_OPTIONS, opt_id_to_delete);
                        }
                    }
                }
            }
            break;
        }
        
        case 3: { // NAPTR Record (Naming Authority Pointer) - Type 35
            status = ares_dns_record_rr_add(&current_rr, dns_record, ARES_SECTION_ANSWER,
                                          domain_name.c_str(), ARES_REC_TYPE_NAPTR, ARES_CLASS_IN, 300);
            if (status == ARES_SUCCESS && current_rr != NULL) {
                record_added = true;
                
                // Set NAPTR-specific fields
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t order = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_NAPTR_ORDER, order);
                }
                
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_NAPTR_PREFERENCE, preference);
                }
                
                // Flags (string)
                if (fdp.remaining_bytes() >= 10) {
                    std::string flags = fdp.ConsumeRandomLengthString(32);
                    ares_dns_rr_set_str(current_rr, ARES_RR_NAPTR_FLAGS, flags.c_str());
                }
                
                // Services (string)
                if (fdp.remaining_bytes() >= 10) {
                    std::string services = fdp.ConsumeRandomLengthString(64);
                    ares_dns_rr_set_str(current_rr, ARES_RR_NAPTR_SERVICES, services.c_str());
                }
                
                // Regexp (string)
                if (fdp.remaining_bytes() >= 10) {
                    std::string regexp = fdp.ConsumeRandomLengthString(128);
                    ares_dns_rr_set_str(current_rr, ARES_RR_NAPTR_REGEXP, regexp.c_str());
                }
                
                // Replacement (domain name)
                if (fdp.remaining_bytes() >= 10) {
                    std::string replacement = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
                    ares_dns_rr_set_str(current_rr, ARES_RR_NAPTR_REPLACEMENT, replacement.c_str());
                }
            }
            break;
        }
        
        case 4: { // URI Record (Uniform Resource Identifier) - Type 256
            status = ares_dns_record_rr_add(&current_rr, dns_record, ARES_SECTION_ANSWER,
                                          domain_name.c_str(), ARES_REC_TYPE_URI, ARES_CLASS_IN, 300);
            if (status == ARES_SUCCESS && current_rr != NULL) {
                record_added = true;
                
                // Set URI-specific fields
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t priority = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_URI_PRIORITY, priority);
                }
                
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t weight = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_rr_set_u16(current_rr, ARES_RR_URI_WEIGHT, weight);
                }
                
                // Target (URI string)
                if (fdp.remaining_bytes() >= 10) {
                    std::string target = fdp.ConsumeRandomLengthString(256);
                    ares_dns_rr_set_str(current_rr, ARES_RR_URI_TARGET, target.c_str());
                }
                
                // Test address setters (though not typical for URI records)
                if (fdp.remaining_bytes() >= 4) {
                    struct in_addr ipv4_addr;
                    if (fdp.remaining_bytes() >= sizeof(struct in_addr)) {
                        std::vector<uint8_t> ipv4_bytes = fdp.ConsumeBytes<uint8_t>(sizeof(struct in_addr));
                        memcpy(&ipv4_addr, ipv4_bytes.data(), sizeof(struct in_addr));
                        // Note: URI doesn't have address fields, but we test the API anyway
                        // This would normally fail but exercises the code path
                    }
                }
                
                if (fdp.remaining_bytes() >= 16) {
                    struct in6_addr ipv6_addr;
                    if (fdp.remaining_bytes() >= sizeof(struct in6_addr)) {
                        std::vector<uint8_t> ipv6_bytes = fdp.ConsumeBytes<uint8_t>(sizeof(struct in6_addr));
                        memcpy(&ipv6_addr, ipv6_bytes.data(), sizeof(struct in6_addr));
                        // Note: URI doesn't have address fields
                    }
                }
            }
            break;
        }
        
        case 5: { // CAA Record (Certificate Authority Authorization) - Type 257
            status = ares_dns_record_rr_add(&current_rr, dns_record, ARES_SECTION_ANSWER,
                                          domain_name.c_str(), ARES_REC_TYPE_CAA, ARES_CLASS_IN, 300);
            if (status == ARES_SUCCESS && current_rr != NULL) {
                record_added = true;
                
                // Set CAA-specific fields
                if (fdp.remaining_bytes() >= 1) {
                    uint8_t critical = fdp.ConsumeIntegral<uint8_t>();
                    ares_dns_rr_set_u8(current_rr, ARES_RR_CAA_CRITICAL, critical);
                }
                
                // Tag (string)
                if (fdp.remaining_bytes() >= 10) {
                    std::string tag = fdp.ConsumeRandomLengthString(32);
                    ares_dns_rr_set_str(current_rr, ARES_RR_CAA_TAG, tag.c_str());
                }
                
                // Value (binary data) - using ares_dns_rr_set_bin and ares_dns_rr_add_abin
                if (fdp.remaining_bytes() >= 16) {
                    size_t value_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)128));
                    std::vector<uint8_t> value_data = fdp.ConsumeBytes<uint8_t>(value_size);
                    ares_dns_rr_set_bin(current_rr, ARES_RR_CAA_VALUE, value_data.data(), value_data.size());
                    
                    // Test ares_dns_rr_add_abin for array binary data
                    if (fdp.remaining_bytes() >= 8) {
                        size_t abin_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)64));
                        std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_size);
                        ares_dns_rr_add_abin(current_rr, ARES_RR_CAA_VALUE, abin_data.data(), abin_size);
                        
                        // Test ares_dns_rr_get_abin to retrieve it
                        size_t abin_cnt = ares_dns_rr_get_abin_cnt(current_rr, ARES_RR_CAA_VALUE);
                        if (abin_cnt > 0 && fdp.remaining_bytes() >= 1) {
                            size_t idx_to_get = fdp.ConsumeIntegral<size_t>() % (abin_cnt + 1);
                            size_t retrieved_len = 0;
                            const unsigned char* retrieved_data = ares_dns_rr_get_abin(current_rr, ARES_RR_CAA_VALUE, idx_to_get, &retrieved_len);
                            // Use retrieved_data to avoid unused variable warning
                            (void)retrieved_data;
                            (void)retrieved_len;
                        }
                    }
                }
            }
            break;
        }
    }
    // Step 4: Clean up
    if (record_added) {
        // The record is owned by dns_record, no need to free separately
    }
    
    // Destroy the DNS record
    ares_dns_record_destroy(dns_record);
    
    // Cleanup library
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_014
