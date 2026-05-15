// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
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
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for any potential file-based parsing
    write_dummy_file(Data, Size);

    // First we need to create a DNS record object to work with
    ares_dns_record_t *dnsrec = NULL;
    
    // Try to parse the input as a DNS packet using ares_dns_parse
    if (Size >= 12) {  // Minimum DNS header size
        ares_status_t status = ares_dns_parse(Data, Size, 0, &dnsrec);
        (void)status; // Status is unused, but we accept failure
    }

    // If we have a valid DNS record, proceed with the API calls
    if (dnsrec) {
        // Try different sections
        const ares_dns_section_t sections[] = {
            ARES_SECTION_ANSWER,
            ARES_SECTION_AUTHORITY,
            ARES_SECTION_ADDITIONAL
        };
        
        for (size_t s = 0; s < sizeof(sections)/sizeof(sections[0]); s++) {
            // 1. ares_dns_record_rr_cnt
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, sections[s]);
            
            // 2. ares_dns_record_rr_get for each record
            for (size_t i = 0; i < rr_count; i++) {
                ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sections[s], i);
                if (!rr) continue;
                
                // 3. ares_dns_rr_get_type
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                
                // Try different keys that might have ABINP datatype
                // Using keys that are defined in the header
                const ares_dns_rr_key_t keys[] = {
                    ARES_RR_A_ADDR,
                    ARES_RR_AAAA_ADDR,
                    ARES_RR_TXT_DATA,
                    ARES_RR_SOA_RNAME,
                    ARES_RR_PTR_DNAME,
                    ARES_RR_MX_EXCHANGE
                };
                
                for (size_t k = 0; k < sizeof(keys)/sizeof(keys[0]); k++) {
                    // 4. ares_dns_rr_get_abin_cnt
                    size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, keys[k]);
                    
                    // 5. ares_dns_rr_get_abin for each index if available
                    for (size_t idx = 0; idx < abin_cnt; idx++) {
                        size_t len = 0;
                        const unsigned char *abin_data = ares_dns_rr_get_abin(rr, keys[k], idx, &len);
                        // Just accessing the data, not using it
                        (void)abin_data;
                    }
                    
                    // 6. ares_dns_rr_get_bin
                    size_t bin_len = 0;
                    const unsigned char *bin_data = ares_dns_rr_get_bin(rr, keys[k], &bin_len);
                    (void)bin_data;
                }
            }
        }
        
        // 7. ares_dns_record_destroy
        ares_dns_record_destroy(dnsrec);
    } else {
        // Even without a valid record, we can test some functions with NULL
        ares_dns_record_rr_cnt(NULL, ARES_SECTION_ANSWER);
        ares_dns_record_rr_get(NULL, ARES_SECTION_ANSWER, 0);
        ares_dns_rr_get_type(NULL);
        ares_dns_rr_get_abin_cnt(NULL, ARES_RR_A_ADDR);
        
        size_t len = 0;
        ares_dns_rr_get_abin(NULL, ARES_RR_A_ADDR, 0, &len);
        ares_dns_rr_get_bin(NULL, ARES_RR_A_ADDR, &len);
        
        ares_dns_record_destroy(NULL);
    }

    return 0;
}