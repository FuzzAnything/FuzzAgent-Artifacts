// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void dummy_pcap_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy handler that does nothing
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to a dummy file for pcap_open_offline
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    // Try to open the dummy file as a pcap savefile
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline(filename, errbuf);
    
    // If that fails, create a dead pcap handle for testing
    if (pcap == NULL) {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap == NULL) {
            unlink(filename);
            return 0;
        }
    }

    // First pcap_geterr (may be NULL if no error yet)
    char *err = pcap_geterr(pcap);
    (void)err;

    // pcap_setnonblock
    char nonblock_errbuf[PCAP_ERRBUF_SIZE];
    int nonblock_mode = (Size > 0) ? (Data[0] & 1) : 0;
    int setnonblock_ret = pcap_setnonblock(pcap, nonblock_mode, nonblock_errbuf);
    (void)setnonblock_ret;

    // pcap_dispatch
    int dispatch_cnt = (Size > 1) ? (Data[1] & 0x7F) : 1;
    if (dispatch_cnt == 0) dispatch_cnt = 1;
    int dispatch_ret = pcap_dispatch(pcap, dispatch_cnt, dummy_pcap_handler, NULL);
    (void)dispatch_ret;

    // pcap_stats
    struct pcap_stat stats;
    int stats_ret = pcap_stats(pcap, &stats);
    (void)stats_ret;

    // Two more pcap_geterr calls
    err = pcap_geterr(pcap);
    (void)err;
    err = pcap_geterr(pcap);
    (void)err;

    // pcap_close
    pcap_close(pcap);
    pcap = NULL;

    // pcap_freecode - need a bpf_program to free
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // Try to compile a dummy filter to create a valid bpf_program
    pcap_t *dummy_pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (dummy_pcap != NULL) {
        // Use a small part of fuzz data as filter string if available
        const char *filter_str = (Size > 10) ? (const char*)Data : "ip";
        if (pcap_compile(dummy_pcap, &fp, filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
            pcap_freecode(&fp);
        }
        pcap_close(dummy_pcap);
    }

    // Clean up
    unlink(filename);
    return 0;
}