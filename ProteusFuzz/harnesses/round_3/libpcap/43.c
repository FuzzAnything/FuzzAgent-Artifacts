// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_get_selectable_fd at pcap.c:3530:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *device = NULL;
    int snaplen = 0;
    int promisc = 0;
    int to_ms = 0;
    int nonblock = 0;
    int rfmon = 0;
    int timeout = 0;
    pcap_direction_t direction = PCAP_D_INOUT;

    // Derive parameters from fuzz input safely
    if (Size >= 4) {
        snaplen = (int)(Data[0] | (Data[1] << 8));
        promisc = (Data[2] % 2);
        to_ms = (int)(Data[3] | (Size % 1000));
    }

    if (Size >= 6) {
        nonblock = (Data[4] % 2);
        rfmon = (Data[5] % 2);
    }

    // Ensure we have enough data for direction before accessing index 8
    if (Size >= 9) {
        timeout = (int)(Data[6] | (Data[7] << 8));
        direction = (pcap_direction_t)(Data[8] % 3); // 0: IN, 1: OUT, 2: INOUT
    } else {
        timeout = 100;
        direction = PCAP_D_INOUT;
    }

    // Use "any" or a dummy device name based on input
    // We must allocate memory for the device string to avoid stack-use-after-scope
    // because pcap_open_live (via pcap_create) may strdup the device string.
    char *device_buf = NULL;
    
    if (Size >= 11) {
        // Construct a dummy device name from input to vary states
        int len = (Size % 10) + 1;
        if (len > 15) len = 15;
        // Ensure we don't read past the buffer
        if (Size < 10 + len) len = Size - 10;
        
        device_buf = (char *)malloc(len + 1);
        if (device_buf != NULL) {
            memcpy(device_buf, Data + 10, len);
            device_buf[len] = '\0';
            device = device_buf;
        } else {
            device = "any";
        }
    } else {
        device = "any";
    }

    // Ensure reasonable bounds for parameters to avoid immediate crashes
    if (snaplen <= 0) snaplen = 65535;
    if (to_ms < 0) to_ms = 100;
    if (timeout < 0) timeout = 100;

    // Initialize error buffer
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    // Step 1: Open live capture
    handle = pcap_open_live(device, snaplen, promisc, to_ms, errbuf);
    
    if (handle == NULL) {
        // If opening fails, we cannot test functions on the handle.
        if (device_buf != NULL) {
            free(device_buf);
        }
        return 0;
    }

    // Step 2: Test pcap_set_rfmon
    // Note: pcap_open_live often activates immediately, so this may return error.
    pcap_set_rfmon(handle, rfmon);

    // Step 3: Test pcap_set_timeout
    pcap_set_timeout(handle, timeout);

    // Step 4: Test pcap_setnonblock
    pcap_setnonblock(handle, nonblock, errbuf);

    // Step 5: Test pcap_setdirection
    pcap_setdirection(handle, direction);

    // Step 6: Test pcap_get_selectable_fd
    int fd = pcap_get_selectable_fd(handle);
    (void)fd;

    // Cleanup
    cleanup_pcap(handle);
    
    if (device_buf != NULL) {
        free(device_buf);
    }

    return 0;
}