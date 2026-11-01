// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device_name;
    pcap_t *handle = NULL;
    int can_rfmon;
    const char *status_str;
    char *err_msg;

    // Step 1: Prepare environment
    // Use the fuzzer input to determine the device name, but ensure it's a valid C string.
    // We limit the length to avoid huge allocations and ensure null termination.
    size_t dev_len = Size > 0 ? (Size < 128 ? Size : 128) : 0;
    
    // Allocate a buffer for the device name
    char *dev_buf = NULL;
    if (dev_len > 0) {
        dev_buf = (char *)malloc(dev_len + 1);
        if (dev_buf) {
            memcpy(dev_buf, Data, dev_len);
            dev_buf[dev_len] = '\0';
            device_name = dev_buf;
        } else {
            device_name = NULL;
        }
    } else {
        device_name = NULL; // Let pcap_create use the default "any"
    }

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Step 2: Invoke pcap_create
    // This initializes the capture handle.
    handle = pcap_create(device_name, errbuf);

    // Step 3: Invoke pcap_can_set_rfmon
    // Only invoke if handle is valid. This checks monitor mode support.
    if (handle != NULL) {
        can_rfmon = pcap_can_set_rfmon(handle);
        (void)can_rfmon; // Use the result to prevent unused variable warnings
    }

    // Step 4: Invoke pcap_geterr
    // Retrieve error message. Safe to call even if handle is NULL in some implementations,
    // but spec says it takes pcap_t*. We guard it.
    if (handle != NULL) {
        err_msg = pcap_geterr(handle);
        (void)err_msg;
    }

    // Step 5: Invoke pcap_statustostr
    // Convert error codes to strings. We try a few standard codes and potentially use input data.
    // PCAP_ERROR is typically -1, PCAP_WARNING is typically 1, 0 is success.
    status_str = pcap_statustostr(PCAP_ERROR);
    (void)status_str;

    status_str = pcap_statustostr(PCAP_WARNING);
    (void)status_str;

    status_str = pcap_statustostr(0);
    (void)status_str;

    // Try to use fuzzer data as a status code if it fits in an int
    if (Size >= sizeof(int)) {
        int status_code;
        memcpy(&status_code, Data, sizeof(int));
        status_str = pcap_statustostr(status_code);
        (void)status_str;
    }

    // Step 6: Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }
    
    if (dev_buf != NULL) {
        free(dev_buf);
    }

    return 0;
}