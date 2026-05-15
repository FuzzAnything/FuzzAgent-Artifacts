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

namespace harness_017 {


// Specialized harness targeting the 94 blocked branches in ares_dns_rr_key_datatype
// Goal: Systematic testing of ALL DNS record keys to exercise every switch case
// and the default case (return 0) in the ares_dns_rr_key_datatype function

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimal input for key selection and operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // 2. Create a minimal DNS record to enable key operations
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Define ALL DNS record keys from the enum to comprehensively test ares_dns_rr_key_datatype
    // These correspond to all possible values in the switch statement in ares_dns_rr_key_datatype()
    ares_dns_rr_key_t all_keys[] = {
        // A record keys
        ARES_RR_A_ADDR,
        
        // NS record keys
        ARES_RR_NS_NSDNAME,
        
        // CNAME record keys
        ARES_RR_CNAME_CNAME,
        
        // SOA record keys
        ARES_RR_SOA_MNAME,
        ARES_RR_SOA_RNAME,
        ARES_RR_SOA_SERIAL,
        ARES_RR_SOA_REFRESH,
        ARES_RR_SOA_RETRY,
        ARES_RR_SOA_EXPIRE,
        ARES_RR_SOA_MINIMUM,
        
        // PTR record keys
        ARES_RR_PTR_DNAME,
        
        // HINFO record keys
        ARES_RR_HINFO_CPU,
        ARES_RR_HINFO_OS,
        
        // MX record keys
        ARES_RR_MX_PREFERENCE,
        ARES_RR_MX_EXCHANGE,
        
        // SIG record keys
        ARES_RR_SIG_TYPE_COVERED,
        ARES_RR_SIG_ALGORITHM,
        ARES_RR_SIG_LABELS,
        ARES_RR_SIG_ORIGINAL_TTL,
        ARES_RR_SIG_EXPIRATION,
        ARES_RR_SIG_INCEPTION,
        ARES_RR_SIG_KEY_TAG,
        ARES_RR_SIG_SIGNERS_NAME,
        ARES_RR_SIG_SIGNATURE,
        
        // TXT record keys
        ARES_RR_TXT_DATA,
        
        // AAAA record keys
        ARES_RR_AAAA_ADDR,
        
        // SRV record keys
        ARES_RR_SRV_PRIORITY,
        ARES_RR_SRV_WEIGHT,
        ARES_RR_SRV_PORT,
        ARES_RR_SRV_TARGET,
        
        // NAPTR record keys
        ARES_RR_NAPTR_ORDER,
        ARES_RR_NAPTR_PREFERENCE,
        ARES_RR_NAPTR_FLAGS,
        ARES_RR_NAPTR_SERVICES,
        ARES_RR_NAPTR_REGEXP,
        ARES_RR_NAPTR_REPLACEMENT,
        
        // OPT record keys
        ARES_RR_OPT_UDP_SIZE,
        ARES_RR_OPT_VERSION,
        ARES_RR_OPT_FLAGS,
        ARES_RR_OPT_OPTIONS,
        
        // TLSA record keys
        ARES_RR_TLSA_CERT_USAGE,
        ARES_RR_TLSA_SELECTOR,
        ARES_RR_TLSA_MATCH,
        ARES_RR_TLSA_DATA,
        
        // SVCB record keys
        ARES_RR_SVCB_PRIORITY,
        ARES_RR_SVCB_TARGET,
        ARES_RR_SVCB_PARAMS,
        
        // HTTPS record keys
        ARES_RR_HTTPS_PRIORITY,
        ARES_RR_HTTPS_TARGET,
        ARES_RR_HTTPS_PARAMS,
        
        // URI record keys
        ARES_RR_URI_PRIORITY,
        ARES_RR_URI_WEIGHT,
        ARES_RR_URI_TARGET,
        
        // CAA record keys
        ARES_RR_CAA_CRITICAL,
        ARES_RR_CAA_TAG,
        ARES_RR_CAA_VALUE,
        
        // RAW_RR record keys
        ARES_RR_RAW_RR_TYPE,
        ARES_RR_RAW_RR_DATA
    };
    
    size_t num_keys = sizeof(all_keys) / sizeof(all_keys[0]);
    
    // 4. Test each key individually with ares_dns_rr_key_datatype
    // Primary goal: Exercise every branch in the switch statement
    for (size_t key_idx = 0; key_idx < num_keys && fdp.remaining_bytes() > 16; key_idx++) {
        ares_dns_rr_key_t key = all_keys[key_idx];
        
        // PRIMARY TARGET: Call ares_dns_rr_key_datatype to exercise switch cases
        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
        
        // Store/use the datatype to prevent compiler optimization
        volatile ares_dns_datatype_t volatile_datatype = datatype;
        (void)volatile_datatype;
        
        // For debugging/logging purposes in fuzzing
        const char* key_str = ares_dns_rr_key_tostr(key);
        (void)key_str;
    }
    
    // 5. Test edge cases and invalid keys
    // Goal: Exercise the default case (return 0) in ares_dns_rr_key_datatype
    
