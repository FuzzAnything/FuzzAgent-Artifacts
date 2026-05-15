// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    /* Create a dummy DNS record to get a valid ares_dns_rr_t pointer */
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_rr_t *rr_out = NULL;
    ares_status_t status;
    
    /* Initialize the library if needed */
    static int initialized = 0;
    if (!initialized) {
        ares_library_init(ARES_LIB_INIT_ALL);
        initialized = 1;
    }
    
    /* Create a DNS record */
    status = ares_dns_record_create(&dnsrec, 0x1234, ARES_FLAG_QR | ARES_FLAG_RD, 
                                    ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    /* Add a resource record to get a valid ares_dns_rr_t pointer */
    const char *name = "example.com";
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    ares_dns_class_t qclass = ARES_CLASS_IN;
    unsigned int ttl = 300;
    
    /* Use fuzzer data to vary parameters if we have enough data */
    if (Size >= 2) {
        type = (ares_dns_rec_type_t)(Data[0] % 256);
        qclass = (ares_dns_class_t)(Data[1] % 256);
    }
    
    /* Add the resource record - fixed to use correct number of arguments */
    status = ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ANSWER, 
                                    name, type, qclass, ttl);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* Now call the functions in the specified order */
    const char *rr_name = ares_dns_rr_get_name(rr_out);
    (void)rr_name;
    
    const char *class_str = ares_dns_class_tostr(qclass);
    (void)class_str;
    
    ares_dns_class_t rr_class = ares_dns_rr_get_class(rr_out);
    (void)rr_class;
    
    const char *type_str = ares_dns_rec_type_tostr(type);
    (void)type_str;
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}