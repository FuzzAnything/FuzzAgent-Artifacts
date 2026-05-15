// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void test_dns_class_tostr(const uint8_t *data, size_t size) {
    if (size > 0) {
        /* Use the first byte to generate a class enum value */
        ares_dns_class_t qclass = (ares_dns_class_t)(data[0] % 256);
        const char *result = ares_dns_class_tostr(qclass);
        (void)result; /* Avoid unused variable warning */
    }
}

static void test_dns_class_fromstr(const uint8_t *data, size_t size) {
    if (size > 0) {
        /* Create a null-terminated string from input data */
        char *str = malloc(size + 1);
        if (!str) return;
        memcpy(str, data, size);
        str[size] = '\0';
        
        ares_dns_class_t qclass;
        ares_bool_t result = ares_dns_class_fromstr(&qclass, str);
        (void)result; /* Avoid unused variable warning */
        free(str);
    }
}

static void test_dns_parse(const uint8_t *data, size_t size) {
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    /* Try parsing with default flags */
    ares_status_t status = ares_dns_parse(data, size, flags, &dnsrec);
    if (status == ARES_SUCCESS && dnsrec) {
        /* If parse succeeded, we can test query_get function */
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        /* Try to get the first query if it exists */
        ares_status_t get_status = ares_dns_record_query_get(
            dnsrec, 0, &name, &qtype, &qclass);
        (void)get_status; /* Avoid unused variable warning */
        
        /* Clean up */
        ares_dns_record_destroy(dnsrec);
    }
}

static void test_dns_record_rr_add(const uint8_t *data, size_t size) {
    /* First create a DNS record by parsing */
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    /* Try to parse to get a valid record, but if it fails,
       we'll create one manually if possible */
    ares_status_t parse_status = ares_dns_parse(data, size, flags, &dnsrec);
    
    if (parse_status != ARES_SUCCESS || !dnsrec) {
        /* If parse failed, we can't proceed without a record */
        return;
    }
    
    /* Try adding a resource record */
    ares_dns_rr_t *rr_out = NULL;
    const char *name = "example.com";
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    ares_dns_class_t rclass = ARES_CLASS_IN;
    unsigned int ttl = 300;
    
    /* Try adding to different sections */
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (size_t i = 0; i < sizeof(sections)/sizeof(sections[0]); i++) {
        ares_status_t add_status = ares_dns_record_rr_add(
            &rr_out, dnsrec, sections[i], name, type, rclass, ttl);
        (void)add_status; /* Avoid unused variable warning */
        
        if (rr_out) {
            /* Test getting class from the added RR */
            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr_out);
            (void)rr_class; /* Avoid unused variable warning */
        }
    }
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
}

static void test_dns_rr_get_class(const uint8_t *data, size_t size) {
    /* Create a DNS record and add an RR to test get_class */
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    /* Try to parse first */
    ares_status_t parse_status = ares_dns_parse(data, size, flags, &dnsrec);
    
    if (parse_status != ARES_SUCCESS || !dnsrec) {
        return;
    }
    
    /* Add a resource record */
    ares_dns_rr_t *rr_out = NULL;
    ares_status_t add_status = ares_dns_record_rr_add(
        &rr_out, dnsrec, ARES_SECTION_ANSWER,
        "test.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 300);
    
    if (add_status == ARES_SUCCESS && rr_out) {
        ares_dns_class_t rr_class = ares_dns_rr_get_class(rr_out);
        (void)rr_class; /* Avoid unused variable warning */
    }
    
    /* Also test with NULL input */
    ares_dns_class_t null_class = ares_dns_rr_get_class(NULL);
    (void)null_class; /* Avoid unused variable warning */
    
    ares_dns_record_destroy(dnsrec);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    /* Test all API functions */
    test_dns_class_tostr(Data, Size);
    test_dns_class_fromstr(Data, Size);
    test_dns_parse(Data, Size);
    test_dns_record_rr_add(Data, Size);
    test_dns_rr_get_class(Data, Size);
    
    return 0;
}