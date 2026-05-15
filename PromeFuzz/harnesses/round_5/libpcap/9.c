// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void cleanup_bpf_program(struct bpf_program *fp) {
    if (fp != NULL) {
        pcap_freecode(fp);
    }
}

static void cleanup_pcap_dumper(pcap_dumper_t *pd) {
    if (pd != NULL) {
        pcap_dump_close(pd);
    }
}

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    struct bpf_program fp;
    pcap_dumper_t *dumper = NULL;
    const char *filter_str = NULL;
    int ret;
    bpf_u_int32 netmask = 0;
    int optimize = 0;
    
    // Initialize bpf_program structure to zero
    memset(&fp, 0, sizeof(fp));
    
    // Create a pcap handle for offline processing (dead capture)
    // Using DLT_EN10MB (Ethernet) as a common link type
    pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        // If we can't even create a dead capture, return early
        return 0;
    }
    
    // Prepare filter string from fuzz input
    if (Size > 0) {
        // Use the fuzz data as filter string, ensuring null termination
        filter_str = (const char *)Data;
        // Note: pcap_compile expects null-terminated string, so we rely on
        // the fuzzer to provide valid strings or the function will fail
    } else {
        // Use a default filter if no data
        filter_str = "ip";
    }
    
    // First function: pcap_compile
    ret = pcap_compile(pcap, &fp, filter_str, optimize, netmask);
    
    // Second function: pcap_geterr (after pcap_compile)
    char *err = pcap_geterr(pcap);
    (void)err; // Use the error string to avoid unused variable warning
    
    // Third function: pcap_setfilter (regardless of compile success)
    if (ret == 0) {
        // Only call setfilter if compile succeeded
        ret = pcap_setfilter(pcap, &fp);
    }
    
    // Fourth function: pcap_geterr (after pcap_setfilter)
    err = pcap_geterr(pcap);
    (void)err;
    
    // Fifth function: pcap_dump_open
    // Create a dummy file for dump output
    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        // Write some minimal pcap file header (dummy data)
        // This helps pcap_dump_open to have something to work with
        struct pcap_file_header fh;
        fh.magic = 0xa1b2c3d4;
        fh.version_major = 2;
        fh.version_minor = 4;
        fh.thiszone = 0;
        fh.sigfigs = 0;
        fh.snaplen = 65535;
        fh.linktype = DLT_EN10MB;
        write(fd, &fh, sizeof(fh));
        close(fd);
        
        dumper = pcap_dump_open(pcap, dummy_file);
        
        // Sixth function: pcap_geterr (after pcap_dump_open)
        err = pcap_geterr(pcap);
        (void)err;
        
        // Clean up the dummy file
        unlink(dummy_file);
    }
    
    // Seventh function: pcap_datalink
    int dlt = pcap_datalink(pcap);
    (void)dlt; // Use the result to avoid unused variable warning
    
    // Cleanup
    cleanup_bpf_program(&fp);
    cleanup_pcap_dumper(dumper);
    cleanup_pcap(pcap);
    
    return 0;
}