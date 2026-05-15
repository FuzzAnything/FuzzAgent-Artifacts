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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"pcap_list_datalinks":[[3018,6,3018,19,0,0,4,0]]},"library_calls":["pcap_init","pcap_create","pcap_create","pcap_fopen_offline","pcap_set_tstamp_precision","pcap_tstamp_type_name_to_val","pcap_set_tstamp_type","pcap_fileno","pcap_dump_open_append","pcap_next","pcap_dump","pcap_inject","pcap_list_datalinks","pcap_free_datalinks","pcap_dump_close","pcap_freecode","pcap_close","pcap_close"],"critical_calls":["pcap_init","pcap_create","pcap_create","pcap_fopen_offline","pcap_set_tstamp_precision","pcap_tstamp_type_name_to_val","pcap_set_tstamp_type","pcap_fileno","pcap_dump_open_append","pcap_next","pcap_dump","pcap_inject","pcap_list_datalinks","pcap_free_datalinks","pcap_dump_close","pcap_freecode","pcap_close","pcap_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
    // Initialize libpcap
    pcap_init(0, NULL);
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = NULL;
    pcap_t *live_handle = NULL;
    pcap_dumper_t *dumper = NULL;
    int *datalinks = NULL;
    struct bpf_program bpf_prog;
    
    // Create a live capture handle for injection
    live_handle = pcap_create("any", errbuf);
    if (!live_handle) {
        // Try loopback as fallback
        live_handle = pcap_create("lo", errbuf);
    }
    
    // Open offline capture from memory
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (pcap_handle) {
        // Set timestamp precision (microsecond)
        pcap_set_tstamp_precision(pcap_handle, PCAP_TSTAMP_PRECISION_MICRO);
        
        // Convert timestamp type name to value and set it
        int tstamp_type = pcap_tstamp_type_name_to_val("host");
        if (tstamp_type >= 0) {
            pcap_set_tstamp_type(pcap_handle, tstamp_type);
        }
        
        // Get file descriptor (though not really needed for offline)
        int fd = pcap_fileno(pcap_handle);
        (void)fd; // Suppress unused variable warning
        
        // Open dumper in append mode
        dumper = pcap_dump_open_append(pcap_handle, "output_file");
        
        // Read and process packets
        struct pcap_pkthdr header;
        const u_char *packet;
        while ((packet = pcap_next(pcap_handle, &header)) != NULL) {
            if (dumper) {
                // Dump packet to output file
                pcap_dump((u_char *)dumper, &header, packet);
            }
            
            // Try to inject packet if we have a live handle
            if (live_handle) {
                pcap_inject(live_handle, packet, header.caplen);
            }
        }
        
        // Get and free datalinks for the live handle
        if (live_handle) {
            int count;
            if (pcap_list_datalinks(live_handle, &datalinks) == 0) {
                pcap_free_datalinks(datalinks);
                datalinks = NULL;
            }
        }
    }
    
    // Cleanup
    if (dumper) {
        pcap_dump_close(dumper);
    }
    
    // Free BPF program if allocated (though we didn't compile one)
    pcap_freecode(&bpf_prog);
    
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    
    if (live_handle) {
        pcap_close(live_handle);
    }
    
    fclose(in_file);
    
    return 0;
}