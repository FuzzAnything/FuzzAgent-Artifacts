// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Use first few bytes for status codes
    int status1 = 0, status2 = 0;
    if (Size >= 1) {
        status1 = Data[0];
    }
    if (Size >= 2) {
        status2 = Data[1];
    }
    
    // First sequence: pcap_statustostr -> pcap_geterr
    const char *status_str1 = pcap_statustostr(status1);
    (void)status_str1; // Use result to avoid unused variable warning
    
    // Create a dummy pcap handle for pcap_geterr
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return 0;
    }
    
    // First pcap_geterr
    char *err1 = pcap_geterr(pcap);
    (void)err1;
    
    // Second sequence: pcap_statustostr -> pcap_geterr
    const char *status_str2 = pcap_statustostr(status2);
    (void)status_str2;
    
    // Second pcap_geterr
    char *err2 = pcap_geterr(pcap);
    (void)err2;
    
    // pcap_lookupnet
    bpf_u_int32 net = 0, mask = 0;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    // Extract device name from fuzzer input - use stack-allocated buffer
    char dev_buf[64] = {0};
    const char *device = "any";
    
    if (Size >= 3) {
        size_t dev_len = Size - 2;
        if (dev_len > 63) dev_len = 63;
        memcpy(dev_buf, Data + 2, dev_len);
        dev_buf[dev_len] = '\0';
        device = dev_buf;
    }
    
    int lookup_result = pcap_lookupnet(device, &net, &mask, errbuf);
    (void)lookup_result;
    
    // pcap_compile
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // Extract filter expression from fuzzer input - use stack-allocated buffer
    char filter_buf[128] = {0};
    const char *filter = "";
    
    if (Size >= 4) {
        size_t filter_len = Size - 3;
        if (filter_len > 127) filter_len = 127;
        memcpy(filter_buf, Data + 3, filter_len);
        filter_buf[filter_len] = '\0';
        filter = filter_buf;
    }
    
    int compile_result = pcap_compile(pcap, &fp, filter, 0, net);
    (void)compile_result;
    
    // pcap_geterr after compile
    char *err3 = pcap_geterr(pcap);
    (void)err3;
    
    // pcap_setfilter
    int setfilter_result = pcap_setfilter(pcap, &fp);
    (void)setfilter_result;
    
    // pcap_geterr after setfilter
    char *err4 = pcap_geterr(pcap);
    (void)err4;
    
    // pcap_breakloop
    pcap_breakloop(pcap);
    
    // pcap_close
    pcap_close(pcap);
    pcap = NULL; // Set to NULL to avoid double-free
    
    // pcap_freecode
    pcap_freecode(&fp);
    
    return 0;
}