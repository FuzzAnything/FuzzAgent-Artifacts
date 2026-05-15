// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Test pcap_open_offline
    char errbuf[PCAP_ERRBUF_SIZE];
    write_dummy_file(Data, Size);
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap != NULL) {
        // Test pcap_next_ex
        struct pcap_pkthdr *header;
        const u_char *packet_data;
        int result;
        
        // Call multiple times to explore different states
        for (int i = 0; i < 5; i++) {
            result = pcap_next_ex(pcap, &header, &packet_data);
            if (result <= 0) {
                break;  // No more packets or error
            }
        }
        
        // Test pcap_stats (may fail for savefiles, but we call it anyway)
        struct pcap_stat stats;
        pcap_stats(pcap, &stats);
        
        // Test pcap_major_version
        pcap_major_version(pcap);
        
        // Test pcap_is_swapped
        pcap_is_swapped(pcap);
        
        pcap_close(pcap);
    }
    
    // Test pcap_activate with a created handle
    pcap_t *live_pcap = pcap_create("lo", errbuf);  // Try loopback interface
    if (live_pcap != NULL) {
        // Try to activate
        int activate_result = pcap_activate(live_pcap);
        
        // If activation succeeded, test other functions
        if (activate_result >= 0) {
            // Test pcap_next_ex with timeout
            struct pcap_pkthdr *header2;
            const u_char *packet_data2;
            pcap_next_ex(live_pcap, &header2, &packet_data2);
            
            // Test pcap_stats on live capture
            struct pcap_stat stats2;
            pcap_stats(live_pcap, &stats2);
            
            // Test pcap_major_version
            pcap_major_version(live_pcap);
            
            // Test pcap_is_swapped (should return 0 or PCAP_ERROR_NOT_ACTIVATED)
            pcap_is_swapped(live_pcap);
        }
        
        pcap_close(live_pcap);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}