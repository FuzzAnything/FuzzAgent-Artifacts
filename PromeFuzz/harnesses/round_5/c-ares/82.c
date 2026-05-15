// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void fuzz_ares_dns_record_create(const uint8_t *data, size_t size) {
    ares_dns_record_t *dnsrec = NULL;
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (size >= 2) {
        id = (data[0] << 8) | data[1];
    }
    if (size >= 4) {
        flags = (data[2] << 8) | data[3];
    }
    if (size >= 5) {
        opcode = (ares_dns_opcode_t)data[4];
    }
    if (size >= 6) {
        rcode = (ares_dns_rcode_t)data[5];
    }
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status == ARES_SUCCESS && dnsrec != NULL) {
        ares_dns_record_get_opcode(dnsrec);
        ares_dns_record_get_id(dnsrec);
        ares_dns_record_get_flags(dnsrec);
        
        if (size >= 8) {
            unsigned short new_id = (data[6] << 8) | data[7];
            ares_dns_record_set_id(dnsrec, new_id);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
}

static void fuzz_ares_dns_parse(const uint8_t *data, size_t size) {
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    if (size >= 4) {
        flags = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    }
    
    ares_status_t status = ares_dns_parse(data, size, flags, &dnsrec);
    if (status == ARES_SUCCESS && dnsrec != NULL) {
        ares_dns_record_get_opcode(dnsrec);
        ares_dns_record_get_id(dnsrec);
        ares_dns_record_get_flags(dnsrec);
        
        if (size >= 6) {
            unsigned short new_id = (data[4] << 8) | data[5];
            ares_dns_record_set_id(dnsrec, new_id);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_ares_dns_record_create(Data, Size);
    fuzz_ares_dns_parse(Data, Size);
    
    return 0;
}