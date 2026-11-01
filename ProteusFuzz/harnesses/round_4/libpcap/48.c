// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    const char *device = "lo"; // Loopback device is commonly available
    int snapshot_len = 65535;
    int promisc = 0;
    int to_ms = 100;

    // If data is too small, we can still test the handle creation and activation
    if (Size == 0) {
        Size = 1;
    }

    // Attempt to open a live handle
    handle = pcap_create(device, errbuf);
    if (handle == NULL) {
        // Fallback to open_live if create fails
        handle = pcap_open_live(device, snapshot_len, promisc, to_ms, errbuf);
        if (handle == NULL) {
            // If we cannot open a live handle, we cannot test the target functions effectively
            return 0;
        }
    }

    // Activate the handle if it was created via pcap_create
    // pcap_open_live handles are activated immediately.
    if (pcap_activate(handle) != 0) {
        pcap_close(handle);
        handle = NULL;
        return 0;
    }

    // 1. Test pcap_snapshot
    ret = pcap_snapshot(handle);
    (void)ret;

    // 2. Test pcap_minor_version
    ret = pcap_minor_version(handle);
    (void)ret;

    // 3. Test pcap_inject
    // Ensure we have a valid buffer size to inject. 
    // pcap_inject requires size > 0.
    if (Size > 0) {
        // Use the fuzz input as the packet data
        ret = pcap_inject(handle, Data, Size);
        (void)ret;
    }

    // 4. Test pcap_major_version
    ret = pcap_major_version(handle);
    (void)ret;

    // 5. Test pcap_datalink_ext
    ret = pcap_datalink_ext(handle);
    (void)ret;

    // 6. Test pcap_bufsize
    ret = pcap_bufsize(handle);
    (void)ret;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}