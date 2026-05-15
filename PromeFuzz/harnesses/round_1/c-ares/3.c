// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_class_t get_random_class(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return ARES_CLASS_IN;
    uint8_t val = data[*offset];
    (*offset)++;
    switch (val % 2) {
        case 0: return ARES_CLASS_IN;
        case 1: return ARES_CLASS_NONE;
        default: return ARES_CLASS_IN;
    }
}

static ares_dns_rec_type_t get_random_type(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return ARES_REC_TYPE_A;
    uint8_t val = data[*offset];
    (*offset)++;
    switch (val % 20) {
        case 0: return ARES_REC_TYPE_A;
        case 1: return ARES_REC_TYPE_NS;
        case 2: return ARES_REC_TYPE_CNAME;
        case 3: return ARES_REC_TYPE_SOA;
        case 4: return ARES_REC_TYPE_PTR;
        case 5: return ARES_REC_TYPE_HINFO;
        case 6: return ARES_REC_TYPE_MX;
        case 7: return ARES_REC_TYPE_TXT;
        case 8: return ARES_REC_TYPE_SIG;
        case 9: return ARES_REC_TYPE_AAAA;
        case 10: return ARES_REC_TYPE_SRV;
        case 11: return ARES_REC_TYPE_NAPTR;
        case 12: return ARES_REC_TYPE_OPT;
        case 13: return ARES_REC_TYPE_TLSA;
        case 14: return ARES_REC_TYPE_SVCB;
        case 15: return ARES_REC_TYPE_HTTPS;
        case 16: return ARES_REC_TYPE_URI;
        case 17: return ARES_REC_TYPE_CAA;
        case 18: return ARES_REC_TYPE_ANY;
        case 19: return ARES_REC_TYPE_RAW_RR;
        default: return ARES_REC_TYPE_A;
    }
}

static ares_dns_section_t get_random_section(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return ARES_SECTION_ANSWER;
    uint8_t val = data[*offset];
    (*offset)++;
    switch (val % 4) {
        case 0: return ARES_SECTION_ANSWER;
        case 1: return ARES_SECTION_AUTHORITY;
        case 2: return ARES_SECTION_ADDITIONAL;
        case 3: return ARES_SECTION_ANSWER;
        default: return ARES_SECTION_ANSWER;
    }
}

static ares_dns_opcode_t get_random_opcode(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return ARES_OPCODE_QUERY;
    uint8_t val = data[*offset];
    (*offset)++;
    switch (val % 5) {
        case 0: return ARES_OPCODE_QUERY;
        case 1: return ARES_OPCODE_IQUERY;
        case 2: return ARES_OPCODE_STATUS;
        case 3: return ARES_OPCODE_NOTIFY;
        case 4: return ARES_OPCODE_UPDATE;
        default: return ARES_OPCODE_QUERY;
    }
}

static ares_dns_rcode_t get_random_rcode(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return ARES_RCODE_NOERROR;
    uint8_t val = data[*offset];
    (*offset)++;
    switch (val % 6) {
        case 0: return ARES_RCODE_NOERROR;
        case 1: return ARES_RCODE_FORMERR;
        case 2: return ARES_RCODE_SERVFAIL;
        case 3: return ARES_RCODE_NXDOMAIN;
        case 4: return ARES_RCODE_NOTIMP;
        case 5: return ARES_RCODE_REFUSED;
        default: return ARES_RCODE_NOERROR;
    }
}

static ares_dns_rr_key_t get_random_rr_key(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    uint8_t val = data[*offset];
    (*offset)++;
    return (ares_dns_rr_key_t)(val % 1000);
}

int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size) {
    size_t offset = 0;
    
    if (size < 10) return 0;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    unsigned short id = 0;
    unsigned short flags = 0;
    
    if (offset + 2 <= size) {
        id = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }
    
    if (offset + 2 <= size) {
        flags = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }
    
    ares_dns_opcode_t opcode = get_random_opcode(data, size, &offset);
    ares_dns_rcode_t rcode = get_random_rcode(data, size, &offset);
    
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS) {
        if (dnsrec) ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    const char *query_name = "example.com";
    ares_dns_rec_type_t qtype = get_random_type(data, size, &offset);
    ares_dns_class_t qclass = get_random_class(data, size, &offset);
    
    status = ares_dns_record_query_add(dnsrec, query_name, qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    ares_dns_rr_t *rr = NULL;
    ares_dns_section_t sect = get_random_section(data, size, &offset);
    const char *rr_name = "test.example.com";
    ares_dns_rec_type_t rr_type = get_random_type(data, size, &offset);
    ares_dns_class_t rr_class = get_random_class(data, size, &offset);
    unsigned int ttl = 300;
    
    if (offset + 4 <= size) {
        ttl = (data[offset] << 24) | (data[offset + 1] << 16) | 
              (data[offset + 2] << 8) | data[offset + 3];
        offset += 4;
    }
    
    status = ares_dns_record_rr_add(&rr, dnsrec, sect, rr_name, rr_type, rr_class, ttl);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    ares_dns_rr_key_t u16_key1 = get_random_rr_key(data, size, &offset);
    unsigned short u16_val1 = 12345;
    if (offset + 2 <= size) {
        u16_val1 = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }
    ares_dns_rr_set_u16(rr, u16_key1, u16_val1);
    
    ares_dns_rr_key_t u8_key = get_random_rr_key(data, size, &offset);
    unsigned char u8_val = 200;
    if (offset < size) {
        u8_val = data[offset];
        offset++;
    }
    ares_dns_rr_set_u8(rr, u8_key, u8_val);
    
    ares_dns_rr_key_t u16_key2 = get_random_rr_key(data, size, &offset);
    unsigned short u16_val2 = 54321;
    if (offset + 2 <= size) {
        u16_val2 = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }
    ares_dns_rr_set_u16(rr, u16_key2, u16_val2);
    
    ares_dns_rr_key_t opt_key = get_random_rr_key(data, size, &offset);
    unsigned short opt_val = 99;
    if (offset + 2 <= size) {
        opt_val = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }
    
    size_t remaining = size - offset;
    const unsigned char *opt_data = NULL;
    if (remaining > 0 && offset < size) {
        opt_data = data + offset;
    }
    
    ares_dns_rr_set_opt(rr, opt_key, opt_val, opt_data, remaining);
    
    ares_dns_record_destroy(dnsrec);
    return 0;
}