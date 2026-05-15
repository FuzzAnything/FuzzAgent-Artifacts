// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Use fuzz data to generate parameters
    int immediate_mode = 0;
    int timeout = 0;
    
    if (Size >= 1) {
        immediate_mode = Data[0] & 1;
    }
    if (Size >= 2) {
        timeout = Data[1];
    }
    
    // Create a dummy device name from fuzz data
    char device[64] = "any";
    if (Size > 2) {
        size_t len = Size - 2;
        if (len > 63) len = 63;
        memcpy(device, Data + 2, len);
        device[len] = '\0';
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Create pcap handle using pcap_create
    pcap_t *p = pcap_create(device, errbuf);
    if (p == NULL) {
        return 0;
    }
    
    // Call functions in specified order
    // 1. pcap_set_immediate_mode
    int status1 = pcap_set_immediate_mode(p, immediate_mode);
    
    // 2. pcap_statustostr
    const char *str1 = pcap_statustostr(status1);
    (void)str1;
    
    // 3. pcap_set_timeout
    int status2 = pcap_set_timeout(p, timeout);
    
    // 4. pcap_statustostr
    const char *str2 = pcap_statustostr(status2);
    (void)str2;
    
    // 5. pcap_activate
    int status3 = pcap_activate(p);
    
    // 6. pcap_statustostr
    const char *str3 = pcap_statustostr(status3);
    (void)str3;
    
    // 7. pcap_geterr
    char *err = pcap_geterr(p);
    (void)err;
    
    // Cleanup
    pcap_close(p);
    
    return 0;
}