    // Test with intentionally invalid key values
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 4; i++) {
        // Generate random key values that might be outside the enum range
        ares_dns_rr_key_t random_key = static_cast<ares_dns_rr_key_t>(
            fdp.ConsumeIntegralInRange<int>(1000, 2000)); // Values outside normal enum range
        
        ares_dns_datatype_t invalid_datatype = ares_dns_rr_key_datatype(random_key);
        (void)invalid_datatype; // Should be 0 (default case)
        
        // Also test boundary values around known enum values
        for (int offset = -5; offset <= 5; offset++) {
            if (offset == 0) continue; // Skip the valid cases we already tested
            
            for (size_t base_key_idx = 0; base_key_idx < num_keys && base_key_idx < 5; base_key_idx++) {
                ares_dns_rr_key_t boundary_key = static_cast<ares_dns_rr_key_t>(
                    all_keys[base_key_idx] + offset);
                
                ares_dns_datatype_t boundary_datatype = ares_dns_rr_key_datatype(boundary_key);
                (void)boundary_datatype;
            }
        }
    }
    
    // 6. Test datatype-specific operations with a simple DNS record
    // Add a simple A record to test actual API usage
    if (fdp.remaining_bytes() > 32) {
        // Choose a section for the RR
        ares_dns_section_t section = ARES_SECTION_ANSWER;
        
        // Create a simple domain name
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        if (fdp.remaining_bytes() >= name_len) {
            std::string rr_name = fdp.ConsumeBytesAsString(name_len);
            
            // Add an A record
            ares_dns_rr_t *dns_rr = NULL;
            status = ares_dns_record_rr_add(&dns_rr, dnsrec, section, 
                                           rr_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN, 3600);
            
            if (status == ARES_SUCCESS && dns_rr != NULL) {
                // Get keys for A record (should be just ARES_RR_A_ADDR)
                size_t key_count = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_count);
                
                if (keys != NULL && key_count > 0) {
                    ares_dns_rr_key_t a_key = keys[0]; // ARES_RR_A_ADDR
                    
                    // Call ares_dns_rr_key_datatype for this specific key
                    ares_dns_datatype_t a_datatype = ares_dns_rr_key_datatype(a_key);
                    (void)a_datatype; // Should be ARES_DATATYPE_INADDR
                    
                    // Test corresponding setter/getter if we have data
                    if (fdp.remaining_bytes() >= 4) {
                        std::vector<uint8_t> addr_data = fdp.ConsumeBytes<uint8_t>(4);
                        struct in_addr addr;
                        memcpy(&addr, addr_data.data(), 4);
                        
                        // Test setter
                        ares_dns_rr_set_addr(dns_rr, a_key, &addr);
                        
                        // Test getter
                        const struct in_addr* retrieved_addr = ares_dns_rr_get_addr(dns_rr, a_key);
                        (void)retrieved_addr;
                    }
                }
            }
        }
    }
    
    // 7. Test with a TXT record to exercise ABINP datatype path
    if (fdp.remaining_bytes() > 64) {
        ares_dns_section_t section = ARES_SECTION_ANSWER;
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        
        if (fdp.remaining_bytes() >= name_len) {
            std::string txt_name = fdp.ConsumeBytesAsString(name_len);
            
            ares_dns_rr_t *txt_rr = NULL;
            status = ares_dns_record_rr_add(&txt_rr, dnsrec, section, 
                                           txt_name.c_str(), ARES_REC_TYPE_TXT, ARES_CLASS_IN, 3600);
            
            if (status == ARES_SUCCESS && txt_rr != NULL) {
                // Get TXT record key (ARES_RR_TXT_DATA)
                size_t key_count = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_TXT, &key_count);
                
                if (keys != NULL && key_count > 0) {
                    ares_dns_rr_key_t txt_key = keys[0]; // ARES_RR_TXT_DATA
                    
                    // Call ares_dns_rr_key_datatype for TXT key
                    ares_dns_datatype_t txt_datatype = ares_dns_rr_key_datatype(txt_key);
                    (void)txt_datatype; // Should be ARES_DATATYPE_ABINP
                    
                    // Test ABINP operations if we have data
                    if (fdp.remaining_bytes() > 16) {
                        size_t txt_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        if (fdp.remaining_bytes() >= txt_len) {
                            std::vector<uint8_t> txt_data = fdp.ConsumeBytes<uint8_t>(txt_len);
                            
                            // Test ares_dns_rr_add_abin
                            ares_dns_rr_add_abin(txt_rr, txt_key, txt_data.data(), txt_data.size());
                            
                            // Test ares_dns_rr_get_abin_cnt
                            size_t abin_count = ares_dns_rr_get_abin_cnt(txt_rr, txt_key);
                            (void)abin_count;
                            
                            // Test ares_dns_rr_get_abin if we have entries
                            if (abin_count > 0) {
                                size_t get_len = 0;
                                const uint8_t* get_data = ares_dns_rr_get_abin(txt_rr, txt_key, 0, &get_len);
                                (void)get_data;
                                (void)get_len;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 8. Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_017
