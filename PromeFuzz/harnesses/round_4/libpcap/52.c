// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/bpf.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Extract parameters from fuzzing input
    if (Size < 20) return 0; // Minimum size for basic parameters
    
    // Parse first 16 bytes as integers for various parameters
    int snaplen = *(const int*)Data;
    int linktype = *(const int*)(Data + 4);
    int optimize = *(const int*)(Data + 8) & 1; // Ensure 0 or 1
    bpf_u_int32 mask = *(const bpf_u_int32*)(Data + 12);
    
    // Use remaining data as filter expression string
    const char *filter_str = (const char*)(Data + 16);
    size_t filter_len = Size - 16;
    
    // Ensure null termination for filter string
    char *null_term_filter = malloc(filter_len + 1);
    if (!null_term_filter) return 0;
    memcpy(null_term_filter, filter_str, filter_len);
    null_term_filter[filter_len] = '\0';
    
    // 1. Test pcap_compile_nopcap
    struct bpf_program prog_nopcap = {0};
    pcap_compile_nopcap(snaplen, linktype, &prog_nopcap, null_term_filter, optimize, mask);
    pcap_freecode(&prog_nopcap);
    
    // 2. Test pcap_open_dead_with_tstamp_precision
    u_int precision = mask & 1; // Microsecond (0) or nanosecond (1)
    pcap_t *dead_pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
    if (dead_pcap) {
        // 4. Test pcap_compile with dead pcap handle
        struct bpf_program prog = {0};
        pcap_compile(dead_pcap, &prog, null_term_filter, optimize, mask);
        pcap_freecode(&prog);
        
        // 5. Test bpf_dump if compilation succeeded
        if (prog.bf_insns && prog.bf_len > 0) {
            bpf_dump(&prog, 0); // Level 0: human readable
            bpf_dump(&prog, 1); // Level 1: raw output
            
            // 3. Test bpf_validate
            int valid = bpf_validate(prog.bf_insns, prog.bf_len);
            
            // 6. Test bpf_filter if program is valid
            if (valid && Size > 0) {
                // Use first part of input as packet data
                u_int packet_len = Size > 65535 ? 65535 : (u_int)Size;
                bpf_filter(prog.bf_insns, Data, packet_len, packet_len);
            }
        }
        
        pcap_close(dead_pcap);
    }
    
    // Test with different timestamp precision
    pcap_t *dead_pcap_nano = pcap_open_dead_with_tstamp_precision(linktype, snaplen, 1);
    if (dead_pcap_nano) {
        struct bpf_program prog2 = {0};
        pcap_compile(dead_pcap_nano, &prog2, null_term_filter, optimize, mask);
        pcap_freecode(&prog2);
        pcap_close(dead_pcap_nano);
    }
    
    // Test with invalid precision (should default to microsecond)
    pcap_t *dead_pcap_invalid = pcap_open_dead_with_tstamp_precision(linktype, snaplen, 99);
    if (dead_pcap_invalid) {
        pcap_close(dead_pcap_invalid);
    }
    
    // Test bpf_validate with random instruction data
    if (Size >= sizeof(struct bpf_insn) + 20) {
        const struct bpf_insn *insns = (const struct bpf_insn*)(Data + 20);
        int insn_count = (int)((Size - 20) / sizeof(struct bpf_insn));
        if (insn_count > 0) {
            bpf_validate(insns, insn_count);
        }
    }
    
    free(null_term_filter);
    return 0;
}