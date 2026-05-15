// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy callback that does nothing
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *pcap_handle = NULL;
    int nonblock_ret;
    int activate_ret;
    int loop_ret;
    char *error_msg;
    
    // Clear error buffer
    memset(errbuf, 0, sizeof(errbuf));
    
    // 1. pcap_findalldevs
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Error finding devices, but continue with other tests
    }
    
    // 2. pcap_freealldevs
    pcap_freealldevs(alldevs);
    
    // Create device name from fuzzer input
    char device_name[256];
    if (Size > 0 && Size < sizeof(device_name)) {
        memcpy(device_name, Data, Size);
        device_name[Size] = '\0';
    } else if (Size >= sizeof(device_name)) {
        memcpy(device_name, Data, sizeof(device_name) - 1);
        device_name[sizeof(device_name) - 1] = '\0';
    } else {
        // Use default device name if no input
        device_name[0] = '\0';
    }
    
    // 3. pcap_create
    pcap_handle = pcap_create(device_name, errbuf);
    if (pcap_handle == NULL) {
        // Failed to create handle, cannot proceed with most tests
        return 0;
    }
    
    // 4. pcap_setnonblock (before activation)
    nonblock_ret = pcap_setnonblock(pcap_handle, Size > 0 ? Data[0] & 1 : 0, errbuf);
    
    // 5. pcap_getnonblock (before activation)
    pcap_getnonblock(pcap_handle, errbuf);
    
    // 6. pcap_activate
    activate_ret = pcap_activate(pcap_handle);
    
    // 7. pcap_getnonblock (after activation)
    pcap_getnonblock(pcap_handle, errbuf);
    
    // 8. pcap_setnonblock (after activation)
    if (Size > 1) {
        nonblock_ret = pcap_setnonblock(pcap_handle, Data[1] & 1, errbuf);
    }
    
    // 9. pcap_getnonblock
    pcap_getnonblock(pcap_handle, errbuf);
    
    // 10. pcap_setnonblock
    if (Size > 2) {
        nonblock_ret = pcap_setnonblock(pcap_handle, Data[2] & 1, errbuf);
    }
    
    // 11. pcap_getnonblock
    pcap_getnonblock(pcap_handle, errbuf);
    
    // 12. pcap_loop - use small count to avoid hanging
    int loop_count = (Size > 3) ? (Data[3] % 10) : 1; // Limit to 10 packets max
    loop_ret = pcap_loop(pcap_handle, loop_count, dummy_callback, NULL);
    
    // 13. pcap_geterr
    error_msg = pcap_geterr(pcap_handle);
    (void)error_msg; // Use variable to avoid unused warning
    
    // 14. pcap_setnonblock
    if (Size > 4) {
        nonblock_ret = pcap_setnonblock(pcap_handle, Data[4] & 1, errbuf);
    }
    
    // 15. pcap_setnonblock
    if (Size > 5) {
        nonblock_ret = pcap_setnonblock(pcap_handle, Data[5] & 1, errbuf);
    }
    
    // Cleanup - close the pcap handle
    if (pcap_handle != NULL) {
        // pcap_close is the proper way to clean up an activated pcap_t
        // Note: pcap_close is not in the API list but is necessary for cleanup
        // The pcap.h header should declare it
        pcap_close(pcap_handle);
    }
    
    return 0;
}