// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ares.h>
#include <ares_dns.h>
#include "ares_dns_record.h"

static unsigned short read_u16(const uint8_t **data, size_t *size) {
    if (*size < 2) return 0;
    unsigned short val = (*data)[0] << 8 | (*data)[1];
    *data += 2;
    *size -= 2;
    return val;
}

static unsigned char read_u8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    unsigned char val = (*data)[0];
    *data += 1;
    *size -= 1;
    return val;
}

static const char *read_string(const uint8_t **data, size_t *size, size_t max_len) {
    static char buffer[256];
    size_t i = 0;
    while (i < max_len && i < *size && (*data)[i] != 0 && i < sizeof(buffer)-1) {
        buffer[i] = (*data)[i];
        i++;
    }
    buffer[i] = '\0';
    *data += i;
    *size -= i;
    if (i < *size) {
        (*data)++;
        (*size)--;
    }
    return buffer;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    // Read parameters for ares_dns_record_create
    unsigned short id = read_u16(&ptr, &remaining);
    unsigned short flags = read_u16(&ptr, &remaining);
    unsigned short opcode_val = read_u16(&ptr, &remaining) % 16;
    unsigned short rcode_val = read_u16(&ptr, &remaining) % 16;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, 
                                                  (ares_dns_opcode_t)opcode_val,
                                                  (ares_dns_rcode_t)rcode_val);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    // Read parameters for ares_dns_record_query_add
    const char *name = read_string(&ptr, &remaining, 64);
    unsigned short qtype_val = read_u16(&ptr, &remaining);
    unsigned short qclass_val = read_u16(&ptr, &remaining);
    
    status = ares_dns_record_query_add(dnsrec, name, 
                                       (ares_dns_rec_type_t)qtype_val,
                                       (ares_dns_class_t)qclass_val);
    
    // Read parameters for ares_dns_record_rr_add
    unsigned short sect_val = read_u16(&ptr, &remaining) % 4;
    const char *rr_name = read_string(&ptr, &remaining, 64);
    unsigned short rr_type_val = read_u16(&ptr, &remaining);
    unsigned short rr_class_val = read_u16(&ptr, &remaining);
    unsigned int ttl = read_u16(&ptr, &remaining) << 16 | read_u16(&ptr, &remaining);
    
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, dnsrec, (ares_dns_section_t)sect_val,
                                    rr_name, (ares_dns_rec_type_t)rr_type_val,
                                    (ares_dns_class_t)rr_class_val, ttl);
    
    if (status == ARES_SUCCESS && rr != NULL) {
        // First ares_dns_rr_set_u16
        unsigned short key1 = read_u16(&ptr, &remaining);
        unsigned short val1 = read_u16(&ptr, &remaining);
        ares_dns_rr_set_u16(rr, (ares_dns_rr_key_t)key1, val1);
        
        // ares_dns_rr_set_u8
        unsigned short key2 = read_u16(&ptr, &remaining);
        unsigned char val2 = read_u8(&ptr, &remaining);
        ares_dns_rr_set_u8(rr, (ares_dns_rr_key_t)key2, val2);
        
        // Second ares_dns_rr_set_u16
        unsigned short key3 = read_u16(&ptr, &remaining);
        unsigned short val3 = read_u16(&ptr, &remaining);
        ares_dns_rr_set_u16(rr, (ares_dns_rr_key_t)key3, val3);
        
        // ares_dns_rr_set_opt
        unsigned short key4 = read_u16(&ptr, &remaining);
        unsigned short opt = read_u16(&ptr, &remaining);
        size_t val_len = remaining > 256 ? 256 : remaining;
        const unsigned char *opt_val = ptr;
        if (val_len > 0) {
            ares_dns_rr_set_opt(rr, (ares_dns_rr_key_t)key4, opt, opt_val, val_len);
        } else {
            ares_dns_rr_set_opt(rr, (ares_dns_rr_key_t)key4, opt, NULL, 0);
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}