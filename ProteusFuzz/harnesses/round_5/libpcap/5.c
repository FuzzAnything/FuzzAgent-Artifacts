// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    const char *status_str;

    // Ensure we have enough data to work with for fuzzing logic
    if (Size < 4) {
        return 0;
    }

    // 1. pcap_findalldevs
    // Initialize errbuf
    memset(errbuf, 0, sizeof(errbuf));
    ret = pcap_findalldevs(&alldevs, errbuf);
    
    // 2. pcap_freealldevs
    // Always free if alldevs was populated, even if findalldevs failed (it might have partial data)
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. pcap_create
    // Use a dummy device name or NULL. 
    const char *device_name = "any";
    if (Size > 0 && Data[0] == 0) {
        device_name = NULL;
    }
    
    memset(errbuf, 0, sizeof(errbuf));
    handle = pcap_create(device_name, errbuf);

    if (handle == NULL) {
        // If creation fails, we can't test the subsequent setters.
        return 0;
    }

    // 4. pcap_set_snaplen
    // Use input data to determine snaplen. Clamp to reasonable range to avoid crashes.
    int snaplen = 0;
    if (Size >= 2) {
        snaplen = (Data[0] << 8) | Data[1];
    }
    // Ensure snaplen is positive and not absurdly large
    if (snaplen <= 0) snaplen = 64;
    if (snaplen > 65535) snaplen = 65535;
    
    ret = pcap_set_snaplen(handle, snaplen);
    
    // 5. pcap_statustostr (for snaplen result)
    status_str = pcap_statustostr(ret);
    (void)status_str;

    // 6. pcap_set_promisc
    // Use next byte to determine promisc mode (0 or 1)
    int promisc = (Size >= 3) ? (Data[2] % 2) : 0;
    ret = pcap_set_promisc(handle, promisc);

    // 7. pcap_statustostr (for promisc result)
    status_str = pcap_statustostr(ret);
    (void)status_str;

    // 8. pcap_set_rfmon
    // Use next byte to determine rfmon mode (0 or 1)
    int rfmon = (Size >= 4) ? (Data[3] % 2) : 0;
    ret = pcap_set_rfmon(handle, rfmon);
    
    // Cleanup: Close the handle if it was successfully created
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}