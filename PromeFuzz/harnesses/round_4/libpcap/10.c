// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <string.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create null-terminated filter string from fuzzer input
    char *filter_str = NULL;
    if (Size > 0) {
        filter_str = (char *)malloc(Size + 1);
        if (!filter_str) return 0;
        memcpy(filter_str, Data, Size);
        filter_str[Size] = '\0';
    } else {
        // Use default filter if no input
        filter_str = strdup("ip");
        if (!filter_str) return 0;
    }

    // Step 1: Create dummy pcap handle
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        free(filter_str);
        return 0;
    }

    // Step 2: Compile filter
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    int compile_ret = pcap_compile(pcap, &fp, filter_str, 0, 0);
    
    // Step 3: Set filter if compilation succeeded
    if (compile_ret == 0) {
        int setfilter_ret = pcap_setfilter(pcap, &fp);
        
        // Step 4: Print error if filter setting failed
        if (setfilter_ret != 0) {
            pcap_perror(pcap, "pcap_setfilter failed");
        }
    } else {
        // Step 4: Print error if compilation failed
        pcap_perror(pcap, "pcap_compile failed");
    }

    // Step 5: Close pcap handle
    pcap_close(pcap);
    
    // Step 6: Free compiled BPF program
    pcap_freecode(&fp);
    
    // Clean up
    free(filter_str);
    
    return 0;
}