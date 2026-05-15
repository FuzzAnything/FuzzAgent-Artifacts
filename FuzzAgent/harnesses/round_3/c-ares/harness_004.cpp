#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_nameser.h"
#include "ares_dns_record.h"

namespace harness_004 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic DNS record creation
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Test 1: Create a DNS record from scratch and manipulate it
    {
        // Consume parameters for DNS record creation
        uint16_t id = fdp.ConsumeIntegral<uint16_t>();
        uint16_t flags = fdp.ConsumeIntegral<uint16_t>();
        uint8_t opcode_val = fdp.ConsumeIntegral<uint8_t>() % 6; // ARES_OPCODE_ values
        uint8_t rcode_val = fdp.ConsumeIntegral<uint8_t>() % 16; // Valid RCODE range
        
        ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(opcode_val);
        ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(rcode_val);
        
        // Create DNS record
        ares_dns_record_t* dnsrec = NULL;
        ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
        
        if (status == ARES_SUCCESS && dnsrec != NULL) {
            // Test getting record properties
            unsigned short record_id = ares_dns_record_get_id(dnsrec);
            unsigned short record_flags = ares_dns_record_get_flags(dnsrec);
            ares_dns_opcode_t record_opcode = ares_dns_record_get_opcode(dnsrec);
            ares_dns_rcode_t record_rcode = ares_dns_record_get_rcode(dnsrec);
            
            (void)record_id;
            (void)record_flags;
            (void)record_opcode;
            (void)record_rcode;
            
            // Test setting ID
            uint16_t new_id = fdp.ConsumeIntegral<uint16_t>();
            ares_dns_record_set_id(dnsrec, new_id);
            
            // Add queries to the record
            uint8_t query_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
            for (uint8_t i = 0; i < query_count && fdp.remaining_bytes() > 10; i++) {
                // Consume query name
                std::string qname = fdp.ConsumeRandomLengthString(253);
                uint16_t qtype_val = fdp.ConsumeIntegral<uint16_t>() % 65536;
                uint16_t qclass_val = fdp.ConsumeIntegral<uint16_t>() % 256;
                
                ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(qtype_val);
                ares_dns_class_t qclass = static_cast<ares_dns_class_t>(qclass_val);
                
                status = ares_dns_record_query_add(dnsrec, qname.c_str(), qtype, qclass);
                // Don't check status - let it fail naturally if invalid
            }
            
            // Get query count
            size_t actual_query_cnt = ares_dns_record_query_cnt(dnsrec);
            
            // Try to get query details if available
            for (size_t i = 0; i < actual_query_cnt && i < 5; i++) {
                const char* name = NULL;
                ares_dns_rec_type_t qtype;
                ares_dns_class_t qclass;
                if (ares_dns_record_query_get(dnsrec, i, &name, &qtype, &qclass) == ARES_SUCCESS) {
                    (void)name;
                    (void)qtype;
                    (void)qclass;
                }
            }
            
            // Add resource records to different sections
            uint8_t sections_to_test = fdp.ConsumeIntegral<uint8_t>();
            
            // Test ANSWER section
            if ((sections_to_test & 0x01) || fdp.ConsumeBool()) {
                uint8_t answer_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 5);
                for (uint8_t i = 0; i < answer_count && fdp.remaining_bytes() > 20; i++) {
                    ares_dns_rr_t* rr = NULL;
                    std::string rr_name = fdp.ConsumeRandomLengthString(253);
                    uint16_t rr_type_val = fdp.ConsumeIntegral<uint16_t>() % 65536;
                    uint16_t rr_class_val = fdp.ConsumeIntegral<uint16_t>() % 256;
                    uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                    
                    ares_dns_rec_type_t rr_type = static_cast<ares_dns_rec_type_t>(rr_type_val);
                    ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(rr_class_val);
                    
                    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                                   rr_name.c_str(), rr_type, rr_class, ttl);
                    
                    if (status == ARES_SUCCESS && rr != NULL) {
                        // Test setting various RR data based on type
                        uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                        
                        // For A records
                        if (rr_type == ARES_REC_TYPE_A && fdp.remaining_bytes() >= 4) {
                            struct in_addr addr;
                            auto addr_bytes = fdp.ConsumeBytes<uint8_t>(4);
                            memcpy(&addr, addr_bytes.data(), 4);
                            ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                        }
                        // For AAAA records  
                        else if (rr_type == ARES_REC_TYPE_AAAA && fdp.remaining_bytes() >= 16) {
                            struct ares_in6_addr addr6;
                            auto addr6_bytes = fdp.ConsumeBytes<uint8_t>(16);
                            memcpy(&addr6, addr6_bytes.data(), 16);
                            ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
                        }
                        // For TXT records
                        else if (rr_type == ARES_REC_TYPE_TXT && fdp.remaining_bytes() > 0) {
                            std::string txt_str = fdp.ConsumeRandomLengthString(255);
                            ares_dns_rr_set_str(rr, ARES_RR_TXT_DATA, txt_str.c_str());
                        }
                        // For MX records
                        else if (rr_type == ARES_REC_TYPE_MX && fdp.remaining_bytes() > 2) {
                            uint16_t preference = fdp.ConsumeIntegral<uint16_t>();
                            std::string mx_host = fdp.ConsumeRandomLengthString(253);
                            ares_dns_rr_set_u16(rr, ARES_RR_MX_PREFERENCE, preference);
                            ares_dns_rr_set_str(rr, ARES_RR_MX_EXCHANGE, mx_host.c_str());
                        }
                        // For SOA records
                        else if (rr_type == ARES_REC_TYPE_SOA && fdp.remaining_bytes() > 20) {
                            std::string mname = fdp.ConsumeRandomLengthString(253);
                            std::string rname = fdp.ConsumeRandomLengthString(253);
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
                        // For generic binary data
                        else if (fdp.remaining_bytes() > 10) {
                            size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                            if (fdp.remaining_bytes() >= bin_len) {
                                auto bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                                ares_dns_rr_set_bin(rr, ARES_RR_RAW_RR_DATA, bin_data.data(), bin_len);
                            }
                        }
                    }
                }
            }
            
            // Test AUTHORITY section
            if ((sections_to_test & 0x02) || fdp.ConsumeBool()) {
                uint8_t auth_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
                for (uint8_t i = 0; i < auth_count && fdp.remaining_bytes() > 15; i++) {
                    ares_dns_rr_t* rr = NULL;
                    std::string rr_name = fdp.ConsumeRandomLengthString(253);
                    uint16_t rr_type_val = fdp.ConsumeIntegral<uint16_t>() % 65536;
                    uint16_t rr_class_val = fdp.ConsumeIntegral<uint16_t>() % 256;
                    uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                    
                    ares_dns_rec_type_t rr_type = static_cast<ares_dns_rec_type_t>(rr_type_val);
                    ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(rr_class_val);
                    
                    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_AUTHORITY,
                                                   rr_name.c_str(), rr_type, rr_class, ttl);
                    
                    if (status == ARES_SUCCESS && rr != NULL) {
                        // Just set a TTL for authority records
                        uint32_t new_ttl = fdp.ConsumeIntegral<uint32_t>();
                        ares_dns_rr_set_ttl(rr, new_ttl);
                    }
                }
            }
            
            // Test ADDITIONAL section
            if ((sections_to_test & 0x04) || fdp.ConsumeBool()) {
                uint8_t add_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
                for (uint8_t i = 0; i < add_count && fdp.remaining_bytes() > 15; i++) {
                    ares_dns_rr_t* rr = NULL;
                    std::string rr_name = fdp.ConsumeRandomLengthString(253);
                    uint16_t rr_type_val = fdp.ConsumeIntegral<uint16_t>() % 65536;
                    uint16_t rr_class_val = fdp.ConsumeIntegral<uint16_t>() % 256;
                    uint32_t ttl = fdp.ConsumeIntegral<uint32_t>();
                    
                    ares_dns_rec_type_t rr_type = static_cast<ares_dns_rec_type_t>(rr_type_val);
                    ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(rr_class_val);
                    
                    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ADDITIONAL,
                                                   rr_name.c_str(), rr_type, rr_class, ttl);
                    // Don't need to do anything with rr if successful
                }
            }
            
            // Get RR counts for each section
            size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            size_t authority_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
            size_t additional_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
            
            (void)answer_cnt;
            (void)authority_cnt;
            (void)additional_cnt;
            
            // Test getting RRs from sections
            for (size_t i = 0; i < answer_cnt && i < 5; i++) {
                ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, i);
                if (rr != NULL) {
                    const char* name = ares_dns_rr_get_name(rr);
                    ares_dns_rec_type_t type = ares_dns_rr_get_type(rr);
                    ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                    unsigned int rr_ttl = ares_dns_rr_get_ttl(rr);
                    
                    (void)name;
                    (void)type;
                    (void)rr_class;
                    (void)rr_ttl;
                }
            }
            
            // Write DNS record to wire format
            unsigned char* wire_buf = NULL;
            size_t wire_len = 0;
            status = ares_dns_write(dnsrec, &wire_buf, &wire_len);
            
            if (status == ARES_SUCCESS && wire_buf != NULL && wire_len > 0) {
                // Test parsing the wire format back
                ares_dns_record_t* parsed_rec = NULL;
                ares_status_t parse_status = ares_dns_parse(wire_buf, wire_len, 0, &parsed_rec);
                
                if (parse_status == ARES_SUCCESS && parsed_rec != NULL) {
                    // Compare some properties
                    unsigned short parsed_id = ares_dns_record_get_id(parsed_rec);
                    unsigned short parsed_flags = ares_dns_record_get_flags(parsed_rec);
                    
                    (void)parsed_id;
                    (void)parsed_flags;
                    
                    // Test duplication
                    ares_dns_record_t* duplicated = ares_dns_record_duplicate(parsed_rec);
                    if (duplicated != NULL) {
                        ares_dns_record_destroy(duplicated);
                    }
                    
                    ares_dns_record_destroy(parsed_rec);
                }
                
                // Free the wire buffer
                ares_free_string(wire_buf);
            }
            
            // Test deletion of RRs
            if (answer_cnt > 0) {
                // Try to delete first RR if it exists
                ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, 0);
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // Test 2: Parse raw fuzzer input as DNS message using modern parser
    if (fdp.remaining_bytes() > 12) {
        auto raw_dns_data = fdp.ConsumeRemainingBytes<unsigned char>();
        ares_dns_record_t* parsed_rec = NULL;
        uint32_t parse_flags = fdp.ConsumeIntegral<uint32_t>();
        
        ares_status_t parse_status = ares_dns_parse(raw_dns_data.data(), raw_dns_data.size(), 
                                                   parse_flags, &parsed_rec);
        
        if (parse_status == ARES_SUCCESS && parsed_rec != NULL) {
            // Exercise the parsed record
            size_t query_cnt = ares_dns_record_query_cnt(parsed_rec);
            for (size_t i = 0; i < query_cnt && i < 5; i++) {
                const char* name = NULL;
                ares_dns_rec_type_t qtype;
                ares_dns_class_t qclass;
                if (ares_dns_record_query_get(parsed_rec, i, &name, &qtype, &qclass) == ARES_SUCCESS) {
                    (void)name;
                    (void)qtype;
                    (void)qclass;
                }
            }
            
            // Try to write it back
            unsigned char* out_buf = NULL;
            size_t out_len = 0;
            ares_dns_write(parsed_rec, &out_buf, &out_len);
            if (out_buf != NULL) {
                ares_free_string(out_buf);
            }
            
            ares_dns_record_destroy(parsed_rec);
        }
    }
    
    ares_library_cleanup();
    return 0;
}

} // namespace harness_004
