// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/namedb.h"
#include "pcap/pcap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void null_terminate_string(const uint8_t *data, size_t size, char *buffer, size_t buffer_size) {
    if (buffer_size == 0) return;
    size_t copy_len = (size < buffer_size - 1) ? size : buffer_size - 1;
    if (copy_len > 0) {
        memcpy(buffer, data, copy_len);
    }
    buffer[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    char str_buffer[256];
    null_terminate_string(Data, Size, str_buffer, sizeof(str_buffer));

    // Test pcap_nametoeproto
    (void)pcap_nametoeproto(str_buffer);

    // Test pcap_nametollc
    (void)pcap_nametollc(str_buffer);

    // Test pcap_nametoaddr (deprecated, not thread-safe)
    (void)pcap_nametoaddr(str_buffer);

    // Test pcap_nametonetaddr
    (void)pcap_nametonetaddr(str_buffer);

    // Test pcap_lookupnet
    bpf_u_int32 net, mask;
    char errbuf[PCAP_ERRBUF_SIZE];
    (void)pcap_lookupnet(str_buffer, &net, &mask, errbuf);

    // Test pcap_open_live
    int snaplen = 65535;
    int promisc = 0;
    int timeout_ms = 1000;
    
    if (Size >= 4) {
        // Use first 4 bytes to influence parameters
        snaplen = (Data[0] << 8) | Data[1];
        if (snaplen < 0) snaplen = 68;
        if (snaplen > 65535) snaplen = 65535;
        
        promisc = Data[2] & 1;
        timeout_ms = Data[3] * 10;
        if (timeout_ms < 0) timeout_ms = 1000;
    }
    
    char open_errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_live(str_buffer, snaplen, promisc, timeout_ms, open_errbuf);
    if (pcap) {
        pcap_close(pcap);
    }

    // Try with different combinations of parameters
    if (Size >= 2) {
        int alt_snaplen = Data[0] * 100;
        int alt_promisc = (Data[1] >> 1) & 1;
        int alt_timeout = Data[1] * 50;
        
        pcap_t *pcap2 = pcap_open_live(str_buffer, alt_snaplen, alt_promisc, alt_timeout, open_errbuf);
        if (pcap2) {
            pcap_close(pcap2);
        }
    }

    // Try with empty device name
    pcap_t *pcap3 = pcap_open_live("", snaplen, promisc, timeout_ms, open_errbuf);
    if (pcap3) {
        pcap_close(pcap3);
    }

    // Try with "any" device
    pcap_t *pcap4 = pcap_open_live("any", snaplen, promisc, timeout_ms, open_errbuf);
    if (pcap4) {
        pcap_close(pcap4);
    }

    return 0;
}