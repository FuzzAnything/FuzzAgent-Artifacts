// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "pcap/pcap.h"
#include "pcap/bpf.h"

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Empty callback for pcap_loop
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Create dummy pcap handle using pcap_open_dead
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return 0;
    }

    // Initialize bpf_program structures
    struct bpf_program fp1, fp2;
    memset(&fp1, 0, sizeof(fp1));
    memset(&fp2, 0, sizeof(fp2));

    // Prepare filter string from fuzzer input
    char *filter_str = NULL;
    if (Size > 0) {
        filter_str = (char *)malloc(Size + 1);
        if (filter_str) {
            memcpy(filter_str, Data, Size);
            filter_str[Size] = '\0';
        }
    }

    // 1. Test pcap_compile with dummy pcap handle
    if (filter_str) {
        int optimize = Size % 2;
        bpf_u_int32 netmask = Size;
        pcap_compile(pcap, &fp1, filter_str, optimize, netmask);
    }

    // 2. Test pcap_setfilter with compiled program
    if (fp1.bf_insns != NULL) {
        pcap_setfilter(pcap, &fp1);
    }

    // 3. Test pcap_compile_nopcap (deprecated but still testable)
    if (filter_str) {
        pcap_compile_nopcap(65535, DLT_EN10MB, &fp2, filter_str, 1, 0);
    }

    // 4. Test bpf_validate with compiled BPF instructions
    if (fp1.bf_insns != NULL && fp1.bf_len > 0) {
        bpf_validate(fp1.bf_insns, fp1.bf_len);
    }
    if (fp2.bf_insns != NULL && fp2.bf_len > 0) {
        bpf_validate(fp2.bf_insns, fp2.bf_len);
    }

    // 5. Test bpf_filter with packet data
    if (fp1.bf_insns != NULL && Size > 0) {
        // Use first part of input as packet data
        size_t packet_len = Size > 100 ? 100 : Size;
        u_int data_available = (u_int)packet_len;
        bpf_filter(fp1.bf_insns, Data, (u_int)packet_len, data_available);
    }

    // 6. Test bpf_dump for debugging
    if (fp1.bf_insns != NULL) {
        bpf_dump(&fp1, 1);
    }
    if (fp2.bf_insns != NULL) {
        bpf_dump(&fp2, 2);
    }

    // Create dummy packet header for testing
    struct pcap_pkthdr pkthdr;
    pkthdr.caplen = Size > 0 ? (Size > 65535 ? 65535 : (bpf_u_int32)Size) : 0;
    pkthdr.len = pkthdr.caplen;
    gettimeofday(&pkthdr.ts, NULL);

    // Test pcap_loop with dummy callback
    if (pcap != NULL) {
        // Write dummy data to file for offline reading
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            // Write pcap file header (simplified)
            struct pcap_file_header fh;
            fh.magic = 0xa1b2c3d4;
            fh.version_major = 2;
            fh.version_minor = 4;
            fh.thiszone = 0;
            fh.sigfigs = 0;
            fh.snaplen = 65535;
            fh.linktype = DLT_EN10MB;
            fwrite(&fh, sizeof(fh), 1, f);
            
            // Write packet header
            fwrite(&pkthdr, sizeof(pkthdr), 1, f);
            
            // Write packet data if available
            if (Size > 0) {
                size_t write_size = Size > 65535 ? 65535 : Size;
                fwrite(Data, 1, write_size, f);
            }
            fclose(f);
            
            // Try to open the dummy file
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_t *offline_pcap = pcap_open_offline("./dummy_file", errbuf);
            if (offline_pcap) {
                // Test pcap_loop on offline capture
                pcap_loop(offline_pcap, 1, dummy_callback, NULL);
                pcap_close(offline_pcap);
            }
            remove("./dummy_file");
        }
    }

    // Cleanup
    if (fp1.bf_insns != NULL) {
        pcap_freecode(&fp1);
    }
    if (fp2.bf_insns != NULL) {
        pcap_freecode(&fp2);
    }
    if (filter_str) {
        free(filter_str);
    }
    if (pcap) {
        pcap_close(pcap);
    }

    return 0;
}