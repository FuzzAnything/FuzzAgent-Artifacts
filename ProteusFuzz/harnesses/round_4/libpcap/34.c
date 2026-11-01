// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_breakloop(handle);
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a device name string from input data, ensuring it's null-terminated
    // Limit the device name length to prevent excessive allocation
    size_t device_len = Size > 256 ? 256 : Size;
    char *device_name = (char *)malloc(device_len + 1);
    if (device_name == NULL) {
        return 0;
    }
    memcpy(device_name, Data, device_len);
    device_name[device_len] = '\0';

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Initialize pcap handle
    pcap_t *handle = pcap_create(device_name, errbuf);
    
    // Free the device name as it's no longer needed by pcap_create
    free(device_name);

    if (handle == NULL) {
        // pcap_create failed, which is a valid state for fuzzing
        return 0;
    }

    // Fuzz pcap_setsampling
    struct pcap_samp *samp = pcap_setsampling(handle);
    if (samp != NULL) {
        // Randomly set sampling parameters based on input if available
        if (Size >= 2) {
            samp->method = Data[0] % 4; // Assuming 4 methods exist
            samp->value = Data[1] % 100;
        }
    }

    // Fuzz pcap_set_immediate_mode
    if (Size >= 3) {
        int immediate_mode = Data[2] % 2;
        pcap_set_immediate_mode(handle, immediate_mode);
    }

    // Fuzz pcap_set_buffer_size
    if (Size >= 5) {
        int buffer_size = ((int)Data[3] << 8) | (int)Data[4];
        if (buffer_size > 0 && buffer_size < 10000000) { // Reasonable limit
            pcap_set_buffer_size(handle, buffer_size);
        }
    }

    // Fuzz pcap_set_snaplen
    if (Size >= 7) {
        int snaplen = ((int)Data[5] << 8) | (int)Data[6];
        if (snaplen > 0 && snaplen < 65536) { // Reasonable limit
            pcap_set_snaplen(handle, snaplen);
        }
    }

    // Fuzz pcap_breakloop (safe to call even if not in a loop)
    pcap_breakloop(handle);

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}