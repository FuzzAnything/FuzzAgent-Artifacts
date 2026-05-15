// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
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
#include "ares.h"
#include "ares_dns_record.h"

static const char* generate_name(const uint8_t* data, size_t size) {
    static char buffer[256];
    size_t length = size < 255 ? size : 255;
    if (length == 0) {
        return "example.com";
    }
    memcpy(buffer, data, length);
    buffer[length] = '\0';
    
    for (size_t i = 0; i < length; i++) {
        unsigned char c = buffer[i];
        if (c < 32 || c > 126) {
            buffer[i] = 'a' + (c % 26);
        }
    }
    return buffer;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    ares_dns_record_t* dnsrec = NULL;
    ares_dns_rr_t* rr_out = NULL;
    
    unsigned short id = (unsigned short)((Data[0] << 8) | Data[1]);
    unsigned short flags = (unsigned short)((Data[2] << 8) | Data[3]);
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(Data[4] % 16);
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(Data[5] % 16);
    
    ares_status_t create_status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (create_status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }
    
    uint8_t sect_byte = Data[6] % 4;
    ares_dns_section_t sect = (ares_dns_section_t)sect_byte;
    
    uint8_t type_byte = Data[7] % 32;
    ares_dns_rec_type_t type = (ares_dns_rec_type_t)type_byte;
    
    uint8_t class_byte = Data[8] % 4;
    ares_dns_class_t rclass = (ares_dns_class_t)class_byte;
    
    unsigned int ttl = (unsigned int)Data[9];
    
    const char* name = generate_name(Data + 10, Size - 10);
    
    ares_status_t add_status = ares_dns_record_rr_add(
        &rr_out, dnsrec, sect, name, type, rclass, ttl
    );
    
    if (add_status == ARES_SUCCESS) {
        uint8_t idx_byte = Data[6];
        size_t idx = (size_t)(idx_byte % 4);
        
        ares_status_t del_status = ares_dns_record_rr_del(dnsrec, sect, idx);
        
        ares_dns_rr_t* rr_get = ares_dns_record_rr_get(dnsrec, sect, idx);
        
        if (rr_get) {
            const char* rr_name = ares_dns_rr_get_name(rr_get);
            if (rr_name) {
                volatile const char* unused __attribute__((unused)) = rr_name;
            }
        }
    }
    
    ares_dns_record_destroy(dnsrec);
    return 0;
}