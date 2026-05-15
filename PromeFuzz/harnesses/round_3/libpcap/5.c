// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/bpf.h"
#include "pcap/pcap.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create a dummy pcap_t handle using pcap_open_dead
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return 0;
    }

    // First call to pcap_geterr (should return empty string initially)
    char *err1 = pcap_geterr(pcap);
    (void)err1; // Use result to avoid unused variable warning

    // Parse fuzzer input for BPF instructions
    // We'll use the first part of input as BPF instructions
    // Minimum size for one bpf_insn
    if (Size < sizeof(struct bpf_insn)) {
        pcap_close(pcap);
        return 0;
    }

    // Create a BPF program from fuzzer data
    struct bpf_program prog;
    // Use reasonable limits: max 100 instructions
    size_t max_insns = 100;
    size_t insn_bytes = sizeof(struct bpf_insn);
    size_t available_insns = Size / insn_bytes;
    if (available_insns > max_insns) {
        available_insns = max_insns;
    }
    
    prog.bf_len = (u_int)available_insns;
    prog.bf_insns = (struct bpf_insn *)Data;

    // Call bpf_validate
    int valid = bpf_validate(prog.bf_insns, prog.bf_len);
    (void)valid; // Use result

    // Call bpf_dump (output goes to stdout, but that's OK for fuzzing)
    bpf_dump(&prog, 1);

    // Create a dummy pcap file for pcap_next_ex
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        // Write a simple pcap file header (global header)
        struct pcap_file_header hdr;
        hdr.magic = 0xa1b2c3d4;
        hdr.version_major = 2;
        hdr.version_minor = 4;
        hdr.thiszone = 0;
        hdr.sigfigs = 0;
        hdr.snaplen = 65535;
        hdr.linktype = DLT_EN10MB;
        
        fwrite(&hdr, sizeof(hdr), 1, dummy_file);
        
        // Write one dummy packet using remaining fuzzer data
        if (Size > 0) {
            struct pcap_pkthdr pkthdr;
            pkthdr.ts.tv_sec = 0;
            pkthdr.ts.tv_usec = 0;
            pkthdr.caplen = (Size > 65535) ? 65535 : (bpf_u_int32)Size;
            pkthdr.len = pkthdr.caplen;
            
            fwrite(&pkthdr, sizeof(pkthdr), 1, dummy_file);
            fwrite(Data, 1, pkthdr.caplen, dummy_file);
        }
        fclose(dummy_file);
    }

    // Open the dummy file for offline reading
    pcap_t *offline_pcap = pcap_open_offline("./dummy_file", NULL);
    if (offline_pcap) {
        struct pcap_pkthdr *header;
        const u_char *packet_data;
        
        // Call pcap_next_ex
        int ret = pcap_next_ex(offline_pcap, &header, &packet_data);
        (void)ret; // Use result
        
        // Second call to pcap_geterr
        char *err2 = pcap_geterr(offline_pcap);
        (void)err2; // Use result

        // Prepare data for pcap_offline_filter
        if (header && packet_data && prog.bf_len > 0) {
            // Create a compiled BPF program for pcap_offline_filter
            struct bpf_program compiled_prog;
            char errbuf[PCAP_ERRBUF_SIZE];
            
            // Try to compile a simple filter
            if (pcap_compile(pcap, &compiled_prog, "ip", 1, PCAP_NETMASK_UNKNOWN) == 0) {
                // Call pcap_offline_filter
                int filter_result = pcap_offline_filter(&compiled_prog, header, packet_data);
                (void)filter_result; // Use result
                pcap_freecode(&compiled_prog);
            }
        }
        
        pcap_close(offline_pcap);
    }

    // Clean up
    pcap_close(pcap);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}