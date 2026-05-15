#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_dns_record.h"

namespace harness_014 {
/* Comprehensive fuzzing harness for c-ares library targeting DNS record type
 * manipulation APIs with focus on ares_dns_rr_data_ptr internal function.
 * 
 * The CoverageAnalyzerAgent identified ares_dns_rr_data_ptr as the top branch
 * blocker with 110/130 branches blocked, representing extensive uncovered
 * DNS record type manipulation.
 * 
 * This harness targets comprehensive DNS record type handling, focusing on:
 * - ares_dns_rr_data_ptr internal data access patterns (via public APIs)
 * - DNS record type-specific data access and manipulation
 * - Related DNS record type manipulation APIs
 * 
 * Semantic diversity from existing harnesses:
 * - harness_007: DNS record binary data manipulation (BIN/BINP/ABINP datatypes)
 * - harness_012: DNS mapping string conversion functions
 * - harness_014: DNS record type-specific data access and manipulation (NEW)
 * 
 * This harness comprehensively tests all DNS record type get/set operations
 * using the public API layer that internally exercises ares_dns_rr_data_ptr.
 */



/* Helper function to create and test a specific DNS record type */
static void test_record_type_comprehensive(FuzzedDataProvider& fdp, 
                                          ares_dns_record_t* dnsrec,
                                          ares_dns_section_t section,
                                          ares_dns_rec_type_t rec_type) {
    /* Get keys for this record type */
    size_t key_count = 0;
    const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(rec_type, &key_count);
    if (keys == nullptr || key_count == 0) {
        return;
    }
    
    /* Create RR with random name and class */
    std::string name = fdp.ConsumeRandomLengthString(64);
    ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(
        fdp.ConsumeIntegralInRange<uint16_t>(1, 255));
    
    ares_dns_rr_t* rr = nullptr;
    ares_status_t status = ares_dns_record_rr_add(&rr, dnsrec, section, 
                                                 name.c_str(), rec_type, 
                                                 rr_class, 0);
    if (status != ARES_SUCCESS || rr == nullptr) {
        return;
    }
    
    /* Set TTL */
    unsigned int ttl = fdp.ConsumeIntegral<unsigned int>();
    ares_dns_rr_set_ttl(rr, ttl);
    
    /* Test each key based on its datatype */
    for (size_t i = 0; i < key_count && fdp.remaining_bytes() > 8; i++) {
        ares_dns_rr_key_t key = keys[i];
        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
        
        switch (datatype) {
            case ARES_DATATYPE_INADDR: {
                /* IPv4 address */
                struct in_addr addr;
                if (fdp.remaining_bytes() >= sizeof(struct in_addr)) {
                    std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(
                        sizeof(struct in_addr));
                    memcpy(&addr, addr_bytes.data(), sizeof(struct in_addr));
                    ares_dns_rr_set_addr(rr, key, &addr);
                    
                    /* Test getter */
                    const struct in_addr* retrieved = ares_dns_rr_get_addr(rr, key);
                    (void)retrieved;
                }
                break;
            }
            
            case ARES_DATATYPE_INADDR6: {
                /* IPv6 address */
                struct ares_in6_addr addr6;
                if (fdp.remaining_bytes() >= sizeof(struct ares_in6_addr)) {
                    std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(
                        sizeof(struct ares_in6_addr));
                    memcpy(&addr6, addr_bytes.data(), sizeof(struct ares_in6_addr));
                    ares_dns_rr_set_addr6(rr, key, &addr6);
                    
                    /* Test getter */
                    const struct ares_in6_addr* retrieved = ares_dns_rr_get_addr6(rr, key);
                    (void)retrieved;
                }
                break;
            }
            
            case ARES_DATATYPE_U8: {
                /* 8-bit unsigned integer */
                unsigned char val = fdp.ConsumeIntegral<uint8_t>();
                ares_dns_rr_set_u8(rr, key, val);
                
                /* Test getter */
                unsigned char retrieved = ares_dns_rr_get_u8(rr, key);
                (void)retrieved;
                break;
            }
            
            case ARES_DATATYPE_U16: {
                /* 16-bit unsigned integer */
                unsigned short val = fdp.ConsumeIntegral<uint16_t>();
                ares_dns_rr_set_u16(rr, key, val);
                
                /* Test getter */
                unsigned short retrieved = ares_dns_rr_get_u16(rr, key);
                (void)retrieved;
                break;
            }
            
            case ARES_DATATYPE_U32: {
                /* 32-bit unsigned integer */
                unsigned int val = fdp.ConsumeIntegral<uint32_t>();
                ares_dns_rr_set_u32(rr, key, val);
                
                /* Test getter */
                unsigned int retrieved = ares_dns_rr_get_u32(rr, key);
                (void)retrieved;
                break;
            }
            
            case ARES_DATATYPE_NAME:
            case ARES_DATATYPE_STR: {
                /* String or domain name */
                std::string str_val = fdp.ConsumeRandomLengthString(128);
                ares_dns_rr_set_str(rr, key, str_val.c_str());
                
                /* Test getter */
                const char* retrieved = ares_dns_rr_get_str(rr, key);
                (void)retrieved;
                break;
            }
            
            case ARES_DATATYPE_BIN:
            case ARES_DATATYPE_BINP: {
                /* Binary data */
                size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (fdp.remaining_bytes() >= bin_len) {
                    std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                    ares_dns_rr_set_bin(rr, key, bin_data.data(), bin_len);
                    
                    /* Test getter */
                    size_t retrieved_len = 0;
                    const unsigned char* retrieved = ares_dns_rr_get_bin(rr, key, &retrieved_len);
                    (void)retrieved;
                    (void)retrieved_len;
                }
                break;
            }
            
            case ARES_DATATYPE_ABINP: {
                /* Array of binary data */
                size_t array_count = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                for (size_t j = 0; j < array_count && fdp.remaining_bytes() > 8; j++) {
                    size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    if (fdp.remaining_bytes() >= bin_len) {
                        std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                        ares_dns_rr_add_abin(rr, key, bin_data.data(), bin_len);
                    }
                }
                
                /* Test array count getter */
                size_t count = ares_dns_rr_get_abin_cnt(rr, key);
                (void)count;
                
                /* Test individual element getter */
                if (count > 0) {
                    for (size_t j = 0; j < count && j < 4; j++) {
                        size_t elem_len = 0;
                        const unsigned char* elem = ares_dns_rr_get_abin(rr, key, j, &elem_len);
                        (void)elem;
                        (void)elem_len;
                    }
                }
                break;
            }
            
            case ARES_DATATYPE_OPT: {
                /* Options array */
                size_t opt_count = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                for (size_t j = 0; j < opt_count && fdp.remaining_bytes() > 4; j++) {
                    unsigned short opt_id = fdp.ConsumeIntegral<uint16_t>();
                    size_t opt_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    if (fdp.remaining_bytes() >= opt_len) {
                        std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_len);
                        ares_dns_rr_set_opt(rr, key, opt_id, opt_data.data(), opt_len);
                    }
                }
                
                /* Test options count getter */
                size_t count = ares_dns_rr_get_opt_cnt(rr, key);
                (void)count;
                
                /* Test individual option getter */
                if (count > 0) {
                    for (size_t j = 0; j < count && j < 4; j++) {
                        const unsigned char* opt_data = nullptr;
                        size_t opt_len = 0;
                        unsigned short opt_id = ares_dns_rr_get_opt(rr, key, j, &opt_data, &opt_len);
                        (void)opt_data;
                        (void)opt_id;
                        (void)opt_len;
                    }
                }
                break;
            }
            
            default:
                /* Unknown datatype, skip */
                break;
        }
    }
    
