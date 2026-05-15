// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
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

static void dummy_cleanup(pcap_t *pcap, struct bpf_program *bpf) {
    if (bpf != NULL) {
        pcap_freecode(bpf);
    }
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // First two pcap_statustostr and pcap_geterr calls (without valid pcap_t)
    if (Size > 0) {
        int status1 = (int)(Data[0] % 128); // Use small values for status codes
        const char *str1 = pcap_statustostr(status1);
        (void)str1;
    }
    
    // First pcap_geterr with NULL (should handle gracefully)
    char *err1 = pcap_geterr(NULL);
    (void)err1;
    
    if (Size > 1) {
        int status2 = (int)(Data[1] % 128);
        const char *str2 = pcap_statustostr(status2);
        (void)str2;
    }
    
    // Second pcap_geterr with NULL
    char *err2 = pcap_geterr(NULL);
    (void)err2;
    
    // Prepare for pcap_open_live
    char device[64];
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Use fuzz data to generate device name (truncate if needed)
    size_t device_len = Size < sizeof(device) - 1 ? Size : sizeof(device) - 1;
    if (device_len > 0) {
        memcpy(device, Data, device_len);
        device[device_len] = '\0';
    } else {
        strcpy(device, "lo"); // Default device
    }
    
    // Use fuzz data for parameters
    int snaplen = 65535;
    int promisc = 0;
    int timeout = 1000;
    
    if (Size > 2) {
        snaplen = (Data[2] << 8) | Data[3];
        if (snaplen < 0) snaplen = 65535;
    }
    if (Size > 4) {
        promisc = Data[4] & 1;
    }
    if (Size > 5) {
        timeout = (Data[5] << 8) | Data[6];
        if (timeout < 0) timeout = 1000;
    }
    
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
    
    if (pcap != NULL) {
        // pcap_fileno
        int fd = pcap_fileno(pcap);
        (void)fd;
        
        // pcap_compile
        struct bpf_program bpf;
        memset(&bpf, 0, sizeof(bpf));
        
        // Prepare filter string from fuzz data
        const char *filter = "ip";
        char filter_buf[256];
        
        if (Size > 7) {
            size_t filter_len = Size - 7;
            if (filter_len > sizeof(filter_buf) - 1) {
                filter_len = sizeof(filter_buf) - 1;
            }
            memcpy(filter_buf, Data + 7, filter_len);
            filter_buf[filter_len] = '\0';
            filter = filter_buf;
        }
        
        int optimize = 1;
        bpf_u_int32 netmask = 0xffffff00;
        
        if (Size > 8) {
            optimize = Data[8] & 1;
        }
        if (Size > 12) {
            netmask = (Data[9] << 24) | (Data[10] << 16) | (Data[11] << 8) | Data[12];
        }
        
        int compile_ret = pcap_compile(pcap, &bpf, filter, optimize, netmask);
        
        // pcap_geterr after compile
        char *compile_err = pcap_geterr(pcap);
        (void)compile_err;
        
        if (compile_ret == 0) {
            // pcap_setfilter
            int setfilter_ret = pcap_setfilter(pcap, &bpf);
            
            // pcap_geterr after setfilter
            char *setfilter_err = pcap_geterr(pcap);
            (void)setfilter_err;
            
            // Clean up bpf program
            pcap_freecode(&bpf);
        } else {
            // Clean up bpf program even on failure
            pcap_freecode(&bpf);
        }
        
        // pcap_close
        pcap_close(pcap);
    }
    
    return 0;
}