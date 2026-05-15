// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
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

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for pcap_open_offline
    write_dummy_file(Data, Size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap_handle != NULL) {
        // Test pcap_major_version
        int major_version = pcap_major_version(pcap_handle);
        (void)major_version; // Use result to avoid unused variable warning
        
        // Test pcap_stats
        struct pcap_stat stats;
        int stats_result = pcap_stats(pcap_handle, &stats);
        (void)stats_result;
        
        // Test pcap_next_ex multiple times to explore different states
        struct pcap_pkthdr *pkt_header = NULL;
        const u_char *pkt_data = NULL;
        
        // Try reading up to 10 packets to explore different states
        for (int i = 0; i < 10; i++) {
            int next_result = pcap_next_ex(pcap_handle, &pkt_header, &pkt_data);
            if (next_result <= 0) {
                break; // No more packets or error
            }
        }
        
        // Test pcap_fileno
        int fileno_result = pcap_fileno(pcap_handle);
        (void)fileno_result;
        
        // Test pcap_is_swapped
        int swapped_result = pcap_is_swapped(pcap_handle);
        (void)swapped_result;
        
        // Cleanup
        pcap_close(pcap_handle);
    }
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}