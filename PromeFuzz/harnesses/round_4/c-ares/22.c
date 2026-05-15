// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_rr_set_addr6 at ares_dns_record.c:1121:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static ares_dns_record_t *create_dns_record(void) {
    ares_dns_record_t *dnsrec = NULL;
    /* Using ares_dns_record_create to initialize a DNS record */
    if (ares_dns_record_create(&dnsrec, 0x1234, ARES_FLAG_QR, 
                               ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        return NULL;
    }
    return dnsrec;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Create DNS record for testing */
    ares_dns_record_t *dnsrec = create_dns_record();
    if (!dnsrec) {
        return 0;
    }

    /* First ares_dns_record_rr_add */
    ares_dns_rr_t *rr1 = NULL;
    ares_status_t status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER,
                                                  "example.com", ARES_REC_TYPE_AAAA,
                                                  ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }

    /* ares_inet_pton */
    struct ares_in6_addr addr6;
    char ipv6_str[40] = {0};
    size_t ip_len = Size > 39 ? 39 : Size;
    memcpy(ipv6_str, Data, ip_len);
    ipv6_str[ip_len] = '\0';
    
    int pton_result = ares_inet_pton(AF_INET6, ipv6_str, &addr6);
    (void)pton_result; /* Result intentionally unused for fuzzing */

    /* ares_dns_rr_set_addr6 */
    if (rr1) {
        ares_dns_rr_set_addr6(rr1, ARES_RR_AAAA_ADDR, &addr6);
    }

    /* Second ares_dns_record_rr_add */
    ares_dns_rr_t *rr2 = NULL;
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_ANSWER,
                                    "test.com", ARES_REC_TYPE_A,
                                    ARES_CLASS_IN, 600);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }

    /* ares_dns_rr_set_u16 */
    if (rr2) {
        unsigned short val = (Data[0] << 8) | Data[1];
        ares_dns_rr_set_u16(rr2, ARES_RR_A_ADDR, val);
    }

    /* Cleanup */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}