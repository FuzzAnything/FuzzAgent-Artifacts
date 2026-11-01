// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <limits.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    if (Size < 1) {
        return 0;
    }

    // Attempt to create a pcap handle using pcap_create
    // Using "lo" as a common loopback interface. If it fails, we test error paths.
    handle = pcap_create("lo", errbuf);
    if (handle == NULL) {
        // If creation fails, we cannot test further.
        // This is a valid execution path for the fuzzer to explore.
        return 0;
    }

    // Try to activate the handle
    ret = pcap_activate(handle);
    
    // Test functions that check activation status
    // If ret != 0, handle is not active, these should return PCAP_ERROR_NOT_ACTIVATED
    (void)pcap_snapshot(handle);
    (void)pcap_datalink_ext(handle);
    (void)pcap_bufsize(handle);
    (void)pcap_major_version(handle);
    (void)pcap_minor_version(handle);

    if (ret != 0) {
        // Handle activation failed. Close and return.
        pcap_close(handle);
        return 0;
    }

    // If activated, re-test functions that require an active handle
    (void)pcap_snapshot(handle);
    (void)pcap_datalink_ext(handle);
    (void)pcap_bufsize(handle);
    (void)pcap_major_version(handle);
    (void)pcap_minor_version(handle);

    // Test pcap_inject
    // Ensure size is within valid range (>0 and <= INT_MAX)
    size_t inject_size = Size;
    const uint8_t *inject_data = Data;

    if (inject_size > INT_MAX) {
        inject_size = INT_MAX;
    }
    
    // pcap_inject requires size > 0. Size is guaranteed > 0 by initial check.
    ret = pcap_inject(handle, inject_data, inject_size);
    (void)ret; // Ignore return value

    // Cleanup
    pcap_close(handle);

    return 0;
}