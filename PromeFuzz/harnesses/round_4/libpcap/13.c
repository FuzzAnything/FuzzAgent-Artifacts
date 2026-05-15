// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void cleanup_pcap_handle(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // 1. Prepare environment for pcap_close: need a pcap_t handle
    // Create a dead capture handle since it doesn't require actual device
    pcap_t *dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!dead_handle) {
        return 0;
    }
    
    // Call pcap_close first as specified
    pcap_close(dead_handle);
    // Set to NULL as instructed to avoid use-after-free
    dead_handle = NULL;
    
    // 2. Prepare for pcap_lookupnet
    bpf_u_int32 netp = 0;
    bpf_u_int32 maskp = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Extract device name from fuzzer input (null-terminated)
    char *device = NULL;
    if (Size > 0) {
        size_t device_len = Size > PCAP_ERRBUF_SIZE ? PCAP_ERRBUF_SIZE - 1 : Size;
        device = (char *)malloc(device_len + 1);
        if (!device) {
            return 0;
        }
        memcpy(device, Data, device_len);
        device[device_len] = '\0';
    }
    
    // Call pcap_lookupnet
    int lookup_result = pcap_lookupnet(device, &netp, &maskp, errbuf);
    
    // 3. Prepare for pcap_datalink_name_to_val
    // Extract link-layer type name from fuzzer input
    char *dlt_name = NULL;
    if (Size > 0) {
        // Use a different portion of input for variety
        size_t offset = Size > 100 ? 50 : 0;
        size_t remaining = Size - offset;
        size_t name_len = remaining > PCAP_ERRBUF_SIZE ? PCAP_ERRBUF_SIZE - 1 : remaining;
        
        if (name_len > 0) {
            dlt_name = (char *)malloc(name_len + 1);
            if (dlt_name) {
                memcpy(dlt_name, Data + offset, name_len);
                dlt_name[name_len] = '\0';
            }
        }
    }
    
    // Call pcap_datalink_name_to_val
    int dlt_val = -1;
    if (dlt_name) {
        dlt_val = pcap_datalink_name_to_val(dlt_name);
    }
    
    // 4. Prepare for pcap_set_datalink: need another pcap_t handle
    pcap_t *live_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!live_handle) {
        free(device);
        free(dlt_name);
        return 0;
    }
    
    // Call pcap_set_datalink with the obtained DLT value
    // Also try with some other values from fuzzer input for more coverage
    int set_dlt_result = pcap_set_datalink(live_handle, dlt_val);
    
    // Try another call with different DLT value from fuzzer input
    if (Size >= sizeof(int)) {
        int alt_dlt_val = *(const int *)Data;
        pcap_set_datalink(live_handle, alt_dlt_val);
    }
    
    // 5. Call pcap_geterr
    char *error_msg = pcap_geterr(live_handle);
    // Use the error message to avoid unused variable warning
    (void)error_msg;
    
    // Additional call: pcap_geterr on the dead handle (should be safe even though closed)
    if (dead_handle == NULL) {
        // Create another handle for testing pcap_geterr after error conditions
        pcap_t *temp_handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (temp_handle) {
            // Try to set an invalid datalink to generate error
            pcap_set_datalink(temp_handle, -1);
            char *temp_error = pcap_geterr(temp_handle);
            (void)temp_error;
            pcap_close(temp_handle);
        }
    }
    
    // Cleanup
    cleanup_pcap_handle(live_handle);
    free(device);
    free(dlt_name);
    
    return 0;
}