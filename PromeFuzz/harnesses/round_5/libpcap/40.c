// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile at gencode.c:1105:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pcap/pcap.h"
#include "pcap/bpf.h"

static void dummy_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = NULL;
    struct bpf_program bpf_prog = {0};
    int optimize = 1;
    bpf_u_int32 netmask = 0xFFFFFF00;
    int ret;
    int linktype;
    int snaplen;
    const struct bpf_insn *insns = NULL;
    int insn_count = 0;
    FILE *dummy_file = NULL;

    /* Create a dummy pcap handle for testing */
    linktype = DLT_EN10MB;
    snaplen = 65535;
    pcap_handle = pcap_open_dead(linktype, snaplen);
    if (!pcap_handle) {
        goto cleanup;
    }

    /* Create a dummy file for potential use */
    dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy_file);
        }
        fclose(dummy_file);
        dummy_file = NULL;
    }

    /* Extract filter string from input */
    size_t filter_len = Size > 256 ? 256 : Size;
    char *filter_str = (char *)malloc(filter_len + 1);
    if (!filter_str) {
        goto cleanup;
    }
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    /* Test pcap_compile */
    memset(&bpf_prog, 0, sizeof(bpf_prog));
    ret = pcap_compile(pcap_handle, &bpf_prog, filter_str, optimize, netmask);
    if (ret == 0) {
        /* Test pcap_setfilter if compilation succeeded */
        int setfilter_ret = pcap_setfilter(pcap_handle, &bpf_prog);
        (void)setfilter_ret; /* Ignore result, just test the call */

        /* Test bpf_dump with the compiled program */
        bpf_dump(&bpf_prog, 1);
        bpf_dump(&bpf_prog, 2);
    }
    pcap_freecode(&bpf_prog);

    /* Test pcap_compile_nopcap (deprecated but still testable) */
    memset(&bpf_prog, 0, sizeof(bpf_prog));
    int nopcap_linktype = DLT_EN10MB;
    int nopcap_snaplen = 65535;
    pcap_compile_nopcap(nopcap_linktype, nopcap_snaplen, &bpf_prog, filter_str, optimize, netmask);
    pcap_freecode(&bpf_prog);

    /* Test bpf_validate with some dummy instructions */
    if (Size >= sizeof(struct bpf_insn) * 2) {
        /* Use part of input as BPF instructions for validation */
        insns = (const struct bpf_insn *)Data;
        insn_count = (int)(Size / sizeof(struct bpf_insn));
        if (insn_count > 100) {
            insn_count = 100; /* Limit to reasonable size */
        }
        bpf_validate(insns, insn_count);
    }

    /* Test with different optimization flags */
    memset(&bpf_prog, 0, sizeof(bpf_prog));
    optimize = 0;
    pcap_compile(pcap_handle, &bpf_prog, filter_str, optimize, netmask);
    pcap_freecode(&bpf_prog);

    /* Test with different netmask */
    memset(&bpf_prog, 0, sizeof(bpf_prog));
    netmask = 0xFFFF0000;
    pcap_compile(pcap_handle, &bpf_prog, filter_str, 1, netmask);
    pcap_freecode(&bpf_prog);

    /* Test with empty filter string */
    memset(&bpf_prog, 0, sizeof(bpf_prog));
    pcap_compile(pcap_handle, &bpf_prog, "", 1, netmask);
    pcap_freecode(&bpf_prog);

    free(filter_str);

cleanup:
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    if (dummy_file) {
        fclose(dummy_file);
    }
    return 0;
}