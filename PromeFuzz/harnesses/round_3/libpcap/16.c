// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy packet handler - does nothing
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a dummy pcap file with the fuzzer input
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write a minimal pcap file header (global header)
    struct pcap_file_header pfh;
    pfh.magic = 0xa1b2c3d4;  // TCPDUMP_MAGIC
    pfh.version_major = 2;
    pfh.version_minor = 4;
    pfh.thiszone = 0;
    pfh.sigfigs = 0;
    pfh.snaplen = 65535;
    pfh.linktype = 1;  // DLT_EN10MB
    
    write(fd, &pfh, sizeof(pfh));
    
    // Write packet header and data if we have enough input
    if (Size >= sizeof(struct pcap_pkthdr)) {
        struct pcap_pkthdr pkthdr;
        memcpy(&pkthdr, Data, sizeof(pkthdr));
        pkthdr.caplen = pkthdr.len = Size - sizeof(pkthdr);
        if (pkthdr.caplen > 65535) pkthdr.caplen = 65535;
        if (pkthdr.len > 65535) pkthdr.len = 65535;
        
        write(fd, &pkthdr, sizeof(pkthdr));
        write(fd, Data + sizeof(pkthdr), pkthdr.caplen);
    }
    close(fd);
    
    // Try to open the dummy file for offline capture
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline(filename, errbuf);
    
    if (pcap == NULL) {
        // If file open fails, try creating a dead capture handle
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap == NULL) {
            unlink(filename);
            return 0;
        }
    }
    
    // Extract parameters from fuzzer input
    int datalink_type = 0;
    int rfmon_flag = 0;
    int snaplen_val = 0;
    int protocol_val = 0;
    
    if (Size >= 4) {
        datalink_type = Data[0] | (Data[1] << 8);
        rfmon_flag = Data[2] & 1;
        snaplen_val = Data[3] | (Data[3] << 8);
        protocol_val = Data[0];
    }
    
    // Try pcap_can_set_rfmon first (works on inactive handles)
    (void)pcap_can_set_rfmon(pcap);
    
    // Try pcap_set_datalink
    (void)pcap_set_datalink(pcap, datalink_type);
    
    // Try pcap_set_rfmon
    (void)pcap_set_rfmon(pcap, rfmon_flag);
    
    // Try pcap_set_snaplen
    (void)pcap_set_snaplen(pcap, snaplen_val);
    
    // Try pcap_set_protocol_linux (Linux-specific)
    (void)pcap_set_protocol_linux(pcap, protocol_val);
    
    // Try pcap_activate (may fail if handle is already active)
    (void)pcap_activate(pcap);
    
    // Try to compile a dummy filter
    struct bpf_program fp;
    const char *filter_str = "ip";
    if (pcap_compile(pcap, &fp, filter_str, 0, 0) == 0) {
        (void)pcap_setfilter(pcap, &fp);
        pcap_freecode(&fp);
    }
    
    // Try to loop through packets (if we opened from file)
    if (pcap_datalink(pcap) != -1) {
        (void)pcap_loop(pcap, 1, dummy_handler, NULL);
    }
    
    // Clean up
    pcap_close(pcap);
    unlink(filename);
    
    return 0;
}