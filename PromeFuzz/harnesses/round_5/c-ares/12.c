// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_u32 at ares_dns_record.c:1176:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static const char *dummy_name = "example.com";
static const char *dummy_str = "test";

static void dummy_cleanup(ares_dns_record_t *dnsrec) {
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Initialize DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, ARES_FLAG_QR, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }

    // Create first RR
    ares_dns_rr_t *rr1 = NULL;
    status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER,
                                                  dummy_name, ARES_REC_TYPE_TXT,
                                                  ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || !rr1) {
        dummy_cleanup(dnsrec);
        return 0;
    }

    // Set binary data on first RR
    size_t bin_len = Size > 0 ? Size : 1;
    const unsigned char *bin_data = Size > 0 ? Data : (const unsigned char *)"";
    ares_dns_rr_set_bin(rr1, ARES_RR_TXT_DATA, bin_data, bin_len);

    // Create second RR
    ares_dns_rr_t *rr2 = NULL;
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY,
                                    dummy_name, ARES_REC_TYPE_CNAME,
                                    ARES_CLASS_IN, 600);
    if (status != ARES_SUCCESS || !rr2) {
        dummy_cleanup(dnsrec);
        return 0;
    }

    // Set string data on second RR
    ares_dns_rr_set_str(rr2, ARES_RR_CNAME_CNAME, dummy_str);

    // Create third RR
    ares_dns_rr_t *rr3 = NULL;
    status = ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_ADDITIONAL,
                                    dummy_name, ARES_REC_TYPE_MX,
                                    ARES_CLASS_IN, 1200);
    if (status != ARES_SUCCESS || !rr3) {
        dummy_cleanup(dnsrec);
        return 0;
    }

    // Set string data on third RR (exchange field)
    ares_dns_rr_set_str(rr3, ARES_RR_MX_EXCHANGE, dummy_str);

    // Set another string on third RR (name field)
    ares_dns_rr_set_str(rr3, ARES_RR_MX_EXCHANGE, dummy_str);

    // Set unsigned 32-bit integer on third RR (preference field)
    unsigned int u32_val = Size > 0 ? Data[0] : 10;
    ares_dns_rr_set_u32(rr3, ARES_RR_MX_PREFERENCE, u32_val);

    // Cleanup
    dummy_cleanup(dnsrec);
    return 0;
}