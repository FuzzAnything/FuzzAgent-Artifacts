// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>

static void create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal pcap file header
    struct pcap_file_header header;
    header.magic = 0xa1b2c3d4;
    header.version_major = 2;
    header.version_minor = 4;
    header.thiszone = 0;
    header.sigfigs = 0;
    header.snaplen = 65535;
    header.linktype = 1; // Ethernet (LINKTYPE_ETHERNET)
    
    fwrite(&header, sizeof(header), 1, f);
    
    // Write a minimal packet header and data
    struct pcap_pkthdr pkthdr;
    pkthdr.ts.tv_sec = 0;
    pkthdr.ts.tv_usec = 0;
    pkthdr.caplen = 14;
    pkthdr.len = 14;
    
    fwrite(&pkthdr, sizeof(pkthdr), 1, f);
    
    u_char data[14] = {0};
    fwrite(data, 1, 14, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file for offline capture
    create_dummy_file("./dummy_file");

    // Open offline capture
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline("./dummy_file", errbuf);
    if (!handle) {
        return 0;
    }

    // Activate the handle (required for pcap_compile)
    if (pcap_activate(handle) != 0) {
        pcap_close(handle);
        return 0;
    }

    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    struct bpf_insn *insns = NULL;
    int len = 0;

    // Extract filter string from input if possible
    const char *filter_str = (const char *)Data;
    // Ensure null termination for safety
    char *safe_filter = (char *)malloc(Size + 1);
    if (!safe_filter) {
        pcap_close(handle);
        return 0;
    }
    memcpy(safe_filter, Data, Size);
    safe_filter[Size] = '\0';

    // 1. Test pcap_compile
    int compile_res = pcap_compile(handle, &fp, safe_filter, 1, 0);
    
    if (compile_res == 0) {
        insns = fp.bf_insns;
        len = fp.bf_len;

        // 2. Test bpf_dump with different options
        bpf_dump(&fp, 0); // Human-readable
        bpf_dump(&fp, 1); // C initializer syntax
        bpf_dump(&fp, 2); // Raw numeric fields
        bpf_dump(&fp, 3); // Raw numeric fields (higher)

        // 3. Test bpf_image
        if (insns && len > 0) {
            char *img = bpf_image(insns, len);
            (void)img; // Use the result to avoid warnings
        }

        // 4. Test bpf_validate
        if (insns && len > 0) {
            int val_res = bpf_validate(insns, len);
            (void)val_res;
        }

        // 5. Test pcap_offline_filter
        struct pcap_pkthdr pkthdr;
        u_char *pkt_data = NULL;
        
        // Read a packet to get valid header and data
        if (pcap_next_ex(handle, &pkthdr, &pkt_data) > 0) {
            int match = pcap_offline_filter(&fp, &pkthdr, pkt_data);
            (void)match;
        }
        
        // Test with NULL filter
        int match_null = pcap_offline_filter(NULL, &pkthdr, pkt_data);
        (void)match_null;
    }

    // 6. Test pcap_freecode
    pcap_freecode(&fp);

    // Cleanup
    pcap_close(handle);
    free(safe_filter);
    
    return 0;
}