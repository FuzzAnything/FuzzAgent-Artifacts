// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely extract parameters
    if (Size < 3) {
        return 0;
    }

    // Extract parameters from fuzz input safely
    int linktype = Data[0];
    int precision = Data[1];
    int tstamp_type = Data[2] % 4; // Assume 4 types max for safety

    // Test pcap_open_dead_with_tstamp_precision
    pcap_t *handle = pcap_open_dead_with_tstamp_precision(linktype, precision, 65535);
    if (!handle) {
        return 0;
    }

    // Test pcap_get_tstamp_precision
    int current_precision = pcap_get_tstamp_precision(handle);
    (void)current_precision; // Use return value to suppress warnings

    // Test pcap_list_tstamp_types
    int *tstamp_types = NULL;
    int count = pcap_list_tstamp_types(handle, &tstamp_types);
    if (count > 0 && tstamp_types != NULL) {
        // Iterate and potentially use types
        for (int i = 0; i < count; i++) {
            int type = tstamp_types[i];
            (void)type;
        }
        // Free the allocated list
        pcap_free_tstamp_types(tstamp_types);
    }

    // Test pcap_set_tstamp_type
    int ret_set_type = pcap_set_tstamp_type(handle, tstamp_type);
    (void)ret_set_type;

    // Test pcap_set_tstamp_precision
    int ret_set_prec = pcap_set_tstamp_precision(handle, precision);
    (void)ret_set_prec;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}