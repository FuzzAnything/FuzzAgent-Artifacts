// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include the necessary headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

/* Helper function to create a DNS record using library functions */
static ares_dns_record_t *create_dns_record(void) {
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    /* Create a DNS record with default values */
    status = ares_dns_record_create(&dnsrec, 1234, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    /* Create a DNS record using library function */
    ares_dns_record_t *dnsrec = create_dns_record();
    if (!dnsrec) {
        return 0;
    }
    
    /* Prepare name from fuzzer input */
    char *name = NULL;
    if (Size > 0) {
        /* Ensure null termination */
        name = malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
        }
    } else {
        /* Use a default name if no data */
        name = strdup("test.example.com");
    }
    
    if (!name) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* 1. Call ares_dns_record_rr_add */
    ares_dns_rr_t *rr_out = NULL;
    ares_status_t add_status = ares_dns_record_rr_add(
        &rr_out, dnsrec, ARES_SECTION_ANSWER,
        name, ARES_REC_TYPE_A, ARES_CLASS_IN, 300);
    
    /* 2. Call ares_dns_record_rr_del */
    ares_status_t del_status = ares_dns_record_rr_del(
        dnsrec, ARES_SECTION_ANSWER, 0);
    
    /* 3. Call ares_dns_record_rr_get */
    ares_dns_rr_t *rr_get = ares_dns_record_rr_get(
        dnsrec, ARES_SECTION_ANSWER, 0);
    
    /* 4. Call ares_dns_rr_get_name */
    if (rr_get) {
        const char *rr_name = ares_dns_rr_get_name(rr_get);
        (void)rr_name; /* Use result to avoid unused variable warning */
    }
    
    /* Cleanup */
    free(name);
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}