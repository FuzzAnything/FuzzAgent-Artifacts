// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_cleanup(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static pcap_t *create_test_handle(int linktype, int precision) {
    pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, 65535, precision);
    return pcap;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Extract parameters from fuzzer input with bounds checking
    int linktype = 0;
    int tstamp_type = 0;
    int precision = 0;
    int activate_flag = 0;
    
    // Safe extraction with bounds checking
    linktype = Data[0] % 10;
    
    if (Size > 1) {
        tstamp_type = Data[1] % 8;
    }
    
    if (Size > 2) {
        precision = Data[2] % 3;
    }
    
    if (Size > 3) {
        activate_flag = Data[3] % 2;
    }

    // Create a dummy pcap handle
    pcap_t *pcap = create_test_handle(linktype, precision % 2); // precision 0 or 1
    if (!pcap) {
        // Fallback to default precision if above fails
        pcap = pcap_open_dead_with_tstamp_precision(linktype, 65535, PCAP_TSTAMP_PRECISION_MICRO);
        if (!pcap) return 0;
    }

    // Test pcap_set_tstamp_type (may fail if handle is active)
    pcap_set_tstamp_type(pcap, tstamp_type);

    // Test pcap_get_tstamp_precision
    int retrieved_precision = pcap_get_tstamp_precision(pcap);

    // Test pcap_set_tstamp_precision (may fail if handle is active)
    pcap_set_tstamp_precision(pcap, precision % 2);

    // Test pcap_activate if requested (will likely fail on dead handle)
    if (activate_flag) {
        pcap_activate(pcap);
    }

    // Test pcap_sendpacket if we have packet data
    if (Size > 4) {
        const u_char *packet_data = Data + 4;
        int packet_len = (Size - 4) > 65535 ? 65535 : (int)(Size - 4);
        if (packet_len > 0) {
            pcap_sendpacket(pcap, packet_data, packet_len);
        }
    }

    // Cleanup
    dummy_cleanup(pcap);
    
    return 0;
}