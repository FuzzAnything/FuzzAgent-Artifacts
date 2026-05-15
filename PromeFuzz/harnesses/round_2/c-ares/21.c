// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_rr_set_addr6 at ares_dns_record.c:1121:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static const char *extract_string(const uint8_t *data, size_t size, size_t *pos, size_t max_len) {
    if (*pos >= size) {
        return NULL;
    }
    size_t start = *pos;
    size_t end = start;
    while (end < size && end - start < max_len && data[end] != 0) {
        ++end;
    }
    if (end < size) {
        *pos = end + 1;
    } else {
        *pos = size;
    }
    if (end - start == 0) {
        return "";
    }
    static char buf[256];
    size_t copy_len = (end - start < sizeof(buf) - 1) ? end - start : sizeof(buf) - 1;
    memcpy(buf, data + start, copy_len);
    buf[copy_len] = '\0';
    return buf;
}

static void fuzz_invoke_functions(const uint8_t *data, size_t size) {
    size_t pos = 0;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    
    if (ares_dns_record_create(&dnsrec, 0x1234, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        goto cleanup;
    }
    
    const char *name1 = extract_string(data, size, &pos, 255);
    if (name1 == NULL) {
        name1 = "example.com";
    }
    
    ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, name1, ARES_REC_TYPE_A, ARES_CLASS_IN, 300);
    
    const char *ipv6_str = extract_string(data, size, &pos, 255);
    if (ipv6_str == NULL) {
        ipv6_str = "::1";
    }
    
    struct ares_in6_addr addr6;
    int pton_result = ares_inet_pton(AF_INET6, ipv6_str, &addr6);
    if (pton_result == 1) {
        ares_dns_rr_set_addr6(rr1, ARES_RR_A_ADDR, &addr6);
    }
    
    const char *name2 = extract_string(data, size, &pos, 255);
    if (name2 == NULL) {
        name2 = "test.org";
    }
    
    ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY, name2, ARES_REC_TYPE_TXT, ARES_CLASS_IN, 600);
    
    if (size - pos >= 4) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(data[pos] % 1000);
        unsigned short val = (data[pos + 1] << 8) | data[pos + 2];
        ares_dns_rr_set_u16(rr2, key, val);
    }

cleanup:
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    fuzz_invoke_functions(Data, Size);
    
    return 0;
}