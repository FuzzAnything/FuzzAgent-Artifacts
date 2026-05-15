// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "pcap/pcap.h"
#include "pcap/bpf.h"

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Early return if input is too small for basic operations
    if (Size < 1) {
        return 0;
    }

    // Split input for different purposes
    size_t filter_str_len = Size / 4;
    if (filter_str_len > 256) filter_str_len = 256;
    size_t bpf_insn_len = Size / 8;
    if (bpf_insn_len > 64) bpf_insn_len = 64;
    size_t packet_data_len = Size - filter_str_len - bpf_insn_len;
    if (packet_data_len > 2048) packet_data_len = 2048;

    // Create filter string (null-terminated)
    char *filter_str = malloc(filter_str_len + 1);
    if (!filter_str) return 0;
    memcpy(filter_str, Data, filter_str_len);
    filter_str[filter_str_len] = '\0';

    // Create BPF instructions array
    struct bpf_insn *insns = NULL;
    if (bpf_insn_len >= sizeof(struct bpf_insn)) {
        size_t num_insns = bpf_insn_len / sizeof(struct bpf_insn);
        insns = malloc(num_insns * sizeof(struct bpf_insn));
        if (insns) {
            memcpy(insns, Data + filter_str_len, num_insns * sizeof(struct bpf_insn));
        }
    }

    // Create packet data
    const u_char *packet_data = Data + filter_str_len + bpf_insn_len;

    // Mode selection based on first byte
    int mode = Data[0] % 6;

    // Create pcap handle for functions that need it
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        free(filter_str);
        free(insns);
        return 0;
    }

    // Initialize bpf_program structure
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));

    // Error buffer for pcap functions
    char errbuf[PCAP_ERRBUF_SIZE];

    switch (mode) {
        case 0: { // pcap_offline_filter
            // First compile a filter
            if (pcap_compile(pcap, &fp, filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
                // Create packet header
                struct pcap_pkthdr hdr;
                memset(&hdr, 0, sizeof(hdr));
                gettimeofday(&hdr.ts, NULL);
                hdr.caplen = packet_data_len < 65535 ? packet_data_len : 65535;
                hdr.len = packet_data_len;
                
                // Call offline filter
                pcap_offline_filter(&fp, &hdr, packet_data);
                
                pcap_freecode(&fp);
            }
            break;
        }
        
        case 1: { // bpf_validate
            if (insns) {
                size_t num_insns = bpf_insn_len / sizeof(struct bpf_insn);
                bpf_validate(insns, num_insns);
            }
            break;
        }
        
        case 2: { // bpf_dump
            if (pcap_compile(pcap, &fp, filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
                bpf_dump(&fp, Data[1] % 3); // Different verbosity levels
                pcap_freecode(&fp);
            }
            break;
        }
        
        case 3: { // pcap_compile
            int optimize = Data[1] & 1;
            bpf_u_int32 netmask = (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5];
            
            pcap_compile(pcap, &fp, filter_str, optimize, netmask);
            pcap_freecode(&fp);
            break;
        }
        
        case 4: { // pcap_setfilter
            if (pcap_compile(pcap, &fp, filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
                pcap_setfilter(pcap, &fp);
                pcap_freecode(&fp);
            }
            break;
        }
        
        case 5: { // pcap_compile_nopcap (deprecated but still fuzz)
            int snaplen = (Data[1] << 8) | Data[2];
            if (snaplen <= 0) snaplen = 65535;
            
            int linktype = Data[3] % 20; // Common link types
            
            pcap_compile_nopcap(linktype, snaplen, &fp, filter_str, 1, PCAP_NETMASK_UNKNOWN);
            pcap_freecode(&fp);
            break;
        }
    }

    // Cleanup
    pcap_close(pcap);
    free(filter_str);
    free(insns);
    
    return 0;
}