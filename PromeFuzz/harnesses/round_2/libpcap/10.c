// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Do nothing - just a dummy callback for pcap_dispatch
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create dummy pcap file with fuzzer input
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Open the dummy file as a pcap capture
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    
    // If opening failed, try to create a dead pcap handle instead
    if (pcap == NULL) {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    if (pcap == NULL) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Prepare structures needed for API calls
    struct pcap_stat stats;
    struct bpf_program fp;
    char nonblock_errbuf[PCAP_ERRBUF_SIZE];
    
    // First pcap_geterr call (should be NULL initially)
    char *err_msg = pcap_geterr(pcap);
    
    // pcap_setnonblock - use fuzzer data to determine mode
    int nonblock_mode = (Size > 0) ? (Data[0] & 1) : 0;
    pcap_setnonblock(pcap, nonblock_mode, nonblock_errbuf);
    
    // pcap_dispatch - use fuzzer data for count
    int dispatch_count = (Size > 1) ? (Data[1] % 100) : 10;
    pcap_dispatch(pcap, dispatch_count, dummy_callback, NULL);
    
    // pcap_stats
    pcap_stats(pcap, &stats);
    
    // Two more pcap_geterr calls
    err_msg = pcap_geterr(pcap);
    err_msg = pcap_geterr(pcap);
    
    // Try to compile a dummy filter to test pcap_freecode
    memset(&fp, 0, sizeof(fp));
    if (pcap_compile(pcap, &fp, "ip", 0, 0) == 0) {
        // Filter compiled successfully
    }
    
    // Clean up
    pcap_close(pcap);
    pcap_freecode(&fp);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}

#ifdef __cplusplus
}
#endif