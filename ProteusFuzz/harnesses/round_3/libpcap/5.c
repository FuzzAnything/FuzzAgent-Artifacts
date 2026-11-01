// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <sys/time.h>

// Define bpf_u_int32 if not defined in the system headers
#ifndef bpf_u_int32
typedef uint32_t bpf_u_int32;
#endif

// Forward declaration to ensure the symbol is visible to the linker
extern int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare environment: Create a dummy savefile for offline capture
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    
    // Write minimal pcap header (magic, version, snaplen, linktype)
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // Ethernet
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&thiszone, sizeof(thiszone), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);
    
    // Write a dummy packet header and data if enough data is available
    if (Size >= 16) {
        struct pcap_pkthdr hdr;
        hdr.ts.tv_sec = 1234567890;
        hdr.ts.tv_usec = 0;
        hdr.caplen = 14;
        hdr.len = 14;
        fwrite(&hdr, sizeof(hdr), 1, f);
        // Write 14 bytes of dummy data
        uint8_t dummy_data[14] = {0};
        fwrite(dummy_data, 1, 14, f);
    }
    fclose(f);

    // Open the offline capture
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(filename, errbuf);
    if (!handle) {
        return 0;
    }

    // 1. pcap_geterr
    const char *err_msg = pcap_geterr(handle);
    (void)err_msg; // Use to suppress unused variable warning

    // Prepare BPF instructions from fuzz data
    struct bpf_program fcode;
    fcode.bf_len = 0;
    fcode.bf_insns = NULL;

    // Extract instruction count from input if possible
    int instr_count = 0;
    if (Size >= 4) {
        instr_count = *(const uint32_t *)(Data);
        // Limit to prevent allocation of huge memory
        if (instr_count > 1000) {
            instr_count = 1000;
        }
    } else {
        instr_count = 1;
    }

    if (instr_count > 0) {
        fcode.bf_insns = (struct bpf_insn *)malloc(instr_count * sizeof(struct bpf_insn));
        if (!fcode.bf_insns) {
            pcap_close(handle);
            return 0;
        }
        fcode.bf_len = instr_count;

        // Fill instructions with fuzz data
        size_t offset = 0;
        if (Size >= 4) offset = 4;
        
        for (int i = 0; i < instr_count; i++) {
            struct bpf_insn *insn = &fcode.bf_insns[i];
            // Ensure we have enough data for one instruction (8 bytes)
            if (offset + 8 <= Size) {
                memcpy(insn, Data + offset, 8);
                offset += 8;
            } else {
                // Fill with zeros if not enough data
                memset(insn, 0, 8);
            }
        }
    }

    // 2. bpf_validate
    if (fcode.bf_insns) {
        int ret_validate = bpf_validate(fcode.bf_insns, fcode.bf_len);
        (void)ret_validate;
    }

    // 3. bpf_dump
    if (fcode.bf_insns) {
        // Try different dump options
        bpf_dump(&fcode, 0);
        bpf_dump(&fcode, 1);
        bpf_dump(&fcode, 2);
        bpf_dump(&fcode, 3);
    }

    // 4. pcap_next_ex
    struct pcap_pkthdr *pkt_header = NULL;
    const u_char *pkt_data = NULL;
    int ret_next = pcap_next_ex(handle, &pkt_header, &pkt_data);
    (void)ret_next;
    (void)pkt_header;
    (void)pkt_data;

    // 5. pcap_geterr
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 6. pcap_offline_filter
    if (fcode.bf_insns && pkt_header && pkt_data) {
        int ret_filter = pcap_offline_filter(&fcode, pkt_header, pkt_data);
        (void)ret_filter;
    }
    
    // Cleanup
    if (fcode.bf_insns) {
        free(fcode.bf_insns);
    }
    pcap_close(handle);

    return 0;
}