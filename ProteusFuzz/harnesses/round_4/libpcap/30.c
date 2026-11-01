// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device = NULL;
    pcap_t *handle = NULL;
    int allocated_device = 0;

    // Prepare device name from input if possible, otherwise use NULL
    if (Size > 0 && Data[0] != 0) {
        // Ensure null termination for safety
        char *device_buf = (char *)malloc(Size + 1);
        if (device_buf) {
            memcpy(device_buf, Data, Size);
            device_buf[Size] = '\0';
            device = device_buf;
            allocated_device = 1;
        }
    }

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // 1. pcap_create
    handle = pcap_create(device, errbuf);
    
    if (handle != NULL) {
        // 2. pcap_setsampling (requires ENABLE_REMOTE, will likely fail locally but safe to call)
        struct pcap_samp *samp = pcap_setsampling(handle);
        if (samp != NULL) {
            // Fuzz sampling parameters
            if (Size >= 2) {
                samp->method = (int)Data[0];
                samp->value = (int)Data[1];
            }
        }

        // 3. pcap_set_immediate_mode
        if (Size >= 3) {
            int immediate_mode = (Data[2] % 2);
            pcap_set_immediate_mode(handle, immediate_mode);
        }

        // 4. pcap_set_buffer_size
        if (Size >= 4) {
            int buffer_size = (int)Data[3];
            // Ensure non-negative for meaningful test, but fuzz negative too
            if (buffer_size < 0) buffer_size = -buffer_size;
            pcap_set_buffer_size(handle, buffer_size);
        }

        // 5. pcap_set_snaplen
        if (Size >= 5) {
            int snaplen = (int)Data[4];
            if (snaplen < 0) snaplen = -snaplen;
            pcap_set_snaplen(handle, snaplen);
        }

        // 6. pcap_breakloop (safe to call even if loop isn't running)
        pcap_breakloop(handle);

        // Cleanup
        pcap_close(handle);
    }

    // Free device buffer if allocated
    if (allocated_device && device != NULL) {
        free((void *)device);
    }

    return 0;
}