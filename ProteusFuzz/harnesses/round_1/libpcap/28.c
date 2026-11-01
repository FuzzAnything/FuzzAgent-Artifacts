// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <time.h>

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        pcap_freecode(prog);
        prog->bf_len = 0;
        prog->bf_insns = NULL;
    }
}

static pcap_t *create_dead_handle(int linktype) {
    pcap_t *handle = pcap_open_dead(linktype, 65535);
    if (!handle) {
        return NULL;
    }
    // Ensure the handle is activated for pcap_compile to work
    pcap_activate(handle);
    return handle;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Parse input to determine operations and parameters
    uint8_t op = Data[0] % 6; // 0-5 for 6 target functions
    uint8_t linktype = Data[1];

    // Prepare common structures
    struct bpf_program prog = {0};
    struct pcap_pkthdr pkthdr = {0};
    u_char dummy_packet[1024];
    memset(&pkthdr, 0, sizeof(pkthdr));
    pkthdr.caplen = (Size > 10) ? (Size - 10) : 0;
    pkthdr.len = pkthdr.caplen;
    pkthdr.ts.tv_sec = 1234567890;
    pkthdr.ts.tv_usec = 123456;
    memset(dummy_packet, 0, sizeof(dummy_packet));
    if (Size > 10) {
        memcpy(dummy_packet, &Data[10], pkthdr.caplen);
    }

    // Prepare filter string from input data
    // Ensure null termination
    size_t filter_len = Size - 2;
    char *filter_str = (char *)malloc(filter_len + 1);
    if (!filter_str) return 0;
    memcpy(filter_str, &Data[2], filter_len);
    filter_str[filter_len] = '\0';

    int ret = 0;
    pcap_t *handle = NULL;
    bpf_u_int32 netmask = 0;
    int optimize = 0;
    int snaplen = 65535;

    // Safely extract optional parameters if data is large enough
    if (Size >= 10) {
        netmask = *(const bpf_u_int32 *)&Data[6];
        optimize = (Data[7] % 2);
        snaplen = Data[8] * 100;
        if (snaplen == 0) snaplen = 65535;
    }

    switch (op) {
        case 0: {
            // pcap_compile
            // Requires an activated handle
            handle = create_dead_handle(linktype);
            if (handle) {
                ret = pcap_compile(handle, &prog, filter_str, optimize, netmask);
                if (ret == 0) {
                    // Success, try to use the program
                    bpf_validate(prog.bf_insns, prog.bf_len);
                    bpf_filter(prog.bf_insns, dummy_packet, pkthdr.caplen, pkthdr.len);
                    pcap_offline_filter(&prog, &pkthdr, dummy_packet);
                    bpf_dump(&prog, 0);
                    bpf_dump(&prog, 1);
                    bpf_dump(&prog, 2);
                }
                cleanup_bpf_program(&prog);
                pcap_close(handle);
            }
            break;
        }
        case 1: {
            // bpf_filter
            // Needs a valid program. We create one first.
            handle = create_dead_handle(linktype);
            if (handle) {
                if (pcap_compile(handle, &prog, filter_str, optimize, netmask) == 0) {
                    u_int result = bpf_filter(prog.bf_insns, dummy_packet, pkthdr.caplen, pkthdr.len);
                    (void)result;
                    cleanup_bpf_program(&prog);
                }
                pcap_close(handle);
            }
            break;
        }
        case 2: {
            // bpf_validate
            // Needs a valid program.
            handle = create_dead_handle(linktype);
            if (handle) {
                if (pcap_compile(handle, &prog, filter_str, optimize, netmask) == 0) {
                    int valid = bpf_validate(prog.bf_insns, prog.bf_len);
                    (void)valid;
                    cleanup_bpf_program(&prog);
                }
                pcap_close(handle);
            }
            break;
        }
        case 3: {
            // pcap_compile_nopcap
            ret = pcap_compile_nopcap(linktype, snaplen, &prog, filter_str, optimize, netmask);
            if (ret == 0) {
                bpf_validate(prog.bf_insns, prog.bf_len);
                bpf_filter(prog.bf_insns, dummy_packet, pkthdr.caplen, pkthdr.len);
                pcap_offline_filter(&prog, &pkthdr, dummy_packet);
                bpf_dump(&prog, 0);
                cleanup_bpf_program(&prog);
            }
            break;
        }
        case 4: {
            // pcap_offline_filter
            // Needs a valid program
            handle = create_dead_handle(linktype);
            if (handle) {
                if (pcap_compile(handle, &prog, filter_str, optimize, netmask) == 0) {
                    int match = pcap_offline_filter(&prog, &pkthdr, dummy_packet);
                    (void)match;
                    cleanup_bpf_program(&prog);
                }
                pcap_close(handle);
            }
            break;
        }
        case 5: {
            // bpf_dump
            // Needs a valid program
            handle = create_dead_handle(linktype);
            if (handle) {
                if (pcap_compile(handle, &prog, filter_str, optimize, netmask) == 0) {
                    bpf_dump(&prog, 0);
                    bpf_dump(&prog, 1);
                    bpf_dump(&prog, 2);
                    cleanup_bpf_program(&prog);
                }
                pcap_close(handle);
            }
            break;
        }
        default:
            break;
    }

    free(filter_str);
    return 0;
}