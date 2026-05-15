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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data for the filter string
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated filter string from fuzzer input
    char *filter_str = (char *)malloc(Size + 1);
    if (filter_str == NULL) {
        return 0;
    }
    memcpy(filter_str, Data, Size);
    filter_str[Size] = '\0';

    // Open a dead pcap handle for filter compilation
    pcap_t *pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap_handle == NULL) {
        free(filter_str);
        return 0;
    }

    // Initialize bpf_program structure to zero
    struct bpf_program bpf_prog;
    memset(&bpf_prog, 0, sizeof(bpf_prog));

    // Compile the filter expression from fuzzer input
    int compile_result = pcap_compile(pcap_handle, &bpf_prog, filter_str, 0, 0);
    
    // Set filter if compilation succeeded
    if (compile_result == 0) {
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf_prog);
        if (setfilter_result != 0) {
            // Print error if setfilter fails
            pcap_perror(pcap_handle, "pcap_setfilter failed");
        }
    } else {
        // Print error if compilation fails
        pcap_perror(pcap_handle, "pcap_compile failed");
    }

    // Close the pcap handle
    pcap_close(pcap_handle);
    pcap_handle = NULL;

    // Free the compiled BPF program
    pcap_freecode(&bpf_prog);

    // Open another dead pcap handle for the second close
    pcap_t *pcap_handle2 = pcap_open_dead(DLT_RAW, 1500);
    if (pcap_handle2 != NULL) {
        pcap_close(pcap_handle2);
        pcap_handle2 = NULL;
    }

    free(filter_str);
    return 0;
}