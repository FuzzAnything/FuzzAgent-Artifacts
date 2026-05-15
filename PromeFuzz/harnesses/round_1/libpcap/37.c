// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file for pcap_open_offline
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    struct pcap_stat stats;
    struct pcap_pkthdr *pkt_header = NULL;
    const u_char *pkt_data = NULL;
    int ret;
    int swapped;
    
    // Test pcap_open_offline with the dummy file
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap_handle != NULL) {
        // Test pcap_major_version on opened handle
        int major_version = pcap_major_version(pcap_handle);
        (void)major_version; // Use result to avoid unused variable warning
        
        // Test pcap_is_swapped on opened handle
        swapped = pcap_is_swapped(pcap_handle);
        (void)swapped; // Use result
        
        // Test pcap_next_ex in a loop to read packets
        while ((ret = pcap_next_ex(pcap_handle, &pkt_header, &pkt_data)) == 1) {
            // Valid packet - continue reading
            // Use pkt_header and pkt_data to avoid unused warnings
            if (pkt_header && pkt_data) {
                // Access fields to ensure they're used
                (void)pkt_header->ts.tv_sec;
                (void)pkt_header->caplen;
                (void)pkt_data[0];
            }
        }
        
        // Test pcap_stats (may fail for savefiles, but we call it anyway)
        ret = pcap_stats(pcap_handle, &stats);
        (void)ret; // Use result
        
        // Clean up
        pcap_close(pcap_handle);
    }
    
    // Test pcap_activate with a created handle
    pcap_t *live_handle = pcap_create("lo", errbuf); // Use loopback interface
    if (live_handle != NULL) {
        ret = pcap_activate(live_handle);
        if (ret == 0) {
            // Activation successful - test other functions
            swapped = pcap_is_swapped(live_handle);
            (void)swapped;
            
            // Try pcap_next_ex once (non-blocking mode might be needed)
            pcap_setnonblock(live_handle, 1, errbuf);
            ret = pcap_next_ex(live_handle, &pkt_header, &pkt_data);
            (void)ret;
            
            // Test pcap_stats on live handle
            ret = pcap_stats(live_handle, &stats);
            (void)ret;
        }
        pcap_close(live_handle);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}