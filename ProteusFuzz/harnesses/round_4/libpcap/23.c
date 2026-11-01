// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal PCAP header to make pcap_open_offline work
        // Magic number (little endian)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet
        
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        fwrite(&thiszone, sizeof(thiszone), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);
        
        // Write at least one packet if data is available
        if (size > 0) {
            struct pcap_pkthdr hdr;
            struct timeval tv = {0, 0};
            hdr.ts = tv;
            hdr.caplen = size;
            hdr.len = size;
            
            fwrite(&hdr, sizeof(hdr), 1, f);
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create a dummy pcap file for offline handling
    create_dummy_file(Data, Size);

    pcap_t *handle = pcap_open_offline("./dummy_file", NULL);
    if (!handle) {
        return 0;
    }

    // Activate the handle to allow compilation
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_activate(handle) < 0) {
        pcap_close(handle);
        return 0;
    }

    struct bpf_program fcode;
    memset(&fcode, 0, sizeof(fcode));

    // Fuzz pcap_compile with various filter strings derived from input
    // We ensure the string is null-terminated
    char filter_str[256];
    size_t filter_len = (Size > 255) ? 255 : Size;
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    // Try compiling with different masks and optimize flags
    bpf_u_int32 netmask = 0;
    if (Size >= 4) {
        netmask = *(const bpf_u_int32 *)(Data + filter_len);
    }

    int optimize = (Size > filter_len + 4) ? (Data[filter_len + 4] % 2) : 0;

    if (pcap_compile(handle, &fcode, filter_str, optimize, netmask) == 0) {
        // If compilation succeeds, we can test other functions
        
        // 1. bpf_validate
        if (fcode.bf_insns && fcode.bf_len > 0) {
            bpf_validate(fcode.bf_insns, fcode.bf_len);
        }

        // 2. pcap_offline_filter
        // Prepare a dummy packet header and data
        struct pcap_pkthdr pkt_hdr;
        pkt_hdr.ts.tv_sec = 0;
        pkt_hdr.ts.tv_usec = 0;
        pkt_hdr.caplen = (Size > 10) ? Size - 10 : 10;
        pkt_hdr.len = pkt_hdr.caplen;
        const u_char *pkt_data = (Size > 10) ? Data + 10 : Data;
        
        if (pkt_hdr.caplen > 0) {
            pcap_offline_filter(&fcode, &pkt_hdr, pkt_data);
        }

        // 3. bpf_image
        if (fcode.bf_insns && fcode.bf_len > 0) {
            char *img = bpf_image(fcode.bf_insns, fcode.bf_len);
            if (img) {
                // Just access to ensure no crash, ignore content
                (void)img;
            }
        }

        // 4. bpf_dump with different options
        if (fcode.bf_insns && fcode.bf_len > 0) {
            bpf_dump(&fcode, 0);
            bpf_dump(&fcode, 1);
            bpf_dump(&fcode, 2);
            bpf_dump(&fcode, 3);
        }

        // 5. pcap_freecode
        pcap_freecode(&fcode);
    }

    // Cleanup
    pcap_freecode(&fcode); // Safe to call again if already freed
    pcap_close(handle);
    
    return 0;
}