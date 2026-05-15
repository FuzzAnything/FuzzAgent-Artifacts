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
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Do nothing - this is just a dummy callback for pcap_dispatch
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for potential pcap_open_offline
    write_dummy_file(Data, Size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program prog = {0};
    struct pcap_stat stats = {0};
    
    // Try to open the dummy file as a pcap file first
    handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If that fails, create a dead capture handle
    if (handle == NULL) {
        handle = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    if (handle == NULL) {
        // Clean up dummy file
        unlink("./dummy_file");
        return 0;
    }
    
    // 1. pcap_geterr
    char *error_msg = pcap_geterr(handle);
    
    // 2. pcap_setnonblock
    int nonblock = (Size > 0) ? (Data[0] & 1) : 0;
    pcap_setnonblock(handle, nonblock, errbuf);
    
    // 3. pcap_dispatch
    int cnt = (Size > 4) ? (Data[1] << 8) | Data[2] : 10;
    pcap_dispatch(handle, cnt, dummy_callback, (u_char *)Data);
    
    // 4. pcap_stats
    pcap_stats(handle, &stats);
    
    // 5. pcap_geterr
    error_msg = pcap_geterr(handle);
    
    // 6. pcap_geterr (second call)
    error_msg = pcap_geterr(handle);
    
    // 7. pcap_close
    pcap_close(handle);
    
    // 8. pcap_freecode
    pcap_freecode(&prog);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}