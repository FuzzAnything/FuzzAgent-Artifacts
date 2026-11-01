// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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

    // 1. Prepare environment and invoke pcap_findalldevs
    memset(errbuf, 0, sizeof(errbuf));
    ret = pcap_findalldevs(&alldevs, errbuf);
    // Handle potential failure gracefully, but continue to test cleanup
    if (ret == -1) {
        // On failure, alldevs should be NULL, but we still call free to be safe
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 2. Cleanup: Free the device list
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. Create a capture handle
    // Use NULL for device name to attempt "any" or a dummy string derived from input if valid
    const char *device_name = NULL;
    if (Size > 0) {
        // Create a temporary null-terminated string from input for device name
        // We limit the size to avoid huge allocations and ensure null termination
        size_t dev_len = (Size < 256) ? Size : 256;
        char *dev_buf = (char *)malloc(dev_len + 1);
        if (dev_buf) {
            memcpy(dev_buf, Data, dev_len);
            dev_buf[dev_len] = '\0';
            device_name = dev_buf;
        }
    }

    memset(errbuf, 0, sizeof(errbuf));
    handle = pcap_create(device_name, errbuf);
    
    // Free the temporary device buffer if we allocated one
    if (Size > 0 && device_name != NULL && device_name != (const char *)Data) {
        free((void *)device_name);
    }

    if (handle == NULL) {
        // If creation fails, we can't test the subsequent configuration functions
        // This is a valid fuzzing path (testing error handling)
        return 0;
    }

    // 4. Invoke pcap_set_snaplen
    // Use input data to determine snaplen, ensuring it's within reasonable bounds
    int snaplen = 65535;
    if (Size >= 2) {
        uint16_t val = (uint16_t)Data[0] | ((uint16_t)Data[1] << 8);
        // Clamp to valid range (0 to 65535)
        if (val > 65535) val = 65535;
        snaplen = val;
    }
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret; // Suppress unused result warning, we just want to trigger the code path

    // 5. Invoke pcap_statustostr with various status codes derived from input
    int status_code = PCAP_ERROR;
    if (Size >= 1) {
        status_code = (int)Data[0];
    }
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str; // Consume the result

    // 6. Invoke pcap_set_promisc
    int promisc = 0;
    if (Size >= 3) {
        promisc = (Data[2] % 2) ? 1 : 0;
    }
    ret = pcap_set_promisc(handle, promisc);
    (void)ret;

    // 7. Invoke pcap_statustostr again with a different code
    int status_code2 = PCAP_WARNING;
    if (Size >= 4) {
        status_code2 = (int)Data[3];
    }
    const char *status_str2 = pcap_statustostr(status_code2);
    (void)status_str2;

    // 8. Invoke pcap_set_rfmon
    int rfmon = 0;
    if (Size >= 5) {
        rfmon = (Data[4] % 2) ? 1 : 0;
    }
    ret = pcap_set_rfmon(handle, rfmon);
    (void)ret;

    // Cleanup: Close the handle
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}