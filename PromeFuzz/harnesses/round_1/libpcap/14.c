// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file for any potential file operations
    write_dummy_file(Data, Size);
    
    // 1. pcap_close - needs a pcap_t handle
    // Create a dummy pcap_t using pcap_open_dead (safe offline capture)
    pcap_t *pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
    }
    
    // 2. pcap_lookupnet - needs device name, network/mask pointers, error buffer
    bpf_u_int32 netp = 0, maskp = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Extract device name from fuzzer input (null-terminated)
    char device[256];
    size_t device_len = Size < 255 ? Size : 255;
    memcpy(device, Data, device_len);
    device[device_len] = '\0';
    
    pcap_lookupnet(device, &netp, &maskp, errbuf);
    
    // 3. pcap_datalink_name_to_val - needs link-layer type name
    // Extract another string from fuzzer input for datalink name
    char dlt_name[256];
    size_t dlt_offset = device_len < Size ? device_len : 0;
    size_t dlt_len = (Size - dlt_offset) < 255 ? (Size - dlt_offset) : 255;
    if (dlt_len > 0) {
        memcpy(dlt_name, Data + dlt_offset, dlt_len);
        dlt_name[dlt_len] = '\0';
        pcap_datalink_name_to_val(dlt_name);
    }
    
    // 4. pcap_set_datalink - needs pcap_t handle and datalink type
    // Create another pcap handle for this operation
    pcap_t *pcap_handle2 = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap_handle2 != NULL) {
        // Use data from fuzzer input to determine datalink type
        int dlt_val = 0;
        if (Size >= sizeof(int)) {
            memcpy(&dlt_val, Data, sizeof(int));
        }
        pcap_set_datalink(pcap_handle2, dlt_val);
        
        // 5. pcap_geterr - needs pcap_t handle
        pcap_geterr(pcap_handle2);
        
        pcap_close(pcap_handle2);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}