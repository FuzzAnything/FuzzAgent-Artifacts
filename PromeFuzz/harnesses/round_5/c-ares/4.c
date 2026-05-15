// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_get_addr6 at ares_dns_record.c:774:29 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_callback(void *arg, ares_status_t status, 
                          time_t timeout, unsigned char *abuf, int alen) {
    /* Dummy callback, does nothing */
    (void)arg;
    (void)status;
    (void)timeout;
    (void)abuf;
    (void)alen;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    /* Initialize library if needed */
    static int initialized = 0;
    if (!initialized) {
        ares_library_init(ARES_LIB_INIT_ALL);
        initialized = 1;
    }

    /* Create a dummy DNS record for testing */
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, 0);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }

    /* Add a question to the record */
    ares_dns_record_query_add(dnsrec, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);

    /* Create a resource record for testing */
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || rr == NULL) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }

    /* Set some data in the resource record */
    struct in_addr addr;
    if (Size >= 4) {
        memcpy(&addr, Data, 4);
        ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
    }

    /* 1. Call ares_dns_rr_key_datatype */
    ares_dns_datatype_t dt = ares_dns_rr_key_datatype(ARES_RR_A_ADDR);
    (void)dt;

    /* 2. Call ares_inet_ntop (IPv4) */
    char dst1[INET_ADDRSTRLEN];
    if (Size >= 4) {
        const char *result1 = ares_inet_ntop(AF_INET, Data, dst1, sizeof(dst1));
        (void)result1;
    }

    /* 3. Call ares_dns_rr_get_addr */
    const struct in_addr *addr_ptr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
    (void)addr_ptr;

    /* 4. Call ares_inet_ntop (IPv6) */
    char dst2[INET6_ADDRSTRLEN];
    if (Size >= 16) {
        const char *result2 = ares_inet_ntop(AF_INET6, Data, dst2, sizeof(dst2));
        (void)result2;
    }

    /* 5. Call ares_dns_rr_get_addr6 */
    const struct ares_in6_addr *addr6_ptr = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
    (void)addr6_ptr;

    /* 6. Call ares_dns_rr_get_abin_cnt */
    size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_OPT_OPTIONS);
    (void)abin_cnt;

    /* 7. Call ares_dns_write */
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    status = ares_dns_write(dnsrec, &buf, &buf_len);
    if (status == ARES_SUCCESS && buf != NULL) {
        ares_free_string(buf);
    }

    /* 8. Call ares_dns_record_destroy */
    ares_dns_record_destroy(dnsrec);

    /* Additional fuzzing: try to parse the input as a DNS message */
    if (Size > 0) {
        ares_dns_record_t *parsed_rec = NULL;
        status = ares_dns_parse(Data, Size, 0, &parsed_rec);
        if (status == ARES_SUCCESS && parsed_rec != NULL) {
            /* Try to get some resource records from the parsed record */
            size_t ancount = ares_dns_record_rr_cnt(parsed_rec, ARES_SECTION_ANSWER);
            if (ancount > 0) {
                const ares_dns_rr_t *parsed_rr = ares_dns_record_rr_get(parsed_rec, ARES_SECTION_ANSWER, 0);
                if (parsed_rr != NULL) {
                    /* Try various operations on the parsed RR */
                    ares_dns_datatype_t parsed_dt = ares_dns_rr_key_datatype(ARES_RR_A_ADDR);
                    (void)parsed_dt;
                    
                    const struct in_addr *parsed_addr = ares_dns_rr_get_addr(parsed_rr, ARES_RR_A_ADDR);
                    (void)parsed_addr;
                    
                    size_t parsed_abin_cnt = ares_dns_rr_get_abin_cnt(parsed_rr, ARES_RR_TXT_DATA);
                    (void)parsed_abin_cnt;
                }
            }
            ares_dns_record_destroy(parsed_rec);
        }
    }

    return 0;
}