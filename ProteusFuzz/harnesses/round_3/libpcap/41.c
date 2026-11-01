// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        free(prog->bf_insns);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Interpret input data to drive fuzzing logic
    uint32_t seed = *(const uint32_t *)Data;
    Data += 4;
    Size -= 4;

    // 1. Fuzz bpf_validate
    // Construct a bpf_insn array from input
    if (Size >= sizeof(struct bpf_insn)) {
        size_t num_insns = Size / sizeof(struct bpf_insn);
        if (num_insns > 1000) num_insns = 1000; // Limit size to prevent OOM

        struct bpf_insn *insns = malloc(num_insns * sizeof(struct bpf_insn));
        if (insns) {
            memcpy(insns, Data, num_insns * sizeof(struct bpf_insn));
            bpf_validate(insns, (int)num_insns);
            free(insns);
        }
    }

    // 2. Fuzz pcap_compile_nopcap
    // We need a filter string. Try to parse a string from the remaining data if possible,
    // or use a simple default if data is binary.
    // For robustness, we'll create a small buffer and try to null-terminate a substring.
    char filter_str[256];
    size_t str_len = (Size > 255) ? 255 : Size;
    memcpy(filter_str, Data, str_len);
    filter_str[str_len] = '\0';

    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // Use seed to determine linktype (e.g., DLT_EN10MB = 1)
    int linktype = (seed % 2) ? DLT_EN10MB : DLT_NULL;
    int netmask = 0xffffff00;
    
    // pcap_compile_nopcap is deprecated but we test it as requested
    int ret = pcap_compile_nopcap(65535, linktype, &fp, filter_str, 1, netmask);
    if (ret == 0) {
        // If compilation succeeded, we can use fp for other tests
        // But we must free it later
    }
    
    // 3. Fuzz pcap_offline_filter
    // Prepare a packet header and data
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.ts.tv_sec = seed;
    hdr.ts.tv_usec = seed % 1000000;
    hdr.caplen = (Size > 1000) ? 1000 : Size;
    hdr.len = hdr.caplen;

    if (fp.bf_insns) {
        pcap_offline_filter(&fp, &hdr, Data);
    }

    // 4. Fuzz bpf_filter
    // Requires raw instructions. Reuse or create new ones.
    // Let's create a simple valid program for testing if fp is empty, or use fp if valid.
    if (fp.bf_insns && fp.bf_len > 0) {
        u_int len = (Size > 1000) ? 1000 : Size;
        bpf_filter(fp.bf_insns, Data, len, len);
    } else {
        // Create a minimal valid program for bpf_filter if compilation failed
        struct bpf_insn min_insns[2] = {
            { BPF_LD | BPF_B | BPF_ABS, 0, 0, 0 },
            { BPF_RET | BPF_K, 0, 0, 0 }
        };
        u_int len = (Size > 1000) ? 1000 : Size;
        bpf_filter(min_insns, Data, len, len);
    }

    // 5. Fuzz bpf_dump
    if (fp.bf_insns) {
        // Dump with different options based on seed
        int dump_opt = seed % 3;
        bpf_dump(&fp, dump_opt);
    }

    // 6. Fuzz pcap_compile
    // Requires a pcap_t handle. We use pcap_open_dead to create a dummy handle.
    pcap_t *handle = pcap_open_dead(linktype, 65535);
    if (handle) {
        struct bpf_program fp2;
        memset(&fp2, 0, sizeof(fp2));
        
        // Try to compile the same filter string
        pcap_compile(handle, &fp2, filter_str, 1, netmask);
        
        if (fp2.bf_insns) {
            // Use the compiled program for offline filter test
            pcap_offline_filter(&fp2, &hdr, Data);
            // Cleanup
            cleanup_bpf_program(&fp2);
        }
        
        pcap_close(handle);
    }

    // Cleanup for pcap_compile_nopcap result
    cleanup_bpf_program(&fp);

    return 0;
}