// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
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
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create dummy file for potential use
    write_dummy_file(Data, Size);

    // Initialize pcap handle for dead capture
    int linktype = DLT_EN10MB;
    int snaplen = 65535;
    pcap_t *pcap = pcap_open_dead(linktype, snaplen);
    if (!pcap) {
        cleanup_dummy_file();
        return 0;
    }

    // Initialize BPF program structures
    struct bpf_program bpf_prog1, bpf_prog2;
    memset(&bpf_prog1, 0, sizeof(bpf_prog1));
    memset(&bpf_prog2, 0, sizeof(bpf_prog2));

    // Use first byte to determine optimization flag
    int optimize = Data[0] & 1;
    bpf_u_int32 netmask = 0xffffff00;

    // Prepare filter string (null-terminated)
    char *filter_str = (char *)malloc(Size + 1);
    if (!filter_str) {
        pcap_close(pcap);
        cleanup_dummy_file();
        return 0;
    }
    memcpy(filter_str, Data, Size);
    filter_str[Size] = '\0';

    // Fuzz pcap_compile
    if (pcap_compile(pcap, &bpf_prog1, filter_str, optimize, netmask) == 0) {
        // Fuzz pcap_setfilter (requires activated handle)
        pcap_setfilter(pcap, &bpf_prog1);
        
        // Fuzz bpf_validate
        if (bpf_prog1.bf_insns && bpf_prog1.bf_len > 0) {
            bpf_validate(bpf_prog1.bf_insns, bpf_prog1.bf_len);
        }
        
        // Fuzz bpf_dump (output may be ignored)
        bpf_dump(&bpf_prog1, 0);
        bpf_dump(&bpf_prog1, 1);
        
        // Fuzz bpf_filter with sample packet data
        if (bpf_prog1.bf_insns && bpf_prog1.bf_len > 0) {
            // Create minimal Ethernet-like packet (14 bytes header + payload)
            u_char packet[64];
            memset(packet, 0xAA, sizeof(packet));
            if (Size > 14) {
                memcpy(packet + 14, Data, Size > 50 ? 50 : Size);
            }
            bpf_filter(bpf_prog1.bf_insns, packet, sizeof(packet), sizeof(packet));
        }
        
        pcap_freecode(&bpf_prog1);
    }

    // Fuzz pcap_compile_nopcap with various link types
    int linktypes[] = {DLT_EN10MB, DLT_RAW, DLT_LOOP, DLT_NULL};
    int num_linktypes = sizeof(linktypes) / sizeof(linktypes[0]);
    
    for (int i = 0; i < num_linktypes; i++) {
        if (pcap_compile_nopcap(linktypes[i], snaplen, &bpf_prog2, 
                               filter_str, optimize, netmask) == 0) {
            // Validate the compiled program
            if (bpf_prog2.bf_insns && bpf_prog2.bf_len > 0) {
                bpf_validate(bpf_prog2.bf_insns, bpf_prog2.bf_len);
            }
            pcap_freecode(&bpf_prog2);
        }
    }

    // Cleanup
    free(filter_str);
    pcap_close(pcap);
    cleanup_dummy_file();
    
    return 0;
}