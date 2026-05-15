// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void test_ares_dns_class_fromstr(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create a null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    ares_dns_class_t qclass;
    ares_bool_t result = ares_dns_class_fromstr(&qclass, str);
    
    // If conversion succeeded, test tostr
    if (result == ARES_TRUE) {
        const char *class_str = ares_dns_class_tostr(qclass);
        // Just use the result to avoid unused variable warning
        (void)class_str;
    }
    
    free(str);
}

static void test_ares_dns_record_query_get(const ares_dns_record_t *dnsrec) {
    if (!dnsrec) return;
    
    // Try indices 0, 1, 2, 3 to exercise the function
    for (size_t idx = 0; idx < 4; idx++) {
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        ares_status_t status = ares_dns_record_query_get(dnsrec, idx, &name, &qtype, &qclass);
        
        if (status == ARES_SUCCESS) {
            // If successful, test class tostr
            const char *class_str = ares_dns_class_tostr(qclass);
            (void)class_str;
        }
    }
}

static void test_ares_dns_record_rr_add(ares_dns_record_t *dnsrec, const uint8_t *data, size_t size) {
    if (!dnsrec || size < 10) return;
    
    // Extract parameters from fuzzer data
    ares_dns_section_t sect = (ares_dns_section_t)(data[0] % 4); // 4 sections max
    ares_dns_rec_type_t type = (ares_dns_rec_type_t)(data[1] % 256);
    ares_dns_class_t rclass = (ares_dns_class_t)(data[2] % 65536);
    unsigned int ttl = (data[3] << 24) | (data[4] << 16) | (data[5] << 8) | data[6];
    
    // Create name from remaining data
    size_t name_len = size - 7;
    if (name_len > 255) name_len = 255;
    char *name = malloc(name_len + 1);
    if (!name) return;
    
    memcpy(name, data + 7, name_len);
    name[name_len] = '\0';
    
    ares_dns_rr_t *rr_out = NULL;
    ares_status_t status = ares_dns_record_rr_add(&rr_out, dnsrec, sect, name, type, rclass, ttl);
    
    if (status == ARES_SUCCESS && rr_out) {
        // Test getting class from the added RR
        ares_dns_class_t retrieved_class = ares_dns_rr_get_class(rr_out);
        const char *class_str = ares_dns_class_tostr(retrieved_class);
        (void)class_str;
    }
    
    free(name);
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test class conversion functions
    test_ares_dns_class_fromstr(Data, Size);
    
    // Test parsing DNS message
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    // Use first byte for flags if available
    if (Size > 0) {
        flags = Data[0];
    }
    
    ares_status_t parse_status = ares_dns_parse(Data, Size, flags, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // Test query get on parsed record
        test_ares_dns_record_query_get(dnsrec);
        
        // Test adding RR to parsed record (use shifted data to avoid same input)
        if (Size > 1) {
            test_ares_dns_record_rr_add(dnsrec, Data + 1, Size - 1);
        }
        
        // Clean up
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}