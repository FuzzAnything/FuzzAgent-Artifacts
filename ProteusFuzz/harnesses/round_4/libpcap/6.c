// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *device_name = NULL;
    char *device_name_copy = NULL;
    int ret;

    // 1. Prepare environment and invoke pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    
    // Extract a device name from the list if available to use in pcap_create
    if (ret == 0 && alldevs != NULL) {
        // Make a copy of the device name before freeing the list
        device_name_copy = strdup(alldevs->name);
        if (device_name_copy != NULL) {
            device_name = device_name_copy;
        }
    } else {
        device_name = NULL; // Use NULL for "any"
    }

    // 2. Cleanup: Free the device list immediately after extraction
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. Invoke pcap_create
    handle = pcap_create(device_name, errbuf);

    if (handle == NULL) {
        // If creation fails, test pcap_statustostr with a generic error
        (void)pcap_statustostr(PCAP_ERROR);
        free(device_name_copy);
        return 0;
    }

    // 4. Invoke pcap_set_snaplen
    int snaplen = 64;
    if (Size > 0) {
        snaplen = (int)Data[0] + 64; 
        if (snaplen > 65535) snaplen = 65535;
    }
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret;

    // 5. Invoke pcap_statustostr
    int status_code = PCAP_ERROR;
    if (Size > 1) {
        status_code = (int)Data[1];
    }
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str;

    // 6. Invoke pcap_set_promisc
    int promisc = 0;
    if (Size > 2) {
        promisc = (int)Data[2] % 2;
    }
    ret = pcap_set_promisc(handle, promisc);
    (void)ret;

    // 7. Invoke pcap_statustostr again
    if (Size > 3) {
        status_code = (int)Data[3];
        status_str = pcap_statustostr(status_code);
        (void)status_str;
    }

    // 8. Invoke pcap_set_rfmon
    int rfmon = 0;
    if (Size > 4) {
        rfmon = (int)Data[4] % 2;
    }
    ret = pcap_set_rfmon(handle, rfmon);
    (void)ret;

    // Cleanup: Close the handle if it was created
    if (handle != NULL) {
        pcap_close(handle);
    }

    // Free the copied device name
    if (device_name_copy != NULL) {
        free(device_name_copy);
    }

    return 0;
}