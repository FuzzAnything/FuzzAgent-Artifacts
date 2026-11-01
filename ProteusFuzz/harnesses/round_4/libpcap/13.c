// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void safe_pcap_close(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Use input data to determine linktype and snaplen
    // Linktypes are typically small integers, snaplen can be larger
    int linktype = Data[0] % 200; // Limit to a reasonable range
    int snaplen = (Data[0] + Data[1]) % 65536;
    if (snaplen == 0) snaplen = 65535;

    // Create a pcap handle for offline processing
    pcap_t *handle = pcap_open_dead(linktype, snaplen);
    if (handle == NULL) {
        return 0;
    }

    // Prepare filter string from input data
    // Ensure it's null-terminated and safe
    char filter_str[256];
    size_t filter_len = (Size < 255) ? Size : 255;
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    // Replace non-printable characters to increase chance of valid filter syntax
    for (size_t i = 0; i < filter_len; i++) {
        if (filter_str[i] < 32 || filter_str[i] > 126) {
            filter_str[i] = ' ';
        }
    }

    struct bpf_program fp;
    bpf_u_int32 net = 0;
    int optimize = 1;

    // Compile the filter
    int ret = pcap_compile(handle, &fp, filter_str, optimize, net);
    if (ret == -1) {
        // Compilation failed, still proceed to test error handling paths
        pcap_perror(handle, "pcap_compile failed");
        safe_pcap_close(handle);
        return 0;
    }

    // Set the filter
    ret = pcap_setfilter(handle, &fp);
    if (ret == -1) {
        pcap_perror(handle, "pcap_setfilter failed");
        pcap_freecode(&fp);
        safe_pcap_close(handle);
        return 0;
    }

    // Test pcap_perror even on success (might print generic message)
    pcap_perror(handle, "pcap_perror test");

    // Close the handle
    pcap_close(handle);

    // Free the compiled filter code
    pcap_freecode(&fp);

    // The task description lists pcap_close twice. 
    // Since we already closed the handle above, calling it again on a NULL or freed handle 
    // would be unsafe. We skip the second close to prevent undefined behavior.
    
    return 0;
}