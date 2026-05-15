// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include the main c-ares header first to get type definitions */
#include "ares.h"
/* Then include the DNS record header */
#include "ares_dns_record.h"

static const char* get_test_name(const uint8_t* data, size_t size, size_t* consumed) {
    static char name[256];
    size_t max_len = sizeof(name) - 1;
    size_t len = 0;
    
    if (size == 0) {
        name[0] = '\0';
        *consumed = 0;
        return name;
    }
    
    // Use up to max_len bytes or until null terminator
    while (len < max_len && len < size && data[len] != 0) {
        name[len] = data[len];
        len++;
    }
    
    name[len] = '\0';
    *consumed = len + (len < size && data[len] == 0 ? 1 : 0);
    return name;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    size_t consumed = 0;
    
    // Create DNS record
    ares_dns_record_t *dnsrec = NULL;
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size >= 2) {
        id = (Data[0] << 8) | Data[1];
        consumed += 2;
    }
    if (Size - consumed >= 2) {
        flags = (Data[consumed] << 8) | Data[consumed + 1];
        consumed += 2;
    }
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    // Add query
    size_t name_consumed;
    const char* name = get_test_name(Data + consumed, Size - consumed, &name_consumed);
    consumed += name_consumed;
    
    ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
    ares_dns_class_t qclass = ARES_CLASS_IN;
    
    if (Size - consumed >= 1) {
        qtype = Data[consumed] % (ARES_REC_TYPE_HTTPS + 1);
        consumed++;
    }
    if (Size - consumed >= 1) {
        qclass = Data[consumed] % (ARES_CLASS_ANY + 1);
        consumed++;
    }
    
    status = ares_dns_record_query_add(dnsrec, name, qtype, qclass);
    
    // Add resource record
    ares_dns_rr_t *rr = NULL;
    ares_dns_section_t sect = ARES_SECTION_ANSWER;
    ares_dns_rec_type_t type = ARES_REC_TYPE_OPT;
    ares_dns_class_t rclass = ARES_CLASS_IN;
    unsigned int ttl = 300;
    
    if (Size - consumed >= 1) {
        sect = Data[consumed] % (ARES_SECTION_ADDITIONAL + 1);
        consumed++;
    }
    
    name = get_test_name(Data + consumed, Size - consumed, &name_consumed);
    consumed += name_consumed;
    
    if (Size - consumed >= 1) {
        type = Data[consumed] % (ARES_REC_TYPE_HTTPS + 1);
        consumed++;
    }
    if (Size - consumed >= 1) {
        rclass = Data[consumed] % (ARES_CLASS_ANY + 1);
        consumed++;
    }
    if (Size - consumed >= 4) {
        ttl = (Data[consumed] << 24) | (Data[consumed + 1] << 16) | 
              (Data[consumed + 2] << 8) | Data[consumed + 3];
        consumed += 4;
    }
    
    status = ares_dns_record_rr_add(&rr, dnsrec, sect, name, type, rclass, ttl);
    
    if (rr != NULL) {
        // Set U16 field
        ares_dns_rr_key_t key1 = ARES_RR_OPT_UDP_SIZE;
        unsigned short val1 = 4096;
        
        if (Size - consumed >= 2) {
            val1 = (Data[consumed] << 8) | Data[consumed + 1];
            consumed += 2;
        }
        
        status = ares_dns_rr_set_u16(rr, key1, val1);
        
        // Set U8 field
        ares_dns_rr_key_t key2 = ARES_RR_OPT_VERSION;
        unsigned char val2 = 0;
        
        if (Size - consumed >= 1) {
            val2 = Data[consumed];
            consumed++;
        }
        
        status = ares_dns_rr_set_u8(rr, key2, val2);
        
        // Set another U16 field
        ares_dns_rr_key_t key3 = ARES_RR_OPT_FLAGS;
        unsigned short val3 = 0;
        
        if (Size - consumed >= 2) {
            val3 = (Data[consumed] << 8) | Data[consumed + 1];
            consumed += 2;
        }
        
        status = ares_dns_rr_set_u16(rr, key3, val3);
        
        // Set OPT field
        ares_dns_rr_key_t key4 = ARES_RR_OPT_OPTIONS;
        unsigned short opt = 8;
        const unsigned char* opt_val = NULL;
        size_t opt_val_len = 0;
        
        if (Size - consumed >= 2) {
            opt = (Data[consumed] << 8) | Data[consumed + 1];
            consumed += 2;
        }
        
        if (Size - consumed >= 1) {
            opt_val_len = Data[consumed] % 64;
            consumed++;
            
            if (opt_val_len > 0 && Size - consumed >= opt_val_len) {
                opt_val = Data + consumed;
                consumed += opt_val_len;
            }
        }
        
        status = ares_dns_rr_set_opt(rr, key4, opt, opt_val, opt_val_len);
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}