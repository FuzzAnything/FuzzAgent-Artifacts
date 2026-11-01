// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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
#include <stdio.h>
#include <sys/time.h>

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        free(prog->bf_insns);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

static pcap_t *create_dead_handle(int linktype) {
    return pcap_open_dead(linktype, 65535);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from input data
    int linktype = Data[0];
    int snaplen = 65535;
    int optimize = (Data[1] & 0x01);
    int bpf_dump_opt = Data[2];
    bpf_u_int32 netmask = 0;
    if (Size > 4) {
        netmask = Data[3];
    }

    // Prepare filter string from input (ensure null termination)
    char filter_str[256];
    size_t filter_len = (Size > 255) ? 255 : Size - 4;
    if (Size > 4) {
        memcpy(filter_str, Data + 4, filter_len);
    } else {
        filter_len = 0;
    }
    filter_str[filter_len] = '\0';

    // Test 1: pcap_compile_nopcap (deprecated but functional for fuzzing)
    struct bpf_program prog_nopcap;
    memset(&prog_nopcap, 0, sizeof(prog_nopcap));
    int ret_nopcap = pcap_compile_nopcap(linktype, snaplen, &prog_nopcap, filter_str, optimize, netmask);
    if (ret_nopcap == 0) {
        // Validate the compiled program
        bpf_validate(prog_nopcap.bf_insns, prog_nopcap.bf_len);
        // Dump the program
        bpf_dump(&prog_nopcap, bpf_dump_opt);
        // Cleanup
        cleanup_bpf_program(&prog_nopcap);
    }

    // Test 2: pcap_open_dead + pcap_compile
    pcap_t *handle = create_dead_handle(linktype);
    if (handle) {
        struct bpf_program prog;
        memset(&prog, 0, sizeof(prog));
        int ret_compile = pcap_compile(handle, &prog, filter_str, optimize, netmask);
        if (ret_compile == 0) {
            bpf_validate(prog.bf_insns, prog.bf_len);
            bpf_dump(&prog, bpf_dump_opt);
            cleanup_bpf_program(&prog);
        }
        pcap_close(handle);
    }

    // Test 3: bpf_validate with raw instructions derived from input
    if (Size > 8) {
        int num_insns = (Size - 8) / sizeof(struct bpf_insn);
        if (num_insns > 0) {
            struct bpf_insn *raw_insns = malloc(num_insns * sizeof(struct bpf_insn));
            if (raw_insns) {
                memcpy(raw_insns, Data + 8, num_insns * sizeof(struct bpf_insn));
                bpf_validate(raw_insns, num_insns);
                free(raw_insns);
            }
        }
    }

    // Test 4: pcap_offline_filter and bpf_filter
    if (Size > 12) {
        // Re-compile to get a valid program for filtering
        pcap_t *handle2 = create_dead_handle(linktype);
        if (handle2) {
            struct bpf_program prog2;
            memset(&prog2, 0, sizeof(prog2));
            if (pcap_compile(handle2, &prog2, filter_str, optimize, netmask) == 0) {
                pcap_close(handle2);

                // Prepare packet header and data
                struct pcap_pkthdr hdr;
                memset(&hdr, 0, sizeof(hdr));
                hdr.ts.tv_sec = 1234567890;
                hdr.ts.tv_usec = 123456;
                hdr.caplen = (Size > 16) ? (Size - 16) : 0;
                hdr.len = hdr.caplen;

                const u_char *pkt_data = (Size > 16) ? Data + 16 : NULL;

                // Test pcap_offline_filter
                if (pkt_data) {
                    pcap_offline_filter(&prog2, &hdr, pkt_data);
                }

                // Test bpf_filter (deprecated)
                if (pkt_data && prog2.bf_insns) {
                    bpf_filter(prog2.bf_insns, pkt_data, hdr.caplen, hdr.len);
                }

                cleanup_bpf_program(&prog2);
            } else {
                pcap_close(handle2);
            }
        }
    }

    return 0;
}