// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

static const char *dummy_name = "example.com";

static ares_dns_section_t get_section_from_input(const uint8_t *data, size_t size) {
    if (size == 0) return ARES_SECTION_ANSWER;
    return (ares_dns_section_t)(data[0] % 4);
}

static ares_dns_rec_type_t get_type_from_input(const uint8_t *data, size_t size) {
    if (size < 2) return ARES_REC_TYPE_A;
    return (ares_dns_rec_type_t)(data[1] % 256);
}

static ares_dns_class_t get_class_from_input(const uint8_t *data, size_t size) {
    if (size < 3) return ARES_CLASS_IN;
    return (ares_dns_class_t)(data[2] % 256);
}

static unsigned int get_ttl_from_input(const uint8_t *data, size_t size) {
    if (size < 7) return 300;
    unsigned int ttl;
    memcpy(&ttl, data + 3, sizeof(unsigned int));
    return ttl;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0x1234, 0, 0, 0);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }

    ares_dns_section_t sect = get_section_from_input(Data, Size);
    ares_dns_rec_type_t type = get_type_from_input(Data, Size);
    ares_dns_class_t rclass = get_class_from_input(Data, Size);
    unsigned int ttl = get_ttl_from_input(Data, Size);

    ares_dns_rr_t *rr_out = NULL;
    status = ares_dns_record_rr_add(&rr_out, dnsrec, sect, dummy_name, type, rclass, ttl);
    
    if (status == ARES_SUCCESS && rr_out != NULL) {
        ares_dns_record_rr_del(dnsrec, sect, 0);
        
        ares_dns_rr_t *rr_get = ares_dns_record_rr_get(dnsrec, sect, 0);
        
        if (rr_get != NULL) {
            const char *name = ares_dns_rr_get_name(rr_get);
            (void)name;
        }
        
        ares_dns_rr_t *rr_out2 = NULL;
        status = ares_dns_record_rr_add(&rr_out2, dnsrec, sect, dummy_name, type, rclass, ttl);
        
        if (status == ARES_SUCCESS && rr_out2 != NULL) {
            ares_dns_rr_t *rr_get2 = ares_dns_record_rr_get(dnsrec, sect, 0);
            if (rr_get2 != NULL) {
                const char *name2 = ares_dns_rr_get_name(rr_get2);
                (void)name2;
            }
            
            ares_dns_record_rr_del(dnsrec, sect, 999);
            
            ares_dns_rr_t *rr_get_invalid = ares_dns_record_rr_get(dnsrec, sect, 999);
            if (rr_get_invalid != NULL) {
                const char *name_invalid = ares_dns_rr_get_name(rr_get_invalid);
                (void)name_invalid;
            }
        }
    }

    ares_dns_record_destroy(dnsrec);
    
    return 0;
}