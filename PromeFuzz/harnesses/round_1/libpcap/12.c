// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Use first 4 bytes for linktype, next 4 for snaplen, rest for filter string
    if (Size < 8) {
        return 0;
    }
    
    // Extract parameters from fuzzer input
    int linktype = *(const int*)Data;
    int snaplen = *(const int*)(Data + 4);
    size_t filter_len = Size - 8;
    
    // Create null-terminated filter string
    char *filter_str = malloc(filter_len + 1);
    if (!filter_str) {
        return 0;
    }
    memcpy(filter_str, Data + 8, filter_len);
    filter_str[filter_len] = '\0';
    
    // Open dummy pcap handle
    pcap_t *handle = pcap_open_dead(linktype, snaplen);
    if (!handle) {
        free(filter_str);
        return 0;
    }
    
    struct bpf_program fp;
    int optimize = 1;
    bpf_u_int32 netmask = 0xFFFFFF00; // Typical class C netmask
    
    // Try to compile filter
    int compile_result = pcap_compile(handle, &fp, filter_str, optimize, netmask);
    
    // Try to set filter if compilation succeeded
    if (compile_result == 0) {
        pcap_setfilter(handle, &fp);
    }
    
    // Print any errors that occurred
    pcap_perror(handle, "Fuzzer error");
    
    // First close
    pcap_close(handle);
    
    // Free compiled code if compilation succeeded
    if (compile_result == 0) {
        pcap_freecode(&fp);
    }
    
    // Try to open another handle with different parameters
    pcap_t *handle2 = pcap_open_dead(linktype + 1, snaplen + 1);
    if (handle2) {
        // Try compilation again with different parameters
        int compile_result2 = pcap_compile(handle2, &fp, filter_str, 0, netmask);
        
        if (compile_result2 == 0) {
            pcap_setfilter(handle2, &fp);
            pcap_freecode(&fp);
        }
        
        pcap_perror(handle2, "Second handle error");
        pcap_close(handle2);
    }
    
    free(filter_str);
    return 0;
}