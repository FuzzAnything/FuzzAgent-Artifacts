// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
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

void dummy_callback(void *arg, int status, int timeouts, unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize library
    ares_channel channel;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create channel
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Create DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    
    // First ares_dns_record_rr_add
    if (Size > 0) {
        char name[256];
        size_t name_len = Size > 255 ? 255 : Size;
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        
        ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, name, 
                              ARES_REC_TYPE_A, ARES_CLASS_IN, 3600);
    }
    
    // ares_inet_pton
    if (Size > 0) {
        char src[256];
        size_t src_len = Size > 255 ? 255 : Size;
        memcpy(src, Data, src_len);
        src[src_len] = '\0';
        
        struct ares_in6_addr dst6;
        struct in_addr dst4;
        
        // Try both IPv4 and IPv6
        ares_inet_pton(AF_INET, src, &dst4);
        ares_inet_pton(AF_INET6, src, &dst6);
    }
    
    // ares_dns_rr_set_addr6
    if (rr1 != NULL) {
        struct ares_in6_addr addr6;
        memset(&addr6, 0, sizeof(addr6));
        if (Size >= 16) {
            memcpy(&addr6._S6_un._S6_u8, Data, 16);
        }
        
        ares_dns_rr_set_addr6(rr1, ARES_RR_AAAA_ADDR, &addr6);
    }
    
    // Second ares_dns_record_rr_add
    if (Size > 0) {
        char name2[256];
        size_t name2_len = Size > 255 ? 255 : Size;
        memcpy(name2, Data, name2_len);
        name2[name2_len] = '\0';
        
        ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY, name2,
                              ARES_REC_TYPE_NS, ARES_CLASS_IN, 7200);
    }
    
    // ares_dns_rr_set_u16
    if (rr2 != NULL) {
        unsigned short val = 0;
        if (Size >= 2) {
            val = (Data[0] << 8) | Data[1];
        }
        
        ares_dns_rr_set_u16(rr2, ARES_RR_OPT_FLAGS, val);
    }
    
    // Cleanup
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}