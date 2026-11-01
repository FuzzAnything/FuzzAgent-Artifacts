// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
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

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];

    // Use "any" as the device string. 
    const char *device = "any";

    // Create a pcap handle
    pcap_t *handle = pcap_create(device, errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Extract parameters from fuzz input if available
    int snaplen = 0;
    int timeout = 0;
    int protocol = 0;
    int buffer_size = 0;
    int immediate = 0;

    if (Size >= 1) snaplen = (int)Data[0];
    if (Size >= 2) timeout = (int)Data[1];
    if (Size >= 3) protocol = (int)Data[2];
    if (Size >= 4) buffer_size = (int)Data[3];
    if (Size >= 5) immediate = (int)Data[4];

    // Ensure values are within reasonable ranges
    if (snaplen < 0) snaplen = 0;
    if (timeout < 0) timeout = 0;
    if (buffer_size < 0) buffer_size = 0;

    // --- Phase 1: Pre-activation configuration ---
    
    // pcap_set_snaplen
    (void)pcap_set_snaplen(handle, snaplen);
    (void)pcap_set_snaplen(handle, 65535);
    (void)pcap_set_snaplen(handle, 0);

    // pcap_set_timeout
    (void)pcap_set_timeout(handle, timeout);
    (void)pcap_set_timeout(handle, 1000);
    (void)pcap_set_timeout(handle, 0);

    // pcap_set_protocol_linux
    (void)pcap_set_protocol_linux(handle, protocol);
    (void)pcap_set_protocol_linux(handle, 0);
    (void)pcap_set_protocol_linux(handle, 1);
    (void)pcap_set_protocol_linux(handle, 2);

    // pcap_set_buffer_size
    (void)pcap_set_buffer_size(handle, buffer_size);
    (void)pcap_set_buffer_size(handle, 262144);
    (void)pcap_set_buffer_size(handle, 0);
    (void)pcap_set_buffer_size(handle, -1);

    // pcap_set_immediate_mode
    (void)pcap_set_immediate_mode(handle, immediate);
    (void)pcap_set_immediate_mode(handle, 1);
    (void)pcap_set_immediate_mode(handle, 0);

    // --- Phase 2: Activation ---
    int activate_result = pcap_activate(handle);

    // Check if activation was successful before attempting post-activation calls
    // If activation failed, the handle might be in an invalid state for further operations
    // or the internal device string might be NULL, causing the crash seen in the ASan report.
    if (activate_result == 0) {
        // --- Phase 3: Post-activation configuration (Error paths) ---
        // These calls should return PCAP_ERROR_ACTIVATED if activation succeeded.
        
        (void)pcap_set_snaplen(handle, snaplen);
        (void)pcap_set_timeout(handle, timeout);
        (void)pcap_set_protocol_linux(handle, protocol);
        (void)pcap_set_buffer_size(handle, buffer_size);
        (void)pcap_set_immediate_mode(handle, immediate);

        // Try to activate again. 
        // According to docs, this should return PCAP_ERROR_ACTIVATED if already activated.
        (void)pcap_activate(handle);
    }

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}