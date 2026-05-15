#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <pcap/bluetooth.h>
}
extern "C" {
#include <pcap/can_socketcan.h>
}
extern "C" {
#include <pcap/compiler-tests.h>
}
extern "C" {
#include <pcap/dlt.h>
}
extern "C" {
#include <pcap/funcattrs.h>
}
extern "C" {
#include <pcap/ipnet.h>
}
extern "C" {
#include <pcap/nflog.h>
}
extern "C" {
#include <pcap/pcap-inttypes.h>
}
extern "C" {
#include <pcap/pcap.h>
}
extern "C" {
#include <pcap/sll.h>
}
extern "C" {
#include <pcap/socket.h>
}
extern "C" {
#include <pcap/usb.h>
}
extern "C" {
#include <pcap/vlan.h>
}
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"pcap_lex":[[3847,1,3847,8,0,0,4,0]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_fileno","pcap_list_datalinks","pcap_free_datalinks","pcap_tstamp_type_name_to_val","pcap_set_tstamp_type","pcap_dump_open_append","pcap_dump_open","pcap_close","pcap_compile","pcap_next","pcap_offline_filter","pcap_dump","pcap_freecode","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_fileno","pcap_list_datalinks","pcap_free_datalinks","pcap_tstamp_type_name_to_val","pcap_set_tstamp_type","pcap_dump_open_append","pcap_dump_open","pcap_compile","pcap_next","pcap_offline_filter","pcap_dump","pcap_freecode","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
    if (size < 24) return 0; // Minimum size for a pcap header
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // 1. Initialize libpcap
    if (pcap_init(0, errbuf) == -1) {
        return 0;
    }
    
    // 2. Create a pcap handler from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }
    
    // 3. Get file descriptor (required by one of the APIs)
    int fd = pcap_fileno(pcap);
    // For offline captures, this might return -1, but we still call the API
    
    // 4. Get and free datalinks
    int *dlt_list;
    int count = pcap_list_datalinks(pcap, &dlt_list);
    if (count > 0) {
        pcap_free_datalinks(dlt_list);
    }
    
    // 5. Convert timestamp type name to value
    int tstamp_type_val = pcap_tstamp_type_name_to_val("host");
    
    // 6. Try to set timestamp type (may fail for offline captures)
    pcap_set_tstamp_type(pcap, tstamp_type_val);
    
    // 7. Open a dumper in append mode
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (!dumper) {
        // Fallback to regular dump open if append fails
        dumper = pcap_dump_open(pcap, "output_file");
        if (!dumper) {
            pcap_close(pcap);
            fclose(in_file);
            return 0;
        }
    }
    
    // 8. Create a simple filter program that accepts all packets
    struct bpf_program filter;
    // Compile a "true" filter (accept all)
    if (pcap_compile(pcap, &filter, "ip or not ip", 1, 0) >= 0) {
        
        // 9. Process packets with offline filtering
        struct pcap_pkthdr header;
        const u_char *packet;
        
        while ((packet = pcap_next(pcap, &header)) != nullptr) {
            // Apply offline filter
            if (pcap_offline_filter(&filter, &header, packet)) {
                // Write filtered packets to dumper
                pcap_dump((u_char *)dumper, &header, packet);
            }
        }
        
        // Free the filter
        pcap_freecode(&filter);
    }
    
    // 10. Cleanup resources
    pcap_dump_flush(dumper);
    pcap_dump_close(dumper);
    pcap_close(pcap);
    fclose(in_file);
    
    // Remove the output file we created
    remove("output_file");
    
    return 0;
}