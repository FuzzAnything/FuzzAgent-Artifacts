// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_record(const uint8_t *data, size_t size) {
    // Create a dummy file with the fuzzer input data
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return NULL;
    }
    
    fwrite(data, 1, size, f);
    fclose(f);
    
    // Parse the dummy file as a DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    // Try to parse as a DNS response
    f = fopen("./dummy_file", "rb");
    if (!f) {
        return NULL;
    }
    
    // Read the entire file
    uint8_t *buffer = (uint8_t*)malloc(size);
    if (!buffer) {
        fclose(f);
        return NULL;
    }
    
    size_t bytes_read = fread(buffer, 1, size, f);
    fclose(f);
    
    if (bytes_read > 0) {
        // Try to parse the DNS data
        status = ares_dns_parse(buffer, bytes_read, 0, &dnsrec);
    }
    
    free(buffer);
    return dnsrec;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Create a DNS record from fuzzer input
    ares_dns_record_t *dnsrec = create_test_record(Data, Size);
    
    if (!dnsrec) {
        // Even if we couldn't create a record, we can still test the functions with NULL
        size_t cnt1 = ares_dns_record_rr_cnt(NULL, ARES_SECTION_ANSWER);
        size_t cnt2 = ares_dns_record_rr_cnt(NULL, ARES_SECTION_AUTHORITY);
        size_t cnt3 = ares_dns_record_rr_cnt(NULL, ARES_SECTION_ADDITIONAL);
        
        const char *opcode_str = ares_dns_opcode_tostr(ARES_OPCODE_QUERY);
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(NULL);
        const char *rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
        
        (void)cnt1;
        (void)cnt2;
        (void)cnt3;
        (void)opcode_str;
        (void)opcode;
        (void)rcode_str;
        
        return 0;
    }
    
    // Call functions in specified order
    // 1. ares_dns_record_rr_cnt (three times with different sections)
    size_t cnt1 = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    size_t cnt2 = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
    size_t cnt3 = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
    
    // 2. ares_dns_opcode_tostr
    // First get an opcode from the record
    ares_dns_opcode_t record_opcode = ares_dns_record_get_opcode(dnsrec);
    const char *opcode_str = ares_dns_opcode_tostr(record_opcode);
    
    // Also test with some other opcodes from the input data
    if (Size > 0) {
        ares_dns_opcode_t test_opcode = (ares_dns_opcode_t)(Data[0] % 6);
        const char *test_opcode_str = ares_dns_opcode_tostr(test_opcode);
        (void)test_opcode_str;
    }
    
    // 3. ares_dns_record_get_opcode
    ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
    
    // 4. ares_dns_rcode_tostr
    // Get rcode from the record
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    // Try different rcode values based on input
    if (Size > 1) {
        rcode = (ares_dns_rcode_t)(Data[1] % 24);
    }
    const char *rcode_str = ares_dns_rcode_tostr(rcode);
    
    // Test with some edge rcode values
    const char *edge_rcode_str1 = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
    const char *edge_rcode_str2 = ares_dns_rcode_tostr(ARES_RCODE_FORMERR);
    const char *edge_rcode_str3 = ares_dns_rcode_tostr(ARES_RCODE_NOTAUTH);
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    // Use variables to avoid optimization
    (void)cnt1;
    (void)cnt2;
    (void)cnt3;
    (void)opcode_str;
    (void)opcode;
    (void)rcode_str;
    (void)edge_rcode_str1;
    (void)edge_rcode_str2;
    (void)edge_rcode_str3;
    
    return 0;
}