// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Include necessary headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

static const char *test_name = "example.com";

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to generate parameter values
    // We'll extract values from the input in a deterministic way
    
    // We need at least 33 bytes for all the fixed parameters
    if (Size < 33) {
        return 0;
    }
    
    // Extract parameters from fuzzer input with bounds checking
    unsigned short id = (Data[0] << 8) | Data[1];
    unsigned short flags = (Data[2] << 8) | Data[3];
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(Data[4] % 5); // 0-4 for valid opcodes
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(Data[5] % 16); // 0-15 for valid rcodes
    
    // Create DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    // Add query
    ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)((Data[6] << 8) | Data[7]);
    ares_dns_class_t qclass = (ares_dns_class_t)((Data[8] << 8) | Data[9]);
    status = ares_dns_record_query_add(dnsrec, test_name, qtype, qclass);
    
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Add resource record
    ares_dns_rr_t *rr = NULL;
    ares_dns_section_t sect = (ares_dns_section_t)(Data[10] % 4); // 0-3 for valid sections
    ares_dns_rec_type_t rr_type = (ares_dns_rec_type_t)((Data[11] << 8) | Data[12]);
    ares_dns_class_t rr_class = (ares_dns_class_t)((Data[13] << 8) | Data[14]);
    unsigned int ttl = (Data[15] << 24) | (Data[16] << 16) | (Data[17] << 8) | Data[18];
    
    status = ares_dns_record_rr_add(&rr, dnsrec, sect, test_name, rr_type, rr_class, ttl);
    
    if (status != ARES_SUCCESS || rr == NULL) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Set various RR fields using fuzzer input
    // First ares_dns_rr_set_u16
    ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)((Data[19] << 8) | Data[20]);
    unsigned short val1 = (Data[21] << 8) | Data[22];
    ares_dns_rr_set_u16(rr, key1, val1);
    
    // ares_dns_rr_set_u8
    ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[23]);
    unsigned char val2 = Data[24];
    ares_dns_rr_set_u8(rr, key2, val2);
    
    // Second ares_dns_rr_set_u16
    ares_dns_rr_key_t key3 = (ares_dns_rr_key_t)((Data[25] << 8) | Data[26]);
    unsigned short val3 = (Data[27] << 8) | Data[28];
    ares_dns_rr_set_u16(rr, key3, val3);
    
    // ares_dns_rr_set_opt
    ares_dns_rr_key_t key4 = (ares_dns_rr_key_t)((Data[29] << 8) | Data[30]);
    unsigned short opt = (Data[31] << 8) | Data[32];
    
    // Use remaining data for opt value
    size_t opt_val_len = 0;
    const unsigned char *opt_val = NULL;
    
    if (Size > 33) {
        opt_val = Data + 33;
        opt_val_len = Size - 33;
        if (opt_val_len > 100) {
            opt_val_len = 100; // Limit to reasonable size
        }
    }
    
    ares_dns_rr_set_opt(rr, key4, opt, opt_val, opt_val_len);
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}