// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    // Initialize variables
    pcap_t *pcap_handle = NULL;
    struct bpf_program bpf_prog;
    bpf_u_int32 netp = 0, maskp = 0;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    const char *device = "any";
    const char *filter_str = "ip";
    
    // Create a dummy pcap handle using pcap_open_dead
    pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    // First pcap_statustostr -> pcap_geterr sequence
    for (int i = 0; i < 2; i++) {
        // Use fuzz data to generate status codes
        int status_code = (int)Data[i % Size];
        const char *status_str = pcap_statustostr(status_code);
        (void)status_str; // Use result to avoid unused variable warning
        
        char *err_msg = pcap_geterr(pcap_handle);
        (void)err_msg; // Use result
    }
    
    // pcap_lookupnet
    int lookup_result = pcap_lookupnet(device, &netp, &maskp, errbuf);
    (void)lookup_result; // Use result
    
    // pcap_compile
    memset(&bpf_prog, 0, sizeof(bpf_prog));
    int compile_result = pcap_compile(pcap_handle, &bpf_prog, filter_str, 1, maskp);
    (void)compile_result; // Use result
    
    // pcap_geterr after compile
    char *compile_err = pcap_geterr(pcap_handle);
    (void)compile_err; // Use result
    
    // pcap_setfilter
    if (bpf_prog.bf_insns != NULL) {
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf_prog);
        (void)setfilter_result; // Use result
    }
    
    // pcap_geterr after setfilter
    char *setfilter_err = pcap_geterr(pcap_handle);
    (void)setfilter_err; // Use result
    
    // pcap_breakloop
    pcap_breakloop(pcap_handle);
    
    // pcap_close
    pcap_close(pcap_handle);
    
    // pcap_freecode
    pcap_freecode(&bpf_prog);
    
    cleanup_dummy_file();
    return 0;
}