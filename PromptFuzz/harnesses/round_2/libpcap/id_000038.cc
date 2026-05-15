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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_findalldevs","pcap_create","pcap_tstamp_type_val_to_description","pcap_compile_nopcap","pcap_compile","bpf_dump","pcap_freecode","pcap_dump_fopen","pcap_next","pcap_dump","pcap_inject","pcap_dump_close","pcap_freecode","pcap_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_findalldevs","pcap_create","pcap_tstamp_type_val_to_description","pcap_compile_nopcap","pcap_compile","bpf_dump","pcap_freecode","pcap_dump_fopen","pcap_next","pcap_dump","pcap_inject","pcap_dump_close","pcap_freecode","pcap_close","pcap_close","pcap_freealldevs"],"visited":0}
/**/


// Include necessary libpcap headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size) {
    // Return early if input is too small for meaningful processing
    if (size < 24) return 0;

    // 1. Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;

    // 2. Open the input as an offline pcap capture
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    
    // If pcap_fopen_offline fails, cleanup and return
    if (!pcap_handle) {
        fclose(in_file);
        return 0;
    }

    // 3. Get datalink type
    int dlt = pcap_datalink_ext(pcap_handle);

    // 4. Create a dummy device list (for API demonstration)
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Continue execution even if device enumeration fails
    }

    // 5. Create a live capture handle (not activated) for demonstration
    const char *dummy_device = "lo";
    pcap_t *live_handle = pcap_create(dummy_device, errbuf);
    if (live_handle) {
        // Note: We don't activate it since we don't want actual live capture
    }

    // 6. Demonstrate timestamp type description
    const char *ts_desc = pcap_tstamp_type_val_to_description(PCAP_TSTAMP_HOST);

    // 7. Compile a BPF filter using nopcap method
    struct bpf_program nopcap_fprog;
    const char *nopcap_filter = "ip";
    int snaplen = 65535;
    bpf_u_int32 netmask = 0xffffff00;
    
    // Try to compile with nopcap - use dlt from our offline capture
    pcap_compile_nopcap(snaplen, dlt, &nopcap_fprog, nopcap_filter, 1, netmask);

    // 8. Compile a BPF filter for the offline capture
    struct bpf_program fprog;
    const char *filter = "tcp";
    if (pcap_compile(pcap_handle, &fprog, filter, 1, netmask) == 0) {
        // Dump the compiled filter program
        bpf_dump(&fprog, 0);
        pcap_freecode(&fprog);
    }

    // 9. Create output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create a dumper for the output
        pcap_dumper_t *dumper = pcap_dump_fopen(pcap_handle, out_file);
        
        if (dumper) {
            // 10. Read and dump packets from the input
            struct pcap_pkthdr header;
            const u_char *packet;
            
            // Read up to 100 packets or until EOF
            for (int i = 0; i < 100; i++) {
                packet = pcap_next(pcap_handle, &header);
                if (!packet) break;
                
                // Dump the packet to output file
                pcap_dump((u_char *)dumper, &header, packet);
                
                // 11. Try to inject a packet (won't work on offline handle but demonstrates API)
                pcap_inject(pcap_handle, packet, header.caplen);
            }
            
            // Close the dumper
            pcap_dump_close(dumper);
        }
        fclose(out_file);
    }

    // 12. Free the nopcap filter
    pcap_freecode(&nopcap_fprog);

    // 13. Close all pcap handles
    if (live_handle) {
        pcap_close(live_handle);
    }
    pcap_close(pcap_handle);

    // 14. Free device list if allocated
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }

    // 15. Close input file
    fclose(in_file);

    return 0;
}