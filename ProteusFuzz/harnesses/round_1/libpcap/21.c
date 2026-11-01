// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzz input
    int linktype = (int)Data[0];
    int snaplen = (int)Data[1] * 100 + 64; // Ensure minimum reasonable snaplen
    int precision = (int)Data[2];
    int tstamp_type = (int)Data[3];

    // Normalize precision to valid constants if possible, or let API handle invalid
    // PCAP_TSTAMP_PRECISION_MICRO is typically 0, PCAP_TSTAMP_PRECISION_NANO is 1
    if (precision > 1) {
        precision = (precision % 2) ? 1 : 0;
    }

    // 1. Create a "dead" handle with specific timestamp precision
    pcap_t *handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
    
    if (handle == NULL) {
        // Allocation failure or invalid linktype, nothing to fuzz further
        return 0;
    }

    // 2. Test pcap_list_tstamp_types
    int *tstamp_types = NULL;
    int count = pcap_list_tstamp_types(handle, &tstamp_types);
    
    if (count > 0 && tstamp_types != NULL) {
        // Iterate through available types and try to set them
        for (int i = 0; i < count; i++) {
            int ret = pcap_set_tstamp_type(handle, tstamp_types[i]);
            (void)ret; // Ignore return for fuzzing, just ensure no crash
        }
        
        // Also try the input tstamp_type
        pcap_set_tstamp_type(handle, tstamp_type);
        
        // Free the list
        pcap_free_tstamp_types(tstamp_types);
    } else if (count == 0) {
        // If count is 0, it might mean no list returned, try default
        pcap_set_tstamp_type(handle, PCAP_TSTAMP_HOST);
    }
    // If count < 0 (error), we just continue

    // 3. Test pcap_set_tstamp_precision
    // Try setting precision to the input value and its inverse
    pcap_set_tstamp_precision(handle, precision);
    pcap_set_tstamp_precision(handle, 1 - precision);
    
    // Try invalid precision values
    pcap_set_tstamp_precision(handle, -1);
    pcap_set_tstamp_precision(handle, 999);

    // 4. Test pcap_get_tstamp_precision
    int current_precision = pcap_get_tstamp_precision(handle);
    (void)current_precision;

    // 5. Cleanup
    pcap_close(handle);

    return 0;
}