// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_duplicate at ares_dns_record.c:1664:20 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    unsigned short id = (Data[0] << 8) | Data[1];
    unsigned short flags = (Data[2] << 8) | Data[3];
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)Data[4];
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)Data[5];

    ares_dns_record_t *dnsrec = NULL;
    ares_status_t create_result = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);

    if (create_result == ARES_SUCCESS && dnsrec != NULL) {
        unsigned short retrieved_id = ares_dns_record_get_id(dnsrec);
        unsigned short retrieved_flags = ares_dns_record_get_flags(dnsrec);

        ares_dns_record_t *duplicate = ares_dns_record_duplicate(dnsrec);
        if (duplicate != NULL) {
            unsigned short dup_id = ares_dns_record_get_id(duplicate);
            unsigned short dup_flags = ares_dns_record_get_flags(duplicate);
            ares_dns_record_destroy(duplicate);
        }

        ares_dns_record_destroy(dnsrec);
    }

    return 0;
}