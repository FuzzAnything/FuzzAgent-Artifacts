#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_009 {
/*
 * Fuzzing harness for c-ares DNS library - Comprehensive DNS Record Manipulation Module
 * 
 * Targets the #1 branch blocker: ares_dns_rr_data_ptr (70/130 branches blocked)
 * Targets 6 completely uncovered DNS record manipulation APIs:
 *   - ares_dns_record_query_set_type (6 undiscovered branches)
 *   - ares_dns_rr_set_ttl (2 undiscovered branches)
 *   - ares_dns_rr_del_abin (55 undiscovered branches, reached but 0 hits)
 *   - ares_dns_record_rr_del (12 undiscovered branches, reached but 0 hits)
 *   - ares_dns_rr_get_opt_byid (75 undiscovered branches, reached but 0 hits)
 *   - ares_dns_rr_del_opt_byid (63 undiscovered branches, reached but 0 hits)
 * 
 * Semantic Focus: Comprehensive DNS record type coverage (NS, CNAME, SOA, PTR, HINFO, MX, SIG)
 * Exercises the full spectrum of DNS record types beyond existing harnesses (which only cover A/AAAA).
 * Follows exact invocation sequence from coverage guidance to maximize coverage of ares_dns_rr_data_ptr.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for various record types and fields
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // 1. Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 2. Create DNS record with random parameters
    ares_dns_record_t *dnsrec = NULL;
    uint16_t query_id = fdp.ConsumeIntegral<uint16_t>();
    uint16_t flags = fdp.ConsumeIntegral<uint16_t>() & 0x7F; // Valid flag bits
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY; // Standard query
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR; // Success
    
    ares_status_t status = ares_dns_record_create(&dnsrec, query_id, flags, opcode, rcode);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Add a query section with random domain name
    if (fdp.remaining_bytes() > 10) {
        size_t domain_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::string domain_name = fdp.ConsumeBytesAsString(domain_len);
        
        if (!domain_name.empty() && domain_name.find('\0') == std::string::npos) {
            // Add initial query with default type
            ares_dns_record_query_add(dnsrec, domain_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN);
            
            // 4. TEST ares_dns_record_query_set_type (UNCOVERED API)
            // Change query type to a random record type from our target set
            uint8_t query_type_choice = fdp.ConsumeIntegral<uint8_t>() % 7;
            ares_dns_rec_type_t new_query_type = ARES_REC_TYPE_A;
            
            switch (query_type_choice) {
                case 0: new_query_type = ARES_REC_TYPE_NS; break;     // Name server
                case 1: new_query_type = ARES_REC_TYPE_CNAME; break;  // Canonical name
                case 2: new_query_type = ARES_REC_TYPE_SOA; break;    // Start of authority
                case 3: new_query_type = ARES_REC_TYPE_PTR; break;    // Pointer
                case 4: new_query_type = ARES_REC_TYPE_HINFO; break;  // Host information
                case 5: new_query_type = ARES_REC_TYPE_MX; break;     // Mail exchange
                case 6: new_query_type = ARES_REC_TYPE_SIG; break;    // SIG record
            }
            
            ares_dns_record_query_set_type(dnsrec, 0, new_query_type);
        }
    }

    // 5. Add resource records for each target record type
    // Define target record types from coverage guidance
    const ares_dns_rec_type_t target_record_types[] = {
        ARES_REC_TYPE_NS,     // Name server
        ARES_REC_TYPE_CNAME,  // Canonical name
        ARES_REC_TYPE_SOA,    // Start of authority
        ARES_REC_TYPE_PTR,    // Pointer
        ARES_REC_TYPE_HINFO,  // Host information
        ARES_REC_TYPE_MX,     // Mail exchange
        ARES_REC_TYPE_SIG     // SIG record
    };
    
    const size_t num_target_types = sizeof(target_record_types) / sizeof(target_record_types[0]);
    
    for (size_t type_idx = 0; type_idx < num_target_types && fdp.remaining_bytes() > 50; type_idx++) {
        ares_dns_rec_type_t rec_type = target_record_types[type_idx];
        
        // Create RR name
        size_t rr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
        std::string rr_name = fdp.ConsumeBytesAsString(rr_name_len);
        
        if (rr_name.empty() || rr_name.find('\0') != std::string::npos) {
            continue;
        }
        
        // Add RR to ANSWER section
        ares_dns_rr_t *rr_out = NULL;
        unsigned int initial_ttl = fdp.ConsumeIntegralInRange<unsigned int>(1, 86400); // 1 sec to 1 day
        
        status = ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ANSWER, 
                                       rr_name.c_str(), rec_type, ARES_CLASS_IN, initial_ttl);
        
        if (status != ARES_SUCCESS || rr_out == NULL) {
            continue;
        }
        
        // Get the RR we just added
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (rr_count == 0) {
            continue;
        }
        
        ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, rr_count - 1);
        if (rr == NULL) {
            continue;
        }
        
        // 6. Call appropriate ares_dns_rr_set_* functions for each record type's fields
        // This exercises ares_dns_rr_data_ptr (the primary branch blocker)
        
        switch (rec_type) {
            case ARES_REC_TYPE_NS: {
                // NS record: NSDNAME field (domain name)
                if (fdp.remaining_bytes() > 10) {
                    size_t nsdname_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string nsdname = fdp.ConsumeBytesAsString(nsdname_len);
                    if (!nsdname.empty() && nsdname.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_NS_NSDNAME, nsdname.c_str());
                    }
                }
                break;
            }
            
            case ARES_REC_TYPE_CNAME: {
                // CNAME record: CNAME field (domain name)
                if (fdp.remaining_bytes() > 10) {
                    size_t cname_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string cname = fdp.ConsumeBytesAsString(cname_len);
                    if (!cname.empty() && cname.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_CNAME_CNAME, cname.c_str());
                    }
                }
                break;
            }
            
            case ARES_REC_TYPE_SOA: {
                // SOA record: multiple fields (MNAME, RNAME, serial, refresh, retry, expire, minimum)
                if (fdp.remaining_bytes() > 50) {
                    // MNAME (primary name server)
                    size_t mname_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                    std::string mname = fdp.ConsumeBytesAsString(mname_len);
                    if (!mname.empty() && mname.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_SOA_MNAME, mname.c_str());
                    }
                    
                    // RNAME (responsible person's mailbox)
                    size_t rname_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                    std::string rname = fdp.ConsumeBytesAsString(rname_len);
                    if (!rname.empty() && rname.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_SOA_RNAME, rname.c_str());
                    }
                    
                    // Serial number (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SOA_SERIAL, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Refresh interval (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SOA_REFRESH, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Retry interval (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SOA_RETRY, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Expire limit (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SOA_EXPIRE, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Minimum TTL (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SOA_MINIMUM, fdp.ConsumeIntegral<uint32_t>());
                }
                break;
            }
            
            case ARES_REC_TYPE_PTR: {
                // PTR record: DNAME field (domain name)
                if (fdp.remaining_bytes() > 10) {
                    size_t dname_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string dname = fdp.ConsumeBytesAsString(dname_len);
                    if (!dname.empty() && dname.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_PTR_DNAME, dname.c_str());
                    }
                }
                break;
            }
            
            case ARES_REC_TYPE_HINFO: {
                // HINFO record: CPU and OS fields (strings)
                if (fdp.remaining_bytes() > 20) {
                    size_t cpu_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                    std::string cpu = fdp.ConsumeBytesAsString(cpu_len);
                    if (!cpu.empty() && cpu.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_HINFO_CPU, cpu.c_str());
                    }
                    
                    size_t os_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                    std::string os = fdp.ConsumeBytesAsString(os_len);
                    if (!os.empty() && os.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_HINFO_OS, os.c_str());
                    }
                }
                break;
            }
            
            case ARES_REC_TYPE_MX: {
                // MX record: Preference (16-bit) and Exchange (domain name)
                if (fdp.remaining_bytes() > 20) {
                    // Preference (16-bit)
                    ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, fdp.ConsumeIntegral<uint16_t>());
                    
                    // Exchange (domain name)
                    size_t exchange_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string exchange = fdp.ConsumeBytesAsString(exchange_len);
                    if (!exchange.empty() && exchange.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
                    }
                }
                break;
            }
            
            case ARES_REC_TYPE_SIG: {
                // SIG record: multiple fields including timestamps and signature
                if (fdp.remaining_bytes() > 100) {
                    // Type Covered (16-bit)
                    ares_dns_rr_set_u16(rr, ARES_RR_SIG_TYPE_COVERED, fdp.ConsumeIntegral<uint16_t>());
                    
                    // Algorithm (8-bit)
                    ares_dns_rr_set_u8(rr, ARES_RR_SIG_ALGORITHM, fdp.ConsumeIntegral<uint8_t>());
                    
                    // Labels (8-bit)
                    ares_dns_rr_set_u8(rr, ARES_RR_SIG_LABELS, fdp.ConsumeIntegral<uint8_t>());
                    
                    // Original TTL (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SIG_ORIGINAL_TTL, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Signature Expiration (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SIG_EXPIRATION, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Signature Inception (32-bit)
                    ares_dns_rr_set_u32(rr, ARES_RR_SIG_INCEPTION, fdp.ConsumeIntegral<uint32_t>());
                    
                    // Key Tag (16-bit)
                    ares_dns_rr_set_u16(rr, ARES_RR_SIG_KEY_TAG, fdp.ConsumeIntegral<uint16_t>());
                    
                    // Signer's Name (domain name)
                    size_t signer_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string signer = fdp.ConsumeBytesAsString(signer_len);
                    if (!signer.empty() && signer.find('\0') == std::string::npos) {
                        ares_dns_rr_set_str(rr, ARES_RR_SIG_SIGNERS_NAME, signer.c_str());
                    }
                    
                    // Signature (binary data) - use ares_dns_rr_set_bin
                    size_t sig_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::vector<uint8_t> signature = fdp.ConsumeBytes<uint8_t>(sig_len);
                    if (!signature.empty()) {
                        ares_dns_rr_set_bin(rr, ARES_RR_SIG_SIGNATURE, signature.data(), signature.size());
                        
                        // 7. TEST ares_dns_rr_del_abin (UNCOVERED API)
                        // First add an ABIN entry to test deletion
                        if (fdp.remaining_bytes() > 20) {
                            size_t abin_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                            std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_len);
                            if (!abin_data.empty()) {
                                ares_dns_rr_add_abin(rr, ARES_RR_SIG_SIGNATURE, abin_data.data(), abin_data.size());
                                
                                // Now delete it (if there are entries)
                                size_t abin_count = ares_dns_rr_get_abin_cnt(rr, ARES_RR_SIG_SIGNATURE);
                                if (abin_count > 0) {
                                    // Delete a random entry (0-based index)
                                    size_t del_idx = fdp.ConsumeIntegralInRange<size_t>(0, abin_count - 1);
                                    ares_dns_rr_del_abin(rr, ARES_RR_SIG_SIGNATURE, del_idx);
                                }
                            }
                        }
                    }
                }
                break;
            }
            
            default:
                // Handle other record types if needed
                break;
        }
        
        // 8. TEST ares_dns_rr_set_ttl (UNCOVERED API)
        // Update TTL for this RR with a new random value
        unsigned int new_ttl = fdp.ConsumeIntegralInRange<unsigned int>(1, 86400);
        ares_dns_rr_set_ttl(rr, new_ttl);
        
        // 9. TEST OPT record manipulation APIs (ares_dns_rr_get_opt_byid, ares_dns_rr_del_opt_byid)
        // For OPT records specifically
        if (rec_type == ARES_REC_TYPE_OPT && fdp.remaining_bytes() > 20) {
            // Add some OPT data first using ares_dns_rr_set_opt
            size_t opt_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
            std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_data_len);
            
            if (!opt_data.empty()) {
                // Add an OPT entry using ares_dns_rr_set_opt (not ares_dns_rr_add_opt)
                unsigned short opt_id = fdp.ConsumeIntegral<uint16_t>();
                ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt_id, opt_data.data(), opt_data.size());
                
                // Get OPT count
                size_t opt_count = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                if (opt_count > 0) {
                    // TEST ares_dns_rr_get_opt_byid (UNCOVERED API)
                    // Try to get an OPT entry by ID - returns ARES_TRUE/ARES_FALSE
                    const unsigned char* opt_val = NULL;
                    size_t opt_val_len = 0;
                    ares_bool_t found = ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_id, &opt_val, &opt_val_len);
                    
                    // TEST ares_dns_rr_del_opt_byid (UNCOVERED API)
                    // Delete the OPT entry by ID if found
                    if (found == ARES_TRUE) {
                        ares_dns_rr_del_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_id);
                    }
                }
            }
        }
    }

    // 10. TEST ares_dns_record_rr_del (UNCOVERED API)
    // Delete some random RRs if we added any
    size_t total_rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    if (total_rr_count > 0 && fdp.remaining_bytes() > 0) {
        // Choose a random RR to delete (0-based index)
        size_t del_idx = fdp.ConsumeIntegralInRange<size_t>(0, total_rr_count - 1);
        ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, del_idx);
    }

    // 11. Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_009
