// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
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

static void dummy_pcap_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy callback that does nothing
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for pcap_open_offline
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);
    
    // Try to open the dummy file as a pcap savefile
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap != NULL) {
        // Test pcap_next
        struct pcap_pkthdr header;
        const u_char *packet = pcap_next(pcap, &header);
        (void)packet;
        
        // Test pcap_next_ex
        struct pcap_pkthdr *header_ptr;
        const u_char *packet_ptr;
        int ret = pcap_next_ex(pcap, &header_ptr, &packet_ptr);
        (void)ret;
        
        // Test pcap_loop with small count
        pcap_loop(pcap, 1, dummy_pcap_handler, NULL);
        
        // Test pcap_dispatch with small count
        pcap_dispatch(pcap, 1, dummy_pcap_handler, NULL);
        
        // Test pcap_breakloop
        pcap_breakloop(pcap);
        
        // Try pcap_activate - should fail on already activated handle
        int activate_ret = pcap_activate(pcap);
        (void)activate_ret;
        
        pcap_close(pcap);
    }
    
    // Test with pcap_create for pcap_activate
    pcap = pcap_create("lo", errbuf);  // Try loopback interface
    if (pcap != NULL) {
        // Configure some options before activate
        pcap_set_snaplen(pcap, 65535);
        pcap_set_promisc(pcap, 0);
        pcap_set_timeout(pcap, 1000);
        
        // Test pcap_activate
        int activate_ret = pcap_activate(pcap);
        (void)activate_ret;
        
        if (activate_ret >= 0) {
            // Only test these if activation succeeded
            struct pcap_pkthdr header;
            const u_char *packet = pcap_next(pcap, &header);
            (void)packet;
            
            struct pcap_pkthdr *header_ptr;
            const u_char *packet_ptr;
            int ret = pcap_next_ex(pcap, &header_ptr, &packet_ptr);
            (void)ret;
            
            // Test breakloop on live capture
            pcap_breakloop(pcap);
        }
        
        pcap_close(pcap);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}