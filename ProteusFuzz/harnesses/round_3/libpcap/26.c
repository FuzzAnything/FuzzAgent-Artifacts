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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from input data
    int linktype = Data[0];
    int snaplen = (Data[1] << 8) | Data[2];
    int precision = Data[3];

    // Normalize precision to valid values (PCAP_TSTAMP_PRECISION_MICRO or PCAP_TSTAMP_PRECISION_NANO)
    if (precision == 0) {
        precision = PCAP_TSTAMP_PRECISION_MICRO;
    } else if (precision == 1) {
        precision = PCAP_TSTAMP_PRECISION_NANO;
    } else {
        precision = PCAP_TSTAMP_PRECISION_MICRO; // Default fallback
    }

    // Test pcap_open_dead_with_tstamp_precision
    pcap_t *handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
    if (handle == NULL) {
        return 0;
    }

    // Test pcap_get_tstamp_precision
    int current_precision = pcap_get_tstamp_precision(handle);
    (void)current_precision; // Use the value to avoid unused variable warning

    // Test pcap_list_tstamp_types
    int *tstamp_types = NULL;
    int tstamp_count = pcap_list_tstamp_types(handle, &tstamp_types);
    if (tstamp_count > 0 && tstamp_types != NULL) {
        // Optionally use tstamp_types here if needed
        pcap_free_tstamp_types(tstamp_types);
    }

    // Test pcap_set_tstamp_type with various types
    int tstamp_type = PCAP_TSTAMP_HOST;
    if (Size > 4) {
        tstamp_type = Data[4] % 5; // Assume 5 possible types
    }
    int ret_set_type = pcap_set_tstamp_type(handle, tstamp_type);
    (void)ret_set_type;

    // Test pcap_set_tstamp_precision
    int new_precision = (Size > 5) ? Data[5] : 0;
    if (new_precision == 0) {
        new_precision = PCAP_TSTAMP_PRECISION_MICRO;
    } else if (new_precision == 1) {
        new_precision = PCAP_TSTAMP_PRECISION_NANO;
    } else {
        new_precision = PCAP_TSTAMP_PRECISION_MICRO;
    }
    int ret_set_precision = pcap_set_tstamp_precision(handle, new_precision);
    (void)ret_set_precision;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}