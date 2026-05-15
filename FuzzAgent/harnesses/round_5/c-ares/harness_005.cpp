#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_005 {
/* Comprehensive fuzzing harness for c-ares library targeting legacy DNS mapping functions
 * and query creation APIs identified as critical coverage gaps by CoverageAnalyzerAgent:
 * - ares_create_query (deprecated legacy query creation - 500+ undiscovered branches)
 * - ares_mkquery (deprecated legacy query creation - 500+ undiscovered branches)
 * - ares_expand_name (DNS name expansion from wire format)
 * - ares_expand_string (DNS string expansion from wire format)
 * - DNS mapping functions: ares_dns_class_tostr, ares_dns_rec_type_tostr, ares_dns_opcode_tostr
 * - DNS mapping functions: ares_dns_rr_key_tostr, ares_dns_section_tostr, ares_dns_rcode_tostr
 *
 * This harness implements legacy DNS packet creation, name expansion, and comprehensive
 * DNS mapping function testing with semantic diversity from previous harnesses:
 * - harness_000: getaddrinfo and modern APIs
 * - harness_001: DNS processing pipeline
 * - harness_002: event processing and channel management
 * - harness_003: server configuration APIs
 * - harness_004: legacy DNS reply parsing
 *
 * Focus: Legacy query creation functions, DNS name/string expansion, DNS mapping functions,
 * proper buffer management, testing edge cases for DNS compression and encoding.
 */


/* DNS type constants for testing */
#define DNS_TYPE_A      1
#define DNS_TYPE_NS     2
#define DNS_TYPE_CNAME  5
#define DNS_TYPE_SOA    6
#define DNS_TYPE_MX     15
#define DNS_TYPE_TXT    16
#define DNS_TYPE_AAAA   28
#define DNS_TYPE_SRV    33
#define DNS_TYPE_NAPTR  35
#define DNS_TYPE_CAA    257

/* DNS class constants */
#define DNS_CLASS_IN    1
#define DNS_CLASS_CH    3  /* CHAOS class */
#define DNS_CLASS_HS    4  /* Hesiod class */

/* Helper to create DNS compressed name encoding for testing expand functions */
static std::vector<uint8_t> create_dns_encoded_name(FuzzedDataProvider &fdp) {
    std::vector<uint8_t> encoded;
    
    /* Create a simple DNS name with possible compression */
    std::string domain = fdp.ConsumeRandomLengthString(100);
    if (domain.empty()) {
        domain = "example.com";
    }
    
    const char *p = domain.c_str();
    while (*p) {
        const char *dot = strchr(p, '.');
        size_t len = dot ? (size_t)(dot - p) : strlen(p);
        if (len > 63) len = 63; /* DNS label max length */
        
        encoded.push_back((uint8_t)len);
        for (size_t i = 0; i < len && p[i]; i++) {
            encoded.push_back((uint8_t)p[i]);
        }
        
        p = dot ? dot + 1 : p + len;
        if (!*p) {
            encoded.push_back(0); /* null terminator */
            break;
        }
    }
    
    /* Add possible compression pointer (0xc0 mask) */
    if (fdp.ConsumeBool() && encoded.size() > 2) {
        size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, encoded.size() - 1);
        uint8_t compression_byte = 0xc0 | (offset & 0x3f);
        encoded.push_back(compression_byte);
    }
    
    return encoded;
}

