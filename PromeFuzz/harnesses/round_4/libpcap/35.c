// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <pcap/bpf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int validate_and_test_bpf(const struct bpf_insn *insns, int len, pcap_t *pcap_handle) {
    if (len <= 0 || len > 1000) return 0;
    
    int valid = bpf_validate(insns, len);
    if (valid) {
        for (int i = 0; i < len; i++) {
            char *img = bpf_image(&insns[i], 1);
            (void)img;
        }
        
        struct bpf_program prog;
        prog.bf_len = len;
        prog.bf_insns = (struct bpf_insn *)insns;
        
        bpf_dump(&prog, 1);
        
        if (pcap_setfilter(pcap_handle, &prog) == 0) {
            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.ts.tv_sec = time(NULL);
            hdr.ts.tv_usec = 0;
            hdr.caplen = 64;
            hdr.len = 64;
            
            unsigned char packet[64] = {0};
            pcap_offline_filter(&prog, &hdr, packet);
        }
    }
    return 0;
}

static void test_pcap_compile(pcap_t *pcap_handle, const char *filter_str) {
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    
    int compile_result = pcap_compile(pcap_handle, &prog, filter_str, 0, 0);
    
    if (compile_result == 0) {
        bpf_dump(&prog, 1);
        
        if (pcap_setfilter(pcap_handle, &prog) == 0) {
            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.ts.tv_sec = time(NULL);
            hdr.ts.tv_usec = 0;
            hdr.caplen = 64;
            hdr.len = 64;
            
            unsigned char packet[64] = {0};
            pcap_offline_filter(&prog, &hdr, packet);
        }
    }
    
    pcap_freecode(&prog);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap_handle) return 0;
    
    if (Size >= sizeof(struct bpf_insn)) {
        size_t max_insns = Size / sizeof(struct bpf_insn);
        if (max_insns > 1000) max_insns = 1000;
        
        const struct bpf_insn *insns = (const struct bpf_insn *)Data;
        validate_and_test_bpf(insns, (int)max_insns, pcap_handle);
    }
    
    char *filter_str = (char *)malloc(Size + 1);
    if (filter_str) {
        memcpy(filter_str, Data, Size);
        filter_str[Size] = '\0';
        test_pcap_compile(pcap_handle, filter_str);
        free(filter_str);
    }
    
    pcap_close(pcap_handle);
    return 0;
}