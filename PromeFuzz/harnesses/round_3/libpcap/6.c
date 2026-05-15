// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
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

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Do nothing with the packet
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *pcap_handle = NULL;
    int nonblock_ret;
    
    // Step 1: Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Error finding devices - continue anyway
    }
    
    // Step 2: Free devices
    pcap_freealldevs(alldevs);
    
    // Step 3: Create pcap handle
    // Use first null-terminated string from fuzzer input as device name
    const char *device = NULL;
    if (Size > 0) {
        // Find null terminator in input
        size_t i;
        for (i = 0; i < Size && Data[i] != 0; i++);
        if (i < Size) {
            device = (const char *)Data;
        }
    }
    
    pcap_handle = pcap_create(device, errbuf);
    if (pcap_handle == NULL) {
        // Cannot create handle - early return
        return 0;
    }
    
    // Step 4: Set non-blocking mode (before activation)
    nonblock_ret = pcap_setnonblock(pcap_handle, (Size > 0) ? (Data[0] & 1) : 0, errbuf);
    
    // Step 5: Get non-blocking mode (before activation)
    pcap_getnonblock(pcap_handle, errbuf);
    
    // Step 6: Activate the handle
    int activate_ret = pcap_activate(pcap_handle);
    
    // Step 7: Get non-blocking mode (after activation)
    pcap_getnonblock(pcap_handle, errbuf);
    
    // Step 8: Set non-blocking mode (after activation)
    nonblock_ret = pcap_setnonblock(pcap_handle, (Size > 1) ? (Data[1] & 1) : 0, errbuf);
    
    // Step 9: Get non-blocking mode
    pcap_getnonblock(pcap_handle, errbuf);
    
    // Step 10: Set non-blocking mode
    nonblock_ret = pcap_setnonblock(pcap_handle, (Size > 2) ? (Data[2] & 1) : 0, errbuf);
    
    // Step 11: Get non-blocking mode
    pcap_getnonblock(pcap_handle, errbuf);
    
    // Step 12: Start packet loop (with small count)
    if (activate_ret == 0) {
        // Use count from fuzzer input if available, otherwise use 1
        int count = (Size > 3) ? (Data[3] % 10) : 1;
        pcap_loop(pcap_handle, count, dummy_callback, NULL);
    }
    
    // Step 13: Get error message
    char *error_str = pcap_geterr(pcap_handle);
    (void)error_str; // Use error_str to avoid unused variable warning
    
    // Step 14: Set non-blocking mode
    nonblock_ret = pcap_setnonblock(pcap_handle, (Size > 4) ? (Data[4] & 1) : 0, errbuf);
    
    // Step 15: Set non-blocking mode
    nonblock_ret = pcap_setnonblock(pcap_handle, (Size > 5) ? (Data[5] & 1) : 0, errbuf);
    
    // Cleanup
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}