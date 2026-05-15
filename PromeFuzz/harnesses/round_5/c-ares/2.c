// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write input to dummy file if needed
    write_dummy_file(Data, Size);
    
    // Parse DNS message
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    if (Size > 0) {
        flags = Data[0] % 256;
    }
    
    ares_status_t parse_result = ares_dns_parse(Data, Size, flags, &dnsrec);
    
    if (parse_result == ARES_SUCCESS && dnsrec != NULL) {
        // Get query count
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // Iterate through queries
        for (size_t i = 0; i < query_cnt; i++) {
            const char *name = NULL;
            ares_dns_rec_type_t qtype = 0;
            ares_dns_class_t qclass = 0;
            
            ares_dns_record_query_get(dnsrec, i, &name, &qtype, &qclass);
        }
        
        // Define sections to check
        ares_dns_section_t sections[] = {
            ARES_SECTION_ANSWER,
            ARES_SECTION_AUTHORITY,
            ARES_SECTION_ADDITIONAL
        };
        size_t num_sections = sizeof(sections) / sizeof(sections[0]);
        
        // Iterate through each section
        for (size_t s = 0; s < num_sections; s++) {
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sections[s]);
            
            // Iterate through resource records in this section
            for (size_t i = 0; i < rr_cnt; i++) {
                ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sections[s], i);
                
                if (rr != NULL) {
                    // Get RR type
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    
                    // Get keys for this RR type
                    size_t key_cnt = 0;
                    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                    
                    // Try to get keys for a few different types to explore more states
                    if (i == 0) {
                        // Try with some common record types
                        ares_dns_rec_type_t test_types[] = {
                            ARES_REC_TYPE_A,
                            ARES_REC_TYPE_AAAA,
                            ARES_REC_TYPE_CNAME,
                            ARES_REC_TYPE_MX,
                            ARES_REC_TYPE_TXT
                        };
                        size_t num_test_types = sizeof(test_types) / sizeof(test_types[0]);
                        
                        for (size_t t = 0; t < num_test_types; t++) {
                            size_t test_key_cnt = 0;
                            const ares_dns_rr_key_t *test_keys = ares_dns_rr_get_keys(test_types[t], &test_key_cnt);
                            (void)test_keys;
                        }
                    }
                }
            }
        }
        
        // Clean up
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}