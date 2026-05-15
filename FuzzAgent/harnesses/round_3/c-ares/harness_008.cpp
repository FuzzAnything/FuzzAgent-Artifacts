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

namespace harness_008 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for initialization and basic parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Create a new DNS record
    ares_dns_record_t *dns_record = NULL;
    ares_dns_record_create(&dns_record, 0, ARES_FLAG_QR, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (dns_record == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // Step 3: Add a query section
    // Consume domain name for query
    std::string query_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    ares_dns_record_query_add(dns_record, query_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN);
    
    // Step 4: Set query type from fuzzer input
    // Array of common DNS record types to test
    ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_A,
        ARES_REC_TYPE_AAAA,
        ARES_REC_TYPE_MX,
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_CNAME,
        ARES_REC_TYPE_NS,
        ARES_REC_TYPE_SOA,
        ARES_REC_TYPE_PTR,
        ARES_REC_TYPE_SRV,
        ARES_REC_TYPE_CAA,
        ARES_REC_TYPE_URI,
        ARES_REC_TYPE_TLSA,
        ARES_REC_TYPE_SVCB,
        ARES_REC_TYPE_HTTPS
    };
    
    // Choose record type for testing
    size_t type_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(record_types)/sizeof(record_types[0]) - 1);
    ares_dns_rec_type_t selected_type = record_types[type_idx];
    
    // Actually test ares_dns_record_query_set_type
    ares_dns_record_query_set_type(dns_record, 0, selected_type);
    
    // Step 5: Add a resource record to answer section
    // Create resource record with selected type
    // Consume a name for the resource record
    std::string rr_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    ares_dns_rr_t *rr = NULL;
    ares_dns_record_rr_add(&rr, dns_record, ARES_SECTION_ANSWER, rr_name.c_str(), selected_type, ARES_CLASS_IN, 0);
    
    if (rr != NULL) {
        // Step 6: Get keys for the RR type
        size_t key_count = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(selected_type, &key_count);
        
        // Step 7: For each key, get string representation
        if (keys != NULL && key_count > 0) {
            for (size_t i = 0; i < key_count && i < 10; i++) { // Limit to 10 keys for safety
                const char *key_str = ares_dns_rr_key_tostr(keys[i]);
                (void)key_str; // Use to avoid unused variable warning
                
                // Test setting different data types based on key datatype
                ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[i]);
                
                // Consume fuzzer data for different datatypes
                switch (datatype) {
                    case ARES_DATATYPE_U8: {
                        uint8_t val = fdp.ConsumeIntegral<uint8_t>();
                        ares_dns_rr_set_u8(rr, keys[i], val);
                        break;
                    }
                    case ARES_DATATYPE_U16: {
                        uint16_t val = fdp.ConsumeIntegral<uint16_t>();
                        ares_dns_rr_set_u16(rr, keys[i], val);
                        break;
                    }
                    case ARES_DATATYPE_U32: {
                        uint32_t val = fdp.ConsumeIntegral<uint32_t>();
                        ares_dns_rr_set_u32(rr, keys[i], val);
                        break;
                    }
                    case ARES_DATATYPE_STR: {
                        std::string str_val = fdp.ConsumeRandomLengthString(100);
                        ares_dns_rr_set_str(rr, keys[i], str_val.c_str());
                        break;
                    }
                    case ARES_DATATYPE_NAME: {
                        std::string name_val = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
                        ares_dns_rr_set_str(rr, keys[i], name_val.c_str());
                        break;
                    }
                    case ARES_DATATYPE_BIN: {
                        size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                        std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                        if (bin_data.size() == bin_len) {
                            ares_dns_rr_set_bin(rr, keys[i], bin_data.data(), bin_len);
                        }
                        break;
                    }
                    case ARES_DATATYPE_ABINP: {
                        // Step 8: Test ares_dns_rr_add_abin for ABINP datatype
                        size_t abin_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                        std::vector<uint8_t> abin_data = fdp.ConsumeBytes<uint8_t>(abin_len);
                        if (abin_data.size() == abin_len) {
                            ares_dns_rr_add_abin(rr, keys[i], abin_data.data(), abin_len);
                            
                            // Step 9: Test ares_dns_rr_del_abin
                            // Only delete if we successfully added and there's something to delete
                            if (abin_len > 0) {
                                ares_dns_rr_del_abin(rr, keys[i], 0);
                            }
                        }
                        break;
                    }
                    default:
                        // For other datatypes, just continue
                        break;
                }
            }
        }
        
        // Also test some specific record types with their specialized fields
        if (selected_type == ARES_REC_TYPE_A && fdp.remaining_bytes() >= 4) {
            // Test A record with IPv4 address
            std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
            struct in_addr addr;
            memcpy(&addr, ip_bytes.data(), 4);
            ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
        } else if (selected_type == ARES_REC_TYPE_AAAA && fdp.remaining_bytes() >= 16) {
            // Test AAAA record with IPv6 address
            std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            struct ares_in6_addr addr6;
            memcpy(&addr6, ip6_bytes.data(), 16);
            ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
        } else if (selected_type == ARES_REC_TYPE_MX && fdp.remaining_bytes() >= 2) {
            // Test MX record
            uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
            std::string exchange = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
            ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
            ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, exchange.c_str());
        } else if (selected_type == ARES_REC_TYPE_TXT) {
            // Test TXT record with ABINP data
            size_t txt_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            std::vector<uint8_t> txt_data = fdp.ConsumeBytes<uint8_t>(txt_len);
            if (txt_data.size() == txt_len) {
                ares_dns_rr_add_abin(rr, ARES_RR_TXT_DATA, txt_data.data(), txt_len);
            }
        } else if (selected_type == ARES_REC_TYPE_SOA && fdp.remaining_bytes() >= 20) {
            // Test SOA record with various fields
            std::string mname = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
            std::string rname = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
            uint32_t serial = fdp.ConsumeIntegral<uint32_t>();
            uint32_t refresh = fdp.ConsumeIntegral<uint32_t>();
            uint32_t retry = fdp.ConsumeIntegral<uint32_t>();
            uint32_t expire = fdp.ConsumeIntegral<uint32_t>();
            uint32_t minimum = fdp.ConsumeIntegral<uint32_t>();
            
            ares_dns_rr_set_str(rr, ARES_RR_SOA_MNAME, mname.c_str());
            ares_dns_rr_set_str(rr, ARES_RR_SOA_RNAME, rname.c_str());
            ares_dns_rr_set_u32(rr, ARES_RR_SOA_SERIAL, serial);
            ares_dns_rr_set_u32(rr, ARES_RR_SOA_REFRESH, refresh);
            ares_dns_rr_set_u32(rr, ARES_RR_SOA_RETRY, retry);
            ares_dns_rr_set_u32(rr, ARES_RR_SOA_EXPIRE, expire);
            ares_dns_rr_set_u32(rr, ARES_RR_SOA_MINIMUM, minimum);
        }
    }
    ares_dns_rcode_t rcode_values[] = {
        ARES_RCODE_NOERROR,
        ARES_RCODE_FORMERR,
        ARES_RCODE_SERVFAIL,
        ARES_RCODE_NXDOMAIN,
        ARES_RCODE_NOTIMP,
        ARES_RCODE_REFUSED,
        ARES_RCODE_YXDOMAIN,
        ARES_RCODE_YXRRSET,
        ARES_RCODE_NXRRSET,
        ARES_RCODE_NOTAUTH,
        ARES_RCODE_NOTZONE,
        ARES_RCODE_DSOTYPEI,
        ARES_RCODE_BADSIG,
        ARES_RCODE_BADKEY,
        ARES_RCODE_BADTIME,
        ARES_RCODE_BADMODE,
        ARES_RCODE_BADNAME,
        ARES_RCODE_BADALG,
        ARES_RCODE_BADTRUNC,
        ARES_RCODE_BADCOOKIE
    };
    
    size_t rcode_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(rcode_values)/sizeof(rcode_values[0]) - 1);
    const char *rcode_str = ares_dns_rcode_tostr(rcode_values[rcode_idx]);
    (void)rcode_str; // Use to avoid unused variable warning
    
    // Also test some random rcode values outside the defined enum
    uint8_t random_rcode = fdp.ConsumeIntegral<uint8_t>();
    const char *random_rcode_str = ares_dns_rcode_tostr((ares_dns_rcode_t)random_rcode);
    (void)random_rcode_str;
    
    // Step 11: Destroy the DNS record
    ares_dns_record_destroy(dns_record);
    
    // Step 12: Clean up library
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_008
