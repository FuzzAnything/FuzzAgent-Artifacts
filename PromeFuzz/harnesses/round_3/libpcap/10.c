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
// pcap_freecode at gencode.c:1290:1 in pcap.h
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
    /* Empty callback for pcap_dispatch */
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    pcap_t *pcap_handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    struct pcap_stat stats;
    struct bpf_program bpf_prog = {0};
    int nonblock_mode;
    int dispatch_ret;
    int stats_ret;
    const char *err_str;
    
    /* Create a dummy file with fuzz data for pcap_open_offline */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    /* Try to open the dummy file as a pcap capture */
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    /* If file open failed, create a dead capture handle instead */
    if (pcap_handle == NULL) {
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap_handle == NULL) {
            /* Clean up dummy file and return if we can't create any handle */
            unlink("./dummy_file");
            return 0;
        }
    }
    
    /* 1. First pcap_geterr call (may be NULL if no error yet) */
    err_str = pcap_geterr(pcap_handle);
    (void)err_str;
    
    /* 2. pcap_setnonblock - try both blocking and non-blocking modes */
    nonblock_mode = (Size > 0) ? (Data[0] & 1) : 0;
    if (pcap_setnonblock(pcap_handle, nonblock_mode, errbuf) == -1) {
        /* Error case - nonblock may fail if handle not activated */
        (void)errbuf[0];
    }
    
    /* 3. pcap_dispatch - try different count values */
    int dispatch_cnt = (Size > 1) ? (Data[1] % 100) : 10;
    dispatch_ret = pcap_dispatch(pcap_handle, dispatch_cnt, dummy_pcap_handler, NULL);
    (void)dispatch_ret;
    
    /* 4. pcap_stats */
    stats_ret = pcap_stats(pcap_handle, &stats);
    (void)stats_ret;
    
    /* 5. Second pcap_geterr call */
    err_str = pcap_geterr(pcap_handle);
    (void)err_str;
    
    /* 6. Third pcap_geterr call */
    err_str = pcap_geterr(pcap_handle);
    (void)err_str;
    
    /* 7. pcap_close */
    pcap_close(pcap_handle);
    
    /* 8. pcap_freecode - free any potential BPF program */
    pcap_freecode(&bpf_prog);
    
    /* Clean up dummy file */
    unlink("./dummy_file");
    
    return 0;
}