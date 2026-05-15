// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static uint8_t dummy_data[1024];
static size_t dummy_pos = 0;

static const uint8_t* get_data(const uint8_t* Data, size_t Size, size_t needed) {
    if (dummy_pos + needed > sizeof(dummy_data)) {
        dummy_pos = 0;
    }
    if (Size > 0) {
        size_t to_copy = Size < needed ? Size : needed;
        memcpy(dummy_data + dummy_pos, Data, to_copy);
        dummy_pos += to_copy;
        return dummy_data + dummy_pos - to_copy;
    }
    return NULL;
}

static const char* get_string(const uint8_t* Data, size_t Size) {
    static char str[256];
    size_t len = Size < sizeof(str) - 1 ? Size : sizeof(str) - 1;
    if (len > 0) {
        memcpy(str, Data, len);
        str[len] = '\0';
    } else {
        str[0] = '\0';
    }
    return str;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a DNS record for testing
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0x1234, 0x8000, 0, 0);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }

    // Add some queries to the record
    const uint8_t* name_data = get_data(Data, Size, 16);
    
    // Get query count
    size_t query_count = ares_dns_record_query_cnt(dnsrec);

    // Test ares_dns_record_query_get with different indices
    for (size_t idx = 0; idx < query_count + 2; idx++) { // Try out-of-bounds too
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        status = ares_dns_record_query_get(dnsrec, idx, 
                                          (idx % 2 == 0) ? &name : NULL,
                                          (idx % 3 == 0) ? &qtype : NULL,
                                          (idx % 4 == 0) ? &qclass : NULL);
        // Just call the function, ignore return value
    }

    // Test ares_dns_rec_type_fromstr
    const char* type_str = get_string(Data, Size);
    ares_dns_rec_type_t qtype;
    ares_bool_t type_result = ares_dns_rec_type_fromstr(&qtype, type_str);
    
    // Test with different string patterns
    ares_dns_rec_type_fromstr(&qtype, "A");
    ares_dns_rec_type_fromstr(&qtype, "AAAA");
    ares_dns_rec_type_fromstr(&qtype, "MX");
    ares_dns_rec_type_fromstr(&qtype, "INVALID");

    // Test ares_dns_class_fromstr
    const char* class_str = get_string(Data + (Size > 10 ? 10 : 0), Size > 10 ? Size - 10 : 0);
    ares_dns_class_t qclass;
    ares_bool_t class_result = ares_dns_class_fromstr(&qclass, class_str);
    
    // Test with standard class names
    ares_dns_class_fromstr(&qclass, "IN");
    ares_dns_class_fromstr(&qclass, "CH");
    ares_dns_class_fromstr(&qclass, "HS");
    ares_dns_class_fromstr(&qclass, "NONE");

    // Test ares_dns_record_rr_add
    ares_dns_rr_t *rr = NULL;
    const uint8_t* rr_name_data = get_data(Data, Size, 32);
    
    // Try adding to different sections
    ares_dns_section_t sections[] = {ARES_SECTION_ANSWER, ARES_SECTION_AUTHORITY, ARES_SECTION_ADDITIONAL};
    for (int i = 0; i < 3; i++) {
        status = ares_dns_record_rr_add(&rr, dnsrec, sections[i],
                                       rr_name_data ? (const char*)rr_name_data : "rr.example.com",
                                       (ares_dns_rec_type_t)((Size % 28) + 1), // Valid type range
                                       (ares_dns_class_t)((Size % 4) + 1),     // Valid class range
                                       (unsigned int)(Size % 65536));
        
        // Add another with different parameters
        if (i == 0) {
            status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                           "test.rr",
                                           ARES_REC_TYPE_TXT,
                                           ARES_CLASS_IN,
                                           300);
        }
    }

    // Test ares_dns_rr_get_class
    if (rr != NULL) {
        ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
        // Also test with NULL
        ares_dns_rr_get_class(NULL);
    }

    // Test ares_dns_class_tostr
    for (int i = 0; i < 10; i++) {
        const char* class_str = ares_dns_class_tostr((ares_dns_class_t)i);
        // Also test with some out-of-range values
        const char* unknown_str = ares_dns_class_tostr((ares_dns_class_t)999);
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}