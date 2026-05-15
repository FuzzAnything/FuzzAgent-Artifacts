// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/bpf.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzz data to dummy file for pcap_open_offline
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for offline capture
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (!pcap) {
        remove("./dummy_file");
        return 0;
    }
    
    // First pcap_geterr call
    char *err_msg = pcap_geterr(pcap);
    (void)err_msg; // Use result to avoid unused variable warning
    
    // Prepare BPF program from fuzz data
    struct bpf_program bpf_prog;
    struct bpf_insn *insns = NULL;
    int insn_count = 0;
    
    if (Size >= sizeof(struct bpf_insn)) {
        insn_count = Size / sizeof(struct bpf_insn);
        insns = (struct bpf_insn *)Data;
        
        // bpf_validate call
        int valid = bpf_validate(insns, insn_count);
        (void)valid;
        
        // Set up bpf_program for bpf_dump
        bpf_prog.bf_len = insn_count;
        bpf_prog.bf_insns = insns;
        
        // bpf_dump call
        bpf_dump(&bpf_prog, 1);
    }
    
    // pcap_next_ex call
    struct pcap_pkthdr *header;
    const u_char *packet_data;
    int ret = pcap_next_ex(pcap, &header, &packet_data);
    (void)ret;
    
    // Second pcap_geterr call
    err_msg = pcap_geterr(pcap);
    (void)err_msg;
    
    // pcap_offline_filter call if we have valid packet data
    if (header && packet_data && insns && insn_count > 0) {
        // Create a compiled bpf_program using pcap_compile
        struct bpf_program compiled_prog;
        if (pcap_compile(pcap, &compiled_prog, "ip", 1, PCAP_NETMASK_UNKNOWN) == 0) {
            pcap_offline_filter(&compiled_prog, header, packet_data);
            pcap_freecode(&compiled_prog);
        }
    }
    
    // Cleanup
    pcap_close(pcap);
    remove("./dummy_file");
    
    return 0;
}