    /* Test basic RR metadata getters */
    const char* rr_name = ares_dns_rr_get_name(rr);
    (void)rr_name;
    
    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
    (void)rr_type;
    
    ares_dns_class_t rr_class_get = ares_dns_rr_get_class(rr);
    (void)rr_class_get;
    
    unsigned int rr_ttl = ares_dns_rr_get_ttl(rr);
    (void)rr_ttl;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    /* Minimum size check */
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);
    
    /* Create DNS record */
    ares_dns_record_t* dnsrec = nullptr;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 5));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 15));
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == nullptr) {
        ares_library_cleanup();
        return 0;
    }
    
    /* Define record types to test */
    ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_A,      /* IPv4 address */
        ARES_REC_TYPE_NS,     /* Name server */
        ARES_REC_TYPE_CNAME,  /* Canonical name */
        ARES_REC_TYPE_SOA,    /* Start of authority */
        ARES_REC_TYPE_PTR,    /* Pointer */
        ARES_REC_TYPE_HINFO,  /* Host info */
        ARES_REC_TYPE_MX,     /* Mail exchange */
        ARES_REC_TYPE_TXT,    /* Text */
        ARES_REC_TYPE_SIG,    /* Signature */
        ARES_REC_TYPE_AAAA,   /* IPv6 address */
        ARES_REC_TYPE_SRV,    /* Service */
        ARES_REC_TYPE_NAPTR,  /* Naming authority pointer */
        ARES_REC_TYPE_OPT,    /* EDNS0 option */
        ARES_REC_TYPE_TLSA,   /* TLSA */
        ARES_REC_TYPE_SVCB,   /* Service binding */
        ARES_REC_TYPE_HTTPS,  /* HTTPS service binding */
        ARES_REC_TYPE_URI,    /* URI */
        ARES_REC_TYPE_CAA,    /* CAA */
        ARES_REC_TYPE_RAW_RR, /* Raw RR */
    };
    
    /* Test each record type while we have input */
    size_t type_index = 0;
    while (fdp.remaining_bytes() > 128 && type_index < sizeof(record_types)/sizeof(record_types[0])) {
        /* Choose section */
        ares_dns_section_t section = static_cast<ares_dns_section_t>(
            fdp.ConsumeIntegralInRange<uint8_t>(1, 3));
        
        /* Test this record type */
        test_record_type_comprehensive(fdp, dnsrec, section, record_types[type_index]);
        
        type_index++;
    }
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_014
