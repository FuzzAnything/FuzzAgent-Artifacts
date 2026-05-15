// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
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

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static pcap_t *create_dummy_pcap(int linktype, int precision) {
    return pcap_open_dead_with_tstamp_precision(linktype, 65535, precision);
}

static pcap_t *create_inactive_pcap(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_create("any", errbuf);
    return pcap;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    
    /* Test pcap_open_dead_with_tstamp_precision */
    int linktype = Data[0] % 256;
    int precision = Data[1] % 3; /* 0,1,2 where 2 is invalid */
    pcap_t *dead_pcap = pcap_open_dead_with_tstamp_precision(linktype, 65535, precision);
    if (dead_pcap) {
        /* Test pcap_get_tstamp_precision on dead handle */
        int prec = pcap_get_tstamp_precision(dead_pcap);
        (void)prec;
        
        /* Test pcap_sendpacket on dead handle (will fail but should not crash) */
        if (Size > 10) {
            const u_char *packet = Data + 2;
            int packet_len = Size - 2;
            if (packet_len > 0) {
                pcap_sendpacket(dead_pcap, packet, packet_len);
            }
        }
        pcap_close(dead_pcap);
    }

    /* Test with inactive pcap handle created via pcap_create */
    pcap_t *inactive_pcap = pcap_create("any", errbuf);
    if (inactive_pcap) {
        /* Test pcap_set_tstamp_type */
        int tstamp_type = Data[0] % 10; /* Various timestamp types */
        pcap_set_tstamp_type(inactive_pcap, tstamp_type);
        
        /* Test pcap_set_tstamp_precision */
        int set_precision = (Data[1] % 2) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
        pcap_set_tstamp_precision(inactive_pcap, set_precision);
        
        /* Test pcap_get_tstamp_precision on inactive handle */
        int current_prec = pcap_get_tstamp_precision(inactive_pcap);
        (void)current_prec;
        
        /* Test pcap_activate */
        pcap_activate(inactive_pcap);
        
        /* Try to set timestamp type/precision after activation (should fail gracefully) */
        pcap_set_tstamp_type(inactive_pcap, tstamp_type);
        pcap_set_tstamp_precision(inactive_pcap, set_precision);
        
        /* Test pcap_sendpacket if activation succeeded */
        if (Size > 20) {
            const u_char *packet2 = Data;
            int packet_len2 = Size > 65535 ? 65535 : Size;
            pcap_sendpacket(inactive_pcap, packet2, packet_len2);
        }
        
        pcap_close(inactive_pcap);
    }

    /* Test with another dead handle for different code paths */
    pcap_t *dead_pcap2 = create_dummy_pcap(DLT_EN10MB, PCAP_TSTAMP_PRECISION_MICRO);
    if (dead_pcap2) {
        /* Test pcap_set_tstamp_type on dead handle (should fail gracefully) */
        pcap_set_tstamp_type(dead_pcap2, PCAP_TSTAMP_HOST);
        
        /* Test pcap_set_tstamp_precision on dead handle (should fail gracefully) */
        pcap_set_tstamp_precision(dead_pcap2, PCAP_TSTAMP_PRECISION_NANO);
        
        /* Test pcap_activate on dead handle (should fail gracefully) */
        pcap_activate(dead_pcap2);
        
        pcap_close(dead_pcap2);
    }

    /* Test with invalid data for more coverage */
    if (Size > 50) {
        pcap_t *temp_pcap = create_inactive_pcap();
        if (temp_pcap) {
            /* Use fuzz data for timestamp type and precision */
            int weird_type = *(const int*)(Data + 10);
            int weird_prec = *(const int*)(Data + 20);
            pcap_set_tstamp_type(temp_pcap, weird_type);
            pcap_set_tstamp_precision(temp_pcap, weird_prec);
            pcap_activate(temp_pcap);
            pcap_close(temp_pcap);
        }
    }

    return 0;
}