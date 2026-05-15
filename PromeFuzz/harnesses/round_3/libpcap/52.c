// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// bpf_image at bpf_image.c:130:1 in bpf.h
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

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Early return for insufficient data
    if (Size < sizeof(struct bpf_insn)) {
        return 0;
    }

    // Create dummy pcap handle for APIs that require it
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return 0;
    }

    // Test 1: bpf_validate
    // Use first part of input as BPF instructions
    size_t bpf_data_size = Size;
    const struct bpf_insn *insns = (const struct bpf_insn *)Data;
    int insn_count = bpf_data_size / sizeof(struct bpf_insn);
    if (insn_count > 0) {
        bpf_validate(insns, insn_count);
    }

    // Test 2: bpf_image
    // Convert first instruction to string if available
    if (insn_count > 0) {
        char *img = bpf_image(insns, 1);
        (void)img; // Result is thread-local static buffer
    }

    // Test 3: pcap_compile and related functions
    // Use middle section as filter string if possible
    size_t filter_str_len = Size > sizeof(struct bpf_insn) ? 
                           Size - sizeof(struct bpf_insn) : 0;
    
    if (filter_str_len > 0 && filter_str_len < 256) {
        char filter_str[256];
        size_t copy_len = filter_str_len < 255 ? filter_str_len : 255;
        memcpy(filter_str, Data + sizeof(struct bpf_insn), copy_len);
        filter_str[copy_len] = '\0';
        
        struct bpf_program prog;
        memset(&prog, 0, sizeof(prog));
        
        // Try to compile filter
        int compile_result = pcap_compile(pcap, &prog, filter_str, 0, PCAP_NETMASK_UNKNOWN);
        
        if (compile_result == 0) {
            // Test 4: bpf_dump with compiled program
            bpf_dump(&prog, 0);
            bpf_dump(&prog, 1);
            bpf_dump(&prog, 2);
            
            // Test 5: pcap_setfilter
            pcap_setfilter(pcap, &prog);
            
            // Test 6: pcap_offline_filter
            // Create dummy packet header and data
            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.ts.tv_sec = time(NULL);
            hdr.ts.tv_usec = 0;
            
            // Use remaining data for packet content
            size_t packet_data_offset = sizeof(struct bpf_insn) + copy_len;
            size_t packet_data_size = Size > packet_data_offset ? 
                                     Size - packet_data_offset : 0;
            
            if (packet_data_size > 0) {
                const u_char *packet_data = Data + packet_data_offset;
                hdr.caplen = packet_data_size > 65535 ? 65535 : packet_data_size;
                hdr.len = hdr.caplen;
                
                pcap_offline_filter(&prog, &hdr, packet_data);
            }
        }
        
        // Always free code even if compilation failed
        pcap_freecode(&prog);
    }

    // Test bpf_image with random instruction from input
    if (insn_count > 0) {
        // Use last instruction in buffer
        const struct bpf_insn *last_insn = insns + (insn_count - 1);
        char *img2 = bpf_image(last_insn, 1);
        (void)img2;
    }

    // Test bpf_validate with different lengths
    if (insn_count > 1) {
        // Try with half the instructions
        bpf_validate(insns, insn_count / 2);
        
        // Try with single instruction
        bpf_validate(insns, 1);
    }

    // Cleanup
    pcap_close(pcap);
    
    return 0;
}