/* Helper to create DNS encoded string for testing */
static std::vector<uint8_t> create_dns_encoded_string(FuzzedDataProvider &fdp) {
    std::vector<uint8_t> encoded;
    
    /* Create length-prefixed string as used in DNS */
    size_t length = fdp.ConsumeIntegralInRange<size_t>(1, 255);
    encoded.push_back((uint8_t)length);
    
    for (size_t i = 0; i < length; i++) {
        encoded.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return encoded;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum size check - need enough data for operation selection and basic parameters */
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);

    /* Consume fixed-size data for operation selection and parameters */
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint16_t query_id = fdp.ConsumeIntegral<uint16_t>();
    uint16_t dns_class_val = fdp.ConsumeIntegral<uint16_t>();
    uint16_t dns_type_val = fdp.ConsumeIntegral<uint16_t>();
    uint8_t rd_flag = fdp.ConsumeBool() ? 1 : 0; /* Recursion desired flag */
    uint16_t max_udp_size = fdp.ConsumeIntegralInRange<uint16_t>(512, 4096);
    /* Consume variable-size data for domain names */
    std::string domain_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    if (domain_name.empty()) {
        domain_name = "example.com";
    }

    /* Operation switch for testing different legacy APIs */
    int result = ARES_SUCCESS;
    
    switch (operation) {
        case 0: {
            /* Test ares_create_query - legacy query creation with max UDP size */
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            result = ares_create_query(domain_name.c_str(), 
                                      (int)dns_class_val, 
                                      (int)dns_type_val,
                                      query_id,
                                      rd_flag,
                                      &query_buf,
                                      &query_buflen,
                                      (int)max_udp_size);
            
            /* Clean up allocated buffer */
            if (query_buf) {
                free(query_buf);
            }
            break;
        }
        
        case 1: {
            /* Test ares_mkquery - legacy query creation without max UDP size */
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            result = ares_mkquery(domain_name.c_str(),
                                 (int)dns_class_val,
                                 (int)dns_type_val,
                                 query_id,
                                 rd_flag,
                                 &query_buf,
                                 &query_buflen);
            
            /* Clean up allocated buffer */
            if (query_buf) {
                free(query_buf);
            }
            break;
        }
        
        case 2: {
            /* Test ares_expand_name - DNS name expansion */
            std::vector<uint8_t> encoded_name = create_dns_encoded_name(fdp);
            std::vector<uint8_t> dns_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            
            if (encoded_name.size() > 0 && dns_buffer.size() > encoded_name.size()) {
                char *expanded_name = NULL;
                long enclen = 0;
                
                /* Use the encoded name within the larger buffer context */
                const unsigned char *encoded_ptr = encoded_name.data();
                const unsigned char *abuf = dns_buffer.data();
                int alen = (int)dns_buffer.size();
                
                result = ares_expand_name(encoded_ptr, abuf, alen, &expanded_name, &enclen);
                
                /* Clean up allocated name */
                if (expanded_name) {
                    free(expanded_name);
                }
            }
            break;
        }
        
        case 3: {
            /* Test ares_expand_string - DNS string expansion */
            std::vector<uint8_t> encoded_string = create_dns_encoded_string(fdp);
            std::vector<uint8_t> dns_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            
            if (encoded_string.size() > 0 && dns_buffer.size() > encoded_string.size()) {
                unsigned char *expanded_string = NULL;
                long enclen = 0;
                
                /* Use the encoded string within the larger buffer context */
                const unsigned char *encoded_ptr = encoded_string.data();
                const unsigned char *abuf = dns_buffer.data();
                int alen = (int)dns_buffer.size();
                
                result = ares_expand_string(encoded_ptr, abuf, alen, &expanded_string, &enclen);
                
                /* Clean up allocated string */
                if (expanded_string) {
                    free(expanded_string);
                }
            }
            break;
        }
        
        case 4: {
            /* Test multiple query creation with different parameters */
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                unsigned char *query_buf = NULL;
                int query_buflen = 0;
                
                /* Consume new parameters for each iteration */
                uint16_t iter_id = fdp.ConsumeIntegral<uint16_t>();
                uint16_t iter_class = fdp.ConsumeIntegral<uint16_t>() % 5;
                uint16_t iter_type = fdp.ConsumeIntegral<uint16_t>() % 20;
                std::string iter_name = fdp.ConsumeRandomLengthString(50);
                
                if (iter_name.empty()) {
                    iter_name = "test";
                }
                
                result = ares_mkquery(iter_name.c_str(),
                                     (int)iter_class,
                                     (int)iter_type,
                                     iter_id,
                                     fdp.ConsumeBool() ? 1 : 0,
                                     &query_buf,
                                     &query_buflen);
                
                if (query_buf) {
                    free(query_buf);
                }
            }
            break;
        }
        
        case 5: {
            /* Test edge cases: empty name, invalid class/type values */
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            /* Test with potentially invalid parameters */
            int test_class = (int)fdp.ConsumeIntegral<uint16_t>(); /* Could be invalid */
            int test_type = (int)fdp.ConsumeIntegral<uint16_t>();  /* Could be invalid */
            
            /* Sometimes use empty string, sometimes valid domain */
            std::string test_name = fdp.ConsumeBool() ? "" : domain_name;
            
            result = ares_create_query(test_name.empty() ? NULL : test_name.c_str(),
                                      test_class,
                                      test_type,
                                      query_id,
                                      rd_flag,
                                      &query_buf,
                                      &query_buflen,
                                      (int)max_udp_size);
            
            if (query_buf) {
                free(query_buf);
            }
            break;
        }
        
        case 6: {
            /* Test DNS mapping functions: tostr conversions */
            ares_dns_class_t class_val = (ares_dns_class_t)(dns_class_val % 256);
            ares_dns_rec_type_t type_val = (ares_dns_rec_type_t)(dns_type_val % 65536);
            ares_dns_opcode_t opcode_val = (ares_dns_opcode_t)(fdp.ConsumeIntegral<uint8_t>() % 6);
            ares_dns_section_t section_val = (ares_dns_section_t)(fdp.ConsumeIntegral<uint8_t>() % 4);
            ares_dns_rcode_t rcode_val = (ares_dns_rcode_t)(fdp.ConsumeIntegral<uint8_t>() % 24);
            
            /* Test all tostr functions */
            const char *class_str = ares_dns_class_tostr(class_val);
            const char *type_str = ares_dns_rec_type_tostr(type_val);
            const char *opcode_str = ares_dns_opcode_tostr(opcode_val);
            const char *section_str = ares_dns_section_tostr(section_val);
            const char *rcode_str = ares_dns_rcode_tostr(rcode_val);
            
            /* Use results to avoid unused variable warnings */
            (void)class_str;
            (void)type_str;
            (void)opcode_str;
            (void)section_str;
            (void)rcode_str;
            break;
        }
        
        case 7: {
            /* Test DNS fromstr conversion functions (parse strings to enums) */
            std::string class_str_input = fdp.ConsumeRandomLengthString(50);
            std::string type_str_input = fdp.ConsumeRandomLengthString(50);
            
            ares_dns_class_t parsed_class;
            ares_dns_rec_type_t parsed_type;
            
            /* Test conversion from string to enum */
            ares_bool_t class_conv_result = ares_dns_class_fromstr(&parsed_class, class_str_input.c_str());
            ares_bool_t type_conv_result = ares_dns_rec_type_fromstr(&parsed_type, type_str_input.c_str());
            
            /* Use results to avoid unused variable warnings */
            (void)class_conv_result;
            (void)type_conv_result;
            break;
        }
        case 8: {
            /* Test DNS RR key tostr function with various keys */
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 2; i++) {
                ares_dns_rr_key_t key_val = (ares_dns_rr_key_t)(fdp.ConsumeIntegral<uint16_t>() % 3000);
                const char *key_str = ares_dns_rr_key_tostr(key_val);
                (void)key_str;
            }
            break;
        }
        
        case 9: {
            /* Comprehensive test: combine query creation with mapping functions */
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            result = ares_create_query(domain_name.c_str(), 
                                      (int)dns_class_val, 
                                      (int)dns_type_val,
                                      query_id,
                                      rd_flag,
                                      &query_buf,
                                      &query_buflen,
                                      (int)max_udp_size);
            
            /* If query was created, also test mapping functions */
            if (result == ARES_SUCCESS) {
                ares_dns_class_t class_val = (ares_dns_class_t)(dns_class_val % 256);
                ares_dns_rec_type_t type_val = (ares_dns_rec_type_t)(dns_type_val % 65536);
                
                const char *class_str = ares_dns_class_tostr(class_val);
                const char *type_str = ares_dns_rec_type_tostr(type_val);
                
                (void)class_str;
                (void)type_str;
            }
            
            /* Clean up allocated buffer */
            if (query_buf) {
                free(query_buf);
            }
            break;
        }
    }

    /* Clean up library */
    ares_library_cleanup();

    return 0;
}

} // namespace harness_005
