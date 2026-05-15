// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"

static void dummy_read_op(pcap_t *pcap) {
    // Dummy implementation to satisfy compiler
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Create a null-terminated filter string from fuzzer input
    char *filter_str = NULL;
    if (Size > 0) {
        filter_str = (char *)malloc(Size + 1);
        if (!filter_str) return 0;
        memcpy(filter_str, Data, Size);
        filter_str[Size] = '\0';
    } else {
        // Use empty string if no data
        filter_str = strdup("");
        if (!filter_str) return 0;
    }
    
    // Open dummy pcap handle
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        free(filter_str);
        return 0;
    }
    
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // Compile filter expression
    int compile_result = pcap_compile(pcap, &fp, filter_str, 0, 0);
    
    // Set filter if compilation succeeded
    if (compile_result == 0) {
        int setfilter_result = pcap_setfilter(pcap, &fp);
        
        // Print error if setfilter failed
        if (setfilter_result != 0) {
            pcap_perror(pcap, "pcap_setfilter failed");
        }
    } else {
        // Print error if compilation failed
        pcap_perror(pcap, "pcap_compile failed");
    }
    
    // Close the handle
    pcap_close(pcap);
    
    // Free compiled BPF program
    pcap_freecode(&fp);
    
    // Open another dummy handle and close it (second pcap_close call)
    pcap_t *pcap2 = pcap_open_dead(DLT_RAW, 1514);
    if (pcap2) {
        pcap_close(pcap2);
    }
    
    free(filter_str);
    return 0;
}