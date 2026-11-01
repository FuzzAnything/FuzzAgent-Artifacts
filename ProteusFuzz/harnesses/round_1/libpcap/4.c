// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // 1. Prepare environment
    pcap_t *handle = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    struct bpf_program prog = {0};
    struct bpf_insn *insns = NULL;
    int len = 0;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Create a dummy file for offline capture
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write minimal pcap header (magic, version, etc.)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet
        fwrite(&magic, sizeof(magic), 1, fp);
        fwrite(&version_major, sizeof(version_major), 1, fp);
        fwrite(&version_minor, sizeof(version_minor), 1, fp);
        fwrite(&thiszone, sizeof(thiszone), 1, fp);
        fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
        fwrite(&snaplen, sizeof(snaplen), 1, fp);
        fwrite(&network, sizeof(network), 1, fp);
        fclose(fp);
    }

    // Open offline capture
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (!handle) {
        return 0;
    }

    // Step 1: Invoke pcap_geterr (Initial state check)
    char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Prepare BPF instructions for bpf_validate and bpf_dump
    if (Size >= 24) {
        len = 4;
        insns = malloc(len * sizeof(struct bpf_insn));
        if (insns) {
            insns[0].code = 0x20; insns[0].jt = 0; insns[0].jf = 0; insns[0].k = 0;
            insns[1].code = 0x06; insns[1].jt = 0; insns[1].jf = 0; insns[1].k = 1;
            insns[2].code = 0x06; insns[2].jt = 0; insns[2].jf = 0; insns[2].k = 0;
            insns[3].code = 0x06; insns[3].jt = 0; insns[3].jf = 0; insns[3].k = 0;

            prog.bf_len = len;
            prog.bf_insns = insns;
        }
    } else {
        insns = malloc(1 * sizeof(struct bpf_insn));
        if (insns) {
            insns[0].code = 0x06; insns[0].jt = 0; insns[0].jf = 0; insns[0].k = 1;
            prog.bf_len = 1;
            prog.bf_insns = insns;
        }
    }

    // Step 2: Invoke bpf_validate
    if (insns) {
        int ret_val = bpf_validate(insns, prog.bf_len);
        (void)ret_val;
    }

    // Step 3: Invoke bpf_dump
    if (insns) {
        bpf_dump(&prog, 0);
        bpf_dump(&prog, 1);
        bpf_dump(&prog, 2);
        bpf_dump(&prog, 3);
    }

    // Step 4: Invoke pcap_next_ex
    int next_ex_ret = pcap_next_ex(handle, &header, &packet);
    (void)next_ex_ret;

    // Step 5: Invoke pcap_geterr again
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Step 6: Invoke pcap_offline_filter
    struct pcap_pkthdr dummy_header = {0};
    const u_char *dummy_packet = NULL;
    
    if (header && packet) {
        dummy_header = *header;
        dummy_packet = packet;
    } else {
        dummy_header.ts.tv_sec = 0;
        dummy_header.ts.tv_usec = 0;
        dummy_header.caplen = 0;
        dummy_header.len = 0;
        dummy_packet = NULL;
    }

    int filter_ret = pcap_offline_filter(&prog, &dummy_header, dummy_packet);
    (void)filter_ret;

    // Cleanup
    if (insns) {
        free(insns);
    }
    if (handle) {
        pcap_close(handle);
    }

    remove("./dummy_file");

    return 0;
}