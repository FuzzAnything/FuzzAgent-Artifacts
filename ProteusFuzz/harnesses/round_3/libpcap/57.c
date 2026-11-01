// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_pcap_api(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    int ret;
    int snaplen;
    int timeout;
    int protocol;
    int buffer_size;
    int immediate_mode;

    if (Size < 5) {
        return 0;
    }

    // Extract fuzzing parameters from input data
    snaplen = (int)Data[0];
    timeout = (int)Data[1];
    protocol = (int)Data[2];
    buffer_size = (int)Data[3];
    immediate_mode = (int)Data[4];

    // Create a pcap handle (deferred activation)
    // Using "any" as a generic device name; on some systems this might fail, which is fine for fuzzing
    handle = pcap_create("any", errbuf);
    if (handle == NULL) {
        // If creation fails, try a loopback interface as a fallback
        handle = pcap_create("lo", errbuf);
        if (handle == NULL) {
            return 0;
        }
    }

    // 1. pcap_set_snaplen
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret; // Ignore return for fuzzing exploration

    // 2. pcap_set_timeout
    ret = pcap_set_timeout(handle, timeout);
    (void)ret;

    // 3. pcap_set_protocol_linux (Linux specific, but safe to call on others as it may be a no-op or fail gracefully)
    ret = pcap_set_protocol_linux(handle, protocol);
    (void)ret;

    // 4. pcap_set_buffer_size
    ret = pcap_set_buffer_size(handle, buffer_size);
    (void)ret;

    // 5. pcap_set_immediate_mode
    ret = pcap_set_immediate_mode(handle, immediate_mode);
    (void)ret;

    // 6. pcap_activate
    ret = pcap_activate(handle);
    (void)ret;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    fuzz_pcap_api(Data, Size);
    return 0;
}