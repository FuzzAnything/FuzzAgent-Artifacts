// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_get_selectable_fd at pcap.c:3530:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid out-of-bounds reads
    if (Size < 7) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    // Extract parameters from fuzz input safely
    int snapshot = Data[0] * 64 + Data[1];
    int promisc = (Data[1] >> 4) & 1;
    int timeout = (Data[2] % 1000) + 1;
    int direction_val = Data[3] % 3; // 0: PCAP_D_IN, 1: PCAP_D_OUT, 2: PCAP_D_INOUT
    int rfmon_val = Data[4] % 2;
    int nonblock_val = Data[5] % 2;
    int timeout_set = (Data[6] % 1000) + 1;

    pcap_direction_t direction;
    switch (direction_val) {
        case 0: direction = PCAP_D_IN; break;
        case 1: direction = PCAP_D_OUT; break;
        case 2: direction = PCAP_D_INOUT; break;
        default: direction = PCAP_D_IN; break;
    }

    // Open live capture
    // Note: pcap_open_live requires a valid network device. 
    // Using NULL often defaults to "any" or fails depending on the backend.
    // If it fails, handle is NULL and we skip the rest.
    handle = pcap_open_live(NULL, snapshot, promisc, timeout, errbuf);
    if (handle == NULL) {
        // If open fails, we cannot test the other functions on this handle.
        return 0;
    }

    // Test pcap_setdirection
    ret = pcap_setdirection(handle, direction);
    (void)ret; // Ignore return value for fuzzing, just trigger code path

    // Test pcap_get_selectable_fd
    int fd = pcap_get_selectable_fd(handle);
    (void)fd;

    // Test pcap_setnonblock
    ret = pcap_setnonblock(handle, nonblock_val, errbuf);
    (void)ret;

    // Test pcap_set_timeout (should fail if handle is active, which it is after pcap_open_live)
    // pcap_open_live activates the handle immediately.
    // So this call is expected to return PCAP_ERROR_ACTIVATED.
    ret = pcap_set_timeout(handle, timeout_set);
    (void)ret;

    // Test pcap_set_rfmon (should fail if handle is active)
    ret = pcap_set_rfmon(handle, rfmon_val);
    (void)ret;

    cleanup_pcap(handle);

    return 0;
}