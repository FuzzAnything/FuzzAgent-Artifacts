// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Include required headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static uint8_t get_byte(const uint8_t *data, size_t size, size_t offset) {
    if (offset < size) {
        return data[offset];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Generate enum values from fuzzer input
    ares_dns_rr_key_t rr_key = (ares_dns_rr_key_t)(get_byte(Data, Size, 0) | (get_byte(Data, Size, 1) << 8));
    ares_dns_section_t section = (ares_dns_section_t)(get_byte(Data, Size, 2) | (get_byte(Data, Size, 3) << 8));
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(get_byte(Data, Size, 4) | (get_byte(Data, Size, 5) << 8));
    ares_dns_rec_type_t rec_type = (ares_dns_rec_type_t)(get_byte(Data, Size, 6) | (get_byte(Data, Size, 7) << 8));
    ares_dns_class_t dns_class = (ares_dns_class_t)(get_byte(Data, Size, 8) | (get_byte(Data, Size, 9) << 8));
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(get_byte(Data, Size, 10) | (get_byte(Data, Size, 11) << 8));

    // Call target API functions
    const char *rr_key_str = ares_dns_rr_key_tostr(rr_key);
    (void)rr_key_str;
    
    const char *section_str = ares_dns_section_tostr(section);
    (void)section_str;
    
    const char *rcode_str = ares_dns_rcode_tostr(rcode);
    (void)rcode_str;
    
    const char *rec_type_str = ares_dns_rec_type_tostr(rec_type);
    (void)rec_type_str;
    
    const char *class_str = ares_dns_class_tostr(dns_class);
    (void)class_str;
    
    const char *opcode_str = ares_dns_opcode_tostr(opcode);
    (void)opcode_str;

    return 0;
}