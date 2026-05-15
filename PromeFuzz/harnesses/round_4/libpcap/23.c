// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/bpf.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void init_pcap_pkthdr(struct pcap_pkthdr *hdr, const uint8_t *data, size_t size) {
    if (size < sizeof(struct pcap_pkthdr)) {
        memset(hdr, 0, sizeof(struct pcap_pkthdr));
        return;
    }
    memcpy(hdr, data, sizeof(struct pcap_pkthdr));
    if (hdr->caplen > size - sizeof(struct pcap_pkthdr)) {
        hdr->caplen = size - sizeof(struct pcap_pkthdr);
    }
    if (hdr->len < hdr->caplen) {
        hdr->len = hdr->caplen;
    }
}

static pcap_t* create_test_pcap_handle(int linktype) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_dead(linktype, 65535);
    if (!pcap) {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
    }
    return pcap;
}

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog->bf_insns) {
        pcap_freecode(prog);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program prog = {0};
    struct bpf_program prog2 = {0};
    int ret;
    
    // Extract parameters from fuzzer input
    int linktype = Data[0] % 256;
    int optimize = Data[1] % 2;
    bpf_u_int32 netmask = *(const bpf_u_int32*)(Data + 2);
    size_t filter_str_len = Size > 100 ? 100 : Size - 4;
    char *filter_str = malloc(filter_str_len + 1);
    if (!filter_str) return 0;
    memcpy(filter_str, Data + 4, filter_str_len);
    filter_str[filter_str_len] = '\0';
    
    // Test pcap_compile_nopcap
    memset(&prog, 0, sizeof(prog));
    ret = pcap_compile_nopcap(linktype, 65535, &prog, filter_str, optimize, netmask);
    cleanup_bpf_program(&prog);
    
    // Test bpf_validate with random instructions
    if (Size > 20) {
        size_t insn_size = (Size - 20) / sizeof(struct bpf_insn);
        if (insn_size > 0) {
            struct bpf_insn *insns = (struct bpf_insn*)(Data + 20);
            ret = bpf_validate(insns, insn_size > 1000 ? 1000 : insn_size);
        }
    }
    
    // Test pcap_compile and pcap_setfilter
    pcap_t *pcap = create_test_pcap_handle(linktype);
    if (pcap) {
        memset(&prog2, 0, sizeof(prog2));
        ret = pcap_compile(pcap, &prog2, filter_str, optimize, netmask);
        if (ret == 0) {
            ret = pcap_setfilter(pcap, &prog2);
            
            // Test bpf_dump
            bpf_dump(&prog2, 1);
            bpf_dump(&prog2, 0);
            
            // Test pcap_offline_filter if filter was successfully set
            if (ret == 0 && Size > 50) {
                struct pcap_pkthdr hdr;
                const u_char *packet_data = Data + 50;
                size_t packet_size = Size - 50;
                
                init_pcap_pkthdr(&hdr, Data + 20, Size - 20);
                if (hdr.caplen > packet_size) {
                    hdr.caplen = packet_size;
                }
                if (hdr.len < hdr.caplen) {
                    hdr.len = hdr.caplen;
                }
                
                ret = pcap_offline_filter(&prog2, &hdr, packet_data);
            }
        }
        cleanup_bpf_program(&prog2);
        pcap_close(pcap);
    }
    
    free(filter_str);
    return 0;
}