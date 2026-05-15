// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // 1. pcap_datalink_name_to_val
    // Create null-terminated string from fuzzer input
    char *name = NULL;
    if (Size > 0) {
        name = (char *)malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            (void)pcap_datalink_name_to_val(name);
            free(name);
        }
    }

    // Generate integer values from fuzzer input for the other functions
    int dlt_val = 0;
    int tstamp_val = 0;
    
    if (Size >= sizeof(int)) {
        // Use first sizeof(int) bytes for DLT value
        memcpy(&dlt_val, Data, sizeof(int));
        // Use next sizeof(int) bytes for timestamp type value (if available)
        if (Size >= 2 * sizeof(int)) {
            memcpy(&tstamp_val, Data + sizeof(int), sizeof(int));
        } else {
            // Use the same value if not enough data
            tstamp_val = dlt_val;
        }
    } else if (Size > 0) {
        // Use first byte for both values if less than sizeof(int)
        dlt_val = Data[0];
        tstamp_val = Data[0];
    }

    // 2. pcap_datalink_val_to_name
    (void)pcap_datalink_val_to_name(dlt_val);

    // 3. pcap_datalink_val_to_description_or_dlt
    (void)pcap_datalink_val_to_description_or_dlt(dlt_val);

    // 4. pcap_tstamp_type_val_to_name
    (void)pcap_tstamp_type_val_to_name(tstamp_val);

    // 5. pcap_tstamp_type_val_to_description
    (void)pcap_tstamp_type_val_to_description(tstamp_val);

    // 6. pcap_datalink_val_to_description
    (void)pcap_datalink_val_to_description(dlt_val);

    return 0;
}