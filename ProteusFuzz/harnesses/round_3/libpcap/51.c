// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
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

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *device = "any";

    if (Size < 1) {
        return 0;
    }

    // Create a pcap handle
    handle = pcap_create(device, errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Fuzz pcap_can_set_rfmon
    int ret = pcap_can_set_rfmon(handle);
    (void)ret;

    // Fuzz pcap_set_snaplen
    int snaplen = 0;
    if (Size >= 2) {
        snaplen = Data[0] | (Data[1] << 8);
        if (snaplen <= 0) snaplen = 65535;
    } else {
        snaplen = 65535;
    }
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret;

    // Fuzz pcap_set_timeout
    int timeout = 0;
    if (Size >= 4) {
        timeout = Data[2] | (Data[3] << 8);
        if (timeout < 0) timeout = 1000;
    } else {
        timeout = 1000;
    }
    ret = pcap_set_timeout(handle, timeout);
    (void)ret;

    // Fuzz pcap_set_protocol_linux (Linux specific)
    int protocol = 0;
    if (Size >= 6) {
        protocol = Data[4] | (Data[5] << 8);
    } else {
        protocol = 0;
    }
    ret = pcap_set_protocol_linux(handle, protocol);
    (void)ret;

    // Fuzz pcap_set_buffer_size
    int buffer_size = 0;
    if (Size >= 8) {
        buffer_size = Data[6] | (Data[7] << 8);
        if (buffer_size <= 0) buffer_size = 262144;
    } else {
        buffer_size = 262144;
    }
    ret = pcap_set_buffer_size(handle, buffer_size);
    (void)ret;

    // Fuzz pcap_set_immediate_mode
    int immediate_mode = 0;
    if (Size >= 10) {
        immediate_mode = Data[8] & 0x01;
    } else {
        immediate_mode = 1;
    }
    ret = pcap_set_immediate_mode(handle, immediate_mode);
    (void)ret;

    // Attempt activation
    ret = pcap_activate(handle);
    (void)ret;

    // Do NOT call configuration functions after activation.
    // The library may free internal buffers (like device name) or change state
    // such that these functions dereference NULL pointers or invalid memory.
    // The previous code crashed here because pcap_can_set_rfmon_linux 
    // accessed handle->opt.device which was freed/NULLed during activation.

    cleanup_pcap(handle);
    return 0;
}