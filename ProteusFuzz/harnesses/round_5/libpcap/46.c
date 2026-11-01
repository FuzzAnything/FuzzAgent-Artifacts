// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    struct pcap_samp *samp;
    int buffer_size;
    int promisc;
    int snaplen;
    int loop_break;

    if (Size < 16) {
        return 0;
    }

    // Extract parameters from fuzz input
    buffer_size = *(int32_t *)(Data + 0);
    promisc = *(int32_t *)(Data + 4);
    snaplen = *(int32_t *)(Data + 8);
    loop_break = *(int32_t *)(Data + 12);

    // Create a pcap handle
    handle = pcap_create(NULL, errbuf);
    if (handle == NULL) {
        // pcap_create failed, nothing to test
        return 0;
    }

    // Test pcap_set_buffer_size
    if (buffer_size > 0) {
        ret = pcap_set_buffer_size(handle, buffer_size);
        // Ignore return value, just exercising the path
        (void)ret;
    }

    // Test pcap_set_promisc
    ret = pcap_set_promisc(handle, promisc);
    (void)ret;

    // Test pcap_set_snaplen
    if (snaplen > 0) {
        ret = pcap_set_snaplen(handle, snaplen);
        (void)ret;
    }

    // Test pcap_setsampling
    samp = pcap_setsampling(handle);
    if (samp != NULL) {
        // Configure sampling if supported
        samp->method = loop_break % 3; // 0, 1, or 2
        samp->value = loop_break % 100;
    }

    // Test pcap_breakloop (safe to call even if not in a loop)
    pcap_breakloop(handle);

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}