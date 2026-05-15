// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static void test_pcap_handle(pcap_t *pcap, const uint8_t *data, size_t size) {
    if (!pcap) return;

    // Use data to influence decisions
    int choice = 0;
    if (size > 0) choice = data[0] % 6;
    
    // Test pcap_set_tstamp_precision before activation
    if (choice == 0) {
        int prec = (size > 1) ? (data[1] % 2 ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO) : PCAP_TSTAMP_PRECISION_MICRO;
        pcap_set_tstamp_precision(pcap, prec);
    }
    
    // Test pcap_set_snaplen before activation
    if (choice == 1) {
        int snaplen = (size > 2) ? (data[2] * 256 + data[3]) : 65535;
        pcap_set_snaplen(pcap, snaplen);
    }
    
    // Test pcap_set_tstamp_type before activation
    if (choice == 2) {
        int tstamp_type = (size > 4) ? (data[4] % 8) : PCAP_TSTAMP_HOST;
        pcap_set_tstamp_type(pcap, tstamp_type);
    }
    
    // Try to activate
    int activate_ret = pcap_activate(pcap);
    
    // Test pcap_snapshot after potential activation
    if (activate_ret >= 0 || choice == 3) {
        pcap_snapshot(pcap);
    }
    
    // Test pcap_get_tstamp_precision regardless of activation state
    if (choice == 4) {
        pcap_get_tstamp_precision(pcap);
    }
    
    // Test pcap_set_tstamp_precision after potential activation
    if (choice == 5) {
        int prec = (size > 5) ? (data[5] % 2 ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO) : PCAP_TSTAMP_PRECISION_MICRO;
        pcap_set_tstamp_precision(pcap, prec);
    }
}

static pcap_t *create_dummy_handle(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Try to create a handle from dummy file first
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size > 1024 ? 1024 : size);
        }
        close(fd);
        
        pcap_t *pcap = pcap_open_offline(filename, errbuf);
        if (pcap) {
            unlink(filename);
            return pcap;
        }
    }
    unlink(filename);
    
    // Fall back to pcap_create with dummy device
    pcap_t *pcap = pcap_create("any", errbuf);
    if (!pcap) {
        pcap = pcap_create("lo", errbuf);
    }
    return pcap;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
    // Create a dummy pcap handle
    pcap_t *pcap = create_dummy_handle(data, size);
    
    if (pcap) {
        // Test the handle with various API calls
        test_pcap_handle(pcap, data, size);
        
        // Clean up
        pcap_close(pcap);
    }
    
    return 0;
}