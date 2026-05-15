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

static int get_int_from_data(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) {
        return 0;
    }
    int value = Data[*offset];
    (*offset)++;
    return value;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Create a pcap handle for a dummy device
    pcap_t *pcap = pcap_create("lo", errbuf);
    if (pcap == NULL) {
        return 0;
    }
    
    // Call functions in specified order
    int immediate_mode = get_int_from_data(Data, Size, &offset);
    int status1 = pcap_set_immediate_mode(pcap, immediate_mode);
    (void)pcap_statustostr(status1);
    
    int timeout = get_int_from_data(Data, Size, &offset);
    int status2 = pcap_set_timeout(pcap, timeout);
    (void)pcap_statustostr(status2);
    
    int status3 = pcap_activate(pcap);
    (void)pcap_statustostr(status3);
    
    (void)pcap_geterr(pcap);
    
    // Cleanup
    pcap_close(pcap);
    
    return 0;
}