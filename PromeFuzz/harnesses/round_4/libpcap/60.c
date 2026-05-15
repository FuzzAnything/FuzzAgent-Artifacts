// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "pcap/pcap.h"

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static pcap_t *create_dummy_pcap(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    return pcap_open_dead(DLT_EN10MB, 65535);
}

static pcap_t *open_live_from_input(const uint8_t *data, size_t size, char *errbuf) {
    if (size < 1) return NULL;
    
    size_t devname_len = data[0] % 32;
    if (devname_len > size - 1) devname_len = size - 1;
    
    char devname[32];
    if (devname_len > 0) {
        memcpy(devname, data + 1, devname_len);
        devname[devname_len] = '\0';
    } else {
        strcpy(devname, "any");
    }
    
    int snaplen = 65535;
    int promisc = (size > 2) ? (data[2] % 2) : 0;
    int timeout = (size > 6) ? *(const int *)(data + 3) % 1000 : 100;
    
    return pcap_open_live(devname, snaplen, promisc, timeout, errbuf);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    if (size < 1) return 0;
    
    pcap_t *pcap = NULL;
    int use_live = data[0] % 2;
    
    if (use_live) {
        pcap = open_live_from_input(data + 1, size - 1, errbuf);
    } else {
        pcap = create_dummy_pcap();
    }
    
    if (!pcap) {
        return 0;
    }
    
    int fd = pcap_fileno(pcap);
    if (fd >= 0) {
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags >= 0) {
            fcntl(fd, F_SETFL, flags);
        }
    }
    
    if (size > 4) {
        const u_char *packet_data = data + 4;
        size_t packet_size = size - 4;
        if (packet_size > 0) {
            int send_size = packet_size;
            if (send_size > 65535) send_size = 65535;
            
            pcap_sendpacket(pcap, packet_data, send_size);
            
            if (packet_size <= INT_MAX) {
                pcap_inject(pcap, packet_data, packet_size);
            }
        }
    }
    
    int nonblock_mode = (size > 1) ? (data[1] % 2) : 0;
    pcap_setnonblock(pcap, nonblock_mode, errbuf);
    
    const char *error_msg = pcap_geterr(pcap);
    if (error_msg && error_msg[0] != '\0') {
    }
    
    cleanup_pcap(pcap);
    return 0;
}