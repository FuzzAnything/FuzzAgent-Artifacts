// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
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
#include <time.h>

static void write_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal pcap header (24 bytes)
    // Magic number, version, timezone, sigfigs, snaplen, network type
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t timezone = 0;
    int32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // Ethernet
    
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&timezone, sizeof(timezone), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);
    
    // Write a dummy packet header (16 bytes)
    struct pcap_pkthdr hdr;
    hdr.ts.tv_sec = 0;
    hdr.ts.tv_usec = 0;
    hdr.caplen = 14;
    hdr.len = 14;
    fwrite(&hdr, sizeof(hdr), 1, f);
    
    // Write dummy packet data (14 bytes)
    uint8_t data[14] = {0};
    fwrite(data, sizeof(data), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Ensure dummy file exists for offline operations
    write_dummy_file();

    // Initialize pcap handle for offline reading
    pcap_t *handle = pcap_open_offline("./dummy_file", NULL);
    if (!handle) return 0;

    // Prepare structures
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    struct pcap_pkthdr pkthdr;
    memset(&pkthdr, 0, sizeof(pkthdr));
    pkthdr.caplen = 14;
    pkthdr.len = 14;
    pkthdr.ts.tv_sec = 1;
    pkthdr.ts.tv_usec = 0;
    
    // Create a buffer for packet data
    u_char pkt_data[1024];
    memset(pkt_data, 0, sizeof(pkt_data));
    if (Size > 0) {
        size_t copy_len = (Size < sizeof(pkt_data)) ? Size : sizeof(pkt_data);
        memcpy(pkt_data, Data, copy_len);
    }

    // Try to compile a filter string if we have enough data
    // We use the input data as a potential filter string, but ensure it's null-terminated
    char filter_str[256];
    size_t filter_len = (Size < sizeof(filter_str) - 1) ? Size : sizeof(filter_str) - 1;
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    int ret = pcap_compile(handle, &fp, filter_str, 0, 0);
    if (ret == 0) {
        // Compilation successful, test other functions
        
        // 1. bpf_validate
        if (fp.bf_insns && fp.bf_len > 0) {
            bpf_validate(fp.bf_insns, fp.bf_len);
        }

        // 2. bpf_image
        if (fp.bf_insns && fp.bf_len > 0) {
            char *img = bpf_image(fp.bf_insns, fp.bf_len);
            (void)img; // Use the result to avoid warnings
        }

        // 3. bpf_dump
        if (fp.bf_insns && fp.bf_len > 0) {
            bpf_dump(&fp, 1);
            bpf_dump(&fp, 2);
            bpf_dump(&fp, 3);
        }

        // 4. pcap_offline_filter
        if (fp.bf_insns && fp.bf_len > 0) {
            pcap_offline_filter(&fp, &pkthdr, pkt_data);
        }

        // 5. pcap_freecode
        pcap_freecode(&fp);
    }

    // Test bpf_validate with manually crafted instructions
    // Create a small array of instructions from input data
    if (Size >= 4) {
        struct bpf_insn manual_insns[10];
        int manual_len = 0;
        
        // Fill instructions based on input data
        for (int i = 0; i < 10 && i * 8 < Size; i++) {
            manual_insns[i].code = Data[i * 8] & 0xFF;
            manual_insns[i].jt = (i * 8 + 1 < Size) ? Data[i * 8 + 1] : 0;
            manual_insns[i].jf = (i * 8 + 2 < Size) ? Data[i * 8 + 2] : 0;
            manual_insns[i].k = (i * 8 + 3 < Size) ? Data[i * 8 + 3] : 0;
            manual_len++;
        }
        
        if (manual_len > 0) {
            bpf_validate(manual_insns, manual_len);
            bpf_image(manual_insns, manual_len);
        }
    }

    // Cleanup
    pcap_close(handle);
    return 0;
}