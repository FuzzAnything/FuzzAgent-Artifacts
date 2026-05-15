// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "pcap/pcap.h"

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Empty callback for packet processing
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Early return if input is too small
    if (Size < 1) return 0;

    // Create dummy file for potential offline capture
    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    // Initialize error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Try to open live capture with fuzzer data as device name
    // Limit device name length to prevent buffer overflows
    size_t dev_name_len = Size < 100 ? Size : 100;
    char *dev_name = (char *)malloc(dev_name_len + 1);
    if (!dev_name) return 0;
    memcpy(dev_name, Data, dev_name_len);
    dev_name[dev_name_len] = '\0';

    // Extract parameters from fuzzer data
    int snap_len = 65535;
    int promisc = (Size > 0) ? (Data[0] & 1) : 0;
    int timeout = 1000;
    
    // Adjust parameters based on fuzzer input
    if (Size > 1) snap_len = (Data[1] << 8) | Data[0];
    if (Size > 2) promisc = Data[2] & 1;
    if (Size > 6) timeout = (Data[6] << 24) | (Data[5] << 16) | (Data[4] << 8) | Data[3];
    
    // Ensure sane values
    if (snap_len < 0) snap_len = 0;
    if (snap_len > 65535) snap_len = 65535;
    if (timeout < 0) timeout = 0;
    if (timeout > 10000) timeout = 10000;

    // Try to open live capture
    pcap_t *handle = pcap_open_live(dev_name, snap_len, promisc, timeout, errbuf);
    free(dev_name);

    if (handle != NULL) {
        // Test pcap_setnonblock with fuzzer-derived value
        int nonblock = (Size > 7) ? (Data[7] & 1) : 0;
        int setnonblock_result = pcap_setnonblock(handle, nonblock, errbuf);
        
        // Test pcap_fileno
        int fileno_result = pcap_fileno(handle);
        
        // Test pcap_sendpacket if we have packet data
        if (Size > 8) {
            size_t packet_len = Size - 8;
            if (packet_len > 0 && packet_len <= INT_MAX) {
                const u_char *packet_data = Data + 8;
                pcap_sendpacket(handle, packet_data, (int)packet_len);
            }
        }

        // Test pcap_inject if we have packet data
        if (Size > 16) {
            size_t inject_len = Size - 16;
            if (inject_len > 0 && inject_len <= INT_MAX) {
                const void *inject_data = Data + 16;
                pcap_inject(handle, inject_data, inject_len);
            }
        }

        // Test pcap_geterr - call after operations that might fail
        char *error_msg = pcap_geterr(handle);
        
        // Try to process some packets if in non-blocking mode
        if (setnonblock_result == 0) {
            int packet_count = pcap_dispatch(handle, 1, dummy_callback, NULL);
        }

        // Cleanup
        pcap_close(handle);
    } else {
        // Test pcap_open_live failure path - try with different parameters
        char *alt_dev_name = "lo"; // Try loopback as alternative
        pcap_t *alt_handle = pcap_open_live(alt_dev_name, 96, 0, 100, errbuf);
        if (alt_handle != NULL) {
            // Test the other APIs with this handle
            pcap_setnonblock(alt_handle, 1, errbuf);
            pcap_fileno(alt_handle);
            
            if (Size > 0) {
                int send_len = Size > INT_MAX ? INT_MAX : (int)Size;
                pcap_sendpacket(alt_handle, Data, send_len);
                pcap_inject(alt_handle, Data, Size);
            }
            
            pcap_geterr(alt_handle);
            pcap_close(alt_handle);
        }
    }

    // Clean up dummy file
    unlink(dummy_file);
    
    return 0;
}