// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device = "lo";
    
    if (Size >= 4) {
        int snaplen = (Data[0] << 8) | Data[1];
        int promisc = Data[2] & 1;
        int timeout = Data[3] * 1000;
        
        pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
        if (pcap != NULL) {
            int snapshot_len = pcap_snapshot(pcap);
            (void)snapshot_len;
            
            int bufsize = pcap_bufsize(pcap);
            (void)bufsize;
            
            pcap_close(pcap);
        }
    }
    
    pcap_t *pcap = pcap_create(device, errbuf);
    if (pcap != NULL) {
        if (Size >= 8) {
            int snaplen = (Data[4] << 8) | Data[5];
            int tstamp_precision = Data[6] & 1 ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
            
            pcap_set_snaplen(pcap, snaplen);
            pcap_set_tstamp_precision(pcap, tstamp_precision);
        }
        
        int activate_result = pcap_activate(pcap);
        (void)activate_result;
        
        if (activate_result == 0) {
            int snapshot_len = pcap_snapshot(pcap);
            (void)snapshot_len;
            
            int bufsize = pcap_bufsize(pcap);
            (void)bufsize;
        }
        
        pcap_close(pcap);
    }
    
    pcap_t *pcap2 = pcap_create(device, errbuf);
    if (pcap2 != NULL) {
        int snapshot_err = pcap_snapshot(pcap2);
        (void)snapshot_err;
        
        int bufsize_err = pcap_bufsize(pcap2);
        (void)bufsize_err;
        
        pcap_close(pcap2);
    }
    
    return 0;
}