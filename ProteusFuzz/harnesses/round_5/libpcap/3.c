// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare environment
    pcap_t *handle = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    struct bpf_program bpf_prog;
    struct bpf_insn *insns = NULL;
    int num_insns;
    char *err_str = NULL;
    int dump_option;
    int validation_result;
    int next_ex_result;
    int offline_filter_result;

    // Create a dummy file for offline capture
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write minimal pcap header (24 bytes)
        unsigned char pcap_header[24] = {
            0xd4, 0xc3, 0xb2, 0xa1, // magic
            0x02, 0x00, 0x04, 0x00, // version major/minor
            0x00, 0x00, 0x00, 0x00, // timezone
            0x00, 0x00, 0x00, 0x00, // sigfigs
            0x00, 0x00, 0x00, 0x00, // snaplen
            0x01, 0x00, 0x00, 0x00  // linktype (Ethernet)
        };
        fwrite(pcap_header, 1, 24, fp);
        // Write dummy packet header (16 bytes)
        unsigned char pkt_hdr[16] = {0};
        pkt_hdr[0] = 0x01; // ts_sec
        pkt_hdr[8] = 0x00; // caplen (4 bytes)
        pkt_hdr[12] = 0x00; // len (4 bytes)
        fwrite(pkt_hdr, 1, 16, fp);
        // Write dummy packet data
        if (Size > 24) {
            fwrite((const char *)Data, 1, Size - 24, fp);
        }
        fclose(fp);
    }

    // Attempt to open the dummy file for offline capture
    handle = pcap_open_offline("./dummy_file", NULL);
    if (handle == NULL) {
        remove("./dummy_file");
        return 0;
    }

    // 1. pcap_geterr
    err_str = pcap_geterr(handle);
    (void)err_str;

    // Prepare BPF instructions for validation and dumping
    if (Size >= 12) {
        num_insns = (Data[0] % 10) + 1; // Limit to 1-10 instructions
        insns = (struct bpf_insn *)malloc(num_insns * sizeof(struct bpf_insn));
        if (insns) {
            for (int i = 0; i < num_insns; i++) {
                // Construct instruction from fuzz data
                insns[i].code = (u_short)(Data[1 + i*4] | (Data[2 + i*4] << 8));
                insns[i].jt = Data[3 + i*4];
                insns[i].jf = Data[4 + i*4];
                insns[i].k = (bpf_u_int32)(Data[5 + i*4] | (Data[6 + i*4] << 8) | 
                                          (Data[7 + i*4] << 16) | (Data[8 + i*4] << 24));
            }
            bpf_prog.bf_len = num_insns;
            bpf_prog.bf_insns = insns;

            // 2. bpf_validate
            validation_result = bpf_validate(bpf_prog.bf_insns, bpf_prog.bf_len);
            (void)validation_result;

            // 3. bpf_dump
            dump_option = Data[Size - 1] % 3 + 1; // 1, 2, or 3
            bpf_dump(&bpf_prog, dump_option);

            // 4. pcap_next_ex
            next_ex_result = pcap_next_ex(handle, &header, &packet);
            (void)next_ex_result;

            // 5. pcap_geterr
            err_str = pcap_geterr(handle);
            (void)err_str;

            // 6. pcap_offline_filter
            // Prepare a dummy header if not set by pcap_next_ex
            if (header == NULL) {
                static struct pcap_pkthdr dummy_header;
                memset(&dummy_header, 0, sizeof(dummy_header));
                header = &dummy_header;
            }
            // Use packet data if available, otherwise NULL
            const u_char *pkt_data = (packet != NULL) ? packet : (const u_char *)Data;
            offline_filter_result = pcap_offline_filter(&bpf_prog, header, pkt_data);
            (void)offline_filter_result;

            free(insns);
        }
    }

    // Cleanup
    if (handle) {
        pcap_close(handle);
    }
    remove("./dummy_file");

    return 0;
}