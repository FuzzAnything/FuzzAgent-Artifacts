// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int immediate_mode;
    int timeout_val;
    int ret;

    if (Size < 2) {
        return 0;
    }

    // Extract parameters from fuzz input
    immediate_mode = Data[0] % 2; // 0 or 1
    timeout_val = (int)(Data[1] * 100) + 1; // 1 to 10001 ms

    // Initialize error buffer
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    // Create a pcap handle using pcap_create with a dummy device
    // We use "any" or a dummy interface name; pcap_create will fail gracefully if invalid
    handle = pcap_create("any", errbuf);
    
    if (handle == NULL) {
        // If creation fails, we can still test statustostr with potential error codes
        const char *status_str = pcap_statustostr(PCAP_ERROR);
        (void)status_str; // Suppress unused warning
        return 0;
    }

    // 1. pcap_set_immediate_mode
    ret = pcap_set_immediate_mode(handle, immediate_mode);
    // Check status
    const char *status1 = pcap_statustostr(ret);
    (void)status1;

    // 2. pcap_set_timeout
    ret = pcap_set_timeout(handle, timeout_val);
    // Check status
    const char *status2 = pcap_statustostr(ret);
    (void)status2;

    // 3. pcap_activate
    ret = pcap_activate(handle);
    // Check status
    const char *status3 = pcap_statustostr(ret);
    (void)status3;

    // 4. pcap_geterr
    // Retrieve error message if activation failed or for inspection
    const char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}