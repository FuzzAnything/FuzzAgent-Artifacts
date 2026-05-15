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
#include "ares.h"
#include "ares_dns_record.h"

static int get_string_from_data(const uint8_t *data, size_t size, size_t *offset, char *buf, size_t buf_size) {
    if (*offset >= size || buf_size == 0) {
        return 0;
    }
    
    size_t i = 0;
    while (*offset + i < size && i < buf_size - 1) {
        if (data[*offset + i] == 0) {
            // Found null terminator
            memcpy(buf, data + *offset, i);
            buf[i] = 0;
            *offset += i + 1;  // skip the null terminator
            return 1;
        }
        i++;
    }
    
    // No null terminator found, copy what we have
    size_t copy_len = (size - *offset) < (buf_size - 1) ? (size - *offset) : (buf_size - 1);
    memcpy(buf, data + *offset, copy_len);
    buf[copy_len] = 0;
    *offset += copy_len;
    return 1;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Extract parameters for ares_dns_record_create
    unsigned short id = 0;  // Use zero for new queries
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size - offset >= 2) {
        flags = (Data[offset] << 8) | Data[offset + 1];
        offset += 2;
    }
    
    if (Size - offset >= 1) {
        opcode = (ares_dns_opcode_t)Data[offset];
        offset++;
    }
    
    if (Size - offset >= 1) {
        rcode = (ares_dns_rcode_t)Data[offset];
        offset++;
    }
    
    // Create DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    // Add query
    char query_name[256];
    if (get_string_from_data(Data, Size, &offset, query_name, sizeof(query_name))) {
        ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
        ares_dns_class_t qclass = ARES_CLASS_IN;
        
        if (Size - offset >= 1) {
            qtype = (ares_dns_rec_type_t)Data[offset];
            offset++;
        }
        
        if (Size - offset >= 1) {
            qclass = (ares_dns_class_t)Data[offset];
            offset++;
        }
        
        ares_dns_record_query_add(dnsrec, query_name, qtype, qclass);
    }
    
    // Add resource record
    ares_dns_rr_t *rr = NULL;
    char rr_name[256];
    
    if (get_string_from_data(Data, Size, &offset, rr_name, sizeof(rr_name))) {
        ares_dns_section_t sect = ARES_SECTION_ANSWER;
        ares_dns_rec_type_t type = ARES_REC_TYPE_A;
        ares_dns_class_t rclass = ARES_CLASS_IN;
        unsigned int ttl = 300;
        
        if (Size - offset >= 1) {
            sect = (ares_dns_section_t)Data[offset];
            offset++;
        }
        
        if (Size - offset >= 1) {
            type = (ares_dns_rec_type_t)Data[offset];
            offset++;
        }
        
        if (Size - offset >= 1) {
            rclass = (ares_dns_class_t)Data[offset];
            offset++;
        }
        
        if (Size - offset >= 4) {
            ttl = (Data[offset] << 24) | (Data[offset + 1] << 16) | 
                  (Data[offset + 2] << 8) | Data[offset + 3];
            offset += 4;
        }
        
        status = ares_dns_record_rr_add(&rr, dnsrec, sect, rr_name, type, rclass, ttl);
        
        if (status == ARES_SUCCESS && rr != NULL) {
            // Set various fields on the resource record
            ares_dns_rr_key_t key;
            unsigned short val_u16;
            unsigned char val_u8;
            
            // First ares_dns_rr_set_u16
            if (Size - offset >= 2) {
                key = (ares_dns_rr_key_t)((Data[offset] << 8) | Data[offset + 1]);
                offset += 2;
                
                if (Size - offset >= 2) {
                    val_u16 = (Data[offset] << 8) | Data[offset + 1];
                    offset += 2;
                    ares_dns_rr_set_u16(rr, key, val_u16);
                }
            }
            
            // ares_dns_rr_set_u8
            if (Size - offset >= 1) {
                key = (ares_dns_rr_key_t)Data[offset];
                offset++;
                
                if (Size - offset >= 1) {
                    val_u8 = Data[offset];
                    offset++;
                    ares_dns_rr_set_u8(rr, key, val_u8);
                }
            }
            
            // Second ares_dns_rr_set_u16
            if (Size - offset >= 2) {
                key = (ares_dns_rr_key_t)((Data[offset] << 8) | Data[offset + 1]);
                offset += 2;
                
                if (Size - offset >= 2) {
                    val_u16 = (Data[offset] << 8) | Data[offset + 1];
                    offset += 2;
                    ares_dns_rr_set_u16(rr, key, val_u16);
                }
            }
            
            // ares_dns_rr_set_opt
            if (Size - offset >= 2) {
                key = (ares_dns_rr_key_t)((Data[offset] << 8) | Data[offset + 1]);
                offset += 2;
                
                if (Size - offset >= 2) {
                    unsigned short opt = (Data[offset] << 8) | Data[offset + 1];
                    offset += 2;
                    
                    size_t remaining = Size - offset;
                    const unsigned char* val = NULL;
                    size_t val_len = 0;
                    
                    if (remaining > 0) {
                        val = Data + offset;
                        val_len = remaining > 256 ? 256 : remaining;
                    }
                    
                    ares_dns_rr_set_opt(rr, key, opt, val, val_len);
                }
            }
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}