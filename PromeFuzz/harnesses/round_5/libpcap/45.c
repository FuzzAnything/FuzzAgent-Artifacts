// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
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

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_open_offline("./dummy_file", errbuf);

    if (pcap_handle != NULL) {
        int swapped = pcap_is_swapped(pcap_handle);
        (void)swapped;

        int major_version = pcap_major_version(pcap_handle);
        (void)major_version;

        struct pcap_stat stats;
        int stats_result = pcap_stats(pcap_handle, &stats);
        (void)stats_result;

        struct pcap_pkthdr *pkt_header = NULL;
        const u_char *pkt_data = NULL;
        int next_result;
        while ((next_result = pcap_next_ex(pcap_handle, &pkt_header, &pkt_data)) == 1) {
            (void)pkt_header;
            (void)pkt_data;
        }
        (void)next_result;

        int nonblock_result = pcap_setnonblock(pcap_handle, 1, errbuf);
        (void)nonblock_result;

        nonblock_result = pcap_setnonblock(pcap_handle, 0, errbuf);
        (void)nonblock_result;

        pcap_close(pcap_handle);
    }

    pcap_t *dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (dead_handle != NULL) {
        int swapped = pcap_is_swapped(dead_handle);
        (void)swapped;

        int major_version = pcap_major_version(dead_handle);
        (void)major_version;

        struct pcap_stat stats;
        int stats_result = pcap_stats(dead_handle, &stats);
        (void)stats_result;

        struct pcap_pkthdr *pkt_header = NULL;
        const u_char *pkt_data = NULL;
        int next_result = pcap_next_ex(dead_handle, &pkt_header, &pkt_data);
        (void)next_result;

        int nonblock_result = pcap_setnonblock(dead_handle, 1, errbuf);
        (void)nonblock_result;

        pcap_close(dead_handle);
    }

    cleanup_dummy_file();
    return 0;
}