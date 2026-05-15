// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "pcap/pcap.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Prepare dummy file for any file-based operations
    write_dummy_file(Data, Size);
    
    // Extract parameters from fuzz input
    size_t offset = 0;
    
    // Device name (string) - use first 64 bytes or until null byte
    char device[65] = {0};
    size_t device_len = Size < 64 ? Size : 64;
    memcpy(device, Data, device_len);
    device[device_len] = '\0';
    offset += device_len;
    
    // Extract integer parameters
    int snaplen = 65535;
    int promisc = 0;
    int timeout_ms = 1000;
    
    if (Size - offset >= 12) {
        memcpy(&snaplen, Data + offset, 4);
        memcpy(&promisc, Data + offset + 4, 4);
        memcpy(&timeout_ms, Data + offset + 8, 4);
        offset += 12;
    }
    
    // Ensure reasonable values
    if (snaplen < 0) snaplen = 65535;
    if (timeout_ms < 0) timeout_ms = 1000;
    promisc = promisc & 1; // Only 0 or 1
    
    // Open pcap handle
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout_ms, errbuf);
    
    if (pcap != NULL) {
        // Test pcap_geterr even if no error
        char *err = pcap_geterr(pcap);
        (void)err; // Suppress unused warning
        
        // Test pcap_fileno
        int fd = pcap_fileno(pcap);
        (void)fd;
        
        // Test pcap_setnonblock
        int nonblock = (Size > 0) ? (Data[0] & 1) : 0;
        char nonblock_errbuf[PCAP_ERRBUF_SIZE];
        pcap_setnonblock(pcap, nonblock, nonblock_errbuf);
        
        // Test pcap_inject if we have data
        if (Size - offset > 0) {
            size_t inject_size = Size - offset;
            if (inject_size > 0 && inject_size <= INT_MAX) {
                const void *packet = Data + offset;
                pcap_inject(pcap, packet, inject_size);
            }
        }
        
        // Test pcap_sendpacket if we have data
        if (Size - offset > 0) {
            int send_size = (int)(Size - offset);
            if (send_size > 0) {
                const u_char *packet = Data + offset;
                pcap_sendpacket(pcap, packet, send_size);
            }
        }
        
        // Clean up
        pcap_close(pcap);
    }
    
    // Test with different parameter combinations
    if (Size > 20) {
        // Try with different snapshot lengths
        int alt_snaplen = *(int*)(Data + Size - 4) & 0xFFFF;
        if (alt_snaplen < 0) alt_snaplen = 1514;
        
        pcap_t *pcap2 = pcap_open_live("any", alt_snaplen, 1, 0, errbuf);
        if (pcap2) {
            pcap_close(pcap2);
        }
    }
    
    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}