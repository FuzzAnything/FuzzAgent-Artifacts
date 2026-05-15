// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
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

static void dummy_packet_data(struct pcap_pkthdr *hdr, const u_char **data, const uint8_t *fuzz_data, size_t fuzz_size) {
    static u_char packet_buffer[65536];
    
    hdr->ts.tv_sec = time(NULL);
    hdr->ts.tv_usec = 0;
    
    if (fuzz_size > sizeof(packet_buffer)) {
        hdr->caplen = sizeof(packet_buffer);
        hdr->len = sizeof(packet_buffer);
    } else {
        hdr->caplen = fuzz_size;
        hdr->len = fuzz_size;
    }
    
    if (fuzz_size > 0 && fuzz_data != NULL) {
        memcpy(packet_buffer, fuzz_data, hdr->caplen);
    } else {
        hdr->caplen = 64;
        hdr->len = 64;
        memset(packet_buffer, 0xAA, 64);
    }
    
    *data = packet_buffer;
}

static int compile_dummy_filter(pcap_t *pcap, struct bpf_program *fp) {
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netmask = 0xffffff00;
    
    if (pcap_compile(pcap, fp, "ip", 1, netmask) == -1) {
        return -1;
    }
    
    if (pcap_setfilter(pcap, fp) == -1) {
        pcap_freecode(fp);
        return -1;
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap;
    struct bpf_program filter;
    struct bpf_insn *insns = NULL;
    int insn_count = 0;
    
    if (Size < sizeof(struct bpf_insn)) {
        return 0;
    }
    
    pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return 0;
    }
    
    char *err = pcap_geterr(pcap);
    (void)err;
    
    insn_count = Size / sizeof(struct bpf_insn);
    if (insn_count > 1024) {
        insn_count = 1024;
    }
    
    insns = (struct bpf_insn *)Data;
    
    int valid = bpf_validate(insns, insn_count);
    (void)valid;
    
    struct bpf_program prog;
    prog.bf_len = insn_count;
    prog.bf_insns = insns;
    bpf_dump(&prog, 1);
    
    struct pcap_pkthdr hdr;
    const u_char *pkt_data;
    dummy_packet_data(&hdr, &pkt_data, Data, Size);
    
    int ret = pcap_next_ex(pcap, (struct pcap_pkthdr **)&hdr, &pkt_data);
    (void)ret;
    
    err = pcap_geterr(pcap);
    (void)err;
    
    if (compile_dummy_filter(pcap, &filter) == 0) {
        pcap_offline_filter(&filter, &hdr, pkt_data);
        pcap_freecode(&filter);
    }
    
    pcap_close(pcap);
    return 0;
}