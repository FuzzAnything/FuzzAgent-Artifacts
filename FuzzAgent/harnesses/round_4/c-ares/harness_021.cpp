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

namespace harness_021 {
/* Fuzzing harness for c-ares library targeting DNS record construction and manipulation APIs
 * to unblock branches in ares_dns_rr_data_ptr (124/130 branches blocked).
 * 
 * Coverage analysis identified ares_dns_rr_data_ptr as #1 coverage blocker.
 * This harness focuses on construction paths rather than parsing paths to exercise
 * the internal ares_dns_rr_data_ptr function through record creation and manipulation.
 * 
 * Strategy: Comprehensive DNS record construction with:
 * 1. Creation of DNS records with various configurations
 * 2. Addition of all supported resource record types
 * 3. Use of all ares_dns_rr_set_*() functions to manipulate RR data
 * 4. Testing different DNS sections (Answer, Authority, Additional)
 * 5. Edge cases in TTL, class, and record values
 * 
 * Target APIs (construction path to ares_dns_rr_data_ptr):
 * - ares_dns_record_create()
 * - ares_dns_record_rr_add()
 * - ares_dns_rr_set_addr() / ares_dns_rr_set_addr6()
 * - ares_dns_rr_set_str() / ares_dns_rr_set_name()
 * - ares_dns_rr_set_u8() / ares_dns_rr_set_u16() / ares_dns_rr_set_u32()
 * - ares_dns_rr_set_bin() / ares_dns_rr_add_abin()
 * - ares_dns_rr_set_opt()
 * - ares_dns_rr_set_ttl()
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_020: Focuses on legacy reply parsing APIs (parsing path)
 * - This harness: Focuses on record construction and manipulation (construction path)
 *   directly targeting ares_dns_rr_data_ptr through setter functions
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need reasonable input for DNS record construction
    if (size < 50) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Create DNS record with fuzzer parameters
    ares_dns_record_t* dnsrec = NULL;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 5));  // 0-5 valid opcodes
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<int>(0, 16)); // 0-16 valid rcodes
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Add query section (required for valid DNS record)
    std::string query_name = fdp.ConsumeRandomLengthString(64);
    ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
        fdp.ConsumeIntegralInRange<int>(1, 257));  // 1-257 valid DNS types
    ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<int>(1, 255));
    
    // Validate class - only standard DNS classes
    uint16_t qclass_raw = static_cast<uint16_t>(qclass);
    if (qclass_raw != 1 &&     // ARES_CLASS_IN
        qclass_raw != 3 &&     // ARES_CLASS_CHAOS  
        qclass_raw != 4 &&     // ARES_CLASS_HESIOD
        qclass_raw != 254 &&   // ARES_CLASS_NONE
        qclass_raw != 255) {   // ARES_CLASS_ANY
        qclass = ARES_CLASS_IN; // Default to IN class
    }
    
    status = ares_dns_record_query_add(dnsrec, query_name.c_str(), qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_library_cleanup();
        return 0;
    }

    // Determine number of RRs to add based on remaining input
    size_t max_rrs = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    if (max_rrs > fdp.remaining_bytes() / 10) {
        max_rrs = fdp.remaining_bytes() / 10;
    }
    if (max_rrs < 1) {
        max_rrs = 1;
    }

    // Add various resource records to exercise ares_dns_rr_data_ptr through construction
    for (size_t i = 0; i < max_rrs && fdp.remaining_bytes() > 10; i++) {
        ares_dns_rr_t* rr = NULL;
        
        // Choose section
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<int>(1, 3));
        
        // Generate record parameters from fuzzer input
        std::string rr_name = fdp.ConsumeRandomLengthString(32);
        unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
        ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
            fdp.ConsumeIntegralInRange<int>(1, 255));
        
        // Validate class
        uint16_t rr_class_raw = static_cast<uint16_t>(rr_class);
        if (rr_class_raw != 1 && rr_class_raw != 3 && rr_class_raw != 4 && 
            rr_class_raw != 254 && rr_class_raw != 255) {
            rr_class = ARES_CLASS_IN;
        }

        // Choose record type to test different ares_dns_rr_set_* functions
        uint8_t rr_type_choice = fdp.ConsumeIntegral<uint8_t>() % 18;
        ares_dns_rec_type_t rr_type;
        
        switch (rr_type_choice) {
            case 0: rr_type = ARES_REC_TYPE_A; break;
            case 1: rr_type = ARES_REC_TYPE_AAAA; break;
            case 2: rr_type = ARES_REC_TYPE_MX; break;
            case 3: rr_type = ARES_REC_TYPE_CNAME; break;
            case 4: rr_type = ARES_REC_TYPE_TXT; break;
            case 5: rr_type = ARES_REC_TYPE_SIG; break;
            case 6: rr_type = ARES_REC_TYPE_NS; break;
            case 7: rr_type = ARES_REC_TYPE_SOA; break;
            case 8: rr_type = ARES_REC_TYPE_OPT; break;
            case 9: rr_type = ARES_REC_TYPE_PTR; break;
            case 10: rr_type = ARES_REC_TYPE_HINFO; break;
            case 11: rr_type = ARES_REC_TYPE_SRV; break;
            case 12: rr_type = ARES_REC_TYPE_TLSA; break;
            case 13: rr_type = ARES_REC_TYPE_SVCB; break;
            case 14: rr_type = ARES_REC_TYPE_HTTPS; break;
            case 15: rr_type = ARES_REC_TYPE_URI; break;
            case 16: rr_type = ARES_REC_TYPE_CAA; break;
            case 17: rr_type = ARES_REC_TYPE_NAPTR; break;
            default: rr_type = ARES_REC_TYPE_A; break;
        }

        // Add the resource record
        status = ares_dns_record_rr_add(&rr, dnsrec, section, 
                                       rr_name.c_str(), rr_type, rr_class, ttl);
        if (status != ARES_SUCCESS || rr == NULL) {
            continue; // Try next RR
        }

        // Set TTL (exercises ares_dns_rr_data_ptr for TTL field)
        unsigned int new_ttl = fdp.ConsumeIntegral<unsigned int>();
        ares_dns_rr_set_ttl(rr, new_ttl);

        // Set record-specific data based on type
        switch (rr_type) {
            case ARES_REC_TYPE_A: {
                // Set A record address
                struct in_addr addr;
                if (fdp.remaining_bytes() >= 4) {
                    std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
                    memcpy(&addr, addr_bytes.data(), 4);
                    ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                }
                break;
            }
            
            case ARES_REC_TYPE_AAAA: {
                // Set AAAA record address
                struct ares_in6_addr addr6;
                if (fdp.remaining_bytes() >= 16) {
                    std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(16);
                    memcpy(&addr6, addr_bytes.data(), 16);
                    ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
                }
                break;
            }
            
            case ARES_REC_TYPE_MX: {
                // Set MX record preference and exchange
                uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                
                std::string exchange = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                break;
            }
            
            case ARES_REC_TYPE_CNAME: {
                // Set CNAME record
                std::string cname = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_CNAME_CNAME, cname.c_str());
                break;
            }
            
            case ARES_REC_TYPE_TXT: {
                // Set TXT record data (ABINP datatype)
                size_t txt_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (txt_len > fdp.remaining_bytes()) {
                    txt_len = fdp.remaining_bytes();
                }
                if (txt_len > 0) {
                    std::vector<uint8_t> txt_data = fdp.ConsumeBytes<uint8_t>(txt_len);
                    ares_dns_rr_add_abin(rr, ARES_RR_TXT_DATA, txt_data.data(), txt_len);
                }
                break;
            }
            
            case ARES_REC_TYPE_SIG: {
                // Set SIG record fields (exercises multiple setter types)
                ares_dns_rr_set_u16(rr, ARES_RR_SIG_TYPE_COVERED, 
                                   fdp.ConsumeIntegral<uint16_t>());
                ares_dns_rr_set_u8(rr, ARES_RR_SIG_ALGORITHM, 
                                  fdp.ConsumeIntegral<uint8_t>());
                ares_dns_rr_set_u8(rr, ARES_RR_SIG_LABELS, 
                                  fdp.ConsumeIntegral<uint8_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_ORIGINAL_TTL, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_EXPIRATION, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SIG_INCEPTION, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u16(rr, ARES_RR_SIG_KEY_TAG, 
                                   fdp.ConsumeIntegral<uint16_t>());
                
                std::string signer = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_SIG_SIGNERS_NAME, signer.c_str());
                
                size_t sig_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (sig_len > fdp.remaining_bytes()) {
                    sig_len = fdp.remaining_bytes();
                }
                if (sig_len > 0) {
                    std::vector<uint8_t> sig_data = fdp.ConsumeBytes<uint8_t>(sig_len);
                    ares_dns_rr_set_bin(rr, ARES_RR_SIG_SIGNATURE, 
                                       sig_data.data(), sig_len);
                }
                break;
            }
            
            case ARES_REC_TYPE_NS: {
                // Set NS record
                std::string nsdname = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_NS_NSDNAME, nsdname.c_str());
                break;
            }
            
            case ARES_REC_TYPE_SOA: {
                // Set SOA record fields
                std::string mname = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_SOA_MNAME, mname.c_str());
                
                std::string rname = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_SOA_RNAME, rname.c_str());
                
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_SERIAL, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_REFRESH, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_RETRY, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_EXPIRE, 
                                   fdp.ConsumeIntegral<uint32_t>());
                ares_dns_rr_set_u32(rr, ARES_RR_SOA_MINIMUM, 
                                   fdp.ConsumeIntegral<uint32_t>());
                break;
            }
            
            case ARES_REC_TYPE_OPT: {
                // Set OPT record fields
                ares_dns_rr_set_u16(rr, ARES_RR_OPT_UDP_SIZE, 
                                   fdp.ConsumeIntegral<uint16_t>());
                ares_dns_rr_set_u8(rr, ARES_RR_OPT_VERSION, 
                                  fdp.ConsumeIntegral<uint8_t>());
                ares_dns_rr_set_u16(rr, ARES_RR_OPT_FLAGS, 
                                   fdp.ConsumeIntegral<uint16_t>());
                
                // Add an option
                size_t opt_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (opt_len > fdp.remaining_bytes()) {
                    opt_len = fdp.remaining_bytes();
                }
                if (opt_len > 0) {
                    uint16_t opt_id = fdp.ConsumeIntegral<uint16_t>();
                    std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_len);
                    ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt_id, 
                                       opt_data.data(), opt_data.size());
                }
                break;
            }
            
            case ARES_REC_TYPE_PTR: {
                // Set PTR record
                std::string dname = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_PTR_DNAME, dname.c_str());
                break;
            }
            
            case ARES_REC_TYPE_HINFO: {
                // Set HINFO record
                std::string cpu = fdp.ConsumeRandomLengthString(16);
                ares_dns_rr_set_str(rr, ARES_RR_HINFO_CPU, cpu.c_str());
                
                std::string os = fdp.ConsumeRandomLengthString(16);
                ares_dns_rr_set_str(rr, ARES_RR_HINFO_OS, os.c_str());
                break;
            }
            
            case ARES_REC_TYPE_SRV: {
                // Set SRV record
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PRIORITY, 
                                   fdp.ConsumeIntegral<uint16_t>());
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_WEIGHT, 
                                   fdp.ConsumeIntegral<uint16_t>());
                ares_dns_rr_set_u16(rr, ARES_RR_SRV_PORT, 
                                   fdp.ConsumeIntegral<uint16_t>());
                
                std::string target = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_SRV_TARGET, target.c_str());
                break;
            }
            
            case ARES_REC_TYPE_TLSA: {
                // Set TLSA record
                ares_dns_rr_set_u8(rr, ARES_RR_TLSA_CERT_USAGE, 
                                  fdp.ConsumeIntegral<uint8_t>());
                ares_dns_rr_set_u8(rr, ARES_RR_TLSA_SELECTOR, 
                                  fdp.ConsumeIntegral<uint8_t>());
                ares_dns_rr_set_u8(rr, ARES_RR_TLSA_MATCH, 
                                  fdp.ConsumeIntegral<uint8_t>());
                
                size_t tlsa_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (tlsa_len > fdp.remaining_bytes()) {
                    tlsa_len = fdp.remaining_bytes();
                }
                if (tlsa_len > 0) {
                    std::vector<uint8_t> tlsa_data = fdp.ConsumeBytes<uint8_t>(tlsa_len);
                    ares_dns_rr_set_bin(rr, ARES_RR_TLSA_DATA, 
                                       tlsa_data.data(), tlsa_data.size());
                }
                break;
            }
            
            case ARES_REC_TYPE_SVCB:
            case ARES_REC_TYPE_HTTPS: {
                // Set SVCB/HTTPS record (similar structure)
                ares_dns_rr_set_u16(rr, 
                    (rr_type == ARES_REC_TYPE_SVCB) ? ARES_RR_SVCB_PRIORITY : ARES_RR_HTTPS_PRIORITY,
                    fdp.ConsumeIntegral<uint16_t>());
                
                std::string target = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, 
                    (rr_type == ARES_REC_TYPE_SVCB) ? ARES_RR_SVCB_TARGET : ARES_RR_HTTPS_TARGET,
                    target.c_str());
                
                // Add a parameter option
                size_t param_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (param_len > fdp.remaining_bytes()) {
                    param_len = fdp.remaining_bytes();
                }
                if (param_len > 0) {
                    uint16_t param_id = fdp.ConsumeIntegral<uint16_t>();
                    std::vector<uint8_t> param_data = fdp.ConsumeBytes<uint8_t>(param_len);
                    ares_dns_rr_set_opt(rr, 
                        (rr_type == ARES_REC_TYPE_SVCB) ? ARES_RR_SVCB_PARAMS : ARES_RR_HTTPS_PARAMS,
                        param_id, param_data.data(), param_data.size());
                }
                break;
            }
            
            case ARES_REC_TYPE_URI: {
                // Set URI record
                ares_dns_rr_set_u16(rr, ARES_RR_URI_PRIORITY, 
                                   fdp.ConsumeIntegral<uint16_t>());
                ares_dns_rr_set_u16(rr, ARES_RR_URI_WEIGHT, 
                                   fdp.ConsumeIntegral<uint16_t>());
                
                std::string target = fdp.ConsumeRandomLengthString(64);
                ares_dns_rr_set_str(rr, ARES_RR_URI_TARGET, target.c_str());
                break;
            }
            
            case ARES_REC_TYPE_CAA: {
                // Set CAA record
                ares_dns_rr_set_u8(rr, ARES_RR_CAA_CRITICAL, 
                                  fdp.ConsumeIntegral<uint8_t>());
                
                std::string tag = fdp.ConsumeRandomLengthString(8);
                ares_dns_rr_set_str(rr, ARES_RR_CAA_TAG, tag.c_str());
                
                size_t caa_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (caa_len > fdp.remaining_bytes()) {
                    caa_len = fdp.remaining_bytes();
                }
                if (caa_len > 0) {
                    std::vector<uint8_t> caa_data = fdp.ConsumeBytes<uint8_t>(caa_len);
                    ares_dns_rr_set_bin(rr, ARES_RR_CAA_VALUE, 
                                       caa_data.data(), caa_data.size());
                }
                break;
            }
            
            case ARES_REC_TYPE_NAPTR: {
                // Set NAPTR record
                ares_dns_rr_set_u16(rr, ARES_RR_NAPTR_ORDER, 
                                   fdp.ConsumeIntegral<uint16_t>());
                ares_dns_rr_set_u16(rr, ARES_RR_NAPTR_PREFERENCE, 
                                   fdp.ConsumeIntegral<uint16_t>());
                
                std::string flags = fdp.ConsumeRandomLengthString(8);
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_FLAGS, flags.c_str());
                
                std::string services = fdp.ConsumeRandomLengthString(16);
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_SERVICES, services.c_str());
                
                std::string regexp = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_REGEXP, regexp.c_str());
                
                std::string replacement = fdp.ConsumeRandomLengthString(32);
                ares_dns_rr_set_str(rr, ARES_RR_NAPTR_REPLACEMENT, replacement.c_str());
                break;
            }
            
            default:
                // For other types or if we run out of input, just set TTL
                break;
        }
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_021
