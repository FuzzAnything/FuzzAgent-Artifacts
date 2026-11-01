// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>

static void create_dummy_file(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    
    // Write a minimal pcap global header (24 bytes)
    unsigned char header[24];
    // Magic number (little endian)
    header[0] = 0xa1; header[1] = 0xb2; header[2] = 0xc3; header[3] = 0xd4;
    // Version major/minor
    header[4] = 2; header[5] = 0; header[6] = 4; header[7] = 0;
    // Thiszone, sigfigs, snaplen, network
    memset(&header[8], 0, 16);
    header[16] = 65535; // snaplen (little endian)
    header[20] = 1;     // linktype (Ethernet)
    
    fwrite(header, 1, 24, fp);
    
    // Write a minimal packet header (16 bytes)
    unsigned char pkt_header[16];
    memset(pkt_header, 0, 16);
    // caplen and len
    pkt_header[12] = 10; pkt_header[13] = 0; pkt_header[14] = 0; pkt_header[15] = 0;
    fwrite(pkt_header, 1, 16, fp);
    
    // Write dummy packet data (10 bytes)
    unsigned char data[10];
    memset(data, 0, 10);
    fwrite(data, 1, 10, fp);
    
    fclose(fp);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create dummy file for offline capture
    create_dummy_file("./dummy_file");

    // Open the dummy file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If handle is NULL, we cannot proceed with pcap functions that require a valid handle
    if (handle == NULL) {
        remove("./dummy_file");
        return 0;
    }

    // First call: pcap_geterr
    char *err = pcap_geterr(handle);
    (void)err; // Suppress unused variable warning

    // Prepare BPF instructions for fuzzing
    struct bpf_insn *insns = NULL;
    int len = 0;
    
    if (Size >= 8) {
        len = (Size / 8) + 1; 
        if (len < 2) len = 2;
        
        insns = (struct bpf_insn *)malloc(len * sizeof(struct bpf_insn));
        if (!insns) {
            pcap_close(handle);
            remove("./dummy_file");
            return 0;
        }

        // Fill instructions with fuzz data
        for (int i = 0; i < len; i++) {
            int idx = i * 8;
            if (idx + 8 <= Size) {
                insns[i].code = *(const u_short *)(Data + idx);
                insns[i].jt = Data[idx + 2];
                insns[i].jf = Data[idx + 3];
                insns[i].k = *(const bpf_u_int32 *)(Data + idx + 4);
            } else {
                insns[i].code = 0;
                insns[i].jt = 0;
                insns[i].jf = 0;
                insns[i].k = 0;
            }
        }
    }

    struct bpf_program prog;
    prog.bf_len = len;
    prog.bf_insns = insns;

    // Second call: bpf_validate
    if (insns) {
        int ret = bpf_validate(insns, len);
        (void)ret;
    }

    // Third call: bpf_dump
    int dump_opt = 0;
    if (Size > 0) dump_opt = Data[0] % 3; 
    if (insns) {
        bpf_dump(&prog, dump_opt);
    }

    // Fourth call: pcap_next_ex
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret = pcap_next_ex(handle, &header, &packet);
    (void)ret;

    // Fifth call: pcap_geterr
    err = pcap_geterr(handle);
    (void)err;

    // Sixth call: pcap_offline_filter
    if (header && packet) {
        int match = pcap_offline_filter(&prog, header, packet);
        (void)match;
    } else {
        // Construct a minimal dummy header if pcap_next_ex didn't return valid data
        struct pcap_pkthdr dummy_header;
        memset(&dummy_header, 0, sizeof(dummy_header));
        dummy_header.caplen = 0;
        dummy_header.len = 0;
        
        const u_char *dummy_data = (Size > 0) ? (const u_char *)Data : NULL;
        
        int match = pcap_offline_filter(&prog, &dummy_header, dummy_data);
        (void)match;
    }

    // Cleanup
    if (insns) {
        free(insns);
    }
    if (handle) {
        pcap_close(handle);
    }
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}