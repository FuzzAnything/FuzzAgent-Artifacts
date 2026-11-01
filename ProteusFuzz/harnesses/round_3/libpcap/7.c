// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // 1. pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == -1) {
        // Failure is acceptable in fuzzing, just proceed to cleanup
        alldevs = NULL;
    }

    // 2. pcap_freealldevs
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. pcap_create
    // Use "any" as device name, or a dummy string from input if Size is sufficient
    const char *device_name = NULL;
    char *dev_buf = NULL;

    if (Size >= 2 && Data[0] != 0) {
        // Use a portion of input as device name, ensuring null termination
        size_t len = (Size < 255) ? Size : 255;
        dev_buf = (char *)malloc(len + 1);
        if (dev_buf) {
            memcpy(dev_buf, Data, len);
            dev_buf[len] = '\0';
            device_name = dev_buf;
        }
    }
    
    if (device_name == NULL) {
        device_name = "any";
    }

    handle = pcap_create(device_name, errbuf);
    
    if (dev_buf != NULL) {
        free(dev_buf);
    }

    if (handle == NULL) {
        // Handle creation failed, nothing more to do for this input
        return 0;
    }

    // 4. pcap_set_snaplen
    // Use input data to determine snaplen, ensuring it's within reasonable bounds
    int snaplen = 0;
    if (Size >= 2) {
        snaplen = (Data[0] << 8) | Data[1];
        // Clamp to a reasonable range to avoid invalid states immediately
        if (snaplen < 0) snaplen = 0;
        if (snaplen > 65535) snaplen = 65535;
    }
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret; // Ignore return for fuzzing exploration

    // 5. pcap_statustostr
    // Test with various status codes derived from input or constants
    int status_code = 0;
    if (Size >= 4) {
        status_code = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    }
    const char *msg1 = pcap_statustostr(status_code);
    (void)msg1;

    // 6. pcap_set_promisc
    int promisc = 0;
    if (Size >= 4) {
        promisc = Data[3] % 2; // 0 or 1
    }
    ret = pcap_set_promisc(handle, promisc);
    (void)ret;

    // 7. pcap_statustostr (second call)
    int status_code2 = 0;
    if (Size >= 8) {
        status_code2 = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    }
    const char *msg2 = pcap_statustostr(status_code2);
    (void)msg2;

    // 8. pcap_set_rfmon
    int rfmon = 0;
    if (Size >= 8) {
        rfmon = Data[7] % 2; // 0 or 1
    }
    ret = pcap_set_rfmon(handle, rfmon);
    (void)ret;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}