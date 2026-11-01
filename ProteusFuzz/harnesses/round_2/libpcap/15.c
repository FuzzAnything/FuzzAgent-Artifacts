// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device_name = NULL;

    // Use input data to derive a device name string if possible
    // We try to create a valid-looking string from the input, or use NULL
    if (Size > 0) {
        // Create a buffer for the device name
        char *dev_buf = (char *)malloc(Size + 1);
        if (dev_buf != NULL) {
            memcpy(dev_buf, Data, Size);
            dev_buf[Size] = '\0';
            // Only use as device name if it doesn't contain nulls in the middle (already handled by memcpy)
            // We can pass this directly or NULL. Let's try to pass it if it looks like a device name
            // For fuzzing, we can just pass the raw buffer as the device name
            device_name = dev_buf;
        }
    }

    // 1. pcap_create
    pcap_t *handle = pcap_create(device_name, errbuf);

    // 2. pcap_can_set_rfmon
    // This function requires a valid handle. If handle is NULL, skip or handle gracefully.
    int can_rfmon = 0;
    if (handle != NULL) {
        can_rfmon = pcap_can_set_rfmon(handle);
    }

    // 3. pcap_geterr
    // This function requires a valid handle.
    const char *err_msg = NULL;
    if (handle != NULL) {
        err_msg = pcap_geterr(handle);
    }

    // 4. pcap_statustostr
    // We can fuzz with different status codes. Use input data to generate a status code.
    int status_code = 0;
    if (Size > 0) {
        status_code = (int)Data[0];
    }
    // Also try some common error codes
    const char *status_str = pcap_statustostr(status_code);
    
    // Additional exploration: try other status codes from input
    if (Size > 1) {
        int status_code2 = (int)Data[1];
        pcap_statustostr(status_code2);
    }
    if (Size > 2) {
        int status_code3 = (int)Data[2];
        pcap_statustostr(status_code3);
    }

    // Cleanup
    cleanup_pcap(handle);
    
    // Free device name buffer if allocated
    if (device_name != NULL && Size > 0) {
        free((void *)device_name);
    }

    return 0;
}