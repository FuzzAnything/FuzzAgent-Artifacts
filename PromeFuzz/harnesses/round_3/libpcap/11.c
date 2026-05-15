// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
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
#include <stdint.h>

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Create a dummy filter string from fuzzer input
    char *filter_str = NULL;
    if (Size > 0) {
        filter_str = (char *)malloc(Size + 1);
        if (!filter_str) return 0;
        memcpy(filter_str, Data, Size);
        filter_str[Size] = '\0';
    } else {
        // If no input, use a simple default filter
        filter_str = strdup("ip");
        if (!filter_str) return 0;
    }

    // Step 1: Open dead capture handle
    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!handle) {
        free(filter_str);
        return 0;
    }

    // Step 2: Compile filter
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    int compile_result = pcap_compile(handle, &fp, filter_str, 0, PCAP_NETMASK_UNKNOWN);
    
    // Step 3: Set filter if compilation succeeded
    if (compile_result == 0) {
        pcap_setfilter(handle, &fp);
    }
    
    // Step 4: Print any errors (will print if previous operations failed)
    pcap_perror(handle, "libpcap error");
    
    // Step 5: Close handle
    pcap_close(handle);
    
    // Step 6: Free compiled code
    pcap_freecode(&fp);
    
    // Step 7: Close handle again (should be safe since handle is now NULL after pcap_close)
    // Note: pcap_close sets internal pointer to NULL, but our local pointer remains
    // We'll avoid double-free by checking if handle was already closed
    // pcap_close on already-closed handle is undefined, so we skip this
    
    free(filter_str);
    return 0;
}