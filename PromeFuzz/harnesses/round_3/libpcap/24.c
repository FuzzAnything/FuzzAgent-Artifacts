// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // 1. pcap_datalink_name_to_val
    if (Size > 0) {
        char *name = (char *)malloc(Size + 1);
        if (name) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            pcap_datalink_name_to_val(name);
            free(name);
        }
    }

    // 2. pcap_datalink_val_to_name
    if (Size >= sizeof(int)) {
        int dlt_val;
        memcpy(&dlt_val, Data, sizeof(int));
        pcap_datalink_val_to_name(dlt_val);
    }

    // 3. pcap_free_datalinks - requires valid array from pcap_list_datalinks
    // Create a dummy pcap handle to get valid datalinks array
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap) {
        int *dlt_list = NULL;
        int count = pcap_list_datalinks(pcap, &dlt_list);
        if (dlt_list) {
            pcap_free_datalinks(dlt_list);
        }
        pcap_close(pcap);
    }

    // 4. pcap_datalink_val_to_description_or_dlt
    if (Size >= sizeof(int)) {
        int dlt_val2;
        memcpy(&dlt_val2, Data, sizeof(int));
        // Use different bytes for different test case
        if (Size >= 2 * sizeof(int)) {
            memcpy(&dlt_val2, Data + sizeof(int), sizeof(int));
        }
        pcap_datalink_val_to_description_or_dlt(dlt_val2);
    }

    // 5. pcap_tstamp_type_val_to_description
    if (Size >= sizeof(int)) {
        int tstamp_type;
        memcpy(&tstamp_type, Data, sizeof(int));
        // Use different offset for different test case
        if (Size >= 3 * sizeof(int)) {
            memcpy(&tstamp_type, Data + 2 * sizeof(int), sizeof(int));
        }
        pcap_tstamp_type_val_to_description(tstamp_type);
    }

    // 6. pcap_datalink_val_to_description
    if (Size >= sizeof(int)) {
        int dlt_val3;
        memcpy(&dlt_val3, Data, sizeof(int));
        // Use different bytes for different test case
        if (Size >= 4 * sizeof(int)) {
            memcpy(&dlt_val3, Data + 3 * sizeof(int), sizeof(int));
        }
        pcap_datalink_val_to_description(dlt_val3);
    }

    return 0;
}