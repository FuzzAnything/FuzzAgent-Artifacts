// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp, maskp;
    int dlt_val;
    
    // Step 1: pcap_close - need a pcap_t handle first
    // Create a dummy pcap handle using pcap_open_dead
    pcap_t *pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap_handle) {
        pcap_close(pcap_handle);
        pcap_handle = NULL; // Set to NULL as instructed
    }
    
    // Step 2: pcap_lookupnet - need a device name
    // Extract device name from fuzz data (null-terminated)
    size_t dev_name_len = Size > 100 ? 100 : Size;
    char *device_name = (char *)malloc(dev_name_len + 1);
    if (!device_name) return 0;
    
    memcpy(device_name, Data, dev_name_len);
    device_name[dev_name_len] = '\0';
    
    // Call pcap_lookupnet with the extracted device name
    pcap_lookupnet(device_name, &netp, &maskp, errbuf);
    
    // Step 3: pcap_datalink_name_to_val - need a link-layer type name
    // Use the same device_name buffer or extract new name
    // For simplicity, use the same buffer but ensure it's valid
    dlt_val = pcap_datalink_name_to_val(device_name);
    
    // Step 4: pcap_set_datalink - need a pcap_t handle and DLT value
    // Create another pcap handle for this operation
    pcap_t *pcap_handle2 = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap_handle2) {
        // Try with the value from pcap_datalink_name_to_val or a fuzzed value
        int dlt_to_set = dlt_val;
        if (dlt_val == -1) {
            // If name not found, use a value from fuzz data
            dlt_to_set = Data[0] % 256; // Simple modulo to get valid-ish DLT
        }
        pcap_set_datalink(pcap_handle2, dlt_to_set);
        
        // Step 5: pcap_geterr - get error message
        char *error_msg = pcap_geterr(pcap_handle2);
        (void)error_msg; // Use to avoid unused variable warning
        
        cleanup_pcap(pcap_handle2);
    }
    
    free(device_name);
    return 0;
}