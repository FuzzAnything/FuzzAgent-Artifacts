// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#ifdef CARES_STATICLIB
#define CARES_EXTERN
#else
#ifdef _WIN32
#define CARES_EXTERN __declspec(dllimport)
#else
#define CARES_EXTERN
#endif
#endif
#endif

/* Include required headers for missing types */
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Parse the DNS record from the dummy file
    FILE *read_fp = fopen(filename, "rb");
    if (!read_fp) {
        return 0;
    }
    
    // Read the file content to parse as DNS record
    fseek(read_fp, 0, SEEK_END);
    long file_size = ftell(read_fp);
    fseek(read_fp, 0, SEEK_SET);
    
    unsigned char *buf = (unsigned char *)malloc(file_size);
    if (!buf) {
        fclose(read_fp);
        return 0;
    }
    
    fread(buf, 1, file_size, read_fp);
    fclose(read_fp);
    
    // Parse the DNS record - fixed to use correct number of arguments
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_parse(buf, file_size, 0, &dnsrec);
    free(buf);
    
    if (status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }
    
    // Define sections to iterate through
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    // Iterate through each section
    for (size_t sect_idx = 0; sect_idx < sizeof(sections)/sizeof(sections[0]); sect_idx++) {
        ares_dns_section_t sect = sections[sect_idx];
        
        // Call ares_dns_record_rr_cnt
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, sect);
        
        // For each resource record in the section
        for (size_t idx = 0; idx < rr_count; idx++) {
            // Call ares_dns_record_rr_get_const
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sect, idx);
            
            if (rr) {
                // Call ares_dns_rr_get_type
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                
                // Use the result to avoid dead code elimination
                (void)rr_type;
            }
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}