// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Use first bytes for status codes
    int status1 = *(int*)Data % 100;
    int status2 = *(int*)(Data + 4) % 100;
    
    // Call pcap_statustostr and pcap_geterr in specified order
    const char *str1 = pcap_statustostr(status1);
    (void)str1;
    
    char *err1 = pcap_geterr(NULL);
    (void)err1;
    
    const char *str2 = pcap_statustostr(status2);
    (void)str2;
    
    char *err2 = pcap_geterr(NULL);
    (void)err2;
    
    // Prepare parameters for pcap_open_live
    char device[64];
    char errbuf[PCAP_ERRBUF_SIZE];
    int snaplen = 65535;
    int promisc = 1;
    int timeout = 1000;
    
    // Use input data for device name if possible
    if (Size > 8) {
        size_t device_len = Size - 8 > 63 ? 63 : Size - 8;
        memcpy(device, Data + 8, device_len);
        device[device_len] = '\0';
    } else {
        strcpy(device, "eth0");
    }
    
    // Call pcap_open_live
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
    
    if (pcap != NULL) {
        // Call pcap_fileno
        int fd = pcap_fileno(pcap);
        (void)fd;
        
        // Prepare for pcap_compile
        struct bpf_program fp;
        const char *filter = "ip";
        int optimize = 0;
        bpf_u_int32 netmask = 0xffffff00;
        
        // Call pcap_compile
        int compile_ret = pcap_compile(pcap, &fp, filter, optimize, netmask);
        (void)compile_ret;
        
        // Call pcap_geterr
        char *err3 = pcap_geterr(pcap);
        (void)err3;
        
        // Call pcap_setfilter
        int setfilter_ret = pcap_setfilter(pcap, &fp);
        (void)setfilter_ret;
        
        // Call pcap_geterr
        char *err4 = pcap_geterr(pcap);
        (void)err4;
        
        // Cleanup BPF program
        pcap_freecode(&fp);
        
        // Call pcap_close
        pcap_close(pcap);
    }
    
    // Create dummy file with remaining data for other potential uses
    if (Size > 72) {
        write_dummy_file(Data + 72, Size - 72);
    }
    
    return 0;
}