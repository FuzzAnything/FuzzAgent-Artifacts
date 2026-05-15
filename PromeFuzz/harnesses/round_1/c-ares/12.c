// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_rcode at ares_dns_record.c:125:18 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_get_rcode at ares_dns_record.c:125:18 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    /* Try to parse the fuzzer input as a DNS packet */
    status = ares_dns_parse((const unsigned char *)Data, Size, 0, &dnsrec);
    
    /* Call all target functions regardless of parse success */
    ares_dns_record_get_rcode(dnsrec);
    ares_dns_record_get_id(dnsrec);
    ares_dns_record_get_flags(dnsrec);
    ares_dns_record_query_cnt(dnsrec);
    
    /* Also test with NULL pointer */
    ares_dns_record_get_rcode(NULL);
    ares_dns_record_get_id(NULL);
    ares_dns_record_get_flags(NULL);
    ares_dns_record_query_cnt(NULL);
    
    /* Clean up if record was created */
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}