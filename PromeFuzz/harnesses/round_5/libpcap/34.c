// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog) {
        pcap_freecode(prog);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Create dummy pcap handle for pcap_compile */
    pcap_t *pcap = pcap_open_dead_with_tstamp_precision(DLT_EN10MB, 65535, PCAP_TSTAMP_PRECISION_MICRO);
    if (!pcap) {
        return 0;
    }

    /* Prepare filter string from fuzzer input */
    char *filter_str = malloc(Size + 1);
    if (!filter_str) {
        pcap_close(pcap);
        return 0;
    }
    memcpy(filter_str, Data, Size);
    filter_str[Size] = '\0';

    /* Test pcap_compile */
    struct bpf_program prog1 = {0};
    int optimize = Data[0] & 1;
    bpf_u_int32 netmask = (bpf_u_int32)Size;
    pcap_compile(pcap, &prog1, filter_str, optimize, netmask);
    cleanup_bpf_program(&prog1);

    /* Test pcap_compile_nopcap */
    struct bpf_program prog2 = {0};
    int snaplen = (Size % 65535) + 1;
    int linktype = (Data[0] % 20) + 1;
    pcap_compile_nopcap(snaplen, linktype, &prog2, filter_str, optimize, netmask);
    cleanup_bpf_program(&prog2);

    /* Test bpf_dump if we have a valid program */
    struct bpf_program prog3 = {0};
    if (pcap_compile(pcap, &prog3, filter_str, optimize, netmask) == 0) {
        bpf_dump(&prog3, (Data[0] % 3)); /* Different verbosity levels */
    }
    cleanup_bpf_program(&prog3);

    /* Test bpf_validate with fuzzer data as BPF instructions */
    if (Size >= sizeof(struct bpf_insn)) {
        size_t num_insns = Size / sizeof(struct bpf_insn);
        struct bpf_insn *insns = (struct bpf_insn *)Data;
        bpf_validate(insns, (int)num_insns);
    }

    /* Test pcap_open_dead_with_tstamp_precision with various parameters */
    u_int precision = (Data[0] % 2) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
    pcap_t *pcap2 = pcap_open_dead_with_tstamp_precision(DLT_RAW, snaplen, precision);
    if (pcap2) {
        pcap_close(pcap2);
    }

    /* Test bpf_filter with validated instructions */
    if (Size >= sizeof(struct bpf_insn) * 2) {
        size_t num_insns = Size / sizeof(struct bpf_insn);
        struct bpf_insn *insns = (struct bpf_insn *)Data;
        if (bpf_validate(insns, (int)num_insns) != 0) {
            u_int packet_len = (Size % 1500) + 1;
            u_int capture_len = packet_len;
            if (capture_len > Size) {
                capture_len = (u_int)Size;
            }
            bpf_filter(insns, Data, packet_len, capture_len);
        }
    }

    /* Cleanup */
    free(filter_str);
    pcap_close(pcap);
    return 0